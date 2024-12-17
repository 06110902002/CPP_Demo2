//
// Created by Rambo.Liu on 2024/12/17.
// 测试模板 与函数的特化
// https://blog.csdn.net/gatieme/article/details/50953564

#include <iostream>
#include <cstring>

/**--------------------------测试函数模版-----------------------------**/

///  函数模版
template<class T>
int max(const T left, const T right) {
    std::cout << "这是一个函数模板  支持 是的 可计算的类型如,int float double" << std::endl;
    return left > right ? left : right;
}


//  这个是一个特化的函数模版
template<>
int max<const char *>(const char *left, const char *right) {
    std::cout << "这是一个全特化函数模版，注意它的表达方式" << std::endl;

    return strcmp(left, right);
}
//  特化的函数模版, 两个特化的模版本质相同, 因此编译器会报错
// error: redefinition of 'int compare(T, T) [with T = const char*]'|
//template < >
//int compare(const char* left, const char* right)
//{
//    std::cout <<"in special template< >..." <<std::endl;
//
//    return strcmp(left, right);
//}


//  这个其实本质是函数重载
int max(char *left, char *right) {
    std::cout << "in overload function..." << std::endl;
    return strcmp(left, right);
}

void testFuncTemp() {
    int r1 = max(1, 4);
    const char *left = "left";
    const char *right = "right";
    int r2 = max(left, right);

    printf("r1 = %d  r2 = %d \n", r1, r2);
}


/*template<class T>
void func() {
    printf("测试函数模版的特化,这是基础模版\n");
}

template<>
void func() {
    printf("测试函数模版的特化,这是全物特化版本\n");
}*/

template<class T>
void func() {
    printf("测试函数模版的特化,这是基础模版\n");
}

template<>
void func<int>() {
    printf("测试函数模版的特化,这是全物特化版本\n");
}


/**--------------------------测试函数模版特化-----------------------------**/


/**--------------------------测试类模版特化，全特化，偏特化-----------------------------**/
template<class T1, class T2>
class A {
public:
    A(T2 t2) {
        std::cout << "这是类A 的基础模板" << t2 << std::endl;
    }
};

//处理整型
template<class T1>
class A<T1, int> {
public:
    A(int t2) {
        std::cout << "这是类A 的int 偏特化版本" << t2 << std::endl;
    }
};

//处理char*
template<class T1>
class A<T1, char *> {

public:
    A(char *t2) {
        std::cout << "这是类A 的char* 偏特化版本" << t2 << std::endl;
    }
};

void testClassSP() {
    A<float, float> *a1 = new A<float, float>(3.0f);
    A<float, int> *a2 = new A<float, int>(10);
    char *s = "liu";
    A<float, char *> *a3 = new A<float, char *>(s);
}


//测试二 在派生时  类模版特化的使用
class Student {
    int age = 25;
public:
    void mycout() {
        std::cout << "学生年纪 " << age << std::endl;
    }
};

class Animal {
    int a;
public:
    Animal(int t) : a(t) {

    }

    void display() {
        std::cout << "这是基类 " << a << std::endl;
    }
};

template<typename T>
class Dog : public Animal {
    T t;
public:
    Dog(T t) : Animal(123), t(t) {

    }

    void display() {
        Animal::display();
        std::cout << "这是狗 "<<t << std::endl;
    }
};

class Other{
public:
    Other() {
        std::cout<< "这是其他类"<< std::endl;
    }

};

template<>
class Dog<Student> :public Other {
public:
    Dog()  {

    }

    void display() {

        std::cout << "这是狗的特化" << std::endl;
    }
};

void testClassExp() {
    Dog<Student> d;
    d.display();
}


/**--------------------------测试类模版特化，全特化，偏特化-----------------------------**/


template<typename T>
void printType(T value) {
    std::cout << "Generic template" << std::endl;
}


int main() {
    //testFuncTemp();
    //testClassSP();
    testClassExp();

    return 0;
}



