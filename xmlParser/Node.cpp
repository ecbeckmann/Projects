#include "Node.h"

Node::Node(string name){
	Name = name;
	lastSubnode = NULL;
	SubnodesHead = NULL;
	nextSubnode = NULL;
}

void Node::addSubnode(Node* subnode){
	if(lastSubnode == NULL){//linked list has not yet been initialized
		lastSubnode = subnode;
		SubnodesHead = lastSubnode;
	}
	else{
		lastSubnode->setNextSubnode(subnode);
		lastSubnode = subnode;
	}
}

void Node::setNextSubnode(Node* subnode){
	nextSubnode = subnode;
}

void Node::appendData(char ch){
	data.append(1, ch);
}

void Node::appendData(string str){
	data.append(str);
}

void Node::setCategory(string category){
	Category = category;
}

string Node::getCategory(void){
	return Category;
}

Node* Node::getFirstSubnode(void){
	return SubnodesHead;
}

Node* Node::getLastSubnode(void){
	return lastSubnode;
}

Node* Node::getNextSubnode(void){
	return nextSubnode;
}

string Node::getName(void)
{
	return Name;
}

string Node::getData(void){
	return data;
}