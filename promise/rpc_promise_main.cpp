// #include <iostream>
// #include <string>
// #include <functional>
// #include <vector>
// #include <map>
// #include <tuple>
// #include <sstream>
// #include <type_traits>
// #include <utility>
//
// // ==========================================
// // 1. function_traits (保持不变)
// // ==========================================
// template<typename T>
// struct function_traits;
//
// template<typename R, typename... Args>
// struct function_traits<R(Args...)> {
//     static constexpr size_t arity = sizeof...(Args);
//     using result_type = R;
//     using arguments = std::tuple<Args...>;
// };
//
// template<typename R, typename... Args>
// struct function_traits<std::function<R(Args...)> > {
//     static constexpr size_t arity = sizeof...(Args);
//     using result_type = R;
//     using arguments = std::tuple<Args...>;
// };
//
// template<typename R, typename... Args>
// struct function_traits<R(*)(Args...)> : public function_traits<R(Args...)> {};
//
// template<typename ClassType, typename R, typename... Args>
// struct function_traits<R(ClassType::*)(Args...) const> {
//     static constexpr size_t arity = sizeof...(Args);
//     using result_type = R;
//     using arguments = std::tuple<Args...>;
// };
//
// template<typename T>
// struct function_traits : public function_traits<decltype(&T::operator())> {};
//
// // ==========================================
// // 2. Serializer (保持不变)
// // ==========================================
// struct Serializer {
//     template<typename T>
//     static std::string serialize(const T& value) {
//         return std::to_string(value);
//     }
//
//     static std::string serialize(const std::string& value) {
//         return value;
//     }
//
//     template<typename T>
//     static T deserialize(const std::string& str) {
//         std::decay_t<T> value;
//         std::istringstream iss(str);
//         if constexpr (std::is_same_v<std::decay_t<T>, std::string>) {
//             value = str;
//         } else {
//             iss >> value;
//         }
//         return value;
//     }
// };
//
// // ==========================================
// // 3. RPC Server (修复点在这里)
// // ==========================================
// class RpcServer {
// private:
//     std::map<std::string, std::function<std::string(const std::vector<std::string>&)>> services;
//
// public:
//     template<typename Func>
//     void register_service(const std::string& name, Func func) {
//         using Traits = function_traits<Func>;
//         constexpr size_t arg_count = Traits::arity;
//         using ArgsTuple = typename Traits::arguments;
//
//         // 修复点：捕获列表从 [func] 改为 [this, func]
//         // 必须显式捕获 this，因为 Lambda 内部调用了 this->invoke_with_args
//         std::function<std::string(const std::vector<std::string>&)> wrapper =
//             [this, func](const std::vector<std::string>& args_str) -> std::string {
//
//             if (args_str.size() != arg_count) {
//                 return "Error: Argument count mismatch";
//             }
//
//             return invoke_with_args(func, args_str, std::make_index_sequence<arg_count>{});
//         };
//
//         services[name] = wrapper;
//         std::cout << "[注册成功] " << name << std::endl;
//     }
//
//     std::string call(const std::string& name, const std::vector<std::string>& args) {
//         if (services.find(name) != services.end()) {
//             return services[name](args);
//         }
//         return "Error: Service not found";
//     }
//
// private:
//     template<typename Func, size_t... Is>
//     std::string invoke_with_args(Func& func, const std::vector<std::string>& args_str, std::index_sequence<Is...>) {
//         using ArgsTuple = typename function_traits<Func>::arguments;
//
//         auto result = func(Serializer::deserialize<std::tuple_element_t<Is, ArgsTuple>>(args_str[Is])...);
//
//         return Serializer::serialize(result);
//     }
// };
//
// // ==========================================
// // 4. 测试
// // ==========================================
// int main() {
//     RpcServer server;
//
//     server.register_service("add", [](int a, int b,int c) {
//         return a + b + c;
//     });
//
//     server.register_service("say", [](const std::string& msg) {
//         return "Echo: " + msg;
//     });
//
//     std::cout << "Add: " << server.call("add", {"10", "20","30"}) << std::endl;
//     std::cout << "Say: " << server.call("say", {"Hello"}) << std::endl;
//
//     return 0;
// }



// #include <iostream>
// #include <string>
// #include <functional>
// #include <vector>
// #include <map>
// #include <tuple>
// #include <sstream>
// #include <type_traits>
// #include <utility>
//
// // ==========================================
// // 1. 序列化工具
// // ==========================================
// struct Serializer {
//     template<typename T>
//     static void serialize(std::stringstream& ss, const T& value) {
//         ss << value << "\n";
//     }
//
//     template<typename T>
//     static void deserialize(std::stringstream& ss, T& value) {
//         std::string line;
//         if (!std::getline(ss, line)) {
//             throw std::runtime_error("Deserialization error");
//         }
//         std::istringstream item_ss(line);
//         item_ss >> value;
//     }
//
//     template<>
//     static void deserialize<std::string>(std::stringstream& ss, std::string& value) {
//         if (!std::getline(ss, value)) {
//             throw std::runtime_error("Deserialization error");
//         }
//     }
// };
//
// // ==========================================
// // 2. function_traits (修复版)
// // ==========================================
// template<typename T>
// struct function_traits;
//
// // 特化 1：匹配普通函数指针 R(Args...)
// template<typename R, typename... Args>
// struct function_traits<R(Args...)> {
//     static constexpr size_t arity = sizeof...(Args);
//     using result_type = R;
//     using arguments = std::tuple<Args...>;
// };
//
// // 特化 2：匹配函数指针类型 R(*)(Args...)
// template<typename R, typename... Args>
// struct function_traits<R(*)(Args...)> : public function_traits<R(Args...)> {};
//
// // 特化 3：匹配 Lambda 的 operator()
// template<typename ClassType, typename R, typename... Args>
// struct function_traits<R (ClassType::*)(Args...) const> {
//     static constexpr size_t arity = sizeof...(Args);
//     using result_type = R;
//     using arguments = std::tuple<Args...>;
// };
//
// // 特化 4：通用回退（只对具有 operator() 的类型生效）
// template<typename T>
// struct function_traits : public function_traits<decltype(&T::operator())> {};
//
// // ==========================================
// // 3. RpcServer
// // ==========================================
// class RpcServer {
// public:
//     template<typename Func>
//     void register_service(const std::string& name, Func func) {
//         using Traits = function_traits<Func>;
//         constexpr size_t arg_count = Traits::arity;
//
//         std::function<std::string(const std::vector<std::string>&)> wrapper =
//             [this, func, arg_count](const std::vector<std::string>& args_str) -> std::string {
//                 return invoke_with_args(func, args_str, std::make_index_sequence<arg_count>{});
//             };
//
//         services_[name] = wrapper;
//     }
//
//     template<typename T, typename... Args>
//     typename std::enable_if<!std::is_void<T>::value, T>::type
//     call(const std::string& rpc_name, Args&&... args) {
//         std::stringstream ss_in;
//         (Serializer::serialize(ss_in, args), ...);
//
//         std::string result_str = perform_rpc(rpc_name, ss_in.str());
//
//         T result;
//         std::stringstream ss_out(result_str);
//         Serializer::deserialize(ss_out, result);
//
//         return result;
//     }
//
// private:
//     std::map<std::string, std::function<std::string(const std::vector<std::string>&)>> services_;
//
//     template<typename Func, size_t... Is>
//     std::string invoke_with_args(Func func, const std::vector<std::string>& args_str, std::index_sequence<Is...>) {
//         try {
//             using Traits = function_traits<Func>;
//             using ArgsTuple = typename Traits::arguments;
//
//             auto args_tuple = convert_to_tuple<ArgsTuple>(args_str, std::index_sequence<Is...>{});
//
//             auto result = std::apply(func, args_tuple);
//
//             std::stringstream ss;
//             Serializer::serialize(ss, result);
//             return ss.str();
//         } catch (const std::exception& e) {
//             return "RPC Error: " + std::string(e.what());
//         }
//     }
//
//     template<typename Tuple, size_t... Is>
//     Tuple convert_to_tuple(const std::vector<std::string>& args_str, std::index_sequence<Is...>) {
//         if (args_str.size() != sizeof...(Is)) {
//             throw std::runtime_error("Argument count mismatch");
//         }
//         return Tuple{ convert_single<std::tuple_element_t<Is, Tuple>>(args_str[Is])... };
//     }
//
//     template<typename T>
//     T convert_single(const std::string& str) {
//         T val;
//         std::stringstream ss(str);
//         Serializer::deserialize(ss, val);
//         return val;
//     }
//
//     std::string perform_rpc(const std::string& name, const std::string& payload) {
//         std::vector<std::string> args_vec;
//         std::stringstream ss(payload);
//         std::string item;
//         while (std::getline(ss, item, '\n')) {
//             if(!item.empty()) args_vec.push_back(item);
//         }
//
//         auto it = services_.find(name);
//         if (it != services_.end()) {
//             return it->second(args_vec);
//         } else {
//             return "Error: Service not found";
//         }
//     }
// };
//
// // ==========================================
// // 4. 测试用例
// // ==========================================
// int add(int a, int b) {
//     return a + b;
// }
//
// int main() {
//     RpcServer server;
//
//     // 测试普通函数指针
//     server.register_service("add", add);
//
//     // 测试 Lambda
//     server.register_service("calc", [](double x, double y) {
//         return x * y;
//     });
//
//     server.register_service("test_str", [](std::string& str)->std::string {
//        return str;
//    });
//
//     int res1 = server.call<int>("add", 10, 20);
//     std::cout << "Result add: " << res1 << std::endl;
//
//     double res2 = server.call<double>("calc", 2.5, 4.0);
//     std::cout << "Result calc: " << res2 << std::endl;
//
//     std::string name = server.call<std::string>("test_str", "liu xiao rambo");
//     std::cout << "Result test_str: " << name << std::endl;
//
//     return 0;
// }



// 测试2
// #include <iostream>
// #include <string>
// #include <functional>
// #include <vector>
// #include <map>
// #include <tuple>
// #include <sstream>
// #include <type_traits>
// #include <utility>
//
// // ==========================================
// // 1. 自定义对象定义 (User)
// // ==========================================
// struct User {
//     int id;
//     std::string name;
//     int age;
//
//     // 【必须】默认构造函数，反序列化时需要
//     User() : id(0), age(0) {}
//
//     User(int i, std::string n, int a) : id(i), name(std::move(n)), age(a) {}
// };
//
// // ==========================================
// // 2. 重载输入输出运算符 (支持 User)
// // ==========================================
//
// // 格式：id age "name with spaces"
// std::ostream& operator<<(std::ostream& os, const User& user) {
//     os << user.id << " " << user.age << " \"" << user.name << "\"";
//     return os;
// }
//
// std::istream& operator>>(std::istream& is, User& user) {
//     char quote;
//     // 读取 id, age 和左引号
//     if (is >> user.id >> user.age >> quote) {
//         // 读取直到右引号的内容
//         std::getline(is, user.name, '"');
//     }
//     return is;
// }
//
// // ==========================================
// // 3. 序列化工具
// // ==========================================
// struct Serializer {
//     // 通用序列化
//     template<typename T>
//     static void serialize(std::stringstream& ss, const T& value) {
//         ss << value << "\n";
//     }
//
//     // 通用反序列化
//     template<typename T>
//     static void deserialize(std::stringstream& ss, T& value) {
//         std::string line;
//         if (!std::getline(ss, line)) {
//             throw std::runtime_error("Deserialization error: unexpected end of stream");
//         }
//         std::istringstream item_ss(line);
//         item_ss >> value;
//     }
//
//     // 【修复】特化 std::string：去掉 static 关键字
//     template<>
//     void deserialize<std::string>(std::stringstream& ss, std::string& value) {
//         if (!std::getline(ss, value)) {
//             throw std::runtime_error("Deserialization error: string");
//         }
//     }
// };
//
// // ==========================================
// // 4. function_traits
// // ==========================================
// template<typename T>
// struct function_traits;
//
// template<typename R, typename... Args>
// struct function_traits<R(Args...)> {
//     static constexpr size_t arity = sizeof...(Args);
//     using result_type = R;
//     using arguments = std::tuple<Args...>;
// };
//
// template<typename R, typename... Args>
// struct function_traits<R(*)(Args...)> : public function_traits<R(Args...)> {};
//
// template<typename ClassType, typename R, typename... Args>
// struct function_traits<R (ClassType::*)(Args...) const> {
//     static constexpr size_t arity = sizeof...(Args);
//     using result_type = R;
//     using arguments = std::tuple<Args...>;
// };
//
// template<typename T>
// struct function_traits : public function_traits<decltype(&T::operator())> {};
//
// // ==========================================
// // 5. RpcServer
// // ==========================================
// class RpcServer {
// public:
//     template<typename Func>
//     void register_service(const std::string& name, Func func) {
//         using Traits = function_traits<Func>;
//         constexpr size_t arg_count = Traits::arity;
//
//         std::function<std::string(const std::vector<std::string>&)> wrapper =
//             [this, func, arg_count](const std::vector<std::string>& args_str) -> std::string {
//                 return invoke_with_args(func, args_str, std::make_index_sequence<arg_count>{});
//             };
//
//         services_[name] = wrapper;
//     }
//
//     template<typename T, typename... Args>
//     typename std::enable_if<!std::is_void<T>::value, T>::type
//     call(const std::string& rpc_name, Args&&... args) {
//         std::stringstream ss_in;
//         // 序列化所有参数
//         (Serializer::serialize(ss_in, args), ...);
//
//         std::string result_str = perform_rpc(rpc_name, ss_in.str());
//
//         T result;
//         std::stringstream ss_out(result_str);
//         Serializer::deserialize(ss_out, result);
//
//         return result;
//     }
//
// private:
//     std::map<std::string, std::function<std::string(const std::vector<std::string>&)>> services_;
//
//     template<typename Func, size_t... Is>
//     std::string invoke_with_args(Func func, const std::vector<std::string>& args_str, std::index_sequence<Is...>) {
//         try {
//             using Traits = function_traits<Func>;
//             using ArgsTuple = typename Traits::arguments;
//
//             auto args_tuple = convert_to_tuple<ArgsTuple>(args_str, std::index_sequence<Is...>{});
//
//             auto result = std::apply(func, args_tuple);
//
//             std::stringstream ss;
//             Serializer::serialize(ss, result);
//             return ss.str();
//         } catch (const std::exception& e) {
//             return "RPC Error: " + std::string(e.what());
//         }
//     }
//
//     template<typename Tuple, size_t... Is>
//     Tuple convert_to_tuple(const std::vector<std::string>& args_str, std::index_sequence<Is...>) {
//         if (args_str.size() != sizeof...(Is)) {
//             throw std::runtime_error("Argument count mismatch");
//         }
//         auto result = std::make_tuple(args_str[Is]...);
//         return Tuple{ convert_single<std::tuple_element_t<Is, Tuple>>(args_str[Is])... };
//     }
//
//     template<typename T>
//     T convert_single(const std::string& str) {
//         using ValueType = std::decay_t<T>;
//         ValueType val;
//         std::stringstream ss(str);
//         Serializer::deserialize(ss, val);
//         return val;
//     }
//
//     std::string perform_rpc(const std::string& name, const std::string& payload) {
//         std::vector<std::string> args_vec;
//         std::stringstream ss(payload);
//         std::string item;
//         while (std::getline(ss, item, '\n')) {
//             if(!item.empty()) args_vec.push_back(item);
//         }
//
//         auto it = services_.find(name);
//         if (it != services_.end()) {
//             return it->second(args_vec);
//         } else {
//             return "Error: Service not found";
//         }
//     }
// };
//
// // ==========================================
// // 6. 测试用例
// // ==========================================
// int main() {
//     RpcServer server;
//
//     // 测试 1：对象输入，对象输出
//     server.register_service("update_user", [](User u) -> User {
//         u.age += 1;
//         u.name = "Super " + u.name;
//         return u;
//     });
//
//     // 测试 2：基本类型输入，对象输出
//     server.register_service("get_user", [](int id) -> User {
//         return User(id, "Alice", 20);
//     });
//
//     // 测试 2：基本类型输入，对象输出
//     server.register_service("add", [](int a,int b) -> int {
//         return a + b;
//     });
//
//     // 测试 2：基本类型输入，对象输出
//     server.register_service("test_str", [](std::string& name) -> std::string {
//         return name;
//     });
//
//     std::string str_test = server.call<std::string>("test_str", "rambo liu");
//     std::cout << "str_test = " << str_test << std::endl;
//
//     int add_res = server.call<int>("add", 11, 2);
//     std::cout << "test add(11 + 2) = " << add_res << std::endl;
//
//
//
//
//     // std::function<std::string(std::string&)> func =
//     //     [](std::string& name) -> std::string {
//     //         return name;
//     // };
//     //
//     // // 2. 测试调用
//     // std::string my_name = "test";
//     // std::string result = func(my_name);
//     //
//     // std::cout << "563 ------= " << result << std::endl;
//
//     // --- 执行测试 ---
//
//     // 测试对象传输
//     User input_user(101, "Bob", 25);
//     User output_user = server.call<User>("update_user", input_user);
//
//     std::cout << "Input: ID=" << input_user.id
//               << ", Name=" << input_user.name
//               << ", Age=" << input_user.age << std::endl;
//
//     std::cout << "Output: ID=" << output_user.id
//               << ", Name=" << output_user.name
//               << ", Age=" << output_user.age << std::endl;
//
//     // 测试返回对象
//     User new_user = server.call<User>("get_user", 999);
//     std::cout << "New User: " << new_user.name << " (Age: " << new_user.age << ")" << std::endl;
//
//     return 0;
// }



#include <iostream>
#include <string>
#include <functional>
#include <vector>
#include <map>
#include <tuple>
#include <sstream>
#include <type_traits>
#include <utility>

// ==========================================
// 1. 自定义对象定义 (User)
// ==========================================
struct User {
    int id;
    std::string name;
    int age;

    User() : id(0), age(0) {}
    User(int i, std::string n, int a) : id(i), name(std::move(n)), age(a) {}
};

// ==========================================
// 2. 重载输入输出运算符 (User)
// ==========================================
std::ostream& operator<<(std::ostream& os, const User& user) {
    os << user.id << " " << user.age << " \"" << user.name << "\"";
    return os;
}

std::istream& operator>>(std::istream& is, User& user) {
    char quote;
    if (is >> user.id >> user.age >> quote) {
        std::getline(is, user.name, '"');
    }
    return is;
}

// ==========================================
// 3. 序列化工具 (核心修复版)
// ==========================================
struct Serializer {
    // --- 修复点 1：通用序列化 (处理 int, User 等) ---
    template<typename T>
    static void serialize(std::stringstream& ss, const T& value) {
        ss << value << "\n";
    }

    // --- 修复点 2：特化 std::string 的序列化 ---
    // 无论是 string, const string&, 还是 string&，都会匹配到这个
    // 强制写入字符串内容，而不是依赖 operator<<
    static void serialize(std::stringstream& ss, const std::string& value) {
        ss << value << "\n";
    }

    // 通用反序列化
    template<typename T>
    static void deserialize(std::stringstream& ss, T& value) {
        std::string line;
        if (!std::getline(ss, line)) {
            throw std::runtime_error("Deserialization error");
        }
        std::istringstream item_ss(line);
        item_ss >> value;
    }

    // 特化：std::string 反序列化
    static void deserialize(std::stringstream& ss, std::string& value) {
        if (!std::getline(ss, value)) {
            value = "";
        }
    }
};

// ==========================================
// 4. function_traits
// ==========================================
template<typename T>
struct function_traits;

template<typename R, typename... Args>
struct function_traits<R(Args...)> {
    static constexpr size_t arity = sizeof...(Args);
    using result_type = R;
    using arguments = std::tuple<Args...>;
};

template<typename R, typename... Args>
struct function_traits<R(*)(Args...)> : public function_traits<R(Args...)> {};

template<typename ClassType, typename R, typename... Args>
struct function_traits<R (ClassType::*)(Args...) const> {
    static constexpr size_t arity = sizeof...(Args);
    using result_type = R;
    using arguments = std::tuple<Args...>;
};

template<typename T>
struct function_traits : public function_traits<decltype(&T::operator())> {};

// ==========================================
// 5. RpcServer
// ==========================================
class RpcServer {
public:
    template<typename Func>
    void register_service(const std::string& name, Func func) {
        using Traits = function_traits<Func>;
        constexpr size_t arg_count = Traits::arity;

        std::function<std::string(const std::vector<std::string>&)> wrapper =
            [this, func, arg_count](const std::vector<std::string>& args_str) -> std::string {
                return invoke_with_args(func, args_str, std::make_index_sequence<arg_count>{});
            };

        services_[name] = wrapper;
    }

    template<typename T, typename... Args>
    typename std::enable_if<!std::is_void<T>::value, T>::type
    call(const std::string& rpc_name, Args&&... args) {
        std::stringstream ss_in;
        // 这里调用 serialize 时，如果是 string&，现在会匹配到特化版本
        (Serializer::serialize(ss_in, args), ...);

        std::string result_str = perform_rpc(rpc_name, ss_in.str());

        T result;
        std::stringstream ss_out(result_str);
        Serializer::deserialize(ss_out, result);

        return result;
    }

private:
    std::map<std::string, std::function<std::string(const std::vector<std::string>&)>> services_;

    template<typename Func, size_t... Is>
    std::string invoke_with_args(Func func, const std::vector<std::string>& args_str, std::index_sequence<Is...>) {
        try {
            using Traits = function_traits<Func>;
            using ArgsTuple = typename Traits::arguments;

            auto args_tuple = convert_to_tuple<ArgsTuple>(args_str, std::index_sequence<Is...>{});

            auto result = std::apply(func, args_tuple);

            std::stringstream ss;
            Serializer::serialize(ss, result);
            return ss.str();
        } catch (const std::exception& e) {
            return "RPC Error: " + std::string(e.what());
        }
    }

    // 解决方案一
//     template<typename Func, size_t... Is>
// std::string invoke_with_args(Func func, const std::vector<std::string>& args_str, std::index_sequence<Is...>) {
//         try {
//             using Traits = function_traits<Func>;
//             using ArgsTuple = typename Traits::arguments;
//
//             // --- 修改开始 ---
//
//             // 1. 创建一个 tuple 来存储转换后的参数值（不是引用！）
//             // 这样避免了引用绑定的生命周期问题
//             auto args_tuple = std::make_tuple(
//                 // 针对每个参数进行转换
//                 [&args_str]<size_t I>() {
//                     using ArgType = std::tuple_element_t<I, ArgsTuple>;
//
//                     if constexpr (std::is_same_v<std::decay_t<ArgType>, std::string>) {
//                          // 如果是 string 或 string&，直接从 vector 取值
//                          return static_cast<std::decay_t<ArgType>>(args_str[I]);
//                     } else {
//                          // 其他类型 (int, double...)
//                          std::decay_t<ArgType> val;
//                          std::stringstream ss(args_str[I]);
//                          ss >> val;
//                          return val;
//                     }
//                 }.template operator()<Is>()...
//             );
//
//             // 2. 调用函数
//             // 注意：这里我们传递的是 args_tuple 中的值。
//             // 如果 func 参数是 string&，std::apply 会自动将 tuple 中的 string 绑定给引用
//             // 只要 args_tuple 在这里没被销毁，引用就是安全的！
//             auto result = std::apply(func, args_tuple);
//
//             // --- 修改结束 ---
//
//             std::stringstream ss;
//             Serializer::serialize(ss, result);
//             return ss.str();
//         } catch (...) {
//             return "Error: Exception in function call";
//         }
//     }

    // 这种方式时会导致  测试一 测试字符串引用 传递  输出为空。原因：
    // convert_single的返回值为一个std::string 临时值类型 ValueType
    // 结果 变成 std::string&（即为convert_single 前面的返回类型T） 引用了一个临时值类型，convert_single 执行完时
    // 临时值val 就会释放，导致std::string& 引用悬空了
    // 简单的测试demo

    // #include <iostream>
    // using namespace std;
    //
    // std::string& test2(std::string& name) {
    //     std::string s = name;
    //     return s;
    // }
    //
    // template<typename T>
    // T convert_single(const std::string& str) {
    //     std::string val;  // 👈 这里创建了一个【局部临时字符串】
    //     val = str;
    //     return val;     // 👈 你返回了【局部临时变量的引用】
    // }
    //
    // int main() {
    //     std::string name = "liu";
    //     std::string& s1 = convert_single<std::string& >(name);
    //     printf("49----%s\n", s1.c_str());
    //
    //     std::string& s2 = test2(name);
    //     printf("52---%s\n", s1.c_str());
    //     return 0;
    // }


    /**
     * @tparam Tuple
     * @tparam Is
     * @param args_str
     * @return
     */
    template<typename Tuple, size_t... Is>
    Tuple convert_to_tuple(const std::vector<std::string>& args_str, std::index_sequence<Is...>) {
        if (args_str.size() != sizeof...(Is)) {
            throw std::runtime_error("Argument count mismatch");
        }
        return Tuple{ convert_single<std::tuple_element_t<Is, Tuple>>(args_str[Is])... };
    }


    // 解决方案二
    // template<typename Tuple, size_t... Is>
    // auto convert_to_tuple(
    // const std::vector<std::string>& args_str,
    // std::index_sequence<Is...>)
    // {
    //     return std::make_tuple(
    //         convert_single<
    //             std::decay_t<std::tuple_element_t<Is, Tuple>>
    //         >(args_str[Is])...
    //     );
    // }

    template<typename T>
    T convert_single(const std::string& str) {
        using ValueType = std::decay_t<T>;
        ValueType val;
        std::stringstream ss(str);
        Serializer::deserialize(ss, val);
        return val;
    }

    std::string perform_rpc(const std::string& name, const std::string& payload) {
        std::vector<std::string> args_vec;
        std::stringstream ss(payload);
        std::string item;
        while (std::getline(ss, item, '\n')) {
            if(!item.empty()) args_vec.push_back(item);
        }

        auto it = services_.find(name);
        if (it != services_.end()) {
            return it->second(args_vec);
        } else {
            return "Error: Service not found";
        }
    }
};



// // ==========================================
// // 6. 测试用例
// // ==========================================
int main() {

    RpcServer server;

    // 测试一 测试字符串引用 传递
    server.register_service("test_str", [](std::string& name) -> std::string {
        return name;
    });

    std::string str_test = server.call<std::string>("test_str", "rambo liu");
    std::cout << "String Test Result: [" << str_test << "]" << std::endl;


    server.register_service("add", [](int a,int b) -> int {

        return a + b;
    });

    std::string add_test = server.call<std::string>("add", 1,22);
    std::cout << "add test ( 1 + 22) ="   << add_test  << std::endl;


    // 测试：User 对象
    server.register_service("update_user", [](User u) -> User {
        u.age += 1;
        return u;
    });

    User input_user(101, "Bob", 25);
    User output_user = server.call<User>("update_user", input_user);
    std::cout << "User Test Result: ID=" << output_user.id
              << ", Name=" << output_user.name
              << ", Age=" << output_user.age << std::endl;

    return 0;
}



// 针对上述出现 的引用悬空问题 测试例子
#include <iostream>
#include <string>

// 模拟出错的函数
// template<typename T>
// T bad_convert(const std::string& str) {
//     using ValueType = std::decay_t<T>;
//     ValueType val = str; // 模拟反序列化赋值
//     std::cout << "Inside func, val = " << val << std::endl;
//     return val; // 返回局部变量的引用
// }

// 针对引用类型的偏特化或重载
// template<typename T>
// std::decay_t<T> bad_convert(const std::string& str) {
//     // 强制返回值类型为非引用，利用拷贝构造或移动构造
//     using ValueType = std::decay_t<T>;
//     ValueType val;
//     std::stringstream ss(str);
//     Serializer::deserialize(ss, val);
//     return val; // 安全：返回副本
// }
//
// void test_func(std::string& param) {
//     std::cout << "In test_func, param = [" << param << "]" << std::endl;
// }
//
// int main() {
//     std::string input = "rambo liu";
//     // 这里触发了悬空引用
//     auto result = bad_convert<std::string&>(input);
//     // result 此时已经是悬空引用，行为未定义
//     // 但在很多编译器下，这里会表现为空
//     test_func(result);
//     return 0;
// }