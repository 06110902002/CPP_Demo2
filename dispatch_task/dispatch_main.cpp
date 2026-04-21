//
// Created by rambo.liu on 2026/3/10.
//  实现一个分布式任务调度系统，Master分发任务，多个Worker并行执行，Future收集结果

#include <iostream>
#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <future>
#include <unordered_map>
#include <atomic>
#include <memory>
#include <stdexcept>
#include <chrono>
#include <string>

// 全局日志锁：解决多线程输出乱序问题
std::mutex log_mutex;

// 任务基类：抽象任意可执行任务
class Task {
public:
    using Ptr = std::shared_ptr<Task>;

    virtual ~Task() = default;

    virtual std::string execute() = 0; // 任务执行接口
    std::string get_task_id() const { return task_id_; }

    std::string task_id_;
};

// 具体任务实现：示例任务（计算两数之和）
class AddTask : public Task {
public:
    AddTask(int a, int b, std::string task_id) : a_(a), b_(b) {
        task_id_ = std::move(task_id);
    }

    std::string execute() override {
        // 模拟任务执行耗时
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        return std::to_string(a_ + b_);
    }

private:
    int a_, b_;
};

// Worker 节点：执行任务的工作节点
class Worker {
public:
    Worker(int id, std::queue<Task::Ptr> &task_queue,
           std::mutex &queue_mutex,
           std::condition_variable &cv,
           std::unordered_map<std::string, std::promise<std::string> > &result_promises,
           std::mutex &result_mutex)
        : worker_id_(id),
          task_queue_(task_queue),
          queue_mutex_(queue_mutex),
          cv_(cv),
          result_promises_(result_promises),
          result_mutex_(result_mutex),
          running_(true) {
        // 启动 Worker 工作线程
        worker_thread_ = std::thread(&Worker::work_loop, this);
    }

    ~Worker() {
        running_ = false;
        cv_.notify_all(); // 唤醒线程退出循环
        if (worker_thread_.joinable()) {
            worker_thread_.join();
        }

        // 打印退出日志（加锁）
        std::lock_guard<std::mutex> lock(log_mutex);
        std::cout << "Worker " << worker_id_ << " 退出" << std::endl;
    }

    int get_id() const { return worker_id_; }

private:
    void work_loop() {
        while (running_) {
            Task::Ptr task;
            // 加锁获取任务
            {
                std::unique_lock<std::mutex> lock(queue_mutex_);
                // 等待任务或退出信号
                cv_.wait(lock, [this]() { return !task_queue_.empty() || !running_; });

                if (!running_ && task_queue_.empty()) {
                    break; // 退出循环
                }

                if (!task_queue_.empty()) {
                    task = std::move(task_queue_.front());
                    task_queue_.pop();
                }
            }

            // 执行任务并返回结果
            if (task) {
                try {
                    std::string result = task->execute();

                    // 加锁打印日志，解决乱序问题
                    {
                        std::lock_guard<std::mutex> lock(log_mutex);
                        std::cout << "Worker " << worker_id_ << " 完成任务 " << task->get_task_id()
                                << "，结果：" << result << std::endl;
                    }

                    // 存储结果到 promise 并移除已完成的promise
                    std::lock_guard<std::mutex> lock(result_mutex_);
                    auto it = result_promises_.find(task->get_task_id());
                    if (it != result_promises_.end()) {
                        it->second.set_value(result);
                        result_promises_.erase(it); // 关键修复：移除已完成的promise
                    }
                } catch (const std::exception &e) {
                    // 任务执行异常处理
                    std::lock_guard<std::mutex> lock(result_mutex_);
                    auto it = result_promises_.find(task->get_task_id());
                    if (it != result_promises_.end()) {
                        it->second.set_exception(std::current_exception());
                        result_promises_.erase(it); // 异常也移除promise
                    }

                    // 加锁打印错误日志
                    std::lock_guard<std::mutex> lock2(log_mutex);
                    std::cerr << "Worker " << worker_id_ << " 执行任务 " << task->get_task_id()
                            << " 出错：" << e.what() << std::endl;
                }
            }
        }
    }

    int worker_id_;
    std::thread worker_thread_;
    std::atomic<bool> running_; // 控制 Worker 运行状态

    // 共享资源（与 Master 共享）
    std::queue<Task::Ptr> &task_queue_;
    std::mutex &queue_mutex_;
    std::condition_variable &cv_;
    std::unordered_map<std::string, std::promise<std::string> > &result_promises_;
    std::mutex &result_mutex_;
};

// Master 节点：任务分发中心
class Master {
public:
    Master(int worker_count) : next_task_id_(0) {
        // 创建指定数量的 Worker
        for (int i = 0; i < worker_count; ++i) {
            workers_.emplace_back(std::make_unique<Worker>(
                i, task_queue_, queue_mutex_, cv_, result_promises_, result_mutex_));

            // 加锁打印日志
            std::lock_guard<std::mutex> lock(log_mutex);
            std::cout << "创建 Worker 节点 " << i << std::endl;
        }
    }

    ~Master() {
        // 停止所有 Worker
        workers_.clear();
    }

    // 提交任务，返回 Future 用于获取结果
    std::future<std::string> submit_task(std::unique_ptr<Task> task) {
        std::string task_id = "task_" + std::to_string(next_task_id_++);
        task->task_id_ = task_id;

        // 创建 promise 并关联 future
        std::promise<std::string> prom;
        std::future<std::string> fut = prom.get_future();

        {
            std::lock_guard<std::mutex> lock(result_mutex_);
            result_promises_[task_id] = std::move(prom);
        }

        // 将任务加入队列
        {
            std::lock_guard<std::mutex> lock(queue_mutex_);
            task_queue_.push(std::move(task));
        }
        cv_.notify_one(); // 唤醒一个 Worker 执行任务

        // 加锁打印日志
        std::lock_guard<std::mutex> lock(log_mutex);
        std::cout << "Master 提交任务 " << task_id << std::endl;

        return fut;
    }

    // 等待所有任务完成（修复版）
    void wait_all_tasks() {
        // 第一步：等待任务队列清空
        std::unique_lock<std::mutex> queue_lock(queue_mutex_);
        cv_.wait(queue_lock, [this]() { return task_queue_.empty(); });
        queue_lock.unlock();

        // 第二步：等待所有promise处理完成
        bool all_done = false;
        while (!all_done) {
            std::lock_guard<std::mutex> res_lock(result_mutex_);
            all_done = result_promises_.empty();

            if (!all_done) {
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
        }

        std::lock_guard<std::mutex> lock(log_mutex);
        std::cout << "所有任务已处理完成" << std::endl;
    }

private:
    // 任务队列
    std::queue<Task::Ptr> task_queue_;
    std::mutex queue_mutex_;
    std::condition_variable cv_;

    // 结果存储（promise-future 配对）
    std::unordered_map<std::string, std::promise<std::string> > result_promises_;
    std::mutex result_mutex_;

    // Worker 节点管理
    std::vector<std::unique_ptr<Worker> > workers_;
    std::atomic<int> next_task_id_; // 任务 ID 生成器
};

// 测试主函数
int main() {
    try {
        // 1. 创建 Master，启动 3 个 Worker 节点
        Master master(3);

        // 2. 提交一批任务
        std::vector<std::future<std::string> > futures;
        for (int i = 0; i < 10; ++i) {
            int a = rand() % 100;
            int b = rand() % 100;
            futures.push_back(master.submit_task(
                std::make_unique<AddTask>(a, b, "")
            ));
        }

        // 3. 通过 Future 收集结果
        {
            std::lock_guard<std::mutex> lock(log_mutex);
            std::cout << "\n===== 收集任务结果 =====" << std::endl;
        }

        for (size_t i = 0; i < futures.size(); ++i) {
            try {
                // 获取结果（阻塞直到任务完成）
                std::string result = futures[i].get();

                std::lock_guard<std::mutex> lock(log_mutex);
                std::cout << "任务 " << i << " 结果：" << result << std::endl;
            } catch (const std::exception &e) {
                std::lock_guard<std::mutex> lock(log_mutex);
                std::cerr << "任务 " << i << " 执行失败：" << e.what() << std::endl;
            }
        }

        // 4. 等待所有任务完成
        master.wait_all_tasks();

        {
            std::lock_guard<std::mutex> lock(log_mutex);
            std::cout << "\n所有任务执行完成！程序即将退出" << std::endl;
        }
    } catch (const std::exception &e) {
        std::lock_guard<std::mutex> lock(log_mutex);
        std::cerr << "系统异常：" << e.what() << std::endl;
        return 1;
    }

    return 0;
}
