//
// Created by rambo.liu on 2026/2/11.
//

#pragma once
#include <string>

/* ===============================
 * 核心 C++ 类
 * =============================== */
class SdkCore {
public:
    SdkCore() = default;
    explicit SdkCore(int v) : value_(v) {}

    /* 成员方法 */
    void setValue(int v) { value_ = v; }
    int  getValue() const { return value_; }

    int add(int x) const { return value_ + x; }

    /* 类静态方法 */
    static int version() {
        return 1001; // v1.0.1
    }

private:
    int value_ = 0;
};
