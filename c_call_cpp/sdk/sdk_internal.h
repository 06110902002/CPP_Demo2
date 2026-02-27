//
// Created by rambo.liu on 2026/2/11.
//

#pragma once
#include <thread>
#include <chrono>

/* ===============================
 * 模拟耗时任务
 * =============================== */
inline int heavy_work(int input) {
    std::this_thread::sleep_for(
        std::chrono::milliseconds(200)
    );
    return input * 2;
}
