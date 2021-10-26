#include "MyVector.h"
#include "MyQueue.h"
#include "Tree.h"
#include <iostream>
#include <string>
#include <fstream>
using namespace std;
//====================================
//display list of available commands
void listCommands(){
	cout<<"List of available Commands:"<<endl
		<<"help					: Prints the following menu of commands"<<endl
		<<"pwd					: Prints the path of current inode"<<endl
		<<"realpath <filename> 			: Prints the full path of a given file"<<endl
		<<"list 					: Prints the children of the current inode"<<endl
		<<"ls sort 				: Prints the children of the current inode ordered by descending file size"<<endl
		<<"mkdir <foldername>			: Creates a folder under the current folder"<<endl
		<<"touch <filename size>			: Creates a file under the current inode location"<<endl
		<<"cd                     			: Changes current inode to root"<<endl
		<<"cd <foldername>				: Change current inode to the specified folder"<<endl
		<<"cd ..					: Changes current inode to its parent folder"<<endl
		<<"cd -					: Changes current inode to the previous folder"<<endl
		<<"cd /my/path/name			: Changes the current inode to the specified path if it exists"<<endl
		<<"find <foldername/filename>		: Returns the path of the file (or the folder) if it exists"<<endl
		<<"mv <filename foldername>		: Moves a file located under the current inode location, to the specified folder path"<<endl
		<<"rm <foldername/filename>		: Removes the specified folder or file and puts it in a Queue"<<endl
		<<"size <foldername/filename>		: Returns the total size of the folder (including all its subfiles), or the size of the file"<<endl
		<<"emptybin				: Empties the bin"<<endl
		<<"showbin					: Shows the oldest inode of the bin"<<endl
		<<"recover					: Reinstates the oldest inode back from the bin to its original position in the tree"<<endl
		<<"exit					: The program stops and the filesystem is saved in the described format"<<endl;
}

//====================================
//sort given array of nodes by descending order of size
void sortBySize(MyVector<Position*>& children) {
	for (int i=0;i<children.size()-1;i++) {
		for (int j=0;j<children.size()-1-i;j++) {
			if (children[j]->getSize()<children[j+1]->getSize())
				swap(children[j],children[j+1]);
		}
	}
}

//====================================
//display children nodes of given node
void listChildren(Position* p, bool sorted=false) {
	MyVector<Position*> children = p->getChildren();
	if (sorted) sortBySize(children);
	for (int i=0;i<children.size();i++) {
		if (children[i]->isFile()) cout << "file "; else cout << "dir ";
		cout << children[i]->getName() << " ";
		cout << children[i]->getDate() << " ";
		cout << children[i]->getSize() << "bytes\n";
	}
}

//====================================
//split string to substrings by given separator
MyVector<string> split(const string& line, char ch) {
	MyVector<string> vec;
	string tmp;
	for (int i=0;i<line.size();i++) {
		if (line[i]!=ch) tmp+=line[i];
		else {
			vec.push_back(tmp); //cout << vec[vec.size()-1] << endl;
			tmp = "";
		}
	}
	vec.push_back(tmp);
	return vec;
}

//====================================
//display the olderst deleted node in the bin
void showBinOldest(const MyQueue<Position*>& bin) {
	if (bin.empty()) throw "Bin is empty";
	Position* p = bin.front();
	cout << "Next Element to remove: " << p->getParent()->getPath()+p->getName() << " (" << p->getSize();
	cout << " bytes, " << p->getDate() << ")\n";
}

//====================================
//recover the oldest deleted noe in the bin
void recoverOldest(Tree& tree, MyQueue<Position*>& bin) {
	if (bin.empty()) throw "Bin is empty";
	Position* p = bin.front();
	Position* tmp = tree.getRoot();
	try {
		string path = p->getParent()->getPath();
		if (path!="") tmp = tree.findPosition(path.substr(1,path.size()-1),tree.getRoot());
		//if (tmp!=p->getParent()) throw "File or folder cannot be recovered";
	}
	catch(const char* s) {
		throw "File or folder cannot be recovered";
	}

	tmp->addChild(p);
	bin.pop();
}

//====================================
//display path of given node
string currentPath(Position* p) {
	string path = p->getPath();
	if (path.size()==1) return path;
	else return path.substr(0,path.size()-1);
}

//====================================
//read tree from given file
Tree readTree(string filename) {
	Tree tree = Tree();
	ifstream fin(filename);
	string line;
	if (fin) {
		while (getline(fin,line)) {
			MyVector<string> vec = split(line,',');
			if (vec[0]=="/") {
				tree.getRoot()->setDate(vec[2]);
				continue;
			}
			//cout << vec[1] << "-"<<endl;
			string parent_path = vec[0].substr(0,vec[0].rfind('/')+1);
			string name = vec[0].substr(vec[0].rfind('/')+1,vec[0].size()-vec[0].rfind('/'));
			Position* parent = tree.findPosition(parent_path,tree.getRoot());
			Position* child = new Position((name.find('.')!=string::npos),name,stoi(vec[1]));
			child->setDate(vec[2]);
			parent->addChild(child);
		}
	}
	return tree;
}

//====================================
//save tree to given file
void save(string filename,Position* root) {
	ofstream fout;
	fout.open(filename);
	
	MyQueue<Position*> qu(10);
	qu.push(root);
	while (!qu.empty()) {
		Position* p = qu.front(); qu.pop();
		string path = p->getPath(); 
		if (path.size()>1) path = path.substr(0,path.size()-1);
		fout << path << "," << p->getSize() << "," << p->getDate() << endl;
		MyVector<Position*> children = p->getChildren();
		for (int i=0;i<children.size();i++) {
			qu.push(children[i]);
		}
	}

	fout.close();
}

//====================================
//validate folder/file name
void nameValidation(string name, bool isfile, Position* parent) {
	if (parent->hasChild(name)) throw "Such file/folder already exists";
	int valid = 0;
	for (int i=0;i<name.size();i++) {
		if (name[i]>='a' && name[i]<='z' ||
			name[i]>='A' && name[i]<='Z' ||
			name[i]>='0' && name[i]<='9') valid++;
	}
	if (!isfile) {
		if (valid<name.size()) throw "Invalid name";
		else return;
	}
	if (valid!=name.size()-1) throw "Invalid name";
	if (name.find('.')==string::npos) throw "Invalid name";
}
 
int main(){
	MyQueue<Position*> bin(10);
	Tree tree = readTree("vfs.dat");
	Position* cur = tree.getRoot();
	Position* prev = cur;
	listCommands();
	string user_input;
	string command;
	string parameter1;
	string parameter2;

	do
	{
		cout<<">";
		getline(cin,user_input);
		MyVector<string> sp = split(user_input,' ');
		command = sp[0];
		if (sp.size()>1) parameter1 = sp[1];
		if (sp.size()>2) parameter2 = sp[2];
	
		try
		{
			if(command=="display") 				tree.display(tree.getRoot());
			else if(command == "help")			listCommands();
			else if(command == "ls")			{
				if (parameter1=="")				listChildren(cur);
				else if(parameter1=="sort")		listChildren(cur,true);
			}
			else if(command == "mkdir"){
												nameValidation(parameter1,false,cur);
												Position* tmp = new Position(false,parameter1,10);
												tree.addPosition(cur, tmp);
			}
			else if(command == "touch"){
												if (parameter2=="") throw "Invlaid file size";
												nameValidation(parameter1,true,cur);
												Position* tmp = new Position(true,parameter1,stoi(parameter2));
												tree.addPosition(cur, tmp);
			}
			else if(command == "cd") {
				if (sp.size()==1) 								{ prev = cur; cur = tree.getRoot(); }
				else if (parameter1 == "..")					{ prev = cur; cur = cur->getParent(); }
				else if (parameter1 == "-")						{ cur = prev; }
				else if (parameter1.find('.')!=string::npos) 	throw "Invalid folder name";
				else if (parameter1.find('/')==string::npos)	{ prev = cur; cur = tree.findPosition(parameter1,cur); }//cur path + 
				else 											{ prev = cur; cur = tree.findPosition(parameter1,cur); }
			}
			else if(command == "pwd")			cout << currentPath(cur) << endl;
			else if(command == "realpath")		cout << cur->findRealpath(parameter1) << endl;
			else if(command == "find")			{ MyVector<string> ans = tree.findPath(tree.getRoot(),parameter1); for (int i=0;i<ans.size();i++) cout << ans[i] << endl; }
			else if(command == "size")			cout << tree.findPosition(parameter1,tree.getRoot())->subtreeSize() << endl;
			else if(command == "mv"){
				if (parameter1.find('/')!=string::npos)		tree.findPosition(parameter1,tree.getRoot())->moveTo(tree.findPosition(parameter2,tree.getRoot()));
				else 											tree.findPosition(parameter1,cur)->moveTo(tree.findPosition(parameter2,cur));
			}
			else if(command == "rm")			{ Position* p = tree.findPosition(parameter1,cur); bin.push(p); p->remove(); }
			else if(command == "emptybin")		{ bin.makeEmpty(); }
			else if(command == "showbin")		showBinOldest(bin);
			else if(command == "recover")		recoverOldest(tree, bin);
			else if(command == "exit")			{ save("vfs.dat",tree.getRoot()); break; }
			else 								cout<<"Invalid Command !!"<<endl;
		}
		catch(exception &e)
		{
			cout<<"Exception: "<<e.what()<<endl;
		}
		catch(const char* s)
		{
			cout<<"Exception: "<<s<<endl;
		}

		parameter1 = "";
		parameter2 = "";

	}while(command!="exit" and command!="quit");

	return EXIT_SUCCESS;
}