#include <iostream>
#include <string>
#include <fstream>
class Tree;

class Position {
private:
	string name;
	int size;
	string date;
	bool isfile;
	MyVector<Position*> children;
	Position* parent;
public:
	Position(bool isFile, string name, int size); //class constructor
	~Position(); //class destructor
	Position* getParent() { //get pointer to parent node
		if (parent == NULL) throw "Inode is a root";
		return parent;
	}
	void setDate(string date) { this->date = date; } //change date of node
	string getName() { return name; } //get name of node
	int getSize() { return size; } //get size of node
	string getDate() { return date; } //get date of node
	MyVector<Position*>& getChildren() { return children; }  //get list of children pointers
	bool isRoot() { return parent==NULL; } //check is node a root
	bool isExternal() { return children.size()==0; } //check is node external
	bool isFile() { return isfile; } //check is node a file
	void addChild(Position* p) { //add given child node to current node
		p->parent = this;
		children.push_back(p);
	}
	string getPath(); //get path of node
	bool hasChild(string s); //check is file/folder a child of node
	string findRealpath(string filename); //get path of given child node
	int subtreeSize(); //get size of subtree with root at node
	void moveTo(Position* dest); //move node to given destination
	void remove(); //remove node
};

class Tree {
private:
	Position* root;
	MyVector<Position*> positions;
public:
	Tree();
	int size() { return positions.size(); } //get tree size
	bool empty() { return positions.empty(); } //check is tree empty
	Position* getRoot() { return root; } //get root pointer
	MyVector<Position*>& getpositions() { return positions; } //get list of all node pointers
	void display(Position* p); //display tree(for testing purposes)
	Position* findPosition(string path, Position* p); //get node pointer of given path
	void addPosition(Position* parent, Position* child); //add new node
	MyVector<string> findPath(Position* p, string name); //find all paths to given file/folder
	friend class Position;
};

#include "Tree.cpp"