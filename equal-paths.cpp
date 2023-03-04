#ifndef RECCHECK
//if you want to add any #includes like <iostream> you must do them here (before the next endif)
#endif

#include "equal-paths.h"
using namespace std;


// You may add any prototypes of helper functions here
bool imbalance(Node* root);
bool isLeaf(Node* root);

bool equalPaths(Node * root)
{
    // Add your code below
    if (!root) return true;
    if (imbalance(root)) return false;
    
    bool left = true, right = true;
    if (root->left) left = equalPaths(root->left);
    if (root->right) right = equalPaths(root->right);

    return left && right;
}

bool imbalance(Node* root)
{    
    if (root->left && root->right) {
        return isLeaf(root->left) != isLeaf(root->right);
    }
    return false;
}

bool isLeaf(Node* root)
{
    return !root->left && !root->right;
}