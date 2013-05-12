#include <string>

using namespace std;

class Node
{
private:
	string Name;
	string Category;
	Node* SubnodesHead; //head of a linked list of this node's subnodes
	Node* lastSubnode;//the current last subnode in the linked list
	Node* nextSubnode; //if this is a subnode, points to the next subnode in the linked list
	string data;

public:

	Node(string name);

	void addSubnode(Node* subnode);

	void setNextSubnode(Node* subnode);

	void appendData(char ch);

	void appendData(string str);

	void setCategory(string category);

	string getCategory();

	Node* getFirstSubnode(void);

	Node* getLastSubnode(void);

	Node* getNextSubnode(void);

	string getName(void);

	string getData(void);
};