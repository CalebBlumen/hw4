#include <iostream>
#include <map>
#include "bst.h"
#include "avlbst.h"


using namespace std;


int main(int argc, char *argv[])
{
    // Binary Search Tree tests
  /*cout << "bal test: " << endl;
  BinarySearchTree<int, int> bst;

  bst.insert(std::make_pair(1, 9));
  bst.insert(std::make_pair(5, 8));
  bst.insert(std::make_pair(3, 159));
  cout << "Binary Search Tree contents:" << endl;
  cout << "Binary Search Tree contents:" << endl;
  for(BinarySearchTree<int,int>::iterator it = bst.begin(); it != bst.end(); ++it) {
      cout << it->first << " " << it->second << endl;
  }
  cout << "Balanced: " << bst.isBalanced() << endl;

	  
    
    BinarySearchTree<char,int> bt;
    bt.insert(std::make_pair('a',1));
    bt.insert(std::make_pair('b',2));
  
    cout << "Binary Search Tree contents:" << endl;
    for(BinarySearchTree<char,int>::iterator it = bt.begin(); it != bt.end(); ++it) {
        cout << it->first << " " << it->second << endl;
    }
    cout << "Erasing b" << endl;
    bt.remove('b');

    for(BinarySearchTree<char,int>::iterator it = bt.begin(); it != bt.end(); ++it) {
        cout << it->first << " " << it->second << endl;
    }
    if(bt.find('b') != bt.end()) {
        cout << "Found b" << endl;
    }
    else {
        cout << "Did not find b" << endl;
    }
    cout << "Balanced: " << bt.isBalanced() << endl;
    cout << "Erasing b" << endl;
    bt.remove('b');

    cout << "bt contents: " << endl;
    for(BinarySearchTree<char,int>::iterator it = bt.begin(); it != bt.end(); ++it) {
        cout << it->first << " " << it->second << endl;
    }
    */
    
    //AVL Tree Tests

    AVLTree<int, double> bst;
	  bst.insert(std::make_pair(2, 1.0));
	  bst.insert(std::make_pair(3, 1.0));
	  bst.insert(std::make_pair(1, 1.0));
	  bst.insert(std::make_pair(4, 1.0));

   
    cout << "\nAVLTree contents:" << endl;
    
    bst.print();
    
    
    cout << "Erasing" << endl;
  
	  bst.remove(1);

    bst.print();
    
    return 0;
}
