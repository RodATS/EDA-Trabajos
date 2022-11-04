#ifndef BTREE_H
#define BTREE_H



#include <stdlib.h>

#include <iostream>

using namespace std;

struct node {
  int data;
  int hijos=0;
  struct node *left;
  struct node *right;
};

// New node creation
struct node *newNode(int data) {
  struct node *node = (struct node *)malloc(sizeof(struct node));

  node->data = data;

  node->left = NULL;
  node->right = NULL;
  return (node);
}

void Insert(struct node *r,int data){
  if(r->hijos==0){
    r->left = newNode(data);
    r->hijos++;
    return;
  }
  if(r->hijos==1)
    {
    r->right = newNode(data);
    r->hijos++;
    return;
  }

  //descender
  if(r->hijos==2){
    if(r->left->hijos < 2){
      Insert(r->left,data); return ;
      }
    
    if(r->left->hijos >= 2 and r->right->hijos < 2)
      { Insert(r->right,data); return ;
      }
    Insert(r->left,data); return ;
  }
  
}


// Traverse Preorder
void traversePreOrderInd(struct node *temp, int x, int y) {
  if (temp != NULL) {
    if(temp->data == x){
      cout << " " << temp->data;
      x=-1;
    }
    else{
        if(temp->data == y){
          cout << " " << temp->data;
          y=-1;
        }
  
        else {
          if(y==-1 or x==-1)
            cout << " " << temp->data;
          }
    }
    /*
    traversePreOrder(temp->left);
    traversePreOrder(temp->right);
    */

    traversePreOrderInd(temp->left,x,y);
    traversePreOrderInd(temp->right,x,y);
  }
}

// Traverse Inorder
void traverseInOrderInd(struct node *temp,int x, int y) {
  if (temp != NULL) {
    traverseInOrderInd(temp->left,x,y);
    
    if(temp->data == x){
      cout << " " << temp->data;
      x=-1;
    }
    else{
        if(temp->data == y){
          cout << " " << temp->data;
          y=-1;
        }
  
        else {
          if(y==-1 or x==-1)
            cout << " " << temp->data;
          }
    }
    traverseInOrderInd(temp->right,x,y);
  }
}

// Traverse Postorder
void traversePostOrderInd(struct node *temp,int x, int y) {
  if (temp != NULL) {
    traversePostOrderInd(temp->left,x,y);
    traversePostOrderInd(temp->right,x,y);
    if(temp->data == x){
      cout << " " << temp->data;
      x=-1;
    }
    else{
        if(temp->data == y){
          cout << " " << temp->data;
          y=-1;
        }
  
        else {
          if(y==-1 or x==-1)
            cout << " " << temp->data;
          }
    }
  }
}

//Impresiones completas
// Traverse Preorder
void traversePreOrder(struct node *temp) {
  if (temp != NULL) {
    cout << " " << temp->data;
    traversePreOrder(temp->left);
    traversePreOrder(temp->right);
  }
}

// Traverse Inorder
void traverseInOrder(struct node *temp) {
  if (temp != NULL) {
    traverseInOrder(temp->left);
    cout << " " << temp->data;
    traverseInOrder(temp->right);
  }
}

// Traverse Postorder
void traversePostOrder(struct node *temp) {
  if (temp != NULL) {
    traversePostOrder(temp->left);
    traversePostOrder(temp->right);
    cout << " " << temp->data;
  }
}

#endif
