#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <string.h>

#define FILE_NAME "test.txt"//"input.txt"

class Cave {
private:
	char _id;
	
public:
	Cave() {}
	Cave(char id) {
		this->_id = id;
	}
	
	char getID() {
		return this->_id;
	}
	
	bool isSmall() {
		return this->_id >= 'a' && this->_id <= 'z';
	}
};

class Node {
private:
	std::vector<Node*> _caves;
	
	Cave _cave;
	bool _special;
	
	bool _visited;

public:
	Node(char id) {
		this->_cave = Cave(id);
		this->_special = false;
		this->_visited = false;
	}
	
	Node(bool start) {
		this->_cave = Cave(start ? 's' : 'e');
		this->_special = true;
		this->_visited = false;
	}
	
	Cave getNode() {
		return this->_cave;
	}
	
	bool getIsVisited() {
		return this->_visited;
	}
	
	bool getIsStart() {
		return this->_special && this->_cave.getID() == 's';
	}
	
	bool getIsEnd() {
		return this->_special && this->_cave.getID() == 'e';
	}
	
	void setVisited() {
		this->_visited = true;
	}
	
	void addAdjacent(Node *n) {
		this->_caves.push_back(n);
	}
	
	std::vector<Node*> getAdjacents() {
		return this->_caves;
	}
};

class CaveSystem {
private:
	std::vector<Node*> _caves;
	Node *_start, *_end;
	Node *_ptr;
	
	Node *getNode(char *node) {
		if (strcmp(node, "start") == 0) return this->_start;
		if (strcmp(node, "end") == 0) return this->_end;
		
		for (auto e : this->_caves) {
			if (e->getNode().getID() == *node) return e;
		}
		// no existeix
		Node *r = new Node(*node);
		this->_caves.push_back(r);
		return r;
	}
	
public:
	CaveSystem() {
		this->_start = new Node(true);
		this->_caves.push_back(this->_start);
		
		this->_end = new Node(false);
		this->_caves.push_back(this->_end);
		
		this->_ptr = this->_start;
		this->_start->setVisited();
	}
	
	CaveSystem(CaveSystem that) {
		for (auto e : that._caves) {
			if (e.getIsStart()) {
				this->_start = new Node(true);
				this->_caves.push_back(this->_start);
			}
			else if (e.getIsEnd()) {
				this->end = new Node(false);
				this->_caves.push_back(this->_end);
			}
			else this->_caves.push_back(new Node(e.getNode().getID()));
		}
	}
	
	~CaveSystem() {
		for (auto e : this->_caves) delete e;
	}
	
	void addRestriction(char *n1, char *n2) {
		Node *node1 = getNode(n1), *node2 = getNode(n2);
		if (strcmp(n1, "end") != 0) node1->addAdjacent(node2);
		if (strcmp(n1, "start") != 0) node2->addAdjacent(node1);
	}
	
	Node *getStart() {
		return this->_start;
	}
	
	Node *getEnd() {
		return this->_end;
	}
	
	std::vector<CaveSystem> getAllPaths() {
		for (auto e : this->_ptr->getAdjacents()) {
			
		}
	}
};

int getPaths()

int main() {
	FILE *file = NULL;
	file = fopen(FILE_NAME, "r");

	CaveSystem caves;
	char c1[10], c2[10];
	fscanf(file, "%[^-]-%[^\n]%*c", c1, c2);
	while(!feof(file)) {
		caves.addRestriction(c1, c2);
		
		fscanf(file, "%[^-]-%[^\n]%*c", c1, c2);
	}
	
	caves.getStart()->setVisited();
	
	//printf("%c\n", caves.getStart()->getAdjacents()[0]->getNode().getID());

	//printf("%d\n", x*y);
	fclose(file);
}
