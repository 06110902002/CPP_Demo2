//
// Created by Rambo.Liu on 2025/4/22.
//

#ifndef CPP_TEST1_WEAKPTR_H
#define CPP_TEST1_WEAKPTR_H

#include <iostream>


class Myclass {
private:

public:
    Myclass() {
        std::cout << "这里是myclass的构造函数" << std::endl;
    }

    ~ Myclass() {
        std::cout << "这里是myclass的析构函数" << std::endl;
    }

    void DoSomething() {
        std::cout << "do some things" << std::endl;
    }
};


template<typename T>
class _shared_ptr {
public:
    //use_count初始化的意思是，如果初始化shared_ptr的是一个空指针，那么开始引用计数就是0。如果是一个有效指针，那么开始引用计数就是1
    explicit _shared_ptr(T *ptr = nullptr) : use_count(ptr == nullptr ? nullptr : new int(1)), _ptr(ptr) {}

    _shared_ptr(const _shared_ptr &other_ptr) : use_count(other_ptr.use_count), _ptr(other_ptr._ptr) {
        if (use_count != nullptr)//防止用空指针拷贝构造
        {
            (*use_count)++;
        }
    }

    _shared_ptr &operator=(const _shared_ptr &other_ptr) {
        if (&other_ptr != this)//防止自身赋值，自身赋值的话就不用管，引用计数就不用加一了
        {
            release();//先将自身计数减一
            _ptr = other_ptr._ptr;
            use_count = other_ptr.use_count;
            //(*use_count) ++;//这里要注意不能直接++，防止other_ptr是空指针
            if (use_count)
                ++(*use_count);
        }
        return *this;
    }

    ~ _shared_ptr() {
        release();
    }

    T &operator*() const//重载解引用运算符
    {
        return *_ptr;//返回被管理对象的引用
    }

    T *operator->() const//重载->运算符
    {
        return _ptr;
    }

    T *get() const//和->一样，返回的是指针
    {
        return _ptr;
    }

    int get_use_count() const {
        return use_count != nullptr ? (*use_count) : 0;//如果use_count是空指针的话，就说明没有引用计数，说明shared_ptr是空的，就返回0
    }


    void release() {
        if (use_count && --(*use_count) == 0) {
            delete use_count;
            delete _ptr;
        }

    }

    int *use_count;//引用计数
    T *_ptr;//指向所管理的对象
};


template<typename T>
class _weak_ptr {
    friend class _shared_ptr<T>;

public:
    _weak_ptr() : use_count(new int(0)), ptr(nullptr) {}

    _weak_ptr(const _weak_ptr &wp) : use_count(wp ? wp.use_count : new int(0)), ptr(wp.ptr) {}

    _weak_ptr(const _shared_ptr<T> &sp) : use_count(sp.use_count), ptr(sp._ptr) {}

    /**
     * 重载 =
     * 引用计数 不增加，维持原样
     * @param wp
     * @return
     */
    _weak_ptr &operator=(const _weak_ptr &wp) {
        if (&wp != this) {
            ptr = wp.ptr;
            use_count = wp.use_count;
        }
        return *this;
    }

    /**
     * 重载 =
     * 引用计数 不增加，维持原样
     * @param wp
     * @return
     */
    _weak_ptr &operator=(const _shared_ptr<T> &sp) {
        ptr = sp._ptr;
        use_count = sp.use_count;
        return *this;
    }

    T *operator->() {
        return ptr;
    }

    T &operator*() {
        return *ptr;
    }

    int get_use_count() {
        //return *use_count;
        return use_count == nullptr ? 0 : (*use_count);
    }

    bool expire()//根据use——count是否为零来判断
    {
        return use_count == nullptr || *(use_count) == 0;
    }

    /**
     * 获取share_ptr 指针
     * @return
     */
    _shared_ptr<T> &lock() {
        if (expire())//如果expire为true，就返回一个空的shared——ptr指针
            return _shared_ptr<T>();
        return _shared_ptr<T>(*this);
    }

    void reset()//将weak_ptr置为空
    {
        ptr = nullptr;
        use_count = nullptr;
    }

private:
    int *use_count;
    T *ptr;
};

int main() {
    _shared_ptr<Myclass> p1(new Myclass());
    {
        _shared_ptr<Myclass> p2(new Myclass());\
        std::cout << "p2的use_count: " << p2.get_use_count() << std::endl;

    }
    std::cout << ".................................." << std::endl;
    _shared_ptr<Myclass> p3(new Myclass());
    std::cout << "p1的use_count: " << p1.get_use_count() << std::endl;
    _weak_ptr<Myclass> wp2;
    wp2 = p1;
    std::cout << "wp2的use_count是： " << wp2.get_use_count() << std::endl;
    std::cout << "p3的use_count: " << p3.get_use_count() << std::endl;

    p1 = p3;
    std::cout << ".................................." << std::endl;
    std::cout << "p1的use_count: " << p1.get_use_count() << std::endl;
    std::cout << "p3的use_count: " << p3.get_use_count() << std::endl;

    _weak_ptr<Myclass> wp1;
    wp1 = p1;
    std::cout << "wp1的use_count是： " << wp1.get_use_count() << std::endl;
    return 0;
}


#endif //CPP_TEST1_WEAKPTR_H
