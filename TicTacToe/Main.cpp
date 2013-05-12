#include "Game.h"
#include <iostream>
using namespace std;



int main(){

	while(true){
		system("CLS");
		cout << "Welcome to Tic Tac Toe" << endl;//prints out welcome phrase
		Game game;
		game.play();
	}
}