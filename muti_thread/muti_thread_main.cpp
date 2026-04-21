//
// Created by rambo.liu on 2026/3/12.
// 实现三个任务的顺序执行：任务A → 任务B（依赖A结果）→ 任务C（依赖B结果），要求异步非阻塞

#include <iostream>
#include <future>
#include <thread>
#include <functional>


int taskA() {
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "Task A finished\n";
    return 10;
}

int taskB(int a) {
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "Task B finished\n";
    return a * 2;
}

int taskC(int b) {
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "Task C finished\n";
    return b + 5;
}

/**
 * std::async + future 任务链
 */
void test1() {
    printf("使用 std::async + future 任务链 实现\n");
    auto futureA = std::async(std::launch::async, taskA);

    auto futureB = std::async(std::launch::async, [&]{
        int a = futureA.get();
        return taskB(a);
    });

    auto futureC = std::async(std::launch::async, [&]{
        int b = futureB.get();
        return taskC(b);
    });

    std::cout << "main thread continue...\n";

    int result = futureC.get();
    std::cout << "Final result = " << result << std::endl;
}

/****---------------------------------------------*/
/*很多现代异步库都支持：
future.then(...)
C++标准库没有提供 then，但可以自己实现。
*/

template<typename F, typename T>
auto then(std::future<T> fut, F func)
{
    return std::async(std::launch::async,
        [f = std::move(fut), func]() mutable {
            return func(f.get());
        });
}

void test_then_model()
{
    printf("自定义一个 future then 模式实现:\n");
    auto fa = std::async(std::launch::async, taskA);

    auto fb = then(std::move(fa), taskB);

    auto fc = then(std::move(fb), taskC);

    std::cout << "main thread running\n";

    std::cout << "result = " << fc.get() << std::endl;

    // std::function<int()> ff = []()->int {
    //     return 20;
    // };
    //
    // int re = ff();
    // std::cout << "85-----main thread result: " << re << std::endl;
}
/****---------------------------------------------*****/



/****---------------------------------------------*****/
/*实现一个倒计时门闩，允许一个或多个线程等待，直到在其他线程中执行的一组操作完成*/

#include <mutex>
#include <condition_variable>
#include <stdexcept>
#include <vector>

class CountDownLatch {
public:
    // 构造函数：初始化计数器，count 必须大于等于0
    explicit CountDownLatch(int count) : count_(count) {
        if (count < 0) {
            throw std::invalid_argument("Count must be non-negative");
        }
    }

    // 等待计数器归0，阻塞当前线程直到计数器为0
    void await() {
        std::unique_lock<std::mutex> lock(mutex_);
        // 条件变量的等待：当count_ > 0时持续阻塞
        cv_.wait(lock, [this]() { return count_ == 0; });
    }

    // 计数器递减1，如果递减后为0，唤醒所有等待的线程
    void countDown() {
        std::lock_guard<std::mutex> lock(mutex_);
        if (count_ > 0) {
            --count_;
            // 计数器归0时唤醒所有等待线程
            if (count_ == 0) {
                cv_.notify_all();
            }
        }
    }

    // 获取当前计数器值（可选接口）
    int getCount() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return count_;
    }

    // 禁用拷贝和移动（同步工具通常不允许拷贝）
    CountDownLatch(const CountDownLatch&) = delete;
    CountDownLatch& operator=(const CountDownLatch&) = delete;
    CountDownLatch(CountDownLatch&&) = delete;
    CountDownLatch& operator=(CountDownLatch&&) = delete;

private:
    mutable std::mutex mutex_;          // 保护计数器的互斥锁（mutable允许const成员函数修改）
    std::condition_variable cv_;        // 条件变量，用于阻塞/唤醒等待线程
    int count_;                         // 倒计时计数器
};


// 工作线程函数：执行任务后调用countDown
void worker(CountDownLatch& latch, int id) {
    std::cout << "Worker " << id << " start working..." << std::endl;
    // 模拟任务执行（比如耗时操作）
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "Worker " << id << " finish work, count down..." << std::endl;
    latch.countDown();
}

void test_count_down() {
    printf("-----------测试倒计时门闩---------\n");
    const int worker_num = 3;
    CountDownLatch latch(worker_num);  // 初始化计数器为3

    // 创建3个工作线程
    std::vector<std::thread> workers;
    for (int i = 0; i < worker_num; ++i) {
        workers.emplace_back(worker, std::ref(latch), i + 1);
    }

    std::cout << "Main thread wait for all workers finish..." << std::endl;
    latch.await();  // 主线程阻塞，直到计数器归0

    // 等待所有工作线程结束
    for (auto& t : workers) {
        t.join();
    }

    std::cout << "All workers finish! Main thread continue..." << std::endl;
}

/****---------------------------------------------*****/




int main() {

    test1();
    test_then_model();

    test_count_down();

    return 0;

}