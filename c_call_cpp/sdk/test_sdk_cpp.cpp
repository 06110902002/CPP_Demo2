//
// Created by rambo.liu on 2026/2/11.
//
#include <cassert>
#include <iostream>
#include "sdk.h"

#include <atomic>
#include <thread>
#include <cassert>
#include <iostream>
#include "sdk.h"

static void cpp_async_cb(int result, void* user_data) {
    auto* flag = static_cast<std::atomic<int>*>(user_data);
    flag->store(result);
}

int main() {
    sdk_object_t* obj = sdk_create();

    std::atomic<int> value{0};

    sdk_start_async_task(
        obj,
        50,
        cpp_async_cb,
        &value
    );

    std::this_thread::sleep_for(
        std::chrono::milliseconds(500)
    );

    assert(value.load() == 100);

    sdk_destroy(obj);
    std::cout << "ASYNC CALLBACK TEST PASSED\n";
    return 0;
}

