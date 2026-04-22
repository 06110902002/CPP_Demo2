//
// Created by rambo.liu on 2026/4/22.
// 1. 使用模板实现，将字符串的内容转为对应的类型，比如std:string s = "123", 转换结果为：int res = 123 支持基本数据类型转换
// 2.如果是std:string 就不需要转换
// 3.如果是对象类型也转换为目标对应类型

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <stdexcept>
#include <type_traits> // 用于类型萃取

// ==========================================
// 1. 定义一个自定义对象类型
// ==========================================
class User {
public:
    std::string name;
    int age;

    // 假设我们定义一个规则：字符串格式为 "Name,Age"
    // 这是一个接受 string 的构造函数，用于支持从 string 转换
    explicit User(const std::string& str) {
        size_t pos = str.find(',');
        if (pos != std::string::npos) {
            name = str.substr(0, pos);
            age = std::stoi(str.substr(pos + 1));
        } else {
            name = str;
            age = 0;
        }
    }

    void print() const {
        std::cout << "User: " << name << ", Age: " << age << std::endl;
    }
};

// ==========================================
// 2. 模板转换函数实现
// ==========================================

// 通用模板：处理 int, double, float 等数值类型
template <typename T>
T stringTo(const std::string& str) {
    // 使用 istringstream 进行流式转换
    std::istringstream iss(str);
    T result;
    if (!(iss >> result)) {
        throw std::runtime_error("Conversion failed for type");
    }
    // 检查是否有多余字符（保证转换的严格性，可选）
    // if (!iss.eof()) throw ...
    return result;
}

// 模板特化：专门处理 std::string，直接返回，不做任何转换
template <>
std::string stringTo<std::string>(const std::string& str) {
    return str;
}

// ==========================================
// 3. 进阶：使用 C++17 if constexpr 统一处理
// ==========================================
// 这种方式比上面的“通用+特化”更现代，逻辑更集中

template <typename T>
T convertString(const std::string& str) {
    // 编译期分支判断

    // 情况 A: 如果是 std::string，直接返回
    if constexpr (std::is_same_v<T, std::string>) {
        return str;
    }
    // 情况 B: 如果是算术类型 (int, double, char...)
    else if constexpr (std::is_arithmetic_v<T>) {
        std::istringstream iss(str);
        T result;
        if (iss >> result) {
            return result;
        } else {
            throw std::invalid_argument("Cannot convert string to number: " + str);
        }
    }
    // 情况 C: 其他类型（假设是自定义对象，且有接受 string 的构造函数）
    else {
        // 这里利用 C++ 的隐式转换机制，直接构造对象
        return T(str);
    }
}



#include <iostream>
#include <string>
#include <sstream>
#include <type_traits> // 核心头文件
#include <vector>

// ==========================================
// 2. 使用 std::enable_if 实现转换函数
// ==========================================

// --- 情况 1: 算术类型 (int, double, float 等) ---
// 只有当 T 是数字类型时，这个函数才会被实例化
template <typename T>
typename std::enable_if<std::is_arithmetic<T>::value, T>::type
stringTo2(const std::string& str) {
    std::istringstream iss(str);
    T value;
    iss >> value;
    return value;
}

// --- 情况 2: 字符串类型 (std::string) ---
// 只有当 T 严格等于 std::string 时，这个函数才会被实例化
template <typename T>
typename std::enable_if<std::is_same<T, std::string>::value, T>::type
stringTo2(const std::string& str) {
    return str; // 直接返回，无需转换
}

// --- 情况 3: 对象类型 (User 等) ---
// 条件：T 不是数字 && T 不是 string
// 这样就避免了与上面两个函数冲突
template <typename T>
typename std::enable_if<!std::is_arithmetic<T>::value && !std::is_same<T, std::string>::value, T>::type
stringTo2(const std::string& str) {
    return T(str); // 调用 T 的构造函数 T(string)
}

// ==========================================
// 3. 主函数测试
// ==========================================
void test_enable_if() {
    std::string s_int = "123";
    std::string s_double = "3.14";
    std::string s_str = "Hello String";
    std::string s_obj = "Bob,30";

    try {
        // 1. 转为 int
        int res_int = stringTo2<int>(s_int);
        std::cout << "Int: " << res_int << std::endl;

        // 2. 转为 double
        double res_double = stringTo2<double>(s_double);
        std::cout << "Double: " << res_double << std::endl;

        // 3. 转为 string (特殊情况)
        std::string res_str = stringTo2<std::string>(s_str);
        std::cout << "String: " << res_str << std::endl;

        // 4. 转为对象
        User res_user = stringTo2<User>(s_obj);
        std::cout << "User: " << res_user.name << ", " << res_user.age << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}





int test_constexpr() {
    std::string s_int = "123";
    std::string s_double = "3.14159";
    std::string s_str = "Hello World";
    std::string s_obj = "Alice,25";

    try {
        // 1. 转为 int
        int res_int = convertString<int>(s_int);
        std::cout << "Int Result: " << res_int << " (Type: int)" << std::endl;

        // 2. 转为 double
        double res_double = convertString<double>(s_double);
        std::cout << "Double Result: " << res_double << " (Type: double)" << std::endl;

        // 3. 转为 string (不需要转换)
        std::string res_str = convertString<std::string>(s_str);
        std::cout << "String Result: " << res_str << " (Type: string)" << std::endl;

        // 4. 转为对象 (User)
        User res_user = convertString<User>(s_obj);
        std::cout << "Object Result: ";
        res_user.print();

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}


// ==========================================
// 4. 主函数测试
// ==========================================

int main() {
    printf("test if_enable------------------\n");
    test_enable_if();

    printf("test if_test_constexpr------------\n");
    test_constexpr();



    return 0;
}
