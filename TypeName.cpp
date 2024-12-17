//
// Created by Rambo.Liu on 2024/12/5.
// TypeName 的介绍
// 参考 https://developer.aliyun.com/article/1465436

#include <iostream>


template<class T>
void foo() {
    typename T::iterator *iter;
}

struct ContainsAType {
    struct iterator {

    };
};

/***-----------------测试二---------------------*/

// 模板函数，同时使用模板参数列表、函数参数列表和函数返回值类型进行类型检查
template<
        typename T,  // 模板参数 T
        // 在模板参数列表中使用 typename 进行检查，T 必须是整型
        typename std::enable_if<std::is_integral<T>::value>::type * = nullptr
>
// 在返回值类型中使用 typename 进行检查，T 的大小必须大于 4 字节
typename std::enable_if<(sizeof(T) > 4), T>::type
process(
        T value,  // 函数参数 value
        // 在函数参数列表中使用 typename 进行检查，T 必须是有符号类型
        typename std::enable_if<std::is_signed<T>::value, T>::type * = nullptr
) {
    std::cout << value << " is an integral number, its size is greater than 4 bytes, and it is signed." << std::endl;
    return value;
}


// 模板类，使用模板参数列表进行类型检查，T 必须是整型
template<
        typename T,  // 模板参数 T
        typename std::enable_if<std::is_integral<T>::value>::type * = nullptr
>
class MyClass {
};

int main() {
    long l = 42;
    process(l);  // 此处可以成功编译和运行，因为 long 是整型，大于 4 字节，且是有符号类型
    // 下面的代码将无法编译，因为 unsigned long 不是有符号类型
    // unsigned long ul = 42;
    // process(ul);
    MyClass<int> myClass;  // 成功实例化 MyClass，因为 int 是整型
    return 0;
}


