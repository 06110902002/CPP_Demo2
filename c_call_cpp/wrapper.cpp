//
// Created by rambo.liu on 2026/2/9.
//

// wrapper.cpp
#include "math_template.h"
#include <cstdint> // for int32_t

// 显式实例化模板（必须！否则链接时找不到符号）
template int32_t add<int32_t>(int32_t, int32_t);
template double add<double>(double, double);

// C 风格包装函数
extern "C" {
    int32_t c_add_int(int32_t a, int32_t b) {
        return add<int32_t>(a, b);
    }

    double c_add_double(double a, double b) {
        return add<double>(a, b);
    }
}