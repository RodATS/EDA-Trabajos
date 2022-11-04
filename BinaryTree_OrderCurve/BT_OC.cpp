#include<iostream>
#include "BTree.h"
using namespace std;


//0x0F = 15  0x5 = 5  0x3 = 3   0x00FF=255
static const int B[] = {0x55555555, 0x33333333, 0x0F0F0F0F, 0x00FF00FF};
static const int S[] = {1, 2, 4, 8};
int zorder2D(int x, int y){
  /* operador | or como enteros: suma numeros
    operdaor << como enteros multiplica
    operdaor & and: 100 & 110 = 100
*/
    x = (x | (x << S[3])) & B[3]; 
    x = (x | (x << S[2])) & B[2];
    x = (x | (x << S[1])) & B[1];
    x = (x | (x << S[0])) & B[0];
  
    y = (y | (y << S[3])) & B[3];
    y = (y | (y << S[2])) & B[2];
    y = (y | (y << S[1])) & B[1];
    y = (y | (y << S[0])) & B[0];
    return x | (y << 1);
}

int res[1000][1000];



int main() {
  
  //Binary tree
  struct node *root = newNode(0);
  //order curve
   int dim;
  cin>>dim;
    const int nx=dim,ny=dim;
    //int res[ny][nx];

    for(int y=0; y<ny; y++){
        for(int x=0; x<nx; x++){
            res[y][x] = zorder2D(x,y);
          
          //Binary tree
            if(x>0)
            Insert(root,res[y][x]);
          //---------------------------
        }
    }
    for(int y=0; y<ny; y++){
        for(int x=0; x<nx; x++){
            cout<<res[y][x]<<"\t";
        }
        printf("\n");
    }


  
  /*
  Insert(root,2);
  Insert(root,3);
  Insert(root,4);
  Insert(root,5);
  Insert(root,6);*/

  cout << "\n\npreorder traversal: ";
  traversePreOrder(root);
  cout << "\n\nInorder traversal: ";
  traverseInOrder(root);
  cout << "\n\nPostorder traversal: ";
  traversePostOrder(root);
}
