
#include "omp.h"
#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <vector>
#include <list>

//Otro código en C++
// https://codereview.stackexchange.com/questions/191081/k-means-image-segmentation-algorithm

using namespace std;

struct pixel {
  unsigned char R;
  unsigned char G;
  unsigned char B;
};

struct img {
  int width = 0, height = 0;
  int rowstep = 0;
  unsigned char *data;
  unsigned char header[54];
  pixel **rgb_data;

  img() {}
  img(char *filename) { ReadBMP(filename); }

  img(pixel **data, int width, int height) {
    this->rgb_data = data;
    this->width = width;
    this->height = height;
    this->rowstep = width * 3;
  }

  void ReadBMP(char *filename) {
    FILE *f = fopen(filename, "rb");
    if (f == NULL)
      cout << "Archivo no especificado";

    fread(header, sizeof(unsigned char), 54, f);
    width = *(int *)&header[18];
    height = *(int *)&header[22];

    rowstep = (width * 3 + 3) & (~3);
    unsigned char *data = new unsigned char[rowstep];
    unsigned char tmp;
    rgb_data = new pixel *[height];
    for (int i = 0; i < height; i++) {
      rgb_data[i] = new pixel[width];
    }
    for (int i = 0; i < height; i++) {
      int count = 0;
      fread(data, sizeof(unsigned char), rowstep, f);
      for (int j = 0; j < width * 3; j += 3) { // Convert (B, G, R) to (R, G, B)
        tmp = data[j];
        data[j] = data[j + 2];
        data[j + 2] = tmp;
        rgb_data[i][count].R = data[j];
        rgb_data[i][count].G = data[j + 1];
        rgb_data[i][count].B = data[j + 2];
        cout<<rgb_data[i][count].R <<"\t"<<rgb_data[i][count].G<<"\t"<<rgb_data[i][count].B<<endl;
        count++;
      }
    }
    cout<<endl;
    fclose(f);
  }

  void write() {
    char *filename = "comprimido.bmp";
    FILE *aux = fopen(filename, "w");
    unsigned char *data;
    fwrite(header, sizeof(unsigned char), 54, aux);
    for (int i = 0; i < height; i++) {
      data = (unsigned char *)malloc(sizeof(char) * rowstep);
      for (int j = 0; j < width * 3; j += 3) {
        data[j] = rgb_data[i][j / 3].B;
        data[j + 1] = rgb_data[i][j / 3].G;
        data[j + 2] = rgb_data[i][j / 3].R;
       // cout<<rgb_data[i][j / 3].R<<"\t"<<rgb_data[i][j / 3].G<<"\t"<<rgb_data[i][j / 3].B<<endl;
      }
      fwrite(data, sizeof(unsigned char), rowstep, aux);
      free(data);
    }
    fclose(aux);
  }



//formula distancia
  float distancia(pixel a, pixel b){
    return sqrt(pow((float)a.R - (float)b.R,2)+pow((float)a.G-(float)b.G,2)+pow((float)a.B-(float)b.B,2));
  }

//saber a que centroide pertenece cada pixel
  void dis(pixel *k, pixel aux, list<pixel> *dat, int n){
    float prom=distancia(k[0],aux);
    cout<<"prueba"<<endl;
    cout<<prom<<endl;
    int indice=0;
    for(int i=1;i<n;i++){
      cout<<distancia(k[i],aux)<<endl;
      if(distancia(k[i],aux) < prom){
        cout<<distancia(k[i],aux)<<endl;
        indice = i;
      }
    }
    cout<<(aux.R)<<"\t"<<aux.G<<"\t"<<aux.B<<endl;
    dat[indice].push_back(aux);
    return;
  }



//saber si el centroide actual es igual anterior
  bool comparar(pixel *k, pixel *k_ant, int n){
    bool res=true;
    for(int i=0;i<n;i++){
      if(k[i].R != k_ant[i].R && k[i].G != k_ant[i].G && k[i].B != k_ant[i].B)
        res=false;
    }
    return res;
  }




  void calculo_kmeans(pixel *k, pixel *k_ant, int n){
    
    list<pixel> *dat;
    dat = new list<pixel>[n];

    //colocar cada pixel en el centroide que le corresponde
    for(int i=0;i<height; i++){
      for(int j=0;j<width; j++){
        dis(k,rgb_data[i][j],dat, n);
         /*float prom=distancia(k[0],rgb_data[i][j]);
          int indice=0;
          for(int i=1;i<n;i++){
            if(distancia(k[i],rgb_data[i][j]) < prom){
              indice = i;
            }
          }
        cout<<indice<<endl;
          dat[indice].push_back(rgb_data[i][j]);
        */
      }
    }

    //hallar los nuevos centroides
    for(int i=0;i<n;i++){
      float promR, promG, promB;int count;
      cout<<"\t"<<i<<"\t: "<<dat[i].size()<<endl;
      list<pixel> :: iterator j;
      for(j=dat[i].begin(); j != dat[i].end();j++){
        promR += (float)(*j).R;
        promG += (float)(*j).G;
        promB += (float)(*j).B;
        count++;
        }
      k[i].R = (unsigned char) promR/count;
      k[i].G = (unsigned char) promG/count;
      k[i].B = (unsigned char) promB/count;
      
      
      
      }

    
    if(comparar(k, k_ant,n)==true){
      //reemplazar cada valor del pixel por sus centroides respectivos
       for(int i=0;i<n;i++){
      list<pixel> :: iterator j;
      for(j=dat[i].begin(); j != dat[i].end();j++){
        (*j).R = k[i].R;
        (*j).G = k[i].G;
        (*j).B = k[i].B;
        }
     /* cout<<"lo encontrók"<<endl;
         cout<<k[i].R<<"\t"<<k[i].G<<"\t"<<k[i].B<<endl;
         cout<<"matr"<<endl;
         cout<<(*j).R<<"\t"<<(*j).G<<"\t"<<(*j).B<<endl;*/
      }
      return;
    }
    else{
      
      //dat.clear(); //borrar los elementos del centroide 
      //creo que crear la lista con new no es necesario 
      //pero para borrarla?
      for(int i=0;i<n;i++){
        //dat[i].clear();// borrar los elementos de cada centroide
        cout<<"k"<<endl;
         cout<<k_ant[i].R<<"\t"<<k_ant[i].G<<"\t"<<k_ant[i].B<<endl;
      
        k_ant[i].R = k[i].R ;
        k_ant[i].G = k[i].G ;
        k_ant[i].B = k[i].B ;
        cout<<"k_ant"<<endl;
         cout<<k_ant[i].R<<"\t"<<k_ant[i].G<<"\t"<<k_ant[i].B<<endl;
      }
        
      
      calculo_kmeans(k,k_ant, n);

    }
    
    return;
  }

//aca inicia
  void kmeans(int k){
    pixel *ks= new pixel[k];//los centroides
    pixel *kant= new pixel[k];//los centroides para saber si son iguales y no se mueven
    
    for(int i=0;i<k;i++){
      ks[i].R = (unsigned char)(rand() % 255);
      ks[i].G = (unsigned char)(rand() % 180);
      ks[i].B = (unsigned char)(rand() % 200);
      }
    calculo_kmeans(ks,kant, k);

    
    
    cout<<"listo"<<endl;

    write();
  }
};


int main() {
  std::cout << "Hello World!\n";
  char *filename = "test3.bmp";
  img mario(filename);
  mario.kmeans(3);
}
