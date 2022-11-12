#include <cmath>
#include <iostream>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>

using namespace std;

// Estructura para RGB
struct pixel {
  unsigned char R;
  unsigned char G;
  unsigned char B;
};

// Cálculo de distancia hacia los centroides
float distancia(pixel a, pixel b) {
  float r_ = pow((int)a.R - (int)b.R, 2);
  float b_ = pow((int)a.B - (int)b.B, 2);
  float g_ = pow((int)a.G - (int)b.G, 2);
  float dist = sqrt(r_ + b_ + g_);
  return dist;
}

// Verificacion de igualdad de centroides
bool compare(pixel a, pixel b) {
  if (a.R == b.R && a.B == b.B && a.G == b.G)
    return true;
  return false;
}

// Verificar todos los cemtroides
bool compare_all(vector<pixel> c, vector<pixel> caux) {
  for (int i = 0; i < caux.size(); i++) {
    if (!compare(c[i], caux[i]))
      return false;
  }
  return true;
}

// Crear pixel vacío
pixel p_null() {
  pixel null;
  null.R = (unsigned char)1;
  null.G = (unsigned char)1;
  null.B = (unsigned char)1;
  return null;
}

void promedio(pixel &n, vector<pixel *> aux) {
  int r, g, b;
  for (int i = 0; i < aux.size(); i++) {
    r += (int)(*aux[i]).R;
    g += (int)(*aux[i]).G;
    b += (int)(*aux[i]).B;
  }
  r = r / aux.size();
  b = b / aux.size();
  g = g / aux.size();
  n.R = (unsigned char)r;
  n.G = (unsigned char)g;
  n.B = (unsigned char)b;
}

// Porcesado de image --> Read, Write
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
        count++;
      }
    }
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
      }
      fwrite(data, sizeof(unsigned char), rowstep, aux);
      free(data);
    }
    fclose(aux);
  }

  // Procesado de datos, y clusterización para la segmentación de la IMG
  void k_means(int clusters, int attempts = 20) {
    vector<pixel> centroides(clusters);
    vector<pixel> caux(clusters);
    vector<vector<pixel *>> groups(clusters);
    int index;
    for (int i = 0; i < clusters; i++) {
      centroides[i] = rgb_data[rand() % height][rand() % width];
      if (i > 0 && compare(centroides[i], centroides[i - 1]))
        centroides[i] = rgb_data[rand() % height][rand() % width];
      caux[i] = p_null();
    }
    bool igual = false;
    while (attempts--) {
      for (int c = 0; c < clusters; c++) {
        if (!groups[c].empty())
          groups[c].erase(groups[c].begin(),
                          groups[c].begin() + groups[c].size());
      }

      for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
          float min = 99999;
          for (int c = 0; c < clusters; c++) {
            float aux = distancia(centroides[c], rgb_data[i][j]);
            if (aux < min) {
              min = aux;
              index = c;
            }
          }
          groups[index].push_back(&rgb_data[i][j]);
        }
      }
      int cont = 0;
      for (int i = 0; i < clusters; i++) {
        caux[i] = centroides[i];
        promedio(centroides[i], groups[i]);

        if (compare(caux[i], centroides[i])) {
          cont++;
        }
      }
      if (cont == clusters)
        break;
    }

    for (int c = 0; c < clusters; c++) {
      if (!groups[c].empty()) {
        for (int i = 0; i < groups[c].size(); i++) {
          (*groups[c][i]) = centroides[c];

          /*(*groups[c][i]).R = (centroides[c]).R;
          (*groups[c][i]).G = (centroides[c]).G;
          (*groups[c][i]).B = (centroides[c]).B;*/
        }
      }
    }
  }
};

int main() {
  char *filename = "image.bmp";
  img prueba(filename);
  prueba.k_means(2);
  //prueba.write();
}
