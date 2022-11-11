#include <iostream>
#include <cmath>
#include "BMP.h"
#include <list>

using namespace  std;

const int dim=4;
int R[dim][dim];
int G[dim][dim];
int B[dim][dim];
  

void swapp_char(unsigned char &tmp, unsigned char *data, int j){
  tmp = data[j];
  data[j] = data[j+2];
  data[j+2] = tmp;
}


unsigned char* LeerBMP(char* filename)
{
    FILE* f = fopen(filename, "rb");

    if(f == NULL)
        throw "Argument Exception";

    unsigned char info[54];//windows bitmap
    fread(info, sizeof(unsigned char), 54, f);

    // extraer la altura y ancho header BMP
    int height = *(int*)&info[22];
    int width = *(int*)&info[18];
    cout << endl;

    int row_padded = (width*3 + 3) & (~3);
    unsigned char* data = new unsigned char[row_padded];
    unsigned char tmp;

    
    for(int i = 0; i < height; i++)
    {
        fread(data, sizeof(unsigned char), row_padded, f);
        for(int j = 0; j < width*3; j += 3)
        {
            // Lo invierte por la forma de leerlo (B, G, R) a (R, G, B)
          swapp_char(tmp,data,j);
          R[i][j] = (int)data[j];
          G[i][j] = (int)data[j+1];
          B[i][j] = (int)data[j+2];
        }
    }

    fclose(f);
    return data;
}


//--k means
int menor_dis(list<int> *clusx, list<int> *clusy,list<int> *clusz, int datox,int datoy, int datoz, int nk){
  int indice=0;
  int res = 10001;
  
  for(int i=0;i<nk;i++){
    list<int> :: iterator h = clusx[i].begin(),
    j= clusy[i].begin(), 
    k = clusy[i].begin();
    
    int nres= sqrt(pow(datox-*h,2)+pow(datoy - *j,2)+ pow(datoz - *k,2));
    
    if(res > nres)
      indice = i;
  }
  cout<<indice<<endl;
  return indice;
}
void k_means(int k){
  
  list<int> *clusx, *clusy, *clusz;
  clusx=new list<int>[k],  
  clusy=new list<int>[k],
  clusz=new list<int>[k];

  //dar valores al azar
  for(int i=0;i<k;i++){
    clusx[i].push_back(i*10);
    clusy[i].push_back(i*10);
    clusz[i].push_back(i*10);
  }

  //colocar cada punto al cluster que corresponde - aca peta
  for(int i=0;i<dim;i++){
    for(int j=0;j<dim;i++){
      int place = menor_dis(clusx, clusy, clusz, R[i][j],G[i][j],B[i][j], k);
      clusx[place].push_back(R[i][j]);
      clusy[place].push_back(G[i][j]);
      clusz[place].push_back(B[i][j]);
    }
  }
/*
  //sacar promedio y revaluar los cluster
  int promedioR,promedioG, promedioB, promedioantR, promedioantG, promedioantB;
  for(int i=0;i<k;i++){
    list<int> :: iterator h = clusx[i].begin(),
    j= clusy[i].begin(), 
    k = clusy[i].begin();
    h++;j++;k++;
    int count=1;
    promedioR=0;promedioG=0; promedioB=0; 
    for(;h != clusx[i].end(); h ++)
      {
      promedioR+=*h; promedioG+=*j; promedioB+=*k;count++;
      }
    promedioR = promedioR/count;
    promedioG = promedioG/count;
    promedioB = promedioB/count;

    h = clusx[i].begin(),
    j= clusy[i].begin(), 
    k = clusy[i].begin();

    *h=promedioR;
    *j=promedioG;
    *k=promedioB;
  }
*/
  
}

int main() {
  k_means(2);
}
