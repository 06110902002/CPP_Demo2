//
// Created by rambo.liu on 2026/4/21.
// 测试promise 来测试不同线程之间的数据交互
// 通过模版的方式来实现，客户端获取自己订阅的类型


#include <iostream>
#include <thread>
#include <future>
#include <string>
#include <vector>
#include <chrono>
#include <memory>
#include <functional>
#include <type_traits>

// ==========================================
// 1. 模拟业务数据类型
// ==========================================
struct DataObject {
    int id;
    std::string desc;

    DataObject(int i, std::string d) : id(i), desc(std::move(d)) {
    }

    friend std::ostream &operator<<(std::ostream &os, const DataObject &obj) {
        os << "{ID:" << obj.id << ", Desc:" << obj.desc << "}";
        return os;
    }
};

// 模拟业务函数
int compute_int(int x) {
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    return x * 2;
}

std::string process_string(const std::string &s) {
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    return "Processed: " + s;
}

DataObject create_object(long id, const std::string &name) {
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    return DataObject((int) id, name);
}

void do_void_task() {
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    std::cout << "Void task done." << std::endl;
}


// ==========================================
// 2. 通用异步任务封装 (更简洁的版本)
// ==========================================
template<typename T>
class AsyncTask {
public:
    AsyncTask() {
        promise_ = std::make_unique<std::promise<T> >();
        shared_future_ = promise_->get_future().share();
    }

    // 提供对 promise 的访问，让外部可以调用 set_value
    std::promise<T> &get_promise() {
        return *promise_;
    }

    std::shared_future<T> get_future() const {
        return shared_future_;
    }

private:
    std::unique_ptr<std::promise<T> > promise_;
    std::shared_future<T> shared_future_;
};


// ==========================================
// 3. 消费者执行器
// ==========================================
class ConsumerWorker {
public:
    template<typename Func, typename... Args>
    auto call(const std::string &rpc_name, Func &&func, Args &&... args) {
        // 自动推导返回值类型
        using ReturnType = typename std::invoke_result<Func, Args...>::type;

        // 创建对应的 AsyncTask
        AsyncTask<ReturnType> task;

        // 获取 promise 的引用，用于在 lambda 中设置值
        // 注意：这里我们捕获 promise 的引用，但必须保证 task 对象在 lambda 执行期间存活
        // 为了安全，我们将整个 task 对象移动到 lambda 中
        auto task_ptr = std::make_shared<AsyncTask<ReturnType> >(std::move(task));
        auto &promise_ref = task_ptr->get_promise();

        // 启动线程
        std::thread([task_ptr, &promise_ref, func, args...]() mutable {
            try {
                // 使用 if constexpr 在编译期判断返回值是否为 void
                if constexpr (std::is_void<ReturnType>::value) {
                    std::invoke(func, args...);
                    // void 版本：调用无参 set_value
                    promise_ref.set_value();
                } else {
                    // 非 void 版本：计算结果并移动
                    ReturnType result = std::invoke(func, args...);
                    promise_ref.set_value(std::move(result));
                }
            } catch (...) {
                promise_ref.set_exception(std::current_exception());
            }
        }).detach();

        return task_ptr->get_future();
    }
};

// ==========================================
// 4. 主程序演示
// ==========================================
int main() {
    ConsumerWorker worker;
    std::vector<std::future<void> > wait_handles;

    std::cout << "=== 通用异步调用框架演示 ===" << std::endl;

    // --- 1. int 类型 ---
    auto f1 = worker.call("compute_int", compute_int, 10);
    wait_handles.push_back(std::async(std::launch::async, [f1 = std::move(f1)]() mutable {
        int res = f1.get();
        std::cout << "[Int] 结果: " << res << std::endl;
    }));

    // --- 2. String 类型 ---
    auto f2 = worker.call("process_string", process_string, std::string("Hello"));
    wait_handles.push_back(std::async(std::launch::async, [f2 = std::move(f2)]() mutable {
        std::string res = f2.get();
        std::cout << "[String] 结果: " << res << std::endl;
    }));

    // --- 3. Object 类型 ---
    auto f3 = worker.call("create_object", create_object, 888L, std::string("TestObj"));
    wait_handles.push_back(std::async(std::launch::async, [f3 = std::move(f3)]() mutable {
        DataObject res = f3.get();
        std::cout << "[Object] 结果: " << res << std::endl;
    }));

    // --- 4. Void 类型 ---
    auto f4 = worker.call("void_task", do_void_task);
    wait_handles.push_back(std::async(std::launch::async, [f4 = std::move(f4)]() mutable {
        f4.get(); // 等待完成
        std::cout << "[Void] 任务完成" << std::endl;
    }));

    for (auto &f: wait_handles) {
        if (f.valid()) f.wait();
    }

    std::cout << "=== 所有任务完成 ===" << std::endl;
    return 0;
}
