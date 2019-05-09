#include <iostream>
#include <algorithm>
#include <vector>
#include <fstream>
#include <sstream>
#include <queue>
#include <functional>
#include <string>
#include <chrono>
#include <time>
#include <list>

using namespace std;

class node{
	public:

	int nodeNum;
	int parent;
	list<edge> edges;

};

class edge{
	public:

	int weight;
	node * one;
	node * two;	

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
	vector<node> graph;
	int temp1;
	int temp2;
	int temp3;
	bool canAdd;
	srand(time(0));		
	for(int i = 0; i < numNodes; i++){
		graph[i].nodeNum = i;
	}
	for(int i = 0; i < numNodes; i++){
		temp1 = rand() % degreeLim;
		for(int j = 0; i < temp1 ; j++){
			canAdd = true;
			temp2 = rand() % numNodes;
			temp2++;
			for(auto const& k : graph[i].edges){
				if(k->two.nodeNum == temp2){
					canAdd = false;
				}
			}
			if(canAdd){
				temp3 = rand() % weightLim;
				tempE.weight = temp3;
				tempE.one = &graph[i];
				tempE.two = &graph[temp2];
				graph[i].edges.push_back(tempE);
				tempE.one = &graph[temp2];
				tempE.two = &graph[i];
				graph[temp2].edges.push_back(tempE);				
			}
		}
	}
	graph answer;
	answer.nodes = graph;
}

void printGraph(graph input){
	for(int i = 0; i < input.nodes.size(); i++){
		cout << "Node: " << i << ":" << endl;
		for(auto const& j : graph[i].edges){
			cout << " -(" << j.weight << ")-> " << j->two.nodeNum << endl;
		}
		cout << "/n" << endl;
	}
}

graph Kruskal(vector<node> graph){
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
	mst.resize(graph.nodes.size(), tempN);

	priority_queue<edge, vector<edge>, betterThan> PQ;  // A priority queue of all non-tree edges
														// mst edges are filtered out
	for(int i = 0; i < graph.nodes.size(); i++){
		for(auto const& j : graph.nodes[i].edges){
			PQ.push(j);
		}
	}
	temp1 = PQ.top()->one.nodeNum;
	temp2 = PQ.top()->two.nodeNum;
	mst[temp1].nodeNum = temp1;
	mst[temp2].nodeNum = temp2;
	tempE.one = &mst[temp1];
	tempE.two = &mst[temp2];
	mst[temp1].edges.push_back(tempE);
	tempE.one = &mst[temp2];
	tempE.two = &mst[temp1];
	mst[temp2].edges.push_back(tempE);

	PQ.pop();
	int counter = 2;				// REPLACE this who thing with if(cycle)
	while(counter != graph.nodes.size()){
		temp1 = PQ.top()->one.nodeNum;
		temp2 = PQ.top()->two.nodeNum;
		if( (mst[temp1].parent != mst[temp2].parent) && (mst[temp2].parent == INT_MAX) ){
			mst[temp2].nameNum = temp2;
			mst[temp2].parent = mst[temp1].parent;
			tempE.one = &mst[temp1];
			tempE.two = &mst[temp2];
			mst[temp1].edges.push_back(tempE);
			tempE.one = &mst[temp2];
			tempE.two = &mst[temp1];
			mst[temp2].edges.push_back(tempE);
		}
		if( (mst[temp1].parent != mst[temp2].parent) && (mst[temp1].parent == INT_MAX) ){
			mst[temp1].nameNum = temp1;
			mst[temp1].parent = mst[temp2].parent;
			tempE.one = &mst[temp1];
			tempE.two = &mst[temp2];
			mst[temp1].edges.push_back(tempE);
			tempE.one = &mst[temp2];
			tempE.two = &mst[temp1];
			mst[temp2].edges.push_back(tempE);
		}
		if( (mst[temp1].parent != mst[temp2].parent) && ((mst[temp1].parent != INT_MAX) && (mst[temp2].parent != INT_MAX)) ){
			temp3 = mst[temp2].parent;
			tempE.one = &mst[temp1];
			tempE.two = &mst[temp2];
			mst[temp1].edges.push_back(tempE);
			tempE.one = &mst[temp2];
			tempE.two = &mst[temp1];
			mst[temp2].edges.push_back(tempE);
			for(int i = 0; i < mst.size() ; i++){
				if(mst[i].parent == temp3){
					mst[i].parent = mst[temp1].parent;
				}
			}
		}
		if(mst[temp1].parent == INT_MAX && mst[temp2].parent == INT_MAX){
			mst[temp1].nodeNum = temp1;
			mst[temp2].nodeNum = temp2;
			mst[temp1].parent = temp1;
			mst[temp2].parent = temp1;
			tempE.one = &mst[temp1];
			tempE.two = &mst[temp2];
			mst[temp1].edges.push_back(tempE);
			tempE.one = &mst[temp2];
			tempE.two = &mst[temp1];
			mst[temp2].edges.push_back(tempE);
		}
		PQ.pop();
	}
	graph answer;
	answer.nodes = mst;
	return answer;
}



graph Prim(vector<node> graph){
	vector<node> mst;
	node tempN;
	edge tempE;
	int temp1;
	int temp2;
	tempN.nodeNum = INT_MAX;
	mst.resize(graph.nodes.size(), tempN);

	//Find shortest edge in input graph, put those nodes into mst, remove from graph
	mst[0].nodeNum = 0;

	priority_queue<edge, vector<edge>, betterThan> PQ; // a priority queue of all the edges in the MST
														// all edges in MST must be sorted by weight
	for(auto const& j : graph[0].edges){
		PQ.push(j);
	}
	int counter = 1;
	bool match;
	while(counter != mst.size()){
		match = true;
		for(int i = 0; i < mst.size(); i++){
			if(PQ.top()->two.nodeNum == mst[i].nodeNum){
				PQ.pop();
				match = false;
				break;
			}
		}
		if(match){

			temp1 = champ->one.nodeNum;
			temp2 = champ->two.nodeNum;
			mst[temp1].nodeNum = temp1;
			mst[temp2].nodeNum = temp2;
			tempE.one = &mst[temp1];
			tempE.two = &mst[temp2];
			mst[temp1].edges.push_back(tempE);
			tempE.one = &mst[temp2];
			tempE.two = &mst[temp1];
			mst[temp2].edges.push_back(tempE);
			counter++;

			for(auto const& j : graph.nodes[temp1].edges){
				PQ.push(j);
			}	
		}
		PQ.pop();
	}

	graph answer;
	answer.nodes = mst;
	return answer;

}




int main(int argc, char *argv[]){

	using namespace std;

	int i1 = argv[1];
	int i2 = argv[2];
	int i3 = argv[3];

	graph test = makeGraph(i1, i2, i3);

	graph K = Kruskal(test.nodes);
	graph P = Prim(test.nodes);

	cout << "The graph:" << endl;
	printGraph(test);
	cout << "Kruskal's Algorithm:" << endl;
	printGraph(K);
	cout << "Prim's Algorithm:" << endl;
	printGraph(P);

}
