#include "avlbst.h"
#include <iostream>
#include <string>
#include <utility>

using namespace std;

int main() 
{
    AVLTree<int, string> tree;
    tree.insert(std::make_pair(5, "five"));
    tree.insert(std::make_pair(3, "three"));
    tree.insert(std::make_pair(1, "one"));
    tree.insert(std::make_pair(2, "two"));
    tree.insert(std::make_pair(4, "four"));
    tree.insert(std::make_pair(0, "zero"));
    tree.insert(std::make_pair(8, "eight"));
    tree.insert(std::make_pair(7, "seven"));
    tree.insert(std::make_pair(9, "nine"));

    cout << "Tree before rotations: " << endl;
    tree.print();

    AVLNode<int, string>* root = tree.getRoot();
    AVLNode<int, string>* three = root->getLeft();
    tree.rotateRight(three);
    cout << "Tree after rotations: " << endl;
    tree.print();

    return 0;
}