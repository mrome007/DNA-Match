#ifndef HEADERFILE_H
#define HEADERFILE_H


//includes
#include <cstdlib>
#include <iostream>
#include <vector>
#include <map>
#include <stdio.h>
#include <fstream>
#include <sstream>


////////////////////////////////////////////////////////////////////////////////
///////////////////////////    GRAPH STRUCTURES   //////////////////////////////
////////////////////////////////////////////////////////////////////////////////

struct Edge;

struct Node {
	int value;
	bool visited;
	std::vector<Edge*>* neighbors;

	Node(int v) :
			value(v), visited(false) {
	  neighbors = new std::vector<Edge*>();
	}
	void addEdge(Edge * e) {
		neighbors->push_back(e);
	}
	~Node() {
	}
};

struct Edge {
	Node * end;
	int cost;
	char how;
	Edge(Node * en, int c, char h) :
			end(en), cost(c), how(h) {
	}
	~Edge() {
	}
};

struct Graph {
	std::map<int, Node*>* graph;
	Graph() {
		graph = new std::map<int, Node*>();
	}
	~Graph() {
	}
	void addNode(int key, int val) {
		Node * temp = new Node(val);
		graph->insert(std::pair<int, Node*>(key, temp));
	}
	void addEdge(Node *n1, Node *n2, int cst, char hw) {
		Edge *e = new Edge(n2, cst, hw);
		n1->addEdge(e);
	}

};

#endif
