//
// Created by Rambo.Liu on 2024/12/11.
//
#include <iostream>
#include <tuple>
#include <type_traits>
#include "msgpack.hpp"

// 模版参数遍历  参考 http://www.zh0ngtian.tech/posts/18ba8a53.html

/****-----------------std:make_index_sequence  遍历std:tuple--------------------*/
template <typename Tuple, typename Func, size_t ... N>
void func_call_tuple(const Tuple& t, Func&& func, std::index_sequence<N...>) {
    static_cast<void>(std::initializer_list<int>{(func(std::get<N>(t)), 0)...});
}

template <typename ... Args, typename Func>
void travel_tuple(const std::tuple<Args...>& t, Func&& func) {
    func_call_tuple(t, std::forward<Func>(func), std::make_index_sequence<sizeof...(Args)>{});
}
/****-----------------std:make_index_sequence  遍历std:tuple--------------------*/


/**------------------ 递归写法------------------*/

template<size_t I = 0, typename FuncT, typename... Tp>
inline typename std::enable_if_t<I == sizeof...(Tp)>  // 当tp 个数与元组个数 相等时
        expand(std::tuple<Tp...> &t, FuncT f) {
    printf("递归完了  我要退出了-------I = %ld\n", I);

}

template<size_t I = 0, typename FuncT, typename... Tp>
inline typename std::enable_if_t<I < sizeof...(Tp)>
        expand(std::tuple<Tp...> &t, FuncT f) {
    printf("-------I = %ld\n", I);
    f(std::get<I>(t));
    expand<I + 1, FuncT, Tp...>(t, f);
}
/**------------------ 递归写法------------------*/




template <typename T, typename... Args>
typename std::enable_if<!std::is_void<T>::value, T>::type  //返回值空时  使用此方法
call(const std::string &rpc_name, Args &&...args) {
    printf("函数返回值不为空时  name = %s\n",rpc_name.c_str());

    //待补知识点  遍历可变参数模版中的  参数列表
    std::cout << "方法一  使用std:tuple" <<std::endl;
    auto args_tup = std::forward_as_tuple(args...);
    travel_tuple(args_tup, [](auto&& item) {
        std::cout << item << ",";
    });
    std::cout << std::endl;

    std::cout << "方法二  使用递归" <<std::endl;
    auto args_tup2 = std::forward_as_tuple(args...);
    expand(args_tup2, [](auto arg) {
        std::cout << typeid(arg).name() <<"  "<< arg << std::endl;
    });



    return 100;
}

template <typename T, typename... Args>
typename std::enable_if<std::is_void<T>::value, T>::type  //返回值空时  使用此方法
call2(const std::string &rpc_name, Args &&...args) {
    printf("函数返回值为空时  name = %s\n",rpc_name.c_str());

    //待补知识点  遍历可变参数模版中的  参数列表
    std::cout << "方法一  使用std:tuple" <<std::endl;
    auto args_tup = std::forward_as_tuple(args...);
    travel_tuple(args_tup, [](auto&& item) {
        std::cout << item << ",";
    });
    std::cout << std::endl;

    std::cout << "方法二  使用递归" <<std::endl;
    auto args_tup2 = std::forward_as_tuple(args...);
    expand(args_tup2, [args_tup2](auto arg) {
        std::cout << typeid(arg).name() <<"  "<< arg << std::endl;
    });

}

/**------------------测试msgpack------------------*/
struct Person {
    std::string name;
    uint16_t age;
    std::vector<std::string> aliases;

    template<class T>
    void pack(T &pack) {
        pack(name, age, aliases);
    }
};

void testMsgPack() {

    auto person = Person{"John", 22, {"Ripper", "Silverhand"}};
    auto data = msgpack::pack(person); // Pack your object
    auto john = msgpack::unpack<Person>(data.data(),sizeof(data));
}
/**------------------测试msgpack------------------*/


class TestOp {

public:
    template<class ... Types>
    void operator()(const Types &... args) {

        //遍历形参
        auto args_tup = std::forward_as_tuple(args...);
        travel_tuple(args_tup, [](auto&& item) {
            std::cout << item << ",";
        });
        std::cout << std::endl;

        //转发 参数
        (test(std::forward<const Types &>(args)), ...);
    }

    //std::enable_if 写法一
    template<class T>
    void test(const T &value) {
        if constexpr (std::is_integral<T>::value) {
            testInt(value);
        } else {
            testStr((std::string &) value);
        }
    }
    template <typename T, typename = typename std::enable_if<!std::is_integral<T>::value>::type>
    void testStr(T & str) {
        printf("120-------这是字符串api str = %s\n",str.c_str());
    }

    //使用std::enable_if 校验模板参数 类型
    template <typename T, typename = typename std::enable_if<std::is_integral<T>::value>::type>
    void testInt(T& info) {
        printf("124-------这是整型api info = %d\n",info);
    }


};


template <typename T>
T CustomDiv(T lhs, T rhs) {
    // Custom Div的实现
    printf("178-----lhs = %f  rhs = %f",lhs,rhs);
}

template <typename T, bool IsFloat = std::is_floating_point<T>::value>
struct SafeDivide {
    static T Do(T lhs, T rhs) {
        printf("183-----lhs = %f  rhs = %f",lhs,rhs);
        return CustomDiv(lhs, rhs);
    }
};

template <typename T>
struct SafeDivide<T, true>{     // 偏特化A
    static T Do(T lhs, T rhs){
        printf("187-----lhs = %f  rhs = %f",lhs,rhs);
        return lhs/rhs;
    }
};

template <typename T>
struct SafeDivide<T, false>{   // 偏特化B
    static T Do(T lhs, T rhs){
        printf("194-----lhs = %f  rhs = %f",lhs,rhs);
        return lhs;
    }
};



int main() {

//    int ret = call<int,int>("add",23,34,45,56,222);
//    testMsgPack();



//    auto testOp = TestOp{};
//    testOp(23,"liu",3434);


    SafeDivide<float>::Do(1.0f, 2.0f);    // 调用偏特化A
    //SafeDivide<int>::Do(1, 2);



    return 0;
}