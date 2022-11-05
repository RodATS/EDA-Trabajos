#include <stdio.h>
#include<iostream>
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

//Busqueda de coordenadas

void Buscar_coor(int b, int &x, int &y, int nx, int ny){
  for(int y1=0; y1<ny; y1++){
        for(int x1=0; x1<nx; x1++){
            if(b==res[y1][x1])
            {
              x=x1;y=y1;
              return;
            }
          }
    }

}

//--------------------

int main()
{    
    int dim;
  cin>>dim;
    const int nx=dim,ny=dim;
    //int res[ny][nx];

    for(int y=0; y<ny; y++){
        for(int x=0; x<nx; x++){
            res[y][x] = zorder2D(x,y);  
        }
    }
    for(int y=0; y<ny; y++){
        for(int x=0; x<nx; x++){
            cout<<res[y][x]<<"\t";
        }
        printf("\n");
    }

  //Busqueda con iteradores
    int dato_b;
    cout<<"\ndato que se quiere buscar y su región: ";cin>>dato_b;
    int X,Y;
    Buscar_coor(dato_b, Y,X, nx, ny);
  
    for(int indice_f=0; indice_f<=X; indice_f++){
        for(int indice_col=0; indice_col<=Y; indice_col++){
            cout<<res[indice_f][indice_col]<<"\t";
        }
        printf("\n");
    }
    //--------------------------------
    
    return 0;
}
