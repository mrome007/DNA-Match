////////////////////////////////////////////////////////////////////////////////
//  Project 3: DNA Search
//  Team Members:
//  Stephen    sbola004@ucr.edu.com
//  Mike       mrome007@ucr.edu.com
//  Jon        jdean007@ucr.edu.com
//  Nate         nhapeman@gmail.com
//  Aaron       aaronmg83@gmail.com
////////////////////////////////////////////////////////////////////////////////


//includes
#include <cstdlib>
#include <iostream>
#include <limits>
#include <string>
#include <vector>
#include <stack>
#include <map>
#include <stdio.h>
#include <fstream>
#include <sstream>
#include <string.h>
#include "graph.h"
#include <utility>

////////////////////////////////////////  call delete on new  //////////////////////////////////////

// get rid of by scoping?
//using namespace std;


//forward includes
int scores(char ref, char qry);
bool readInputFiles(char* dataBaseName, char* queryName);


//globals
int key = 0;
char * query;
char* dataBase;
int dataBaseLength;
int queryLength;
int num = 0;


int myMax(int val1, int val2) {   // renamed because scoping
	if (val1 >= val2)
		return val1;
	return val2;
}


////////////////////////////////////////////////////////////////////////////////
/////////////////////////////   FIND ALIGNMENT   ///////////////////////////////
////////////////////////////////////////////////////////////////////////////////


void maxAlign(Graph &g, std::vector<Node*> &lastRow, std::vector<Node*> topRow,
		std::vector<Node*> fcol ) {
  std::vector<Node*> check(fcol.size());
  int bottom = queryLength - 1;
  lastRow[0] = fcol[bottom];
	for (unsigned i = 1; i < topRow.size(); i++) {
	  check[0] = topRow[i];
	  for (unsigned j = 1; j < fcol.size(); j++) {
 			int gp_vertical = 0;
			int gp_diagonal = 0;
			int gp_horizontal = 0;
			gp_diagonal = scores(dataBase[i], query[j]);

			bool hasVert = false;
			bool hasHori = false;
			for (unsigned v = 0; v < check[j-1]->neighbors->size(); v++) {
				if (check[j - 1]->neighbors->at(v)->how == 'v')
					hasVert = true;
							}
			
			for (unsigned v = 0; v < fcol[j]->neighbors->size(); v++) {
				if (fcol[j]->neighbors->at(v)->how == 'h')
					hasHori = true;
			}

			if (hasVert == false)
				gp_vertical = -20;
			else
				gp_vertical = -5;
			if (hasHori == false)
				gp_horizontal = -20;
			else
				gp_horizontal = -5;

			int ver = check[j - 1]->value + gp_vertical;
			int dia = fcol[j - 1]->value + gp_diagonal;
			int hor = fcol[j]->value + gp_horizontal;

			int mx = myMax(dia, myMax(ver, hor));
			g.addNode(key, mx);
			check[j] = g.graph->at(key);

			if (dia > ver && dia > hor)
				g.addEdge(check[j], fcol[j - 1], gp_diagonal, 'd');
			else if (ver > dia && ver > hor)
				g.addEdge(check[j], check[j - 1], gp_vertical, 'v');
			else if (hor > dia && hor > ver)
				g.addEdge(check[j], fcol[j], gp_horizontal, 'h');
			else if (dia > ver && dia == hor) {
				g.addEdge(check[j], fcol[j - 1], gp_diagonal, 'd');
				g.addEdge(check[j], fcol[j], gp_horizontal, 'h');
			} else if (dia == ver && dia > hor) {
				g.addEdge(check[j], fcol[j - 1], gp_diagonal, 'd');
				g.addEdge(check[j], check[j - 1], gp_vertical, 'v');
			} else if (hor > dia && hor == ver) {
				g.addEdge(check[j], check[j - 1], gp_vertical, 'v');
				g.addEdge(check[j], fcol[j], gp_horizontal, 'h');
			} else if (dia == ver && dia == hor) {
				g.addEdge(check[j], fcol[j - 1], gp_diagonal, 'd');
				g.addEdge(check[j], fcol[j], gp_horizontal, 'h');
				g.addEdge(check[j], check[j - 1], gp_vertical, 'v');
			}

			key++;
	  }

 		for (unsigned k = 0; k < fcol.size(); k++)
 			fcol[k] = check[k];
 		lastRow[i] = fcol[bottom];
	}
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////   FINDING A PATH   ////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

void findOnePath(Node * start) {
	std::stack<Node *> toVisit;
	Node * first = start;
	toVisit.push(first);

	while (true) {
		Node * curr = toVisit.top();
		std::cout << curr->value << " ";
		curr->visited = true;
		if (curr->neighbors->size() == 0)
			break;
		Edge * temp = NULL;
		for (unsigned i = 0; i < curr->neighbors->size(); i++) {
			temp = curr->neighbors->at(i);
			Node * nei = temp->end;
			if (!nei->visited) {
				nei->visited = true;
				toVisit.push(nei);
			}
		}
		std::cout << temp->how << " ";
	}

}


struct traceBackNodes
{
  int posX;
  int posY;
  int k;
  Node * current;

  traceBackNodes(int x, int y, int key, Node * start) : posX(x), posY(y), k(key), 
							current(start)
  {}
};


void thePaths(Node * start, int pos, char * ref, char * que)
{
  int ky = 0;
  int refPos = pos;
  //std::string refer = ref.substr(0,pos+1);
  int quePos = queryLength-1;
  //std::string quer = que;
  std::stack< traceBackNodes > toVisit;
  std::map<int,std::pair<std::string,std::string> > paths;
  Node * first = start;
  toVisit.push(traceBackNodes(refPos,quePos,ky,first));
  paths.insert(std::make_pair(ky,std::make_pair("","")));
  ky++;
  
  while(!toVisit.empty())
    {
      int rx = toVisit.top().posX;
      int qx = toVisit.top().posY;
      int crnt = toVisit.top().k;
      Node * curr = toVisit.top().current;
      if (curr->neighbors->size() == 0)
	{
	  curr->visited = false;
	  std::cout << paths[crnt].first 
		    << std::endl 
		    << paths[crnt].second
		    << std::endl << std::endl;
	  num++;
	  if(num > 99)
	    break;
	}
	//curr->visited = true;
      Edge * temp = NULL;
      toVisit.pop();
      //std::cout << curr->value << std::endl;
    
      for(int i = 0; i < curr->neighbors->size(); i++)
	{
	  
	  temp = curr->neighbors->at(i);
	  Node * nei = temp->end;
	  //std::cout << temp->how;
	  if (!nei->visited) 
	    {
	      //nei->visited = true;
	      if(temp->how == 'd')
		{
		  int x = rx-1;
		  int y = qx-1;
		  std::string a = ref[rx] + paths[crnt].first;
		  std::string b = que[qx] + paths[crnt].second;
		  paths.insert(std::make_pair(ky,std::make_pair(a,b)));
		  toVisit.push(traceBackNodes(x,y,ky,nei));
		  //std::cout << "hey" << std::endl;
		}
	      else if(temp->how == 'h')
		{
		  int x = rx-1;
		  int y = qx;
		  std::string a = ref[rx] + paths[crnt].first;
		  std::string b = "_" + paths[crnt].second;
		  paths.insert(std::make_pair(ky,std::make_pair(a,b)));
		  toVisit.push(traceBackNodes(x,y,ky,nei));
		  //std::cout << "hey" << std::endl;
		}
	      else if(temp->how == 'v')
		{
		  int x = rx;
		  int y = qx-1;
		  std::string a = "_" + paths[crnt].first;
		  std::string b = que[qx] + paths[crnt].second;
		  paths.insert(std::make_pair(ky,std::make_pair(a,b)));
		  toVisit.push(traceBackNodes(x,y,ky,nei));
		  //std::cout << "hey" << std::endl;
		}
 	      ky++;
 	    }
 	}
      
    }
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////   MAIN FUNCTION   /////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

int main(int argc, char **argv) {

	//used to store command line arguments
	char*dataBaseName;
	char*queryName;
	char*numSequences;
	char*alignment;


	//parsing through input arguments
	for (int i = 0; i < argc; ++i) {
		char*temp = argv[i];
		if (strcmp(temp, "d") == 0)
			dataBaseName = argv[i + 2];
		else if (strcmp(temp, "q") == 0)
			queryName = argv[i + 2];
		else if (strcmp(temp, "n") == 0)
			numSequences = argv[i + 2];
		else if (strcmp(temp, "a") == 0)
			alignment = argv[i + 2];
	}

	//testing validity
	std::cout << "Here are the input arguments" << std::endl;
	std::cout << "DataBase: " << dataBaseName << std::endl;
	std::cout << "Query: " << queryName << std::endl;
	std::cout << "Number Sequences: " << numSequences << std::endl;
	std::cout << "Alignment: " << alignment << std::endl;

	
	readInputFiles(dataBaseName, queryName);
	
	/////////////////////////////////////////////////////

	std::cout << dataBaseLength << std:: endl;
	std::cout << queryLength << std:: endl;
	//top row
	
	dataBaseLength-=1;
	queryLength-=1;
	std::vector<Node*> topRow(dataBaseLength);
	//first col
	std::vector<Node*> firstCol(queryLength);

	Graph g;
	
	for (int i = 0; i < dataBaseLength; i++) {
		g.addNode(key, 0);
		topRow[i] = g.graph->at(key);
		key++;
	}

	
	firstCol[0] = topRow[0];
	for (int i = 1; i < queryLength; i++) {
		if (firstCol[i - 1]->neighbors->size() == 0) {
			int v = -20 + firstCol[i - 1]->value;
			g.addNode(key, v);
			firstCol[i] = g.graph->at(key);
			g.addEdge(firstCol[i], firstCol[i - 1], -20, 'v');
		} else {
			int v = -5 + firstCol[i - 1]->value;
			g.addNode(key, v);
			firstCol[i] = g.graph->at(key);
			g.addEdge(firstCol[i], firstCol[i - 1], -5, 'v');
		}
		key++;
	}
	
	//for (int i = 1; i < queryLength; i++) {
	//std::cout << firstCol[i]->value << "->" << firstCol[i]->neighbors->at(0)->how
	//<< " ";
	//}
	//std::cout << std::endl<<std::endl;


	//for (int i = 0; i < dataBaseLength; i++)
	//std::cout << topRow[i]->value << " ";

	std::vector<Node*> lastRow(dataBaseLength);
	//std::cout << "hello" << std::endl;
	maxAlign(g, lastRow, topRow, firstCol);
	//std::cout << "hello" << std::endl;
	//std::cout << std::endl;
	//for (unsigned i = 0; i < lastRow.size(); i++)
	//std::cout << lastRow[i]->value << " ";

	std::cout << std::endl;
	std::cout << "============================================" << std::endl;
	//std::cout << rf << " " << qy << std::endl;
	
	thePaths(lastRow[8500],8500,
		 dataBase,query);
	std::cout << "score: " << lastRow[8500]->value 
	<< std::endl << "The alignments are: " << std::endl;
	//findOnePath(lastRow[lastRow.size() - 1]);


	////////////////////////////////////////////////////////////////

	// deleting dynamically allocated memory
	delete[] dataBase;
	delete[] query;


	return 0;

}

////////////////////////////////////////////////////////////////////////////////
/////////////////////    Score Calculating Function   //////////////////////////
////////////////////////////////////////////////////////////////////////////////

//switch cases are faster than nested if else statements
//jump tables/ look up tables
int scores(char ref, char qry) {

  //std::cout << ref << std::endl;
  //std::cout << qry << std::endl;
	switch (ref) {
	case 'A':
		switch (qry) {
		case 'A':
			return 100;
			break;
		case 'G':
			return -10;
			break;
		case 'T':
			return -15;
			break;
		case 'C':
			return -10;
			break;
		default:
		  std::cout << "THIS SHOULDNT PRINT!" << ref <<std::endl;
			break;
		}
		break;
	case 'G':
		switch (qry) {
		case 'A':
			return -10;
			break;
		case 'G':
			return 100;
			break;
		case 'T':
			return -10;
			break;
		case 'C':
			return -15;
			break;
		default:
		  std::cout << "THIS SHOULDNT PRINT!"<<ref << std::endl;
			break;
		}
		break;
	case 'T':
		switch (qry) {
		case 'A':
			return -15;
			break;
		case 'G':
			return -10;
			break;
		case 'T':
			return 100;
			break;
		case 'C':
			return -10;
			break;
		default:
		  std::cout << "THIS SHOULDNT PRINT!"<<ref << std::endl;
			break;
		}
		break;
	case 'C':
		switch (qry) {
		case 'A':
			return -10;
			break;
		case 'G':
			return -15;
			break;
		case 'T':
			return -10;
			break;
		case 'C':
			return 100;
			break;
		default:
		  std::cout << "THIS SHOULDNT PRINT!"<<ref << std::endl;
			break;
		}
		break;
	default:
	  std::cout << "THIS SHOULDNT PRINT!" << ref  <<std::endl;
		break;
	}
	return 0;

}

////////////////////////////////////////////////////////////////////////////////
/////////////////////////////   File Reading   /////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

bool readInputFiles(char* dataBaseName, char* queryName) {

	//--------------------------------------------------------- loading dataBase

	std::ifstream dataFile(dataBaseName, std::ifstream::binary);

	//transferring data to database pointer
	if (dataFile) {

		dataFile.seekg(0, dataFile.end);
		dataBaseLength = dataFile.tellg();
		dataFile.seekg(0, dataFile.beg);
		dataBase = new char[dataBaseLength + 1];
		dataFile.read(dataBase, dataBaseLength);
		dataFile.close();
		dataBase[dataBaseLength] = '\0';

	} else {
		std::cout << "Error: Where is dataBase" << std::endl;
		return false;
	}

	std::cout << "\n The dataBase: " << std::endl;
	//std::cout << dataBase;

	//------------------------------------------------------------ loading query

	std::ifstream queryFile(queryName, std::ifstream::binary);

	//transferring data to database pointer
	if (queryFile) {

		queryFile.seekg(0, queryFile.end);
		queryLength = queryFile.tellg();
		queryFile.seekg(0, queryFile.beg);
		query = new char[queryLength + 1];
		queryFile.read(query, queryLength);
		queryFile.close();
		query[queryLength] = '\0';

	} else {
		std::cout << "Error: Where is query" << std::endl;
		return false;
	}

	std::cout << "\n\nThe query string: " << std::endl;
	//std::cout << query;

	return true;
}



