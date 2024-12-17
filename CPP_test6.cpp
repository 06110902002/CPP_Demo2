//
// Created by Rambo.Liu on 2024/3/18.
// std::enable_shared_from_this

#include <iostream>
#include <memory>

/**--------------------示例1----------------**********/

//class Widget {
//
//public:
//
//    Widget() {
//        std::cout << "Widget constructor run 构造对象地址 = " << this << std::endl;
//    }
//
//    ~Widget() {
//        std::cout << "Widget destructor run  释放对象地址 = " << this << std::endl;
//    }
//
//    std::shared_ptr<Widget> GetSharedObject() {
//        return std::shared_ptr<Widget>(this);
//    }
//};
//
//void test1() {
//
//    std::shared_ptr<Widget> p(new Widget());
//    std::shared_ptr<Widget> q = p->GetSharedObject();
//
//    std::cout << "32------p引用计数 = " << p.use_count() << std::endl;
//    std::cout << "33------q引用计数 = " << q.use_count() << std::endl;
//
//}


/**--------------------示例1----------------**********/


/**--------------------示例二----------------**********/
struct Bad
{
    Bad() {
        std::cout<< "Bad 构造实例"<<std::endl;
    }
    ~Bad() {
        std::cout << "~Bad 析构  地址 = " << this << std::endl;
    }
    void fun()
    {
        std::shared_ptr<Bad> sp{ this };
        std::cout << "41----引用计数 = "<<sp.use_count() <<" 对象地址 = "<< sp <<std::endl;
    }
};

/**
 * 在 func 函数构造智能指针时，我们无法确定这个对象是不是被 shared_ptr 管理着，
 * 因此这样构造的 shared_ptr 并不是与其他 shared_ptr 共享一个计数器，那么，在析构时就会导致对象被重复释放，从而引发错误。
 * 在一个对象内部构造该对象的 shared_ptr 时，即使该对象已经被 shared_ptr 管理着，也不会造成对象被两个独立的智能指针管理。
 * 这就要求我们在对象内构造对象的智能指针时，必须能识别有对象是否已经由其他智能指针管理、智能指针的数量，
 * 并且我们创建智能指针后也能让之前的智能指针感知到。
 * 正确做法是继承 enable_shared_from_this 类，调用 shared_from_this() 函数生成 shared_ptr
 */
void test2() {
    std::shared_ptr<Bad> sp{std::make_shared<Bad>()};
    std::cout << "51---引用计数 = " << sp.use_count() << " 对象地址 = " << sp << std::endl;
    sp->fun();
}
/**--------------------示例二----------------**********/



//int main() {
//    //test1();
//    test2();
//    return 0;
//}



#include <iostream>
#include <memory>

class Widget : public std::enable_shared_from_this<Widget> {
public:
    Widget() {
        std::cout << "Widget constructor run" << std::endl;
    }

    ~Widget() {
        std::cout << "Widget destructor run" << std::endl;
    }

    std::shared_ptr<Widget> GetSharedObject() {
        return shared_from_this();
    }
};

int main() {
    std::shared_ptr<Widget> p(new Widget());
    std::shared_ptr<Widget> q = p->GetSharedObject();

    std::cout << p.use_count() << std::endl;
    std::cout << q.use_count() << std::endl;

    return 0;
}