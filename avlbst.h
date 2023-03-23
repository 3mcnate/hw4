#ifndef AVLBST_H
#define AVLBST_H

#include <iostream>
#include <exception>
#include <stdexcept>
#include <cstdlib>
#include <cstdint>
#include <algorithm>
#include <cassert>
#include "bst.h"

//#define DEBUG_AVL

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
    
    #ifdef DEBUG_AVL
    AVLNode<Key,Value>* getRoot() { return static_cast< AVLNode<Key,Value>* >(BinarySearchTree<Key,Value>::root_); }
    #endif 

protected:
    virtual void nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2);

    // Add helper functions here
#ifdef DEBUG_AVL
public:
#endif
    void rotateRight(AVLNode<Key,Value>* z);
    void rotateLeft(AVLNode<Key,Value>* x);

    void insertFix(AVLNode<Key,Value>* p, AVLNode<Key,Value>* n);
    void removeFix(AVLNode<Key,Value>* node, int8_t diff);

    /** 
     * @param n node
     * @param p parent of node
     * @brief returns -1 if n is left child of p, +1 if n is right child of p, 0 if p is null
    */
    int8_t leftOrRightChild(AVLNode<Key,Value>* n, AVLNode<Key,Value>* p);

    void removeHelper(AVLNode<Key, Value>* curr, AVLNode<Key, Value>* parent, AVLNode<Key, Value>* child);

    AVLNode<Key, Value>* cast(Node<Key, Value>* n) {
        return static_cast<AVLNode<Key,Value>*>(n);
    }

    // bool verifyNodeBalances() const;
    // int verifyNodeBalancesHelper(AVLNode<Key,Value>* node) const;
};


template <class Key, class Value>
void AVLTree<Key,Value>::rotateRight(AVLNode<Key,Value>* z)
{
    AVLNode<Key,Value>* p = z->getParent();
    AVLNode<Key,Value>* y = z->getLeft();
    AVLNode<Key,Value>* c = y->getRight();

    // change either p's child or root to y.
    if (p == NULL) {
        BinarySearchTree<Key,Value>::root_ = y;
    }
    else {
        // update parent of z
        if (p->getLeft() == z) {
            p->setLeft(y);
        }
        else {
            p->setRight(y);
        }
    }
    y->setParent(p);
    y->setRight(z);
    z->setParent(y);
    z->setLeft(c);

    // update c's parent
    if (c) {
        c->setParent(z);
    }
}

template <class Key, class Value>
void AVLTree<Key,Value>::rotateLeft(AVLNode<Key,Value>* x)
{
    AVLNode<Key,Value>* p = x->getParent();
    AVLNode<Key,Value>* y = x->getRight();
    AVLNode<Key,Value>* b = y->getLeft();

    // update parent of the entire subtree, or root if it doesn't exist
    if (p == NULL) {
        BinarySearchTree<Key,Value>::root_ = y;
    }
    else {
        if (p->getLeft() == x) {
            p->setLeft(y);
        }
        else {
            p->setRight(y);
        }
    }
    y->setParent(p);
    y->setLeft(x);
    x->setParent(y);
    x->setRight(b);

    // update b's parent, if it exists
    if (b) {
        b->setParent(x);
    }
}

template<class Key, class Value>
int8_t AVLTree<Key, Value>::leftOrRightChild(AVLNode<Key,Value>* n, AVLNode<Key,Value>* p)
{
    if (!p) return 0;
    if (p->getLeft() == n) {
        return -1;
    }
    return 1;
}

/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert(const std::pair<const Key, Value> &new_item)
{
    if (this->empty()) {
        this->root_ = new AVLNode<Key, Value>(new_item.first, new_item.second, NULL);
        return;
    }

    AVLNode<Key, Value>* curr = static_cast<AVLNode<Key,Value>*>(this->root_);
    const Key& key = new_item.first;
    const Value& value = new_item.second;
    
    // walk the tree
    while (true) {
        // edit value already in tree
        if (key == curr->getKey()) {
            curr->setValue(value);
            return;
        }
        if (key < curr->getKey()) {
            AVLNode<Key, Value>* left = curr->getLeft();
            if (left == NULL) {
                curr->setLeft(new AVLNode<Key, Value>(key, value, curr));
        
                // update curr's (aka p's) balance and fix if needed
                if (curr->getBalance() == 1) curr->setBalance(0);
                else {
                    curr->setBalance(-1);
                    insertFix(curr, curr->getLeft());
                }
                return;
            }
            curr = left;
        }
        else {
            AVLNode<Key, Value>* right = curr->getRight();
            if (right == NULL) {
                curr->setRight(new AVLNode<Key,Value>(key, value, curr));
                
                // update curr's (aka p's) balance and fix if needed
                if (curr->getBalance() == -1) curr->setBalance(0);
                else {
                    curr->setBalance(1);
                    insertFix(curr, curr->getRight());
                }
                return;
            }
            curr = right;
        }
    }
}

template<class Key, class Value>
void AVLTree<Key,Value>::insertFix(AVLNode<Key,Value>* p, AVLNode<Key,Value>* n)
{
    if (p == NULL) return;
    AVLNode<Key,Value>* g = p->getParent();
    if (g == NULL) return;

    int direction = leftOrRightChild(p, g); // either -1 or +1
    assert(direction == -1 || direction == 1);
    g->updateBalance(direction);

    // case 1: b(g) = 0, return
    if (g->getBalance() == 0) {
        return;
    }
    // case 2: b(g) = 1 or -1, recurse
    else if (g->getBalance() == direction) {
        insertFix(g, p);
    }
    // case 3: b(g) = 2 or -2, rotate and return
    else {
        // zig-zig
        if (p->getBalance() + direction == g->getBalance()) {
            if (direction == -1) {
                rotateRight(g);
            }
            else {
                rotateLeft(g);
            }
            // update balances
            p->setBalance(0);
            g->setBalance(0);
        }
        // zig-zag
        else {
            if (direction == -1) {
                rotateLeft(p);
                rotateRight(g);
            }
            else {
                rotateRight(p);
                rotateLeft(g);
            }

            // update balances
            // case 3a: b(n) == direction
            if (n->getBalance() == direction) {
                p->setBalance(0);
                g->setBalance(-direction);
            }
            // case 3b: b(n) == 0
            if (n->getBalance() == 0) {
                p->setBalance(0);
                g->setBalance(0);
            }
            // case 3c: b(n) == -direction
            if (n->getBalance() == -direction) {
                p->setBalance(direction);
                g->setBalance(0); 
            }
            // for all cases
            n->setBalance(0);
        }
    }
}


/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::remove(const Key& key)
{
    AVLNode<Key, Value>* curr = cast(this->root_);


    #ifdef DEBUG_AVL
        std::cout << "Old tree: " << std::endl;
        this->print();

        std::cout << "Removing Key: " << key << std::endl;

        typename BinarySearchTree<Key,Value>::PrintTreeOnDestruct p(this);
    #endif
    
    // find the node
    while (true) {
        if (curr == NULL) return; // key doesn't exist
        if (key == curr->getKey()) {
            break;
        }
        if (key < curr->getKey()) {
            curr = curr->getLeft();
        }
        else {
            curr = curr->getRight();
        }
    }

    AVLNode<Key,Value> *left = curr->getLeft(), 
                       *right = curr->getRight(),
                       *parent = curr->getParent();

    // node has two children, we need to swap before doing anything else
    if (left && right) {
        AVLNode<Key,Value>* pred = cast(this->predecessor(curr));
        nodeSwap(curr, pred);
        if (curr == this->root_) {
            this->root_ = pred;
        }

        // update pointers after swapping
        parent = curr->getParent(); 
        left = curr->getLeft(); 
        right = curr->getRight(); 
    }

    // updates balances (we had to swap first, because now the node to remove is in a different place)
    int8_t diff;
    if (parent) {
        if (parent->getLeft() == curr) diff = 1;
        else diff = -1;
    }

    assert(!left || !right);
    
    // check if curr is root. If it is, there can only be 2 nodes in the tree, since it can only have one child.
    // otherwise, it would have swapped with its predecessor
    if (this->root_ == curr) { 
        if (left) {
            this->root_ = left;
            left->setParent(NULL);
        }
        else if (right) {
            this->root_ = right;
            right->setParent(NULL);
        }
        else {
            this->root_ = NULL;
        }
        delete curr;
        return;
    }
 
    // update pointers of nodes in preparation to delete
    if (left) removeHelper(curr, parent, left);
    else if (right) removeHelper(curr, parent, right);
    else removeHelper(curr, parent, NULL);

    delete curr;

    removeFix(parent, diff);
}


template<class Key, class Value>
void AVLTree<Key, Value>::removeHelper(AVLNode<Key, Value>* curr, AVLNode<Key, Value>* parent, AVLNode<Key, Value>* child)
{
    if (curr == this->root_) {
            this->root_ = child;
    }
    else {
        this->editParentToRemove(curr, parent, child);
    }
    if (child) child->setParent(parent);
}


template<class Key, class Value>
void AVLTree<Key, Value>::removeFix(AVLNode<Key,Value>* n, int8_t diff)
{
    if (n == NULL) return;
    AVLNode<Key, Value>* p = n->getParent();
    int8_t nextdiff = -1 * leftOrRightChild(n, p);

    // case 1:
    if (n->getBalance() + diff == 2*diff) {
        AVLNode<Key, Value>* c;
        if (diff == -1) c = n->getLeft();
        else c = n->getRight();
        
        if (c->getBalance() == diff) { // case 1a: zig-zig case
            if (diff == -1) rotateRight(n);
            else rotateLeft(n);
            n->setBalance(0);
            c->setBalance(0);
            removeFix(p, nextdiff);
            return;
        }
        if (c->getBalance() == 0) { // case 1b: zig-zig case
            if (diff == -1) rotateRight(n);
            else rotateLeft(n);
            n->setBalance(diff);
            c->setBalance(-diff);
            return;
        }
        if (c->getBalance() == -diff) { // case 1c: zig-zag case
            AVLNode<Key, Value>* g;
            if (diff == -1) g = c->getRight();
            else g = c->getLeft();
            
            if (diff == -1) {
                rotateLeft(c);
                rotateRight(n);
            }
            else {
                rotateRight(c);
                rotateLeft(n);
            }

            if (g->getBalance() == -diff) {
                n->setBalance(0);
                c->setBalance(diff);
                g->setBalance(0);
            }
            else if (g->getBalance() == 0) {
                n->setBalance(0);
                c->setBalance(0);
                g->setBalance(0);
            }
            else if (g->getBalance() == diff) {
                n->setBalance(-diff);
                c->setBalance(0);
                g->setBalance(0);
            }
            removeFix(p, nextdiff);
            return;
        }
    }
    // case 2
    if (n->getBalance() + diff == diff) {
        n->setBalance(diff);
        return;
    }
    if (n->getBalance() + diff == 0) {
        n->setBalance(0);
        removeFix(p, nextdiff);
    }
}


template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap(AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    int8_t tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}


#endif
