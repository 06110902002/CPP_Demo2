//
// Created by rambo.liu on 2026/2/11.
//

#pragma once
#include <vector>

/* ===============================
 * 核心 C++ 类
 * =============================== */
class SdkObject {
public:
    SdkObject() = default;

    void setValue(int v) {
        value_ = v;
    }

    int getValue() const {
        return value_;
    }

private:
    int value_ = 0;
};

/* ===============================
 * 模板能力
 * =============================== */
template<typename T>
T sumArray(const T* data, int length) {
    T sum{};
    for (int i = 0; i < length; ++i) {
        sum += data[i];
    }
    return sum;
}
