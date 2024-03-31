#ifndef AVLBST_H
#define AVLBST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <cstdint>
#include <algorithm>
#include "bst.h"
using namespace std;
struct KeyError { };

/**
* A special kind of node for an AVL tree, which adds the balance as a data member, plus
* other additional helper functions. You do NOT need to implement any functionality or
* add additional data members or helper functions.
*/
template <typename Key, typename Value>
class AVLNode : public Node<Key, Value>
{
public:
    // Constructor/destructor.
    AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent);
    virtual ~AVLNode();

    // Getter/setter for the node's height.
    int8_t getBalance () const;
    void setBalance (int8_t balance);
    void updateBalance(int8_t diff);

    // Getters for parent, left, and right. These need to be redefined since they
    // return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
    // for more information.
    virtual AVLNode<Key, Value>* getParent() const override;
    virtual AVLNode<Key, Value>* getLeft() const override;
    virtual AVLNode<Key, Value>* getRight() const override;

protected:
    int8_t balance_;    // effectively a signed char
};

/*
  -------------------------------------------------
  Begin implementations for the AVLNode class.
  -------------------------------------------------
*/

/**
* An explicit constructor to initialize the elements by calling the base class constructor
*/
template<class Key, class Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value> *parent) :
    Node<Key, Value>(key, value, parent), balance_(0)
{

}

/**
* A destructor which does nothing.
*/
template<class Key, class Value>
AVLNode<Key, Value>::~AVLNode()
{

}

/**
* A getter for the balance of a AVLNode.
*/
template<class Key, class Value>
int8_t AVLNode<Key, Value>::getBalance() const
{
    return balance_;
}

/**
* A setter for the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::setBalance(int8_t balance)
{
    balance_ = balance;
}

/**
* Adds diff to the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::updateBalance(int8_t diff)
{
    balance_ += diff;
}

/**
* An overridden function for getting the parent since a static_cast is necessary to make sure
* that our node is a AVLNode.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getParent() const
{
    return static_cast<AVLNode<Key, Value>*>(this->parent_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getLeft() const
{
    return static_cast<AVLNode<Key, Value>*>(this->left_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getRight() const
{
    return static_cast<AVLNode<Key, Value>*>(this->right_);
}


/*
  -----------------------------------------------
  End implementations for the AVLNode class.
  -----------------------------------------------
*/


template <class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value>
{
public:
    virtual void insert (const std::pair<const Key, Value> &new_item); // TODO
    virtual void remove(const Key& key);  // TODO
protected:
    virtual void nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2);

    // Add helper functions here
    
    void insertFix(AVLNode<Key,Value>* parent, AVLNode<Key,Value>* curr);
    void removeFix(AVLNode<Key,Value>* curr, int diff);
    virtual void rotateLeft(AVLNode<Key,Value>* curr);
    virtual void rotateRight(AVLNode<Key,Value>* curr);

    AVLNode<Key, Value>* AVLFind(const Key& k) const; //Needed for remove()
    static AVLNode<Key, Value>* AVLPredecessor(AVLNode<Key, Value>* current); //Needed for remove()
    
    

};

/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
    
    Key newKey = new_item.first;
    Value newVal = new_item.second;

    if (this->root_ == NULL) {
        AVLNode<Key,Value>* newAvlNode = new AVLNode<Key,Value>(newKey, newVal, NULL);
        this->root_ = newAvlNode;
        return;
    }
    
    bool rl; //path taken, right (1) or left (0) of parent

    AVLNode<Key, Value> *curr = static_cast<AVLNode<Key, Value>*>(this->root_);
    
    AVLNode<Key, Value> *parent;
    while (curr != nullptr) {
        parent = curr;
        Key currKey = curr->getKey();
        if (newKey < currKey) {
            curr = curr->getLeft();
            rl = 0;
        }
        else if (newKey > currKey) {
            curr = curr->getRight();
            rl = 1;
        }
        else { // keys ==
            curr->setValue(newVal);
            return;
        }
    }

    AVLNode<Key,Value>* newNode = new AVLNode<Key,Value>(newKey, newVal, parent);
    if (rl) {
        parent->setRight(newNode);
    }
    else {
        parent->setLeft(newNode);
    }
    
    if (parent->getBalance()) { // We're done!
        parent->setBalance(0);
        return;
    }

    if (rl) {
        parent->updateBalance(1);
    }
    else {
        parent->updateBalance(-1);
    }
    
    insertFix(parent, newNode);

        // TODO
}

/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>:: remove(const Key& key)
{

  AVLNode<Key,Value>* curr = AVLFind(key);
  
  if (curr == NULL) {
    return;
  }
  if (curr == this->root_ && curr->getLeft() == NULL && curr->getRight() == NULL) { //only one node
    this->root_ = NULL;
    delete curr;
    return;
  }
  
  if (curr->getLeft() != NULL && curr->getRight() != NULL) {
    nodeSwap(curr,AVLPredecessor(curr));
  }
  
  AVLNode<Key,Value>* parent = curr->getParent();
  int lr = -1;
  int diff = 0;
  if (parent != NULL) {
    if (parent->getRight() == curr) {
      diff = -1;
      lr = 1;
    }
    else {
      diff = 1;
      lr = 0;
    }
  }

  if (curr->getLeft() == NULL && curr->getRight() == NULL) {
    if (lr == 1) {
      parent->setRight(NULL);
    }
    else if (lr == 0) {
      parent->setLeft(NULL);
    }
  }
  else if (curr->getLeft() == NULL || curr->getRight() == NULL) {
    if (curr->getLeft() == NULL) {
      if (lr == 1) {
        parent->setRight(curr->getRight());
      }
      else if (lr == 0) {
        parent->setLeft(curr->getRight());
      }
      else {
        this->root_ = curr->getRight();
      }
      curr->getRight()->setParent(parent);
    }
    else {
      if (lr == 1) {
        parent->setRight(curr->getLeft());
      }
      else if (lr == 0) {
        parent->setLeft(curr->getLeft());
      }
      else {
        this->root_ = curr->getLeft();
      }
      curr->getLeft()->setParent(parent);
    }
  }
  delete curr;
  
  
  removeFix(parent, diff);

    // TODO
}

template<class Key, class Value>
void AVLTree<Key,Value>::insertFix(AVLNode<Key,Value>* parent, AVLNode<Key,Value>* curr) {

    if (parent == NULL || parent->getParent()==NULL) { 
        return;
    }
    AVLNode<Key,Value>* grandparent = parent->getParent();
    if (grandparent->getLeft() == parent) {
        grandparent->updateBalance(-1);
        int gbal = grandparent->getBalance();
        if (!gbal) {
            return;
        }
        if (gbal == -1) {
            insertFix(grandparent,parent);
        }
        else if (gbal == -2) { 
            if (parent->getLeft() == curr) { //Zig zig
              rotateRight(grandparent);
              grandparent->setBalance(0);
              parent->setBalance(0);
            }
            else { //Zig zag
              rotateLeft(parent);
              rotateRight(grandparent);
              if (curr->getBalance() == -1) {
                parent->setBalance(0);
                grandparent->setBalance(1);
                curr->setBalance(0);
              }
              else if (curr->getBalance() == 0){
                parent->setBalance(0);
                grandparent->setBalance(0);
                curr->setBalance(0);
              }
              else {
                parent->setBalance(-1);
                grandparent->setBalance(0);
                curr->setBalance(0);
              }
            }
            return;
        }
    }
    else {
        grandparent->updateBalance(1);
        int gbal = grandparent->getBalance();
        if (!gbal) {
            return;
        }
        if (gbal == 1) {
            insertFix(grandparent,parent);
        }
        else if (gbal == 2) {
          if (parent->getRight() == curr) { //Zig zig
              rotateLeft(grandparent);
              grandparent->setBalance(0);
              parent->setBalance(0);
            }
            else { //Zig zag
              rotateRight(parent);
              rotateLeft(grandparent);
              if (curr->getBalance() == 1) {
                parent->setBalance(0);
                grandparent->setBalance(-1);
                curr->setBalance(0);
              }
              else if (curr->getBalance() == 0){
                parent->setBalance(0);
                grandparent->setBalance(0);
                curr->setBalance(0);
              }
              else {
                parent->setBalance(1);
                grandparent->setBalance(0);
                curr->setBalance(0);
              }
            }
        }
    }
    

}
template<class Key, class Value>
void AVLTree<Key,Value>::removeFix(AVLNode<Key,Value>* curr, int diff) {
    if (curr == NULL) return;
    
    AVLNode<Key,Value>* parent = curr->getParent();
    int ndiff = 0;
    if (parent != NULL) {
      if (parent->getLeft() == curr){
        ndiff++;
      }
      else {
        ndiff--;
      }
    }
    
    int currBal = curr->getBalance();
    if (currBal + diff == -2) {
      AVLNode<Key,Value>* left = curr->getLeft();
      int leftBal = left->getBalance();
      if (leftBal == -1) { //Zig zig
        rotateRight(curr);
        curr->setBalance(0);
        left->setBalance(0);
        removeFix(parent,ndiff);
      }
      else if (!leftBal) { //Zig zig
        rotateRight(curr);
        curr->setBalance(-1);
        left->setBalance(1);
        return;
      }
      else { //Zig zag
        AVLNode<Key,Value>* zag = left->getRight();
        rotateLeft(left);
        rotateRight(curr);
        int zagBal = zag->getBalance();
        if (zagBal == 1) {
          curr->setBalance(0);
          left->setBalance(-1);
          zag->setBalance(0);
        }
        else if (zagBal == 0) {
          curr->setBalance(0);
          left->setBalance(0);
          zag->setBalance(0);
        }
        else {
          curr->setBalance(1);
          left->setBalance(0);
          zag->setBalance(0);
        }
        removeFix(parent, ndiff);
      }
    }
    else if (currBal + diff == -1) {
      curr->setBalance(-1);
      return;
    }
    else if (!(currBal + diff)){
      curr->setBalance(0);
      removeFix(parent,ndiff);
    }
    else if (currBal + diff == 1) {
      curr->setBalance(1);
      return;
    }
    else { // 2 case
      AVLNode<Key,Value>* right = curr->getRight();
      int rightBal = right->getBalance();
      if (rightBal == 1) { //Zig zig
        rotateLeft(curr);
        curr->setBalance(0);
        right->setBalance(0);
        removeFix(parent,ndiff);
      }
      else if (!rightBal) { //Zig zig
        rotateLeft(curr);
        curr->setBalance(1);
        right->setBalance(-1);
        return;
      }
      else { //Zig zag
        AVLNode<Key,Value>* zag = right->getLeft();
        rotateRight(right);
        rotateLeft(curr);
        int zagBal = zag->getBalance();
        if (zagBal == -1) {
          curr->setBalance(0);
          right->setBalance(1);
          zag->setBalance(0);
        }
        else if (zagBal == 0) {
          curr->setBalance(0);
          right->setBalance(0);
          zag->setBalance(0);
        }
        else {
          curr->setBalance(-1);
          right->setBalance(0);
          zag->setBalance(0);
        }
        removeFix(parent, ndiff);
      }
    }
}

template<class Key, class Value>
void AVLTree<Key, Value>::rotateLeft(AVLNode<Key,Value>* X) {
    AVLNode<Key,Value>* parent = X->getParent();
    AVLNode<Key,Value>* Y = X->getRight();
    AVLNode<Key,Value>* Yleft = Y->getLeft();

    if (parent != NULL)  {
      if (parent->getRight() == X) {
        parent->setRight(Y);
      }
      else {
        parent->setLeft(Y);
      }
    }
    else {
      this->root_ = Y;
    }
    Y->setParent(parent);
    Y->setLeft(X);
    X->setParent(Y);
    X->setRight(Yleft);
    if (Yleft != NULL) {
      Yleft->setParent(X);
    }

}

template<class Key, class Value>
void AVLTree<Key, Value>::rotateRight(AVLNode<Key,Value>* X) {
    AVLNode<Key,Value>* parent = X->getParent();
    AVLNode<Key,Value>* Y = X->getLeft();
    AVLNode<Key,Value>* Yright = Y->getRight();

    if (parent != NULL)  {
      if (parent->getRight() == X) {
        parent->setRight(Y);
      }
      else {
        parent->setLeft(Y);
      }
    }
    else {
      this->root_ = Y;
    }
    Y->setParent(parent);
    Y->setRight(X);
    X->setParent(Y);
    X->setLeft(Yright);
    if (Yright != NULL) {
      Yright->setParent(X);
    }

}

template<class Key, class Value>
AVLNode<Key, Value>*
AVLTree<Key, Value>::AVLPredecessor(AVLNode<Key, Value>* current) {
  if (current->getLeft() != nullptr){
      current = current->getLeft();
      while (current->getRight() != nullptr) {
        current = current->getRight();
      }
      return current;
    }
    AVLNode<Key, Value>* parent = current->getParent();
    while (parent != NULL && parent->getRight() != current) {
      current = parent;
      parent = current->getParent();
    }
    current = parent;
    if (parent == NULL) current = NULL; //current is root
    
    return current;
}

template<class Key, class Value>
AVLNode<Key, Value>*
AVLTree<Key, Value>::AVLFind(const Key& key) const {
  AVLNode<Key, Value> *curr = static_cast<AVLNode<Key,Value>*>(this->root_);

  while (curr != nullptr) {
    Key currKey = curr->getKey();
    if (key < currKey) {
      curr = curr->getLeft();
    }
    else if (key > currKey) {
      curr = curr->getRight();
    }
    else { // keys ==
      return curr;
    }
  }

  return NULL;
}

template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    int8_t tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}


#endif
