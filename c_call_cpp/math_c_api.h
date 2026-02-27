//
// Created by rambo.liu on 2026/2/9.
// C 调用C++ 模板的头文件

#ifndef C_CALL_CPP_ADVANCED_MATH_C_API_H
#define C_CALL_CPP_ADVANCED_MATH_C_API_H

#ifdef __cplusplus
extern "C" {
#endif

    int32_t c_add_int(int32_t a, int32_t b);
    double c_add_double(double a, double b);

#ifdef __cplusplus
}
#endif


#endif //C_CALL_CPP_ADVANCED_MATH_C_API_H