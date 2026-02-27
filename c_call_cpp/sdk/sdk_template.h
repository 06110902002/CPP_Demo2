//
// Created by rambo.liu on 2026/2/11.
//

#pragma once
#include <type_traits>

/* ===============================
 * 普通模板
 * =============================== */
template<typename T>
T sum(const T* data, int len) {
    T s{};
    for (int i = 0; i < len; ++i)
        s += data[i];
    return s;
}

/* ===============================
 * 模板特化
 * =============================== */
template<>
inline const char* sum<const char*>(const char* const*, int) {
    return "unsupported";
}

/* ===============================
 * 模板元编程：类型判断
 * =============================== */
template<typename T>
struct TypeInfo {
    static constexpr bool is_supported =
        std::is_arithmetic<T>::value;
};

/* ===============================
 * constexpr + if constexpr
 * =============================== */
template<typename T>
constexpr int type_id() {
    if constexpr (std::is_same_v<T, int>) return 1;
    if constexpr (std::is_same_v<T, double>) return 2;
    return -1;
}
