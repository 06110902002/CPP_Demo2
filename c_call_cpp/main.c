#include <stdio.h>
#include "cpp_api.h"
#include "math_c_api.h"


// callback function implemented in C
void on_progress(int progress) {
    printf("[C callback] progress = %d%%\n", progress);
}


void c_call_cpp_temp() {
    int32_t i = c_add_int(10, 20);
    double d = c_add_double(3.14, 2.86);

    printf("Int result: %d\n", i);        // 输出 30
    printf("Double result: %.2f\n", d);   // 输出 6.00
}



int main() {
    // 1. static method
    int sum = cpp_static_add(3, 7);
    printf("Static add result: %d\n", sum);

    // 2. C -> C++ params, return string
    const char* result = cpp_process(42, "hello from C");
    printf("Returned string: %s\n", result);
    cpp_free_string(result);

    // 3 & 4. operate C++ object
    CppObject obj = cpp_object_create(10);
    int value = cpp_object_compute(obj, 5);
    printf("Object compute result: %d\n", value);

    // 5. callback
    cpp_object_run_with_callback(obj, on_progress);

    cpp_object_destroy(obj);

    printf("下面测试C 调用C++ 模板\n");
    c_call_cpp_temp();



    return 0;
}