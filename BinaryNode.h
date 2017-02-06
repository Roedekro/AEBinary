//
// Created by Martin on 06-02-2017.
//

#ifndef AEBINARY_BINARYNODE_H
#define AEBINARY_BINARYNODE_H


class BinaryNode {
public:
    BinaryNode(int* array,int x, int y, BinaryNode* p);
    virtual ~BinaryNode();
    void terminateMe();
    void printMe();
    int value;
    BinaryNode* parent;
    BinaryNode* left;
    BinaryNode* right;
};


#endif //AEBINARY_BINARYNODE_H
