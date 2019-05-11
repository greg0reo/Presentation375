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




using namespace std;

//an edge in any graph
//has a source and destination (one and two), as well as a weight
//since edge has a direction, it must have a duplicate edge (of equal weight, opposite direction) to mirror it
class edge{
	public:

	int weight;
	int one;
	int two;

};

//a node in any graph
//stores a "parent" int for handling disjoint sets (Kruskal) and keys (Prim)
//stores its edges in a linked list (Adjacency list graph implementation)
//nodeNum used as an index to the node
class node{
	public:

	int nodeNum;
	int parent;
	list<edge> edges;

};

//a graph
//only holds a vector of nodes, the nodes of the graph
class graph{
	public:

	vector<node> nodes;

};


//overwrite operator for priority queue (to make it a min-queue)
struct betterThan{
	public:
	bool operator()(edge const& e1, edge const& e2){
		return e1.weight > e2.weight;
	}
};


//builds a complete graph, where all the nodes have a degree of numNodes-1
//writes numNodes-1 edges to the first node, numNodes-2 edges to the second node, etc (since there is already an edge coming to node2 from node1)
//assumes numNodes is greater than 1, since that would be a boring graph
graph completeGraph(int numNodes, int weightLim){
	node tempN; // a temp node holder; used primarily for filling graph2 with nodes
	edge tempE; // a temp edge holder; used primarily to set edges of a node
	vector<node> graph2; // the vector that will hold the output graph (will later be added to a graph class
	int temp1; // used primarily for saving nodeNums, and setting them to the source/destination of tempE (when adding edge to a node)
	int temp2; // same as temp1
	int temp3; // same as temp2
	srand(time(0));

	for(int i = 0; i < numNodes; i++){
		tempN.nodeNum = i;
		graph2.push_back(tempN); //filling graph2 with nodes
	}
	for(int i = 0; i < numNodes; i++){ // for each node...
		for(int j = numNodes-1; j > i; j--){ 	// add an edge to a foreign node
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
	graph answer; // will be the answer
	answer.nodes = graph2; //setting the constructed graph2 into a graph
	return answer;

}


//builds a graph of variable size and variable density
// numNodes: the total number of nodes in the graph
// degreeLim: the highest degree of any node
// weightLim: the highest weight of ay edge
// will always be a connected graph (although this may cause some nodes to have a higher degree than degreeLim, but this did not seriously affect our tests)
graph makeGraph(int numNodes, int degreeLim, int weightLim){
	node tempN; //temp node, used for filling graph with nodes
	edge tempE; //temp edge, used to add edges to a node
	vector<node> graph2;
	int temp1; //a set of temp ints, used to keep track of nodeNums
	int temp2; //same as temp1
	int temp3; //same as temp2
	int temp4; //same as temp3
	bool canAdd; //used when checking if the current edge about to be written already exists in the graph
	srand(time(0));

	for(int i = 0; i < numNodes; i++){

		tempN.nodeNum = i;
		tempN.parent = i;
		graph2.push_back(tempN);
	}
	for(int i = 0; i < numNodes; i++){		//for each node...
		temp1 = (rand() % degreeLim)+1;		//add a random amount of nodes less than degreeLim
		for(int j = 0; j < temp1 ; j++){
			canAdd = true;
			temp2 = rand() % numNodes;

			for(auto const& k : graph2[i].edges){ //checks if edge k already exists
				if(k.two == temp2){
					canAdd = false;
				}
				if(temp2 == i){
					canAdd = false;
				}
			}
			if(canAdd){ //if edge doesn't exist... add it to the graph
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
	bool connected = false; //keeps track of whether the graph is connected
	while(!connected){	//function uses node.parent and disjoint sets to make sure the graph is connected
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

	graph answer; //answer graph
	answer.nodes = graph2;
	return answer;
}

//prints a graph
//"cout"s nodeNum, every edge attached to it, and their weight
//contrary to most functions, takes a graph as an input
void printGraph(graph input){
	for(int i = 0; i < input.nodes.size(); i++){
		cout << "Node: " << i << ":" << endl;
		for(auto const& j : input.nodes[i].edges){
			cout << " -(" << j.weight << ")-> " << j.two << endl;
		}
		cout << endl;
	}
}

//Kruskal's MST selection algorithm
//loads all edges into a priority queue, then pulls them out least first
//if the edge doesn't cause a cycle, add it (and the connected nodes) to the MST
//"disjointed sets" issue dealt with node.parent
graph Kruskal(vector<node> graph2){
	vector<node> mst; //minimal spanning tree built here
	node tempN; //temp Node, used to fill MST with nodes
	edge tempE; //temp edge. used to add edges to nodes
//	bool oneT;
//	bool twoT; <- remnants of a more primitive Kruskal
	int temp1; //used to keep track of nodes
	int temp2; // same as temp1
	int temp3; //same as temp2
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
	temp1 = PQ.top().one; //adds first two nodes (attached to shortest edge) to the MST
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
	int counter = 1; // keeps track of how many edges are added to the MST, cannot be greater than numNode-1
	while(counter != graph2.size()-1){ // there should be n-1 edges
		temp1 = PQ.top().one;
		temp2 = PQ.top().two;
		tempE.weight = PQ.top().weight;
		if( (mst[temp1].parent != mst[temp2].parent) && (mst[temp2].parent == INT_MAX) ){ //"disjointed sets"
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
		if( (mst[temp1].parent != mst[temp2].parent) && (mst[temp1].parent == INT_MAX) ){ // excesive if statements to minimize Union
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
			for(int i = 0; i < mst.size() ; i++){ //Union
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


//old interpretation of Prim's algorithm, made better in prim2()
//not used for project, but it's a unique implementation
//ran almost identically to Kruskals
graph Prim(vector<node> graph2){
	vector<node> mst; //build MST here
	node tempN; //temp node, used to fill MST
	edge tempE; //temp edge, used to add edges to nodes
	int temp1; //keeps track of nodes
	int temp2; //same as temp1
	list <int> mstL;
	tempN.nodeNum = INT_MAX;
	tempN.parent = 5; //does NOT have to be 5, must only be != 0
	mst.resize(graph2.size(), tempN); //^ used to keep track of MST nodes

	//Find shortest edge in input graph, put those nodes into mst, remove from graph
	mst[0].nodeNum = 0;
	mst[0].parent = 0;

	priority_queue<edge, vector<edge>, betterThan> PQ; // a priority queue of all the edges in the MST
														// all edges in MST must be sorted by weight
	for(auto const& j : graph2[0].edges){ //pushes all edges of starter node into queue to better determine next explored path
		PQ.push(j);
	}
	int counter = 0; //tracks how many edges have been added to the MST, if counter == numNodes-1, then MST complete
	mstL.push_back(0);
	bool match; //tracks whether an edge should be added to an MST
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
		if(mst[PQ.top().two].parent == 0){ //is the destination part of the MST already? if so, do NOT add this edge
			match = false;
			PQ.pop();
		}

		if(match){

			temp1 = PQ.top().one; //example of temp1, temp2, and tempE working together to add an edge
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
			for(auto const& j : graph2[temp2].edges){ //push all edges from the newly added node to the priority queue
				PQ.push(j);
			}
		}
	}

	graph answer; //answer graph
	answer.nodes = mst;
	return answer;

}


//the better implementation of Prim's algorithm
//like the previous algorithm, but adds only one edge (shortest) to the priority queue
//takes shortest edge from queue, checks if it will make a cycle, if not, add it to MST
graph prim2(vector<node> graph2){
	vector<node> mst; //build MST here
	node tempN; //temp node, used to fill MST
	edge tempE; //temp edge, used to add edges to nodes
	int temp1; //used to keep track of nodes
	int temp2; //same as temp1
	int counter = 0; //counts how many edges have been added to MST, if edges == numNodes-1, then MST is done
	tempN.nodeNum = INT_MAX; //nodeNum remains infinite until it is recognized by MST
	tempN.parent = INT_MAX; //parent doubles as key[] array mentioned in class
	mst.resize(graph2.size(), tempN);
	mst[0].nodeNum = 0;
	mst[0].parent = 0;
	priority_queue<edge, vector<edge>, betterThan> PQ; //min priority queue

	for(auto i : graph2[0].edges){
		if(mst[i.two].parent > i.weight){
			mst[i.two].parent = i.weight;
			PQ.push(i); //only push shortest edge to a node found thus far
		}
	}
//	counter++;

	bool match = true; //should a specific edge be added to MST?
	while(counter != graph2.size()-1){
		bool match = true;
		tempE = PQ.top();
		if(mst[tempE.two].parent == 0){ //i edge leads back to MST, dont add it
			match = false;
			PQ.pop();
		}

		if(match){ //no cycle found, add edge

			temp1 = PQ.top().one;
			temp2 = PQ.top().two;
			tempE.weight = PQ.top().weight;
//			mst[temp1].nodeNum = temp1;
			mst[temp2].nodeNum = temp2;
			tempE.one = temp1;
			tempE.two = temp2;
			mst[temp1].edges.push_back(tempE);
			tempE.one = temp2;
			tempE.two = temp1;
			mst[temp2].edges.push_back(tempE);
			counter++;
			mst[temp2].parent = 0;
//			mstL.push_back(temp2);
			PQ.pop();
			for(auto const& j : graph2[temp2].edges){
				if(mst[j.two].parent > j.weight){
					mst[j.two].parent = j.weight;
					PQ.push(j); //only add the shortest edge to the queue
				}
			}
		}
	}

	graph answer; //answer graph
	answer.nodes = mst;
	return answer;
}






int main(int argc, char *argv[]){

	using namespace std;
	using namespace std::chrono; //used to measure time

	int decision = 0;

	if(argv[1] != NULL){
		decision = stoi(argv[1]); // lets user decide what this file does
	}


	if(decision == 1){ //if you want to print out a graph, and see the Kruskal and Prim solution

		int i1 = stoi(argv[2]);
		int i2 = stoi(argv[3]);
		int i3 = stoi(argv[4]);

		graph test = makeGraph(i1, i2, i3);

		cout << "Graph:" <<endl;
		printGraph(test);
		cout << "\n" << endl;

		graph K = Kruskal(test.nodes);
		cout << "Kruskal's Algorithm:" << endl;
		printGraph(K);

		cout << "\n" << endl;
		graph P = prim2(test.nodes);
		cout << "Prim's Algorithm:" << endl;
		printGraph(P);


	}else if(decision == 2){ //if you want to see the results of testing on complete graphs

		graph test; //graph that will be turned into an MST
		graph K; //graph that will store MST from Kruskal
		graph P; //graph that will store ST from prim2

		ofstream out; //print to output file
		out.open("completeGraph.csv"); //output as if it is a csv file, this makes it easier to graph


		for(int i = 50; i <= 1000; i+=50){ //this for loop prints results of testing on complete graphs
			test = completeGraph(i, 100);
			//all "auto" variables are for timing the function

			cout << "Complete Graph with numNodes: " << i << endl;
			auto start = high_resolution_clock::now();
			K = Kruskal(test.nodes);
			auto stop = high_resolution_clock::now();
			auto duration = duration_cast<microseconds>(stop-start);
			cout << "  Kruskal's: " << duration.count() << endl;

			start = high_resolution_clock::now();
			P = prim2(test.nodes);
			stop = high_resolution_clock::now();
			auto duration2 = duration_cast<microseconds>(stop-start);
			cout << "  Prim's: " << duration2.count() << "\n" << endl;

			out << duration.count() << "," << duration2.count() << endl; //cvs based output
		}

		out.close(); //closing the output file

	}else{ //if you want to see the results of varied graph test

		graph test; // graph to be tested on
		graph K; //graph that will store Kruskal's MST
		graph P; //graph that will store prim2's MST

		ofstream out;
		out.open("variedGraph.csv"); //write to a csv file to make graphing data easier

		int greg; //Hi, I wrote this project
		int fidel; //Me too!
		for(int i = 50; i <= 1000; i+= 50){
			cout << "numNodes: " << i << endl;
			test = makeGraph(i,i/10, 100);
			// All auto variables are for timing the algorithms

			cout << "  Edge Limit: #nodes/10" << endl;
			auto start = high_resolution_clock::now();
			K = Kruskal(test.nodes);
			auto stop = high_resolution_clock::now();
			auto duration = duration_cast<microseconds>(stop - start);
			cout << "    Kruskal's: " << duration.count() << endl;

			start = high_resolution_clock::now();
			P = prim2(test.nodes);
			stop = high_resolution_clock::now();
			auto duration2 = duration_cast<microseconds>(stop - start);
			cout << "    Prim's: " << duration2.count() << "\n" << endl;


			test = makeGraph(i, i/3, 100);

			cout << "  Edge Limit: #nodes/3" << endl;
			start = high_resolution_clock::now();
			K = Kruskal(test.nodes);
			stop = high_resolution_clock::now();
			auto duration3 = duration_cast<microseconds>(stop - start);
			cout << "    Kruskal's: " << duration3.count() << endl;

			start = high_resolution_clock::now();
			P = prim2(test.nodes);
			stop = high_resolution_clock::now();
			auto duration4 = duration_cast<microseconds>(stop - start);
			cout << "    Prim's: " << duration4.count() << "\n" << endl;


			test = makeGraph(i, i, 100);

			cout << "  Edge Limit: #nodes" << endl;
			start = high_resolution_clock::now();
			K = Kruskal(test.nodes);
			stop = high_resolution_clock::now();
			auto duration5 = duration_cast<microseconds>(stop - start);
			cout << "    Kruskal's: " << duration5.count() << endl;

			start = high_resolution_clock::now();
			P = prim2(test.nodes);
			stop = high_resolution_clock::now();
			auto duration6 = duration_cast<microseconds>(stop - start);
			cout << "    Prim's: " << duration6.count() << "\n" << endl;

			out << duration.count() << "," << duration2.count() << ",," << duration3.count() << "," << duration4.count() << ",," << duration5.count() << "," << duration6.count() << endl;
			//^^ output as a csv file
		}
	}
}
