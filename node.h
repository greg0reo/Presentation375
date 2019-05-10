#include <iostream>
#include <algorithm>
#include <vector>
#include <fstream>
#include <sstream>
#include <queue>
#include <functional>
#include <string>
#include <chrono>
#include <time.h>
#include <list>
#include "edge.h"

using namespace std;

class node{
	public:

	int nodeNum;
	int parent;
	list<edge> edges;

};
