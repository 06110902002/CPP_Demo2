//
// Created by Rambo.Liu on 2025/4/18.
//

#include "./datastruct/LinkList.h"
#include <iostream>

/**
 * 单逆转链表
 * @tparam T
 * @param node
 */
template<typename T>
void reverseList(Node<T> *node) {
    Node<T> *next = nullptr;
    Node<T> *cur = node;
    Node<T> *pre = nullptr;
    while (cur->next != nullptr) {
        next = cur->next;
        cur->next = pre;
        pre = cur;
        cur = next;
    }
    //处理末尾结点
    cur->next = pre;
    printf("就地逆转之后  链表为:\n");
    Node<T> *tmp = cur;
    while (tmp->next) {
        printf("21-----data = %d\n", tmp->data);
        tmp = tmp->next;
    }
    printf("21-----data = %d\n", tmp->data);
}

/**
 * 将2个有序链表合并为一个有序链表
 * @tparam T
 * @param node1
 * @param node2
 */
template<typename T>
void mergeList(Node<T> *node1, Node<T> *node2) {
    Node<T> *p = node1;
    Node<T> *q = node2;
    Node<T> *newList = new Node<T>(-1);
    Node<T> *tmp = newList;
    while (p && q) {
        if (p->data <= q->data) {
            newList->next = p;
            p = p->next;
        } else {
            newList->next = q;
            q = q->next;
        }
        newList = newList->next;
    }
    newList->next = p ? p : q;

    printf("58------合并之后的结果为:\n");

    while (tmp->next) {
        printf("62-----data = %d\n", tmp->data);
        tmp = tmp->next;
    }
    printf("65-----data = %d\n", tmp->data);
}

/**
 * 给定n阶台阶 ，一次只能走1/2 打印所有种走法
 */
int printStage(int n) {
    if (n < 1) {
        printf("参数报错\n");
        return 0;
    }
    if (n == 1) {
        printf("走法只有1种\n");
        return 1;
    }
    if (n == 2) {
        printf("走法只有1种\n");
        return 2;
    }
    return printStage(n - 1) + printStage(n - 2);
}

void preTree() {

}

//算法：一条链表，每k个翻转一次，不足k个则不转；如1->2->3->4->5->6->7 每2个转一次，则转成2->1->4->3->6->5->7
#include <iostream>

using namespace std;

//Definition for singly-linked list.
struct ListNode {
    int val;
    ListNode *next;

    ListNode() : val(0), next(nullptr) {}

    ListNode(int x) : val(x), next(nullptr) {}

    ListNode(int x, ListNode *next) : val(x), next(next) {}
};

//反转第k 个
void reverseK(ListNode *head, int k) {
    ListNode *pre = NULL;
    for (int i = 0; i < k; i++) {
        ListNode *tmp = head->next;
        head->next = pre;
        pre = head;
        head = tmp;
    }
}
ListNode *reverseKGroup(ListNode *head, int k) {
    ListNode *rec = new ListNode(0, head);
    rec->next = head;
    ListNode *pre = rec;                                //每组的前一个
    ListNode *nxt = head;                               //每组的第一个
    ListNode *hed = head;                               //每组反转完的第一个
    //每一组进来一次循环
    while (nxt) {
        ListNode *tail = pre;                           //这一组的末尾
        for (int i = 0; i < k; i++) {
            tail = tail->next;
            if (!tail)
                return rec->next;
        }
        nxt = tail->next;                               //更新nxt，下一组的开始
        hed = head;                                     //记录hed，这一组的开始，作为下一组的pre
        reverseK(head, k);                              //reverse these k
        pre->next = tail;                               //跟pre连，跟前面连起来
        hed->next = nxt;                                //跟nxt连，跟后面连起来
        pre = hed;                                      //更新pre
        head = nxt;                                     //更新head
    }
    return rec->next;
}




int main() {

    Node<int> *head = new Node<int>(0);
    Node<int> *node1 = new Node<int>(1);
    Node<int> *node2 = new Node<int>(2);
    Node<int> *node3 = new Node<int>(3);
    Node<int> *node4 = new Node<int>(4);
    Node<int> *node5 = new Node<int>(5);

    head->next = node1;
    node1->next = node2;
    node2->next = node3;
    node3->next = node4;
    node4->next = node5;

    printf("就地逆转之前  链表为:\n");
    Node<int> *tmp = head;
    while (tmp->next) {
        printf("21-----data = %d\n", tmp->data);
        tmp = tmp->next;
    }
    printf("21-----data = %d\n", tmp->data);

    reverseList(head);


    printf("95--------合并链表\n");

    Node<int> *n1 = new Node<int>(0);
    Node<int> *n2 = new Node<int>(1);
    Node<int> *n3 = new Node<int>(3);
    Node<int> *n4 = new Node<int>(5);
    Node<int> *n5 = new Node<int>(7);
    Node<int> *n6 = new Node<int>(9);
    n1->next = n2;
    n2->next = n3;
    n3->next = n4;
    n4->next = n5;
    n5->next = n6;

    Node<int> *m1 = new Node<int>(2);
    Node<int> *m2 = new Node<int>(4);
    Node<int> *m3 = new Node<int>(6);
    Node<int> *m4 = new Node<int>(8);
    Node<int> *m5 = new Node<int>(10);
    Node<int> *m6 = new Node<int>(12);
    m1->next = m2;
    m2->next = m3;
    m3->next = m4;
    m4->next = m5;
    m5->next = m6;

    mergeList(n1, m1);


    int sum = printStage(4);
    printf(" 台阶 的走法为 sum = %d\n", sum);


    return 0;
}