//
// Created by Rambo.Liu on 2024/3/14.
//

#include <iostream>
#include <stdio.h>
#include <string>

/**------------------------test std::function-------------*/
/**
 * std::function 可以理解为 C 中的函数指针
 * function是一个包装类，它可以接收
 * （1）函数
 * （2）函数指针
 * （3）类成员函数指针
 * （4）任意类型的函数对象（例如：定义了operator()操作符重载的类型）。
 *
 *
 *
 * https://www.cnblogs.com/erichome/p/16965242.html
 *
 * https://zhuanlan.zhihu.com/p/161356621
 *
 * 使用方式参考：
 * https://blog.csdn.net/HW140701/article/details/108776293
 */
void print1(int a, int b) {
    std::cout << "hello, print1 a = " << a << std::endl;
}

void print2(int a, int c) {
    std::cout << "hello, print2 a = " << a << std::endl;
}

int Add(int a, int b) {
    std::cout << "普通函数被调用，结果为:" << a + b << std::endl;
    return a + b;
}

static int StaticAdd(int a, int b) {
    std::cout << "静态函数被调用，结果为:" << a + b << std::endl;
    return a + b;
}


/**
 * 测试对象重载（） 操作符
 */
struct A {
    int operator()(int a) {
        std::cout << "This is A Object  a = " << a << std::endl;
        return a;
    }
};


//使用类模板实现function
//template<typename R, typename Arg0>


void showMes(std::string mes) {
    std::cout << "showMes(string mes)=" << mes << std::endl;
}


/**
 * 测试  std::function  接收普通函数
 */
void test_std_functionForFun() {
    std::function<void(int, int)> func(&print1);
    func(2, 3);

    func = &print2;
    func(4, 4);


    // 1 普通函数
    std::function<int(int, int)> addFunc1 = Add;
    addFunc1(1, 2);

    // 2 普通函数指针
    std::function<int(int, int)> addFunc2 = &Add;
    addFunc2(3, 4);

    // 3 静态函数
    std::function<int(int, int)> staticAddFunc1 = StaticAdd;
    staticAddFunc1(5, 6);

    // 4 静态函数指针
    std::function<int(int, int)> staticAddFunc2 = &StaticAdd;
    staticAddFunc2(7, 8);
}

/**
 * 测试  std::function  接收对象重载了（） 操作符
 */
void test_std_function_obj() {
    A a;
    std::function<int(int)> func;
    func = a;
    int b = func(3);
    printf("92------b = %d \n", b);
}

/**------------------------test std::function-------------*/


/**------------------------test 函数指针-------------*/

//指向函数的指针
//定义的一般形式：函数的返回值类型 (*指针变量名)(形式参数1, 形式参数2, ...);

typedef int (*fun_ptr)(int, int);

int sum(int a, int b) {
    return a + b;
}

class AA {
public:
    AA() {

    }

    ~AA() {

    }

    static int sum(int a, int b);
};

int AA::sum(int a, int b) {
    return a + b;
}

void testFunPtr() {
    fun_ptr funPtr = sum;
    int result1 = funPtr(2, 3);


    AA *a = new AA();
    fun_ptr funPtr2 = a->sum;
    int result2 = funPtr2(3, 7);

    printf("150------result 1 = %d  result2 = %d\n", result1, result2);
}


/**------------------------test 函数指针-------------*/


class PA {
public:
     virtual void func() {
        std::cout << "A func() called." << std::endl;
    }
};

class B : public PA {
public:
    void func()  {
        std::cout << "B func() called." << std::endl;
    }
};



/**------------测试拷贝构造函数的第一个参数为什么要是引用类型------------**/
class CExample {
private:
    int a;
public:
    //构造函数
    CExample(int b) {
        a = b;
        printf("constructor is called\n");
    }

    //拷贝构造函数
    // 不使用引用传递会产生临时对象，临时对象构造时 又要产生临时对象
    // 这样会无穷递归 具体参见：https://blog.csdn.net/cckluv/article/details/109058053
    CExample(const CExample &c) {
        a = c.a;
        printf("copy constructor is called\n");
    }

    //析构函数
    ~CExample() {
        std::cout << "destructor is called\n";
    }

    void Show() {
        std::cout << a << std::endl;
    }
};

void testCopyConstructor() {
    CExample A(100);
    CExample B=A;
    B.Show();
    CExample* B1 = new CExample(23);
}
/**-------------测试拷贝构造函数的第一个参数为什么要是引用类型------------**/

void testVirtualFuc() {
//    PA a;
//    a.func();
//    B b;
//    b.func();

      PA* pa = new B();
      pa->func();
}


//C是一个父类 , 析构函数是虚函数
class C
{
public:
    C()
    {
        std::cout << " C constructor" << std::endl;
    }
    virtual ~C()
    {
        std::cout << " C destructor" << std::endl;
    }
};

//D是C的子类
class D : public C
{
public:

    D()
    {
        std::cout << " D constructor" << std::endl;
    }
    ~D()
    {
        std::cout << " D destructor" << std::endl;
    }
};

#define		UNLIKELY(x)					((__builtin_expect(!!(x), 0)))	// x is likely false


void testRef(int &b) {
    b = 32;
    printf("249-------b = %d \n",b);
}

void testP(int *p) {

    printf("255--------p = %d\n",*p);
}


void testRef2(int b) {
    b = 32;
    printf("252--------b = %d\n",b);
}


/*
必须先有泛化版本，才有特化版本
特化版本编译器会优先选择
*/
#include <iostream>
template <typename T>
struct AccumulationTraits;

template <>
struct AccumulationTraits<char>
{
    using AccT = int;
};
template <>
struct AccumulationTraits<short>
{
    using AccT = int;
};
template <>
struct AccumulationTraits<int>
{
    using AccT = long;
};
template <>
struct AccumulationTraits<unsigned int>
{
    using AccT = unsigned long;
};
template <>
struct AccumulationTraits<float>
{
    using AccT = double;
};

template <typename T>
auto accum(T const *beg, T const *end)
{
    // return type is traits of the element type
    using AccT = typename AccumulationTraits<T>::AccT;
    AccT total{}; // assume this actually creates a zero value
    while (beg != end)
    {
        total += *beg;
        ++beg;
    }
    return total;
}

void testtemp()
{
    // create array of 5 integer values
    int num[] = {1, 2, 3, 4, 5};
    // print average value
    std::cout << "the average value of the integer values is " << accum(num, num + 5) / 5 << "\n";
    // create array of character values
    char name[] = "templates";
    int length = sizeof(name) - 1;
    // (try to) print average character value
    std::cout << "the average value of the characters in \"" << name << "\" is " << accum(name, name + length) / length;
}




void testRefA(std::string&& s) {
    printf("330----------s = %s\n",s.c_str());
}




int main() {
    //testFunPtr();

    //test_std_function_obj();

    //testVirtualFuc();

    //test 4
    //testCopyConstructor();

    std::string json_string = R"(

                {

                "name": "John",

                "age": 30,

                "city": "New York",

                "married": true,

                "children": ["Alice", "Bob"],

                "pets": {

                "dog": "Charlie",

                "cat": "Luna"

                }

                }

                )";
    testRefA(std::move(json_string));




    return 0;
}