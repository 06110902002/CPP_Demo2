#include "cpp_api.h"
#include <string>
#include <cstring>
#include <iostream>

// ---------- 1: static method (exposed as C function) ----------
int cpp_static_add(int a, int b) {
    return a + b;
}

// ---------- 2: C -> C++ params, return string ----------
const char* cpp_process(int value, const char* text) {
    std::string result = "C++ processed: value=" +
                         std::to_string(value) +
                         ", text=" + (text ? text : "");

    // allocate C string (caller frees)
    char* cstr = new char[result.size() + 1];
    std::memcpy(cstr, result.c_str(), result.size() + 1);
    return cstr;
}

void cpp_free_string(const char* str) {
    delete[] str;
}

// ---------- C++ class ----------
class Calculator {
public:
    explicit Calculator(int base) : base_(base) {}

    int compute(int x) const {
        return base_ + x;
    }

    void runWithCallback(c_callback_t cb) {
        if (!cb) return;
        for (int i = 0; i <= 100; i += 25) {
            cb(i); // callback into C
        }
    }

private:
    int base_;
};

// ---------- 3 & 4: object lifecycle ----------
CppObject cpp_object_create(int base) {
    return new Calculator(base);
}

void cpp_object_destroy(CppObject obj) {
    delete static_cast<Calculator*>(obj);
}

int cpp_object_compute(CppObject obj, int x) {
    auto* calc = static_cast<Calculator*>(obj);
    return calc->compute(x);
}

// ---------- 5: callback ----------
void cpp_object_run_with_callback(CppObject obj, c_callback_t cb) {
    auto* calc = static_cast<Calculator*>(obj);
    calc->runWithCallback(cb);
}