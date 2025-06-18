//
// Created by Rambo.Liu on 2025/4/21.
//

#ifndef CPP_TEST1_TREENODE_H
#define CPP_TEST1_TREENODE_H

#include <iostream>


template<typename T>
class TreeNode : public std::enable_shared_from_this<T> {
public:

    TreeNode() {

    }

    TreeNode(T data,TreeNode<T> left,TreeNode<T> right) {
        value = data;
        leftChild = left;
        rightChild = right;
    }
    explicit TreeNode(T data) {
        value = data;
    }
    ~TreeNode() {

    }

    TreeNode* leftChild;
    TreeNode* rightChild;
    T value;

};

template<typename T>
class BinaryTree:std::enable_shared_from_this<T> {

public:
    BinaryTree() {

    }

    ~BinaryTree() {

    }

};

#endif //CPP_TEST1_TREENODE_H
