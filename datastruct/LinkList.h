//
// Created by Rambo.Liu on 2025/4/18.
//

#ifndef CPP_TEST1_LINKLIST_H
#define CPP_TEST1_LINKLIST_H

template<typename T>
class Node {
public:
    Node(int _data) {
        next = nullptr;
        data = _data;
    }

    ~Node() {

    }
    Node<T>* next;
    T data;
};


template<typename T>
class LinkList {
public:
    LinkList() {

    }
    ~LinkList() {

    }
};


#endif //CPP_TEST1_LINKLIST_H
