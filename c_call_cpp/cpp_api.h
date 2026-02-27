#ifndef CPP_API_H
#define CPP_API_H

#ifdef __cplusplus
extern "C" {
#endif

// ---------- 1 & 2: static-like free functions ----------
int cpp_static_add(int a, int b);

// C -> C++ (int + string) -> return heap string (C frees)
const char* cpp_process(int value, const char* text);
void cpp_free_string(const char* str);

// ---------- 3 & 4: C operates C++ class via opaque handle ----------
typedef void* CppObject;

CppObject cpp_object_create(int base);
void cpp_object_destroy(CppObject obj);

// instance method
int cpp_object_compute(CppObject obj, int x);

// ---------- 5: callback from C++ to C ----------
typedef void (*c_callback_t)(int progress);

void cpp_object_run_with_callback(CppObject obj, c_callback_t cb);

#ifdef __cplusplus
}
#endif

#endif // CPP_API_H