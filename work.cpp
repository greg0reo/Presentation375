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


graph completeGraph(int numNodes, int weightLim){
	node tempN;
	edge tempE;
	vector<node> graph2;
	int temp1;
	int temp2;
	int temp3;
	srand(time(0));

	for(int i = 0; i < numNodes; i++){
		tempN.nodeNum = i;
		graph2.push_back(tempN);
	}
	for(int i = 0; i < numNodes; i++){
		for(int j = numNodes-1; j > i; j--){
			temp1 = i;
			temp2 = j;
			temp3 = (rand() % weightLim) + 1;
			tempE.one = temp1;
			tempE.two = temp2;
			tempE.weight = temp3;
			graph2[temp1].edges.push_back(tempE);
			tempE.one = temp2;
			tempE.two = temp1;
			graph2[temp2].edges.push_back(tempE);
		}
	}
	graph answer;
	answer.nodes = graph2;
	return answer;

}



graph makeGraph(int numNodes, int degreeLim, int weightLim){
	node tempN;
	edge tempE;
	vector<node> graph2;
	int temp1;
	int temp2;
	int temp3;
	int temp4;
	bool canAdd;
	srand(time(0));

	for(int i = 0; i < numNodes; i++){
//		graph2[i].nodeNum = i;
		tempN.nodeNum = i;
		tempN.parent = i;
		graph2.push_back(tempN);
	}
	for(int i = 0; i < numNodes; i++){
		temp1 = (rand() % degreeLim)+1;
		for(int j = 0; j < temp1 ; j++){
			canAdd = true;
			temp2 = rand() % numNodes;
//			temp2++;
//			cout << "i: " << i << endl;
			for(auto const& k : graph2[i].edges){
				if(k.two == temp2){
					canAdd = false;
				}
				if(temp2 == i){
					canAdd = false;
				}
			}
			if(canAdd){
				temp3 = (rand() % weightLim) + 1;
				tempE.weight = temp3;
//				cout << temp3 << endl;
				tempE.one = i;
				tempE.two = temp2;
				graph2[i].edges.push_back(tempE);
				tempE.one = temp2;
				tempE.two = i;
				graph2[temp2].edges.push_back(tempE);
				temp3 = graph2[i].parent;
				temp4 = graph2[temp2].parent;
				if(temp3 != temp4){

					for(int k = 0; k < numNodes; k++){
						if(graph2[k].parent == temp4){
							graph2[k].parent = temp3;
						}
					}
				}
			}
		}
	}
	bool connected = false;
	while(!connected){
		temp1 = graph2[0].parent;
		connected = true;
		for(int i = 0; i < numNodes; i++){
			if(temp1 != graph2[i].parent){
				connected = false;
				temp2 = i;
				temp3 = graph2[i].parent;
				break;
			}
		}
		if(!connected){
			tempE.weight = (rand() % weightLim) + 1;
			tempE.one = 0;
			tempE.two = temp2;
			graph2[0].edges.push_back(tempE);
			tempE.one = temp2;
			tempE.two = 0;
			graph2[temp2].edges.push_back(tempE);
			for(int k = 0; k < numNodes; k++){
				if(graph2[k].parent == temp3){
					graph2[k].parent == graph2[0].parent;
				}
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
		cout << endl;
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
//	cout << PQ.top().weight << endl;
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
//	cout << PQ.top().weight << endl;
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
	list <int> mstL;
	tempN.nodeNum = INT_MAX;
	tempN.parent = 5;
	mst.resize(graph2.size(), tempN);

	//Find shortest edge in input graph, put those nodes into mst, remove from graph
	mst[0].nodeNum = 0;
	mst[0].parent = 0;

	priority_queue<edge, vector<edge>, betterThan> PQ; // a priority queue of all the edges in the MST
														// all edges in MST must be sorted by weight
	for(auto const& j : graph2[0].edges){
		PQ.push(j);
	}
	int counter = 0;
	mstL.push_back(0);
	bool match;
	while(counter != mst.size()-1){
		match = true;
//		for(int i = 0; i < mst.size(); i++){
//			if(PQ.top().two == mst[i].nodeNum){
/*		for(auto const& i : mstL){
			if(PQ.top().two == i){

				PQ.pop();
				match = false;
				break;
			}
		}
*/
		if(mst[PQ.top().two].parent == 0){
			match = false;
			PQ.pop();
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
			mst[temp2].parent = 0;
			mstL.push_back(temp2);
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

int countEdges(graph G){
	int counter;
	for(int i = 0; i < G.nodes.size(); i++){
		for(auto i : G.nodes[i].edges){
			counter++;
		}
	}
	return counter;
}



int main(int argc, char *argv[]){

	using namespace std;
	using namespace std::chrono;

/*
	int i1 = stoi(argv[1]);
	int i2 = stoi(argv[2]);
	int i3 = stoi(argv[3]);
//	cout << "do we get to the beginning?" << endl;

	graph test = makeGraph(i1, i2, i3);
*/

	graph test = completeGraph(7, 10);
/*	cout << "Graph:" <<endl;
	printGraph(test);
	cout << "\n" << endl;

//	cout << "before Kruskal's" << endl;
	graph K = Kruskal(test.nodes);
	cout << "Kruskal's Algorithm:" << endl;
	printGraph(K);

	cout << "\n" << endl;
	graph P = Prim(test.nodes);
	cout << "Prim's Algorithm:" << endl;
	printGraph(P);
*/
	graph K;
	graph P;

	for(int i = 50; i <= 1000; i+=50){
		test = completeGraph(i, 100);

		cout << "numNodes: " << i << endl;
		auto start = high_resolution_clock::now();
		K = Kruskal(test.nodes);
		auto stop = high_resolution_clock::now();
		auto duration = duration_cast<microseconds>(stop-start);
		cout << "Kruskal's: " << duration.count() << endl;

		start = high_resolution_clock::now();
		P = Prim(test.nodes);
		stop = high_resolution_clock::now();
		duration = duration_cast<microseconds>(stop-start);
		cout << "Prim's: " << duration.count() << "\n" << endl;

	}


/*
	graph test;
	graph K;
	graph P;


	int greg;
	int fidel;
	for(int i = 50; i <= 1000; i+=50){
		cout << "numNodes: " << i << endl;
		test = makeGraph(i, i/10, 100);
//		cout << "numEdges: "  << countEdges(test) << endl;
		cout << "Edge Limit: #nodes/10" << endl;
		auto start = high_resolution_clock::now();
		K = Kruskal(test.nodes);
		auto stop = high_resolution_clock::now();
		auto duration = duration_cast<microseconds>(stop - start);
		cout << "Kruskal's: " << duration.count() << endl;

		start = high_resolution_clock::now();
		P = Prim(test.nodes);
		stop = high_resolution_clock::now();
		duration = duration_cast<microseconds>(stop - start);
		cout << "Prim's: " << duration.count() << "\n" << endl;


		test = makeGraph(i, i/3, 100);
//		cout << "numEdges: "  << countEdges(test) << endl;
		cout << "Edge Limit: #nodes/3" << endl;
		start = high_resolution_clock::now();
		K = Kruskal(test.nodes);
		stop = high_resolution_clock::now();
		duration = duration_cast<microseconds>(stop - start);
		cout << "Kruskal's: " << duration.count() << endl;

		start = high_resolution_clock::now();
		P = Prim(test.nodes);
		stop = high_resolution_clock::now();
		duration = duration_cast<microseconds>(stop - start);
		cout << "Prim's: " << duration.count() << "\n" << endl;


		test = makeGraph(i, i, 100);
//		cout << "numEdges: "  << countEdges(test) << endl;
		cout << "Edge Limit: #nodes" << endl;
		start = high_resolution_clock::now();
		K = Kruskal(test.nodes);
		stop = high_resolution_clock::now();
		duration = duration_cast<microseconds>(stop - start);
		cout << "Kruskal's: " << duration.count() << endl;

		start = high_resolution_clock::now();
		P = Prim(test.nodes);
		stop = high_resolution_clock::now();
		duration = duration_cast<microseconds>(stop - start);
		cout << "Prim's: " << duration.count() << "\n" << endl;
	}
*/
}
