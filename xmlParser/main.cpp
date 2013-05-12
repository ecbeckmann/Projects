#include <fstream>
#include "Node.h"

Node* parse(string name);
void printTree(Node* treeRoot); //given the root node of a tree structure, will print the tree to an xml file
Node* add(Node* node1, Node* node2); /*given two nodes that point to the same size tree structures, will add together the
data fields of the two trees*/

ifstream* infile;
ofstream outfile;

int main(){

	infile = new ifstream("datacompilation.xml");
	infile->ignore(100,'\n');//ignore the first line of the file, which contains xml version info
	infile->ignore(100, '<'); //read until the first "<"

	string name;

	getline(*infile, name, '>');//get the name of the base node;
	Node* dataTreeRoot = parse(name);
	infile->close();

	infile = new ifstream("test2.xml");
	infile->ignore(100,'\n');//ignore the first line of the file, which contains xml version info
	infile->ignore(100, '<'); //read until the first "<"

	getline(*infile, name, '>');//get the name of the base node;
	Node* dataTreeRoot2 = parse(name);

	Node* sumTreeRoot = add(dataTreeRoot, dataTreeRoot2);//add together the tree structures
	
	outfile.open("testwrite.xml");
	outfile << "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>" << endl;
	printTree(sumTreeRoot);

	return 1;
}

Node* parse(string name){
	Node* node = new Node(name);

	while(true){
		char ch;
		infile->get(ch);
		if(ch == '<'){
			string subname;
			string fullname;
			string category = "";
			getline(*infile, fullname, '>');
			
			int space;
			space = fullname.find_first_of(" ");
			if(space != -1){//name includes a category
				subname = fullname.substr(0, space);
				int quote;
				quote = fullname.find_first_of("\"");
				category = fullname.substr(quote+1,fullname.length()-quote-2);
			}
			else{//no category
				subname = fullname;
			}


			if(subname == ("/" + name)) break;//reached closing tag
			else{
				node->addSubnode(parse(subname));
				node->getLastSubnode()->setCategory(category);
			}
		}
		else if((ch != ' ') && (ch != '\n') && (ch != '\t')) node->appendData(ch);
	}
	return node;
}

void printTree(Node* treeRoot){
	if(treeRoot->getCategory() == "")//no category
	{
		outfile << "<" << treeRoot->getName() << ">" << endl;
	}
	else{
		outfile << "<" << treeRoot->getName() << " category=\"" << treeRoot->getCategory() << "\">" << endl;
	}
	
	outfile << treeRoot->getData() << endl;

	Node* currentSubnode = treeRoot->getFirstSubnode();
	while(currentSubnode != NULL){
		printTree(currentSubnode);
		currentSubnode = currentSubnode->getNextSubnode();
	}

	outfile << "</" << treeRoot->getName() << ">" << endl;

}

Node* add(Node* node1, Node* node2){
	Node* sumTree = new Node(node1->getName());

	sumTree->setCategory(node1->getCategory());

	sumTree->appendData(node1->getData());
	sumTree->appendData('\n');
	sumTree->appendData(node2->getData());

	Node* currentSubnode1 = node1->getFirstSubnode();
	Node* currentSubnode2 = node2->getFirstSubnode();
	while(currentSubnode1 != NULL){
		sumTree->addSubnode(add(currentSubnode1, currentSubnode2));
		currentSubnode1 = currentSubnode1->getNextSubnode();
		currentSubnode2 = currentSubnode2->getNextSubnode();
	}

	return sumTree;

}