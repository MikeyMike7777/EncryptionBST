/**
* file: bst-student-proj3.h
* author: Michael Mathews
* course: CSI 3334
* assignment: project 3
* due date: March 4, 2022
* version: 11.7
*
* This file implements the BSTNode, BST, and Encryption Tree classes
*/

#ifndef BST_STUDENT_PROJ3
#define BST_STUDENT_PROJ3

#include "bst-prof-proj3.h"

/* BSTNode Destructor
*  parameters:
*      none
*  return value: none
*
* This function deletes BSTNodes
*/
template <class Base>
BSTNode<Base>::~BSTNode(){
    if(this->left){
        delete this->left;
    }
    if(this->right){
        delete this->right;
    }
}

/* printPreorder
*  parameters:
*      os - output stream to write to
*      indent - amount of spaces before the data in each node
*  return value: none
*
* This function prints a tree of BSTNodes in preorder
*/
template <class Base>
void BSTNode<Base>::printPreorder(ostream &os, string indent) const{
    const int UNINDENT_START = 0, UNINDENT_END = 2;
    //print Me Left Right
    os << indent << this->data << endl;
    if(this->left){
        indent.append("  ");
        this->left->printPreorder(os, indent);
    }
    else {
        indent.append("  ");
        os << indent << "NULL" << endl;
    }
    
    if(this->right){
        this->right->printPreorder(os, indent);
    }
    else {
        os << indent << "NULL" << endl;
    }
    indent.erase(UNINDENT_START, UNINDENT_END);
}

/* minNode
*  parameters:
*      none
*  return value: smallest BSTNode in a BST
*
* This function finds the smallest Node in a BST (node with the smallest value)
*/
template <class Base>
const BSTNode<Base>* BSTNode<Base>::minNode() const{
    const BSTNode *leftPtr = this;
    while(leftPtr->left){
        leftPtr = leftPtr->left;
    }
    return leftPtr;
}

/* maxNode
*  parameters:
*      none
*  return value: largest BSTNode in a BST
*
* This function finds the largest Node in a BST (node with the largest value)
*/
template <class Base>
const BSTNode<Base>* BSTNode<Base>::maxNode() const{
    const BSTNode *rightPtr = this;
    while(rightPtr->right){
        rightPtr = rightPtr->right;
    }
    return rightPtr;
}

/* insert
*  parameters:
*      item - data to be stored in a BST
*  return value: none
*
* This function inserts a value into its correct place in a BST
*/
template <class Base>
void BST<Base>::insert(const Base &item){
    BSTNode<Base> *ptr = this->root, *parent = this->root;
    bool found = false, first = true;
    //search for item
    while(ptr and !found){
        parent = ptr;
        if(item < ptr->data){
            ptr = ptr->left;
        }
        else if(ptr->data < item){
            ptr = ptr->right;
        }
        else{
            found = true;
        }
        first = false;
    }
    
    if(first){
        BSTNode<Base> *newNode = new BSTNode<Base>(item);
        this->root = newNode;
    }
    //if not in bst, insert
    else if(!found){
        BSTNode<Base> *newNode = new BSTNode<Base>(item);
        if(item < parent->data){
            parent->left = newNode;
        }
        else{
            parent->right = newNode;
        }
    }
}

/* remove
*  parameters:
*      item - data to be removed from a BST
*  return value: none
*
* This function removes a value from a BST
*/
template <class Base>
void BST<Base>::remove(const Base &item){
    BSTNode<Base> *ptr = this->root, *parent = ptr;
    bool found = false;
    //find parent and node to remove
    while(ptr and !found){
        if(item < ptr->data){
            parent = ptr;
            ptr = ptr->left;
        }
        else if(ptr->data < item){
            parent = ptr;
            ptr = ptr->right;
        }
        else{
            found = true;
        }
    }
    
    //if it exists, delete it
    if(found){
        //if two children
        if(ptr->left and ptr->right){
            BSTNode<Base> *leftMost = ptr->right, *leftMostParent = ptr;
            if (leftMost->left != nullptr) {
                while (leftMost->left != nullptr) {
                    leftMostParent = leftMost;
                    leftMost = leftMost->left;
                }
                leftMostParent->left = leftMost->right;
                leftMost->right = ptr->right;
            }
            leftMost->left = ptr->left;
            if (parent->left == ptr) {
                parent->left = leftMost;
            }
            else if(parent->right == ptr){
                parent->right = leftMost;
            }
            else {
                this->root = leftMost;
            }
            ptr->left = ptr->right = nullptr;
            delete ptr;
        }
        //if one child
        else if(ptr->left or ptr->right){
            BSTNode<Base> *child = nullptr;
            if (ptr->left != nullptr) {
                child = ptr->left;
                ptr->left = nullptr;
            }
            else {
                child = ptr->right;
                ptr->right = nullptr;
            }
            if (parent->left == ptr) {
                parent->left = child;
            }
            else if(parent->right == ptr){
                parent->right = child;
            }
            else {
                this->root = child;
            }
            ptr->left = ptr->right = nullptr;
            delete ptr;
        }
        //if no children
        else{
            if (ptr == parent->left) {
                parent->left = nullptr;
            }
            else if(ptr == parent->right) {
                parent->right = nullptr;
            }
            else{
                this->root = nullptr;
            }
            ptr->left = ptr->right = nullptr;
            delete ptr;
        }
    }
}

/* encrypt
*  parameters:
*      item - data to be encrypted using codebook
*  return value: encoded data in the form of a string
*
* This function encodes data based on its location in a BST codebook
*/
template <class Base>
string EncryptionTree<Base>::encrypt(const Base &item) const{
    string secret;
    bool done = false;
    const BSTNode<Base> *ptr = this->root;
    //start with root
    secret.push_back('r');
    //while not done and at least one child exists
    while(!done){
        if(ptr){
            //if less than current node, go left
            if(item < ptr->getData()){
                secret.push_back('0');
                ptr = ptr->getLeft();
            }
            //if greater than current node, go right
            else if(ptr->getData() < item){
                secret.push_back('1');
                ptr = ptr->getRight();
            }
            //if equal to current node, done
            else {
                done = true;
            }
        }
        //if not found
        else {
            done = true;
            secret = "?";
        }
    }
    return secret;
}

/* decrypt
*  parameters:
*      path - directions to be decrypted using codebook
*  return value: pointer to the data found using the path
*
* This function decodes a path to find data based on its location
* in a BST codebook
*/
template <class Base>
const Base * EncryptionTree<Base>::decrypt(const string &path) const{
    const Base *item;
    const BSTNode<Base> *ptr = this->root;
    //make sure path starts at root
    assert(path[0] == 'r');
    //find word in codebook if word exists
    for(unsigned int i = 1; ptr and i < path.length(); i++){
        if(path[i] == '0'){
            ptr = ptr->getLeft();
        }
        else if(path[i] == '1'){
            ptr = ptr->getRight();
        }
    }
    
    if(ptr){
        item = &ptr->getData();
    }
    else {
        item = NULL;
    }
    //return pointer to value or NULL if encrypted path not valid
    return item;
}
#endif

