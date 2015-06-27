/*** Post-order traversal of ***
  ** binary tree             ***
  ** in non recursive manner **/

#include <iostream>
#include <vector>
#include <map>

using namespace std;

struct bnode {
  int data;
  bnode *left;
  bnode *right;
};

void print(bnode* root) {
    if (root) {
        print(root->left);
        cout << "Inorder Data: " << root->data << endl;
        print(root->right);
    }
}


map<int, bool> pushed;

void SetPushed(bnode* node) {
  if (node) {
    pushed[node->data] = true;
  }
}

bool NotPushed(bnode* node) { // Assumption: unique node data
  if (!node)
    return true;

  if (pushed.find(node->data) == pushed.end()) {
    return true;
  }
  return false;
}

void PostOrderNonRec(bnode* root) {

  vector<bnode*> Stack;

  while (true) {

    while (root) {

      Stack.push_back(root);  // put this node in stack

      if (root->right && NotPushed(root->right)) { // Don't traverse child again
        SetPushed(root->right);
        Stack.push_back(root->right);  // put right child in stack
      }

      if (NotPushed(root->left)) { // Dont't traverse child again
        SetPushed(root->left);
        root = root->left; // go to left child
      }
      else {  // Both children already traversed
        break;
      }
    }

    if (Stack.empty()) {
      break;
    }

    bnode* node = (Stack.back());
    Stack.pop_back();

    cout << "Postorder Data : " << node->data << endl;

    if (Stack.empty()) {
      break;
    }

    root = (Stack.back());
    Stack.pop_back();
  }
}


int main() {

  bnode* root = new bnode;
  root->data = 1;

  bnode* node11 = new bnode;
  node11->data = 2;

  bnode* node12 = new bnode;
  node12->data = 3;

  bnode* node21 = new bnode;
  node21->data = 4;

  bnode* node22 = new bnode;
  node22->data = 5;

  bnode* node23 = new bnode;
  node23->data = 6;

  bnode* node31 = new bnode;
  node31->data = 7;

  bnode* node32 = new bnode;
  node32->data = 8;

  bnode* node33 = new bnode;
  node33->data = 9;

  root->left = node11;
  root->right = node12;

  node11->left = node21;
  node11->right = node22;

  node12->left = NULL;
  node12->right = node23;

  node21->left = node31;
  node21->right = node32;

  node22->left = NULL;
  node22->right = node33;

  node23->left = NULL;
  node23->right = NULL;

  node31->left = NULL;
  node31->right = NULL;

  node32->left = NULL;
  node32->right = NULL;

  node33->left = NULL;
  node33->right = NULL;

  print (root);
  PostOrderNonRec(root);

  return 0;
}
