#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#define MAX(x,y)	((x)>(y)?(x):(y))
#define MIN(x,y)	((x)<(y)?(x):(y))
#define TESTNUM 1000000
#define SCOREMAX 150
#define PRINT(x)	{cout << #x << "=" << x << " (line=" << __LINE__ << ", file=" << __FILE__ << ")" << endl;}
using namespace std;	

int myStrategy(int gameState[4][2], int diceValue);