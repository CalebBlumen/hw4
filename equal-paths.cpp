#ifndef RECCHECK
//if you want to add any #includes like <iostream> you must do them here (before the next endif)

#endif

#include "equal-paths.h"
using namespace std;


// You may add any prototypes of helper functions here
int getDepthLeft(Node* root, int depth=0) {
  if (root == nullptr) {
    return depth;
  }
  if (root->left == nullptr) {
    return getDepthLeft(root->right,depth+1);
  }
  return getDepthLeft(root->left,depth+1);
}

int getDepthRight(Node* root, int depth=0) {
  if (root == nullptr) {
    return depth;
  }
  if (root->right == nullptr) {
    return getDepthRight(root->left,depth+1);
  }
  return getDepthRight(root->right,depth+1);
}

bool equalPaths(Node * root)
{
  if (root == nullptr) {
    return true;
  }
  int depthLeft = getDepthLeft(root);
  int depthRight = getDepthRight(root);
  if (root->left != nullptr && root->right != nullptr && depthLeft != depthRight) {
    return false;
  }

  bool left = equalPaths(root->left);
  bool right = equalPaths(root->right);

  if (left && right) {
    return true;
  }
  return false;
}

