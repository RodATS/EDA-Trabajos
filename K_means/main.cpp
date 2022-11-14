#include <iostream>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <sys/types.h>
#include <limits.h>
#include <vector>
using namespace std;

typedef struct rgb 
{
    int r, g, b;
} rgb;

typedef struct img 
{
    unsigned char header[54];
    rgb **data;
    int height, width;
    int row_padded;
} img;

class Cluster 
{
    public:
    rgb color;
    int x, y;
    Cluster();
    Cluster(rgb, int, int);
};

class kmeans 
{
    public:
    int n;
    img *image;
    rgb *means;
    std::vector<Cluster> *clusters;
    kmeans(img*, int);
    void initialize();
    void clusterize(int);
};

Cluster::Cluster()
{}

Cluster::Cluster(rgb col, int ux, int uy): color(col), x(ux), y(uy)
{}

kmeans::kmeans(img *uimage, int uk): image(uimage), n(uk)
{
    means = new rgb[n];
    clusters = new std::vector<Cluster>[n];
}

void kmeans::initialize()
{
    for (int i = 0; i < n; i++) {
        int tx = (rand() % image->width);
        int ty = (rand() % image->height);
        means[i] = image->data[ty][tx];
    }
}

int rgbdistance(rgb a, rgb b)
{
    return sqrt(pow(a.r - b.r, 2) + pow(a.g - b.g, 2) + pow(a.b - b.b, 2));
}

void kmeans::clusterize(int iter)
{ 
    for (int i = 0; i < n; i++) {
        clusters[i].clear();
    }

    for (int i = 0; i < image->height; i++) 
    {
        for (int j = 0; j < image->width; j++) 
        {
            int dist = INT_MAX;
            int mink = -1;
            for (int k = 0; k < n; k++) 
            {
                int tdist = rgbdistance(means[k], image->data[i][j]);
                if (tdist < dist) {
                    dist = tdist;
                    mink = k;
                }
            }
            clusters[mink].push_back(Cluster(image->data[i][j], j, i));
        }
    }

    for (int i = 0; i < n; i++) 
    {
        rgb mean;
        mean.r = mean.g = mean.b = 0;
        for (auto j: clusters[i]) 
        {
            mean.r += j.color.r;
            mean.g += j.color.g;
            mean.b += j.color.b;
        }
        means[i].r = mean.r / clusters[i].size();
        means[i].g = mean.g / clusters[i].size();
        means[i].b = mean.b / clusters[i].size();
    }
    
    for (int i = 0; i < n; i++) 
    {
        for (auto j: clusters[i]) 
        {
            image->data[j.y][j.x] = means[i];
        }
    }
}

void write_modified(img image)
{
  char *fname = "adultered.bmp";
  FILE *res = fopen(fname, "w");
  unsigned char *data;
  cout <<"Archivo escrito: " << fname << " Width: " << image.width << " Height: " << image.height << endl;
  
  //printf("Name of wrote file: %s\nWidth: %d\nHeight: %d\n", fname, image.width, image.height);
  
    fwrite(image.header, sizeof(unsigned char), 54, res);
    for (int i = 0; i < image.height; i++) {
        data = (unsigned char *)malloc(sizeof(char) * image.row_padded);
        for (int j = 0; j < image.width * 3; j+=3) {
            data[j] = image.data[i][j / 3].b;
            data[j + 1] = image.data[i][j / 3].g;
            data[j + 2] = image.data[i][j / 3].r;
            //data[j] = image.data[i][j / 3].b + 20;
            //data[j + 1] = 0; //image.data[i][j / 3].g - 5;
            //data[j + 2] = image.data[i][j / 3].r + 20;
        }
        fwrite(data, sizeof(unsigned char), image.row_padded, res);
        free(data);
    }
    fclose(res);
    cout << "adultered.bmp generado" << endl;
}

img mainBMP(char* filename)
{
    int i;
    FILE* f = fopen(filename, "rb");
    img original;

    if(f == NULL)
        printf("Argument exception\n");

    fread(original.header, sizeof(unsigned char), 54, f); // lee la cabecera

    original.width = 0;
    original.height = 0;
    for (int i = 0; i < 4; i++) 
    {
        original.width += (int)original.header[i + 18] * pow(16, i * 2);
        original.height += (int)original.header[i + 22] * pow(16, i * 2);
    }

    cout <<"Archivo leido: " << filename << " Width: " << original.width << " Height: " << original.height << endl;

    original.row_padded = (original.width * 3 + 3) & (~3);
    unsigned char* data = (unsigned char*)malloc(sizeof(char) * original.row_padded);
    char tmp;

    rgb **rgbtabledata = (rgb**)malloc(sizeof(rgb*) * original.height);
  
    for(int i = 0; i < original.height; i++) 
    {
        rgbtabledata[i] = (rgb*)malloc(sizeof(rgb) * original.width);
        fread(data, sizeof(unsigned char), original.row_padded, f);
        
        for(int j = 0; j < original.width*3; j += 3) 
        {
            rgbtabledata[i][j / 3].r = (int)data[j + 2];
            rgbtabledata[i][j / 3].g = (int)data[j + 1];
            rgbtabledata[i][j / 3].b = (int)data[j];
        }
    }
    original.data = rgbtabledata;
    fclose(f);
    return original;
}

int main() 
{
  char* name = "sp.bmp";
  img image = mainBMP(name);
  kmeans mk(&image, 6);
  mk.initialize();
  mk.clusterize(10);
  write_modified(image);
  return 0;
}
