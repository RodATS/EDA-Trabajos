#include <stdio.h>
#include <iostream>
#include <vector>
using namespace std;
static const unsigned int B[] = {0x55555555, 0x33333333, 0x0F0F0F0F, 0x00FF00FF};
static const unsigned int S[] = {1, 2, 4, 8};

// Used to hold details of a point
struct Point 
{
    int x;
    int y;
    Point(int _x, int _y)
    {
        x = _x;
        y = _y;
    }
    Point()
    {
        x = 0;
        y = 0;
    }
};
 
// The objects that we want stored in the quadtree
struct Node 
{
    Point pos;
    int data;
    Node(Point _pos, int _data)
    {
        pos = _pos;
        data = _data;
    }
    Node() { data = 0; }
};
 
// The main quadtree class
class Quad 
{
    // Hold details of the boundary of this node
    Point topLeft;
    Point botRight;
 
    // Contains details of node
    Node* n;
 
    // Children of this tree
    Quad* topLeftTree;
    Quad* topRightTree;
    Quad* botLeftTree;
    Quad* botRightTree;
    
    //vector of used cood
    vector<int> coordx;
    vector<int> coordy;
 
public:
    Quad()
    {
        topLeft = Point(0, 0);
        botRight = Point(0, 0);
        n = NULL;
        topLeftTree = NULL;
        topRightTree = NULL;
        botLeftTree = NULL;
        botRightTree = NULL;
    }
    Quad(Point topL, Point botR)
    {
        n = NULL;
        topLeftTree = NULL;
        topRightTree = NULL;
        botLeftTree = NULL;
        botRightTree = NULL;
        topLeft = topL;
        botRight = botR;
    }
    void insert(int x, int y, int val);
    Node* search(Point);
    bool inBoundary(Point);
};
 
// Insert a node into the quadtree
void Quad::insert(int x, int y, int val)
{
    if (val == NULL)
        return;
 
    // We are at a quad of unit area
    // We cannot subdivide this quad further
    if (abs(topLeft.x - botRight.x) <= 1 && abs(topLeft.y - botRight.y) <= 1) {
        if (n == NULL)
            n = Node(Point(x,y),val);
        return;
    }
 
    if ((topLeft.x + botRight.x) / 2 >= x) {
        // Indicates topLeftTree
        if ((topLeft.y + botRight.y) / 2 >= y) 
		{
            if (topLeftTree == NULL)
                topLeftTree = new Quad(
                    Point(topLeft.x, topLeft.y),
                    Point((topLeft.x + botRight.x) / 2,
                          (topLeft.y + botRight.y) / 2));
            topLeftTree->insert(x,y,val);
        }
 
        // Indicates botLeftTree
        else {
            if (botLeftTree == NULL)
                botLeftTree = new Quad(
                    Point(topLeft.x,
                          (topLeft.y + botRight.y) / 2),
                    Point((topLeft.x + botRight.x) / 2,
                          botRight.y));
            botLeftTree->insert(x,y,val);
        }
    }
    else {
        // Indicates topRightTree
        if ((topLeft.y + botRight.y) / 2 >= y) 
		{
            if (topRightTree == NULL)
                topRightTree = new Quad(
                    Point((topLeft.x + botRight.x) / 2,
                          topLeft.y),
                    Point(botRight.x,
                          (topLeft.y + botRight.y) / 2));
            topRightTree->insert(x,y,val);
        }
 
        // Indicates botRightTree
        else {
            if (botRightTree == NULL)
                botRightTree = new Quad(
                    Point((topLeft.x + botRight.x) / 2,
                          (topLeft.y + botRight.y) / 2),
                    Point(botRight.x, botRight.y));
            botRightTree->insert(x,y,val);
        }
    }
}
 
// Find a node in a quadtree
Node* Quad::search(Point p)
{
    // Current quad cannot contain it
    if (!inBoundary(p))
        return NULL;
 
    // We are at a quad of unit length
    // We cannot subdivide this quad further
    if (n != NULL)
        return n;
 
    if ((topLeft.x + botRight.x) / 2 >= p.x) {
        // Indicates topLeftTree
        if ((topLeft.y + botRight.y) / 2 >= p.y) {
            if (topLeftTree == NULL)
                return NULL;
            return topLeftTree->search(p);
        }
 
        // Indicates botLeftTree
        else {
            if (botLeftTree == NULL)
                return NULL;
            return botLeftTree->search(p);
        }
    }
    else {
        // Indicates topRightTree
        if ((topLeft.y + botRight.y) / 2 >= p.y) {
            if (topRightTree == NULL)
                return NULL;
            return topRightTree->search(p);
        }
 
        // Indicates botRightTree
        else {
            if (botRightTree == NULL)
                return NULL;
            return botRightTree->search(p);
        }
    }
};
 
// Check if current quadtree contains the point
bool Quad::inBoundary(Point p)
{
    return (p.x >= topLeft.x && p.x <= botRight.x
            && p.y >= topLeft.y && p.y <= botRight.y);
}

bool repeat (int x, int y)
{
	for (int i = 0; i<coordx.size(); i++)
	{
		if (coordx[i] = x && coordy == y)
		{
			return true;
		}
	}
	return false;
}

unsigned int zorder2D(unsigned x, unsigned y)
{
    
    x = (x | (x << S[3])) & B[3];
    x = (x | (x << S[2])) & B[2];
    x = (x | (x << S[1])) & B[1];
    x = (x | (x << S[0])) & B[0];

    y = (y | (y << S[3])) & B[3]; // | suma << multiplicacion
    y = (y | (y << S[2])) & B[2];
    y = (y | (y << S[1])) & B[1];
    y = (y | (y << S[0])) & B[0];
    return x | (y << 1);
}

int main()
{    
	int nx,ny;
  	cin >> nx >> ny;
  
  	unsigned res[ny][nx];
	
	for(int y=0; y<ny; y++)
  {
    for(int x=0; x<nx; x++)
    {
      res[y][x] = zorder2D(x,y);
      //printf("(y = %d ; x = %d) --> %d\n",y,x,res[y][x]);    
      cout << "(x = " << x << " y = " << y <<") --> " << res[y][x] << endl;
    }
    cout << endl;
  }
  for(int y=0; y<ny; y++)
  {
    for(int x=0; x<nx; x++)
    {
      //printf("%-4d",res[y][x]);
      cout << res[y][x] << "\t";
    }
    cout<<endl;
  }
  
	Quad center(Point(0,0), Point(nx,ny));
  	
	for (int i = 0; i<nx; i++)
  {
    for (int j = 0; j<ny; j++)
  	{
  	  center.insert(i,j,res[i][j]);
		}
  }
  	
  int ini = 1;
  int fin = 7;
  	
  for (int i = 0; i<nx; i++)
  {
    for (int j = 0; j<ny; j++)
  	{
  	  if (center.search(Point(i,j)) == ini)
  		{
  			cout << "(" << i << ", " << j << ") --> " << center.search(Point(i,j)) << endl;
			 }
    }
  }

	return 0;
}
