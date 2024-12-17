//
// Created by Rambo.Liu on 2024/12/3.
// 本类主要介绍 function_traits 的概念、实现原理、应用场景
// 参考 https://blog.csdn.net/iShare_Carlos/article/details/140808760
/*
 * 参考 https://www.cnblogs.com/yaoyu126/p/12427845.html
 * function_traits用来获取所有函数语义类型的信息，
 *      (函数类型、返回类型、参数个数和参数的具体类型)。
 *
 * 函数语义类型包括
 *  普通函数
 *  函数指针
 *  function/lambda
 *  成员函数
 *  函数对象
 *
 * 实现function_traits的关键技术
 *  要通过模板特化和可变参数模板来获取函数类型和返回类型。
 *  先定义一个基本的function_traits的模板类：
 *     template<typename T>
 *     struct function_traits;
 *  再通过特化，将返回类型和可变参数模板作为模板参数，就可以获取函数类型、函数返回值和参数的个数了。
 *
 * 如:
 *  int func(int a, string b);
 * ## 获取函数类型
 *      function_traits<decltype(func)>::function_type; // int __cdecl(int, string)
 * # 获取函数返回值
 *      function_traits<decltype(func)>::return_type;   // int
 * # 获取函数的参数个数
 *      function_traits<decltype(func)>::arity;         // 2
 * # 获取函数第一个入参类型
 *      function_traits<decltype(func)>::args<0>::type; // int
 * # 获取函数第二个入参类型
 *      function_traits<decltype(func)>::args<1>::type; // string
 */

#include <iostream>
#include <tuple>
#include <type_traits>


//定义基本模板
template<typename T>
struct function_traits;

//// 特化用于普通函数类型
template<typename R, typename... Args>
struct function_traits<R(Args...)> {
    using result_type = R;
    static const std::size_t arity = sizeof...(Args);

    template<std::size_t N>
    struct arg {
        static_assert(N < arity, "参数索引超出范围");
        using type = typename std::tuple_element<N, std::tuple<Args...>>::type;
    };

    using tuple_type = std::tuple<std::remove_cv_t<std::remove_reference_t<Args>>...>;
    using bare_tuple_type = std::tuple<std::remove_const_t<std::remove_reference_t<Args>>...>;
};
// 普通函数
//template<typename ReturnType, typename... Args>
//struct function_traits<ReturnType(Args...)>
//{
//    enum { arity = sizeof...(Args) };
//    using return_type = ReturnType;
//    using function_type = ReturnType(Args...);
//    using stl_function_type = std::function<function_type>;
//    using pointer = ReturnType(*)(Args...);
//
//    template<size_t I>
//    struct args
//    {
//        static_assert(I < arity, "index is out of range, index must less than sizeof Args");
//        using type = typename std::tuple_element<I, std::tuple<Args...>>::type;
//    };
//
//    using tuple_type = std::tuple<std::remove_cv_t<std::remove_reference_t<Args>>...>;
//    using bare_tuple_type = std::tuple<std::remove_const_t<std::remove_reference_t<Args>>...>;
//};

//普通函数
//template <typename Ret> struct function_traits<Ret()> {
//public:
//    enum { arity = 0 };
//    typedef Ret function_type();
//    typedef Ret return_type;
//    using stl_function_type = std::function<function_type>;
//    typedef Ret (*pointer)();
//
//    typedef std::tuple<> tuple_type;
//    typedef std::tuple<> bare_tuple_type;
//    using args_tuple = std::tuple<std::string>;
//    using args_tuple_2nd = std::tuple<std::string>;
//};

// 特化用于普通函数指针
template<typename R, typename... Args>
struct function_traits<R(*)(Args...)> : function_traits<R(Args...)> {
};

// 特化用于成员函数指针
template<typename C, typename R, typename... Args>
struct function_traits<R(C::*)(Args...)> : function_traits<R(Args...)> {
};

// 特化用于 const 成员函数指针
template<typename C, typename R, typename... Args>
struct function_traits<R(C::*)(Args...) const> : function_traits<R(Args...)> {
};


// 特化用于 Lambda 表达式和函数对象
template<typename T>
struct function_traits : function_traits<decltype(&T::operator())> {
};



/**----------------------通用函数调用器--------------------------*/


/**
 * 调用成员函数
 * @tparam F       成员函数
 * @tparam Self    实例对象
 * @tparam Indexes 参数的索引
 * @tparam Args    参数列表
 * @param f        成员函数地址
 * @param self     实例对象
 * @param tup      成员函数参数元组
 */
template<typename F, typename Self, size_t... Indexes, typename... Args>
static auto call_member_helper(const F &f, Self *self,
                               std::index_sequence<Indexes...>,
                               std::tuple<Args...> tup) {
    std::cout << "Indexes 0 = " << std::get<0>(tup) << " 1= " << std::get<1>(tup) << std::endl;
    return (*self->*f)(std::move(std::get<Indexes>(tup))...);
}

/**
 * 调用成员静态函数
 * @tparam F       成员函数
 * @tparam Self    实例对象
 * @tparam Indexes 参数的索引
 * @tparam Args    参数列表
 * @param f        成员函数地址
 * @param self     实例对象
 * @param tup      成员函数参数元组
 */
template<typename F, typename Self, size_t... Indexes, typename... Args>

void call_static_member_helper(const F &f,
                               Self self,
                               std::index_sequence<Indexes...>,
                               std::tuple<Args...> tup) {

    if (!std::is_member_function_pointer<decltype(f)>::value) {
        printf("是成员函数-----\n");
    } else {
        printf("不是成员函数-----\n");
    }

    (*f)(std::move(std::get<Indexes>(tup))...);
}


template<typename F, typename... Args>
auto invoke_function(F f, Args... args) -> typename function_traits<F>::result_type {
    static_assert(sizeof...(Args) == function_traits<F>::arity, "参数个数不匹配");
    return f(std::forward<Args>(args)...);
}

// 示例函数
int add(int a, int b) {
    return a + b;
}

std::string concatenate(const std::string &a, const std::string &b) {
    return a + b;
}

static int plus(int a, int b) {
    return a + b;
}

struct person {
    int id;
    std::string name;
    int age;

};

person get_person() { return {1, "tom", 20}; }

class MemFunc {
public:
    person* p;

    MemFunc() {

    }

    float mem_divide(int a, float b, std::string c) {
        printf("测试成员函数 this is 3 args a = %d  b= %f  c= %s\n", a, b, c.c_str());
        return 2.0f;
    }

    void mem_fun(const int a, int b) {
        printf("测试成员函数-----a = %d  b = %d \n", a, b);
    }

    static void static_fun(const int a, int b) {
        printf("测试成员静态函数-----a = %d  b = %d \n", a, b);
    }

    person* getP(int age,std::string name) {
        printf("获取成员函数中的返回值 age = %d  name = %s\n",age,name.c_str());
        if (!p) {
            p = new person();
        }
        p->age = age;
        p->name = name;
        return p;
    }
};



void testGenFunc() {
    std::cout << "测试通用函数调用-------------:" << std::endl;
    std::cout << "测试普通函数 Add: " << invoke_function(add, 3, 4) << std::endl; // 输出: Add: 7
    std::cout << "测试普通函数 Concatenate: " << invoke_function(concatenate, "Hello, ", "World!")
              << std::endl; // 输出: Concatenate: Hello, World!
    std::cout << "测试普通静态函数 plus: " << invoke_function(plus, 2, 4)
              << std::endl; // 输出: Concatenate: Hello, World!

     auto p = invoke_function(get_person);
    std::cout << "测试普通获取对象  p.name: " << p.name << " p.age = "<< p.age << std::endl;



    //函数指针
    int (*addPrt)(int, int) = add;
    std::cout << "测试函数指针 : " << invoke_function(addPrt, 30, 4) << std::endl; // 输出: Add: 7


    // 使用 Lambda 表达式
    auto lambda = [](int x, int y) -> int {
        return x * y;
    };
    std::cout << "测试 Lambda: " << invoke_function(lambda, 5, 6) << std::endl;

    printf("使用function_traits  封装成员函数调用器--------\n");
    MemFunc *memFunc = new MemFunc();
    std::tuple<int, float, std::string> argsTuple = std::make_tuple(2, 3.0f, "123");
    call_member_helper(&MemFunc::mem_divide, &memFunc,
                       std::make_index_sequence<3>{},
                       argsTuple);

    std::tuple<int, int> argsTuple2 = std::make_tuple(1, 10);
    call_member_helper(&MemFunc::mem_fun, &memFunc,
                       std::make_index_sequence<2>{},
                       argsTuple2);


    std::tuple<int, int> argsTuple3 = std::make_tuple(10, 100);
    call_static_member_helper(&MemFunc::static_fun, &memFunc,
                              std::make_index_sequence<2>{},
                              argsTuple3);

    std::tuple<int, std::string> argsTuple4 = std::make_tuple(10, "liu");
    auto p2 = call_member_helper(&MemFunc::getP, &memFunc,
                              std::make_index_sequence<2>{},
                              argsTuple4);
    std::cout << "测试类成员函数获取对象  p.name: " << p2->name << " p.age = "<< p2->age << std::endl;

}

/**----------------------通用函数调用器--------------------------*/




/**----------------------测试回调函数--------------------------*/
template<typename F>
void check_callback(F f) {
    using traits = function_traits<F>;
    static_assert(traits::arity == 2, "回调函数必须有两个参数");
    static_assert(std::is_same<typename traits::template arg<0>::type, int>::value, "第一个参数必须是int类型");
    static_assert(std::is_same<typename traits::template arg<1>::type, std::string>::value,
                  "第二个参数必须是std::string类型");

    std::cout << "回调函数类型检查通过\n";
}

void callback(int a, std::string b) {
    std::cout << "回调函数被调用: " << a << ", " << b << '\n';
}
/**----------------------测试回调函数--------------------------*/


/**----------------------测试function_traits 用于普通函数-------------------------*/
void testFunTraitsNormal() {
    using traits = function_traits<void(int, double)>;
    if (std::is_same<traits::result_type, void>::value) {
        printf("函数返回类型 = void\n");
    } else {
        printf("函数返回类型 != void\n");
    }

    if (std::is_same<traits::arg<0>::type, int>::value) {
        printf("第一参数类型 = int\n");
    } else {
        printf("第一参数类型 != int\n");
    }

//    static_assert(std::is_same<traits::result_type, void>, "错误");
//    static_assert(std::is_same<traits::arg<0>::type, int>, "错误");
//    static_assert(traits::arity == 2, "错误");
}
/**----------------------测试function_traits 用于普通函数-------------------------*/


/**----------------------测试打印函数相关信息-------------------------*/
void printFunInfo() {
    using MyFunctionTraits = function_traits<decltype(add)>;
    // 返回类型
    std::cout << "219-----" << typeid(MyFunctionTraits::result_type).name() << std::endl;  // 输出 void

    // 参数数量
    std::cout << "参数数量 = " << MyFunctionTraits::arity << std::endl;  //

    // 第一个参数类型
    using FirstParamType = typename MyFunctionTraits::template arg<0>::type;
    std::cout << "第一个参数类型 " << typeid(FirstParamType).name() << std::endl;  // 输出 int

    // 元组类型
    std::cout << "元组类型 = " << typeid(MyFunctionTraits::tuple_type).name() << std::endl;
    // 输出 std::tuple<int, class std::basic_string<char, struct std::char_traits<char>, class std::allocator<char>>, float>

    // 去除const限定符的元组类型
    std::cout << "去除const限定符的元组类型 = " << typeid(MyFunctionTraits::bare_tuple_type).name() << std::endl;
}

/**----------------------测试打印函数相关信息-------------------------*/





class F {
public:
    int test(int t) {
        return t;
    }

};


int main() {
//    testFunTraitsNormal();
//    check_callback(callback); // 进行编译期类型检查
    testGenFunc();

//    printf("打印函数信息------------------\n");
//    printFunInfo();
//
//    F* f = new F();
//
//    std::function<int(int)> f2 = std::bind(&F::test, f,2);

//    using args_tuple = typename function_traits<decltype(f2)>::bare_tuple_type;
//    using MyFunctionTraits = function_traits<decltype(f2)>;
//    std::cout << "args_tuple "<< typeid(args_tuple).name() << std::endl;
//    std::cout << "212------ "<<typeid(MyFunctionTraits::bare_tuple_type).name() << std::endl;





    return 0;
}