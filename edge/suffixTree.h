#ifndef SUFFIXTREE_H
#define SUFFIXTREE_H
#define _SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS 9999
#include <iostream>
#include<vector>
#include <iostream>
#include <unordered_map> // Required c++11 support.
#include <string>
#include <cassert>
#include <ctime>
#include <chrono>
#include<fstream>
#include<filesystem>
#include<algorithm>
#include<functional>
#include<map>
#include<Windows.h>
#include<conio.h>
#include<stdlib.h>
#include<sstream>
using namespace std;
namespace fs = std::experimental::filesystem;

struct fileData
{
	int no;
	int Totalscore;
	string name;
	int score[4]; // so lan xuat hien trong truong hop co nhieu tu khoa
	int position[4];
};

class Node {
public:
	static int noOfNodes;
	int suffixNode;
	Node() :
		suffixNode(-1) {};
	~Node() {
		//  cout << "destroying node " << id << endl;

	}
};

class suffixTree {
public:
	// TODO rootNode is not a good name, change this.
	int rootNode;   // Origin of the suffix tree
	int startIndex; // Starting index of the string represented.
	int endIndex;   // End index of the string represented.
	// Constructor
	suffixTree() :
		rootNode(0),
		startIndex(-1),
		endIndex(-1) {};//m
	suffixTree(int root, int start, int end) :
		rootNode(root),
		startIndex(start),
		endIndex(end) {};
	// Real means that the suffix string ends at a node and thus the
	// remaining string on that edge would be an empty string.
	bool endReal() { return startIndex > endIndex; }
	// Img means that the suffixTree of current string ends on an imaginary
	// node, which means in between an edge. 
	bool endImg() { return endIndex >= startIndex; }
	void migrateToClosestParent();
};

void task_manager(int &noOfMap);

class Edge {
public:
	// Edges are hash-searched on the basis of startNode.
	// startNode = -1 means that this edge is not valid yet.
	int startNode;
	int endNode;
	int startLabelIndex;//vi tri bat dau chuoi
	int endLabelIndex;//ket thuc chuoi
	long key;
	int leafLabelIndex;//vi tri xuat hien cua la (phan tu can tim trong string)
	void insert();
	void reBulid(int node, int c);
	void remove();
	// node is the starting node and c is the ASCII input char.
	// Static because I want to call it without using an instantiated object.
	static long returnHashKey(int node, int c);
	//static Edge findEdge(int node, int c, int noOfMap);
	static Edge findEdge(int node, int c);
	// Constructors.
	Edge() : startNode(-1) {};
	// everytime a new edge is created, a new node is also created and thus the
	// endNode is declared as below.
	Edge(int start, int first, int last, long ID) :
		startNode(start),
		endNode(Node::noOfNodes++),
		startLabelIndex(first),
		endLabelIndex(last),
		key(ID) {};
	// Destructor
	~Edge() {}
};

// build suffix tree
void buildSufffixTree(suffixTree &tree, int lastIndex);
int breakEdge(suffixTree &s, Edge &e, int lastIndex);
void setSuffixIndexByDFS(Edge &e, int labelHeight, int startNode);
void setSuffixIndex();
void reUseVector(vector<vector<Edge>>&vec, int len);
int edgeLength(Edge e);


// search for pattern
int isMatched(string pattern, Edge e, int len, int &index, int &matchIndex, int noOfMap, string t);
int frequency(string pattern, int noOfMap, int &wordApp, int &firstFound, int searchType);
int countLeafByDFS(Edge e, int startNode, string t, int &wordApp, int &firstFound, int searchType, int patternLength, string pattern);
void printTopFive(int searchType);
void viewFileByNodepad(string fpath);
void patternAnalyze(string pattern, int &searchType);
void DeleteStopWord(string &pattern);
void printAllEdges(int option);
void convertToLower(string &s, int &len);
void BuildSynonymsList();
void findSynonyms(string pattern, int &searchType);
void gotoxy(int x, int y);
void inputData(string &p);
void autoSuggestion(string p);
bool loadHistory();
void saveHistory(string pattern);
void introduce();
void VeKhungCH(int x, int y, int width, int height, bool draw);
void FullScreen();
void clear(); // fuction clear things better than system("clear")
void color(int x);
void gotoxy(int x, int y);
void ShowConsoleCursor(bool showFlag);
void findHightlight(string pattern, int noOfMap, int &firstFound, int searchType, vector<int> &a); // tính s? l?n xu?t hi?n c?a pattern
void hightLight(Edge e, int startNode, string t, int &firstFound, int searchType, int patternLength, vector<int> &a);
#endif                                                                             
