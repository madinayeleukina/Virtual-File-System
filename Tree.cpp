Position::Position(bool isfile, string name, int size=0) {
	this->name = name;
	this->size = size;
	this->isfile = isfile;
	this->parent = NULL;
	
	time_t t = time(NULL);
	tm* tptr = localtime(&t);
	string day = to_string(tptr->tm_mday);
	this->date = this->date + ((day.size()==1)?'0'+day:day) + "-";
	string mon = to_string(tptr->tm_mon+1);
	this->date = this->date + ((mon.size()==1)?'0'+mon:mon) + "-";
	this->date = this->date + to_string((tptr->tm_year)+1900).substr(2,2);
}

Position::~Position() {
	for (int i=0;i<children.size();i++) {
		delete children[i];
	}
}

string Position::getPath(){
	if (this->isRoot()) return "/";
	return parent->getPath() + name + "/";
}

bool Position::hasChild(string s) {
	for (int i=0;i<children.size();i++) {
		if (children[i]->getName()==s) return true;
	}
	return false;
}

string Position::findRealpath(string filename) {
	if (filename.find('.')==string::npos) throw "Invalid file name";
	if (!this->hasChild(filename)) throw "No such file";
	return this->getPath() + '/' + filename;
}

int Position::subtreeSize() {
	if (this->isExternal()) return size;
	int ans = 0;
	for (int i=0;i<children.size();i++) {
		ans += children[i]->subtreeSize();
	}
	return ans;
}

void Position::remove() {
	if (this->isRoot()) throw "Cannot delete root";
	MyVector<Position*> siblings = parent->getChildren();
	int ind = -1;
	for (int i=0;i<siblings.size();i++) {
		if (siblings[i]->getName()==name) {
			ind = i;
			break;
		}
	}
	if (ind!=-1) parent->getChildren().erase(ind);
}

void Position::moveTo(Position* dest) {
	MyVector<Position*> siblings = parent->getChildren();
	int ind = -1;
	for (int i=0;i<siblings.size();i++) {
		if (siblings[i]->getName()==name) {
			ind = i;
			break;
		}
	}
	parent->getChildren().erase(ind);
	dest->addChild(this);
}

Tree::Tree():root(new Position(true,"root",0)) {
	positions.push_back(root);
}

void Tree::display(Position* p) {
	cout << p->getName() << ": ";
	MyVector<Position*> children = p->getChildren();
	for (int i=0;i<children.size();i++) {
		cout << children[i]->getName() << " ";
	}
	cout << endl;
	for (int i=0;i<children.size();i++) {
		display(children[i]);
	}
}

void Tree::addPosition(Position* parent, Position* child) {
	parent->addChild(child);
	positions.push_back(child);
}

Position* Tree::findPosition(string path, Position* p) {
	if (path[0]=='/') path = path.substr(1,path.size()-1);
	if (path=="") return p;
	string next = path;
	string remain = "";
	if (path.find("/")!=string::npos) {
		next = path.substr(0,path.find("/"));
		remain = path.substr(path.find("/")+1,path.size()-path.find("/")-1);
	}
	
	MyVector<Position*> children = p->getChildren();
	for (int i=0;i<children.size();i++)
		if (children[i]->getName()==next)
			return findPosition(remain,children[i]);
	
	throw "Directory not found";
}

MyVector<string> Tree::findPath(Position* p, string name){
	MyVector<string> tmp;
	if (p->getName()==name) tmp.push_back(name);
	else if (p->isExternal()) return tmp;
	
	MyVector<Position*> children = p->getChildren();
	for (int i=0;i<children.size();i++) {
		MyVector<string> found = findPath(children[i], name);
		for(int j=0;j<found.size();j++) {
			if (p->isRoot()) tmp.push_back('/'+found[j]);
			else tmp.push_back(p->getName()+'/'+found[j]);
		}
	}

	return tmp;
}