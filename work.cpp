#include <iostream>
#include <climits>
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
//#include "node.h"
//#include "edge.h"


using namespace std;

class edge{
	public:

	int weight;
	int one;
	int two;

};

class node{
	public:

	int nodeNum;
	int parent;
	list<edge> edges;

};

class graph{
	public:

	vector<node> nodes;

};



struct betterThan{ //overwrite operator for priority queue
	public:
	bool operator()(edge const& e1, edge const& e2){
		return e1.weight > e2.weight;
	}
};

graph makeGraph(int numNodes, int degreeLim, int weightLim){
	node tempN;
	edge tempE;
	vector<node> graph2;
	int temp1;
	int temp2;
	int temp3;
	bool canAdd;
	srand(time(0));	
	for(int i = 0; i < numNodes; i++){
//		graph2[i].nodeNum = i;
		tempN.nodeNum = i;
		graph2.push_back(tempN);
	}
	for(int i = 0; i < numNodes; i++){
		temp1 = (rand() % degreeLim)+1;
		for(int j = 0; j < temp1 ; j++){
			canAdd = true;
			temp2 = rand() % numNodes;
//			temp2++;
			cout << "i: " << i << endl;
			for(auto const& k : graph2[i].edges){
				if(k.two == temp2){
					canAdd = false;
				}
				if(k.two == i){
					canAdd = false;
				}
			}
			if(canAdd){
				temp3 = (rand() % weightLim) + 1;
				tempE.weight = temp3;
				cout << temp3 << endl;
				tempE.one = i;
				tempE.two = temp2;
				graph2[i].edges.push_back(tempE);
				tempE.one = temp2;
				tempE.two = i;
				graph2[temp2].edges.push_back(tempE);
			}
		}
	}
	graph answer;
	answer.nodes = graph2;
	return answer;
}

void printGraph(graph input){
	for(int i = 0; i < input.nodes.size(); i++){
		cout << "Node: " << i << ":" << endl;
		for(auto const& j : input.nodes[i].edges){
			cout << " -(" << j.weight << ")-> " << j.two << endl;
		}
		cout << "/n" << endl;
	}
}

graph Kruskal(vector<node> graph2){
	vector<node> mst;
	node tempN;
	edge tempE;
	bool oneT;
	bool twoT;
	int temp1;
	int temp2;
	int temp3;
	tempN.nodeNum = INT_MAX;
	tempN.parent = INT_MAX;
	mst.resize(graph2.size(), tempN);

	priority_queue<edge, vector<edge>, betterThan> PQ;  // A priority queue of all non-tree edges
														// mst edges are filtered out
	for(int i = 0; i < graph2.size(); i++){
		for(auto const& j : graph2[i].edges){
			PQ.push(j);
		}
	}
	cout << PQ.top().weight << endl;
	temp1 = PQ.top().one;
	temp2 = PQ.top().two;
	mst[temp1].nodeNum = temp1;
	mst[temp2].nodeNum = temp2;
	tempE.weight = PQ.top().weight;
	tempE.one = temp1;
	tempE.two = temp2;
	mst[temp1].edges.push_back(tempE);
	tempE.one = temp2;
	tempE.two = temp1;
	mst[temp2].edges.push_back(tempE);
	mst[temp1].parent = temp1;
	mst[temp2].parent = temp1;

	PQ.pop();
	cout << PQ.top().weight << endl;
	int counter = 1;				// REPLACE this who thing with if(cycle)
	while(counter != graph2.size()-1){ // there should be n-1 edges
		temp1 = PQ.top().one;
		temp2 = PQ.top().two;
		tempE.weight = PQ.top().weight;
		if( (mst[temp1].parent != mst[temp2].parent) && (mst[temp2].parent == INT_MAX) ){
			mst[temp2].nodeNum = temp2;
			mst[temp2].parent = mst[temp1].parent;
			tempE.one = temp1;
			tempE.two = temp2;
			mst[temp1].edges.push_back(tempE);
			tempE.one = temp2;
			tempE.two = temp1;
			mst[temp2].edges.push_back(tempE);
			counter++;
		}
		if( (mst[temp1].parent != mst[temp2].parent) && (mst[temp1].parent == INT_MAX) ){
			mst[temp1].nodeNum = temp1;
			mst[temp1].parent = mst[temp2].parent;
			tempE.one = temp1;
			tempE.two = temp2;
			mst[temp1].edges.push_back(tempE);
			tempE.one = temp2;
			tempE.two = temp1;
			mst[temp2].edges.push_back(tempE);
			counter++;
		}
		if( (mst[temp1].parent != mst[temp2].parent) && ((mst[temp1].parent != INT_MAX) && (mst[temp2].parent != INT_MAX)) ){
			temp3 = mst[temp2].parent;
			tempE.one = temp1;
			tempE.two = temp2;
			mst[temp1].edges.push_back(tempE);
			tempE.one = temp2;
			tempE.two = temp1;
			mst[temp2].edges.push_back(tempE);
			for(int i = 0; i < mst.size() ; i++){
				if(mst[i].parent == temp3){
					mst[i].parent = mst[temp1].parent;
				}
			}
			counter++;
		}
		if(mst[temp1].parent == INT_MAX && mst[temp2].parent == INT_MAX){
			mst[temp1].nodeNum = temp1;
			mst[temp2].nodeNum = temp2;
			mst[temp1].parent = temp1;
			mst[temp2].parent = temp1;
			tempE.one = temp1;
			tempE.two = temp2;
			mst[temp1].edges.push_back(tempE);
			tempE.one = temp2;
			tempE.two = temp1;
			mst[temp2].edges.push_back(tempE);
			counter++;
		}
		PQ.pop();
	}
	graph answer;
	answer.nodes = mst;
	return answer;
}



graph Prim(vector<node> graph2){
	vector<node> mst;
	node tempN;
	edge tempE;
	int temp1;
	int temp2;
	tempN.nodeNum = INT_MAX;
	mst.resize(graph2.size(), tempN);

	//Find shortest edge in input graph, put those nodes into mst, remove from graph
	mst[0].nodeNum = 0;

	priority_queue<edge, vector<edge>, betterThan> PQ; // a priority queue of all the edges in the MST
														// all edges in MST must be sorted by weight
	for(auto const& j : graph2[0].edges){
		PQ.push(j);
	}
	int counter = 0;
	bool match;
	while(counter != mst.size()-1){
		match = true;
		for(int i = 0; i < mst.size(); i++){
			if(PQ.top().two == mst[i].nodeNum){
				PQ.pop();
				match = false;
				break;
			}
		}
		if(match){

			temp1 = PQ.top().one;
			temp2 = PQ.top().two;
			tempE.weight = PQ.top().weight;
			mst[temp1].nodeNum = temp1;
			mst[temp2].nodeNum = temp2;
			tempE.one = temp1;
			tempE.two = temp2;
			mst[temp1].edges.push_back(tempE);
			tempE.one = temp2;
			tempE.two = temp1;
			mst[temp2].edges.push_back(tempE);
			counter++;
			PQ.pop();
			for(auto const& j : graph2[temp2].edges){
				PQ.push(j);
			}
		}
	}

	graph answer;
	answer.nodes = mst;
	return answer;

}




int main(int argc, char *argv[]){

	using namespace std;

	int i1 = stoi(argv[1]);
	int i2 = stoi(argv[2]);
	int i3 = stoi(argv[3]);
//	cout << "do we get to the beginning?" << endl;

	graph test = makeGraph(i1, i2, i3);
	cout << "The graph:" << endl;
	printGraph(test);

	cout << "before Kruskal's" << endl;
	graph K = Kruskal(test.nodes);
	cout << "Kruskal's Algorithm:" << endl;
	printGraph(K);

	cout << "before Prim's" << endl;
	graph P = Prim(test.nodes);
	cout << "Prim's Algorithm:" << endl;
	printGraph(P);

}
