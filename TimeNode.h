//
// Created by Rambo.Liu on 2025/3/27.
//

#ifndef CPP_TEST1_TIMENODE_H
#define CPP_TEST1_TIMENODE_H

#include <cstdlib>
#include <cstdio>
#include <iostream>

struct Node {
    int data;
    Node *prevNode;
    Node *nextNode;
};

Node *initList() {
    Node *head = (Node *) malloc(sizeof(Node));
    if (NULL == head) {
        perror("malloc error:");
        return NULL;
    }
    head->data = 0;
    head->nextNode = NULL;
    head->prevNode = NULL;
    return head;
}

int insertHead(Node *L, int elem) {
    Node *node = (Node *) malloc(sizeof(Node));
    if (NULL == node) {
        perror("malloc error:");
        return -1;
    }

    node->data = elem;
    node->prevNode = L;         //第一步，新节点前驱赋值
    node->nextNode = L->nextNode;   //第二步，新节点后继赋值
    if (L->nextNode != NULL)    //判断头节点有后继
    {
        L->nextNode->prevNode = node; //第三步，原头节点后继的前驱指向新节点
    }
    L->nextNode = node;           //第四步，头节点后继指向新节点
    return 0;
}


class TimeNode: public std::enable_shared_from_this<TimeNode>{
public:
    TimeNode() {

    }
};


#endif //CPP_TEST1_TIMENODE_H
