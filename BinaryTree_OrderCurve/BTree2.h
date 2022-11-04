#ifndef BTREE_H
#define BTREE_H

#include <stdlib.h>

#include <iostream>
#include <vector>

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




vector<int> PrO;
vector<int> IO;
vector<int> PsO;
//Impresiones completas
// Traverse Preorder
void traversePreOrder(struct node *temp) {
  if (temp != NULL) {
    cout << " " << temp->data;
    PrO.push_back(temp->data);
    traversePreOrder(temp->left);
    traversePreOrder(temp->right);
  }
}

// Traverse Inorder
void traverseInOrder(struct node *temp) {
  if (temp != NULL) {
    traverseInOrder(temp->left);
    cout << " " << temp->data;
    IO.push_back(temp->data);
    traverseInOrder(temp->right);
  }
}

// Traverse Postorder
void traversePostOrder(struct node *temp) {
  if (temp != NULL) {
    traversePostOrder(temp->left);
    traversePostOrder(temp->right);
    cout << " " << temp->data;
    PsO.push_back(temp->data);
  }
}



//Busqueda

// Traverse Preorder
void traversePreOrderInd(struct node *temp, int x, int y) {
  if (temp != NULL) {
    for(int i=0;i<PrO.size();i++)
    if(PrO[i] == x){
      cout << " " << PrO[i];
      x=-1;
    }
    else{
        if(PrO[i] == y){
          cout << " " << PrO[i];
          y=-1;
        }
  
        else {
          if(y==-1 and x==-1)
            return;
          if(y==-1 or x==-1)
            cout << " " << PrO[i];
          }
      }
     
  }
}

// Traverse Inorder
void traverseInOrderInd(struct node *temp,int x, int y) {
  if (temp != NULL) {
    for(int i=0;i<IO.size();i++)
    if(IO[i] == x){
      cout << " " << IO[i];
      x=-1;
    }
    else{
        if(IO[i] == y){
          cout << " " << IO[i];
          y=-1;
        }
  
        else {
          if(y==-1 and x==-1)
            return;
          if(y==-1 or x==-1)
            cout << " " << IO[i];
          }
          
      }
     
  }
}



// Traverse Postorder
void traversePostOrderInd(struct node *temp,int x, int y) {
  if (temp != NULL) {
   for(int i=0;i<PsO.size();i++)
    if(PsO[i] == x){
      cout << " " << PsO[i];
      x=-1;
    }
    else{
        if(PsO[i] == y){
          cout << " " << PsO[i];
          y=-1;
        }
  
        else {
          if(y==-1 and x==-1)
            return;
          if(y==-1 or x==-1)
            cout << " " << PsO[i];
          }
      }
  }
}


//funciones juntas
//1
void PreOrder(struct node *temp,int x, int y){
  cout << "\n\npreorder traversal: ";
  traversePreOrder(temp);
  cout << "\n\npreorder traversal: ";
  traversePreOrderInd(temp,x,y);
}

//2
void InOrder(struct node *temp,int x, int y){
cout << "\n\nInorder traversal: ";
  traverseInOrder(temp);
  cout << "\n\nInorder traversal: ";
  traverseInOrderInd(temp,x,y);
}
 //3 

void PostOrder(struct node *temp,int x, int y){
  cout << "\n\nPostorder traversal: ";
  traversePostOrder(temp);
  cout << "\n\nPostorder traversal: ";
  traversePostOrderInd(temp,x,y);
}
#endif
