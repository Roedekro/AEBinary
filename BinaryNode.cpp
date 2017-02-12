//
// Created by Martin on 06-02-2017.
//

/*
 * BinaryNode.cpp
 *
 *  Created on: 27/01/2017
 *      Author: Martin
 */

#include "BinaryNode.h"
#include <iostream>

using namespace std;

BinaryNode::BinaryNode(int* array, int x, int y, BinaryNode* p) {

    int middle = (y-x)/2 + x;
    if((y-x) % 2 == 1) {
        middle++;
        cout << "!\n";
    }
    value = array[middle];
    parent = p;
    left = NULL;
    right = NULL;

    //cout << value << "," << middle << "," << x << "," << y << '\n';

    if(x < middle) {
        left = new BinaryNode(array,x,middle-1,this);
    }
    if(y > middle) {
        right = new BinaryNode(array,middle+1,y,this);
    }

}

BinaryNode::~BinaryNode() {
    // TODO Auto-generated destructor stub
}

void BinaryNode::terminateMe() {

    if(left != NULL) {
        left->terminateMe();
    }
    if(right != NULL) {
        right->terminateMe();
    }

    //cout << "Deleting " << value << '\n';
    delete(this);
}

void BinaryNode::printMe() {

    cout << "Node " << value << '\n';
    if(parent != NULL) {
        cout << "Parent " << parent->value << '\n';
    }
    else {
        cout << "Parent NULL\n";
    }
    if(left != NULL) {
        cout << "Left " << left->value << '\n';
    }
    else {
        cout << "Left NULL" << '\n';
    }
    if(right != NULL) {
        cout << "Right " << right->value << '\n';
    }
    else {
        cout << "Right NULL" << '\n';
    }
    cout << "---\n";

    if(left != NULL) {
        left->printMe();
    }
    if(right != NULL) {
        right->printMe();
    }
}

