//
// Created by Rambo.Liu on 2024/3/15.
// //使用类模板实现function

#include <iostream>
#include <string>
using namespace std;

template<typename T>
class MyFunction3 {

};

template<typename Ty, typename A1>
class MyFunction3<Ty(A1)> {

public:
    typedef Ty(*pFunction)(A1);//定义一个函数指针,指针指向的函数返回类型是Ty,有1个函数参数
    MyFunction3<Ty(A1)>(pFunction _pFunction) : _function(_pFunction) {
    }

    Ty operator()(A1 arg1) {
        return (*_function)(arg1);
    }

private:
    pFunction _function;
};


template<typename Ty,typename A1,typename A2>
class MyFunction3<Ty(A1,A2)> {

public:

    typedef Ty(*pFunction)(A1, A2);//定义一个函数指针,指针指向的函数返回类型是Ty,有两个函数参数
    MyFunction3<Ty(A1, A2)>(pFunction _pFunction):_function(_pFunction) {
    }

    Ty operator()(A1 arg1, A2 arg2) {
        return (*_function)(arg1, arg2);
    }

private:
    pFunction _function;
};


void showMes(string mes) {
    cout << "showMes(string mes)=" << mes << endl;
}


int sum1(int x, int y) {
    cout << "sum1 "<<(x+y) << endl;
    return x + y;
}
int sum2(int x, int y) {
    cout << "sum2 " << (x + y) << endl;
    return x + y;
}

int main() {

    MyFunction3<int(int, int)> f1(sum1);
    f1(20,30);


    MyFunction3<void(string)> f2(showMes);
    f2("AAAA");


    return 0;
}
