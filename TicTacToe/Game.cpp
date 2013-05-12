#include "Game.h"
#include <iostream>
#include <stdlib.h>
#include <string>

using namespace std;

int Game:: getConfigurationMask(symbols symbol){
	int configurationMask = 0;

	for(int i = 0; i < 9; i++){
		if(configuration[i] == symbol){
			configurationMask += mask(i+1);
		}
	}
	return configurationMask;
}

Game::Game(){
	gameState = PLAYER_ONE_TURN;
	turnCount = 0;
	for(int i = 0; i < 9; i++){
		configuration[i] = NONE;
	}

	string numPlayers;
	while(true){
		cout << "Input number of players(0, 1 or 2): ";
		getline(cin, numPlayers);
		if(numPlayers.size() != 1){
			cout << "Incorrect Input!" << endl;
		}
		else{
			if(numPlayers.at(0) - '0' == 0){
				Player1 = new ComputerPlayer(O);
				Player2 = new ComputerPlayer(X);
				break;
			}
			else if(numPlayers.at(0) - '0' == 1){
				Player1 = new ComputerPlayer(O);
				Player2 = new Player(X);
				/*Player1 = Player(false, O);
				Player2 = Player(true, X);*/
				break;
			}
			else if(numPlayers.at(0) - '0' == 2){
				Player1 = new Player(O);
				Player2 = new Player(X);
				/*Player1 = Player(false, O);
				Player2 = Player(false, X);*/
				break;
			}
			else{
				cout << "Incorrect Input!" << endl;
			}
		}
	}


	/*WinMasks[0] = mask(1)+ mask(2) + mask(3);
	WinMasks[1] = mask(4)+ mask(5) + mask(6);
	WinMasks[2] = mask(7)+ mask(8) + mask(9);
	WinMasks[3] = mask(1)+ mask(4) + mask(7);
	WinMasks[4] = mask(2)+ mask(5) + mask(8);
	WinMasks[5] = mask(3)+ mask(6) + mask(9);
	WinMasks[6] = mask(1)+ mask(5) + mask(9);
	WinMasks[7] = mask(3)+ mask(5) + mask(7);*/
}

void Game::play(){
	draw();
	cout << "Player One Turn" << endl;

	while((gameState == PLAYER_ONE_TURN) || (gameState == PLAYER_TWO_TURN)){
		int location;
		symbols symbol;
		while(true){
			switch(gameState){
			case PLAYER_ONE_TURN:
				location = Player1->getMove(getConfigurationMask(Player1->getSymbol()), getConfigurationMask(Player2->getSymbol()));
				symbol = Player1->getSymbol();
				break;
			case PLAYER_TWO_TURN:
				location = Player2->getMove(getConfigurationMask(Player2->getSymbol()), getConfigurationMask(Player1->getSymbol()));
				symbol = Player2->getSymbol();
				break;
			}
			if(!setMove(location, symbol)) cout << "Incorrect input!" << endl;
			else break;
		}
		turnCount++;

		draw();
		checkWinState();

		if(gameState == PLAYER_ONE_TURN){
			gameState = PLAYER_TWO_TURN;
			cout << "Player Two Turn" << endl;
		}
		else if (gameState == PLAYER_TWO_TURN){
			gameState = PLAYER_ONE_TURN;
			cout << "Player One Turn" << endl;
		}
	}
	switch(gameState){
	case PLAYER_ONE_WIN:
		cout << "Player One Wins!" << endl;
		break;
	case PLAYER_TWO_WIN:
		cout << "Player Two Wins!" << endl;
		break;
	case DRAW:
		cout << "Cat's Game" << endl;
		break;
	}
	system("pause");
}

string getSymbolCharacter(symbols symbol) {
	return symbol == O ? "O"
		: symbol == X ? "X"
		: " ";
}

void Game::draw(){
	system("CLS");
	cout << "   |   |    " << endl;

	cout << " " << getSymbolCharacter(configuration[0]) << " | " << getSymbolCharacter(configuration[1]) << " | " << getSymbolCharacter(configuration[2]) << " " << endl;

	cout << "---|---|---" << endl ;

	cout << " " << getSymbolCharacter(configuration[3]) << " | " << getSymbolCharacter(configuration[4]) << " | " << getSymbolCharacter(configuration[5]) << " " << endl;

	cout  << "---|---|---" << endl ;

	cout << " " << getSymbolCharacter(configuration[6]) << " | " << getSymbolCharacter(configuration[7]) << " | " << getSymbolCharacter(configuration[8]) << " " << endl ;

	cout << "   |   |   " << endl;
}


/*int Game::getUserInput(){
int input;

while(true){
cout << "Input move (1-9):";
cin >> input;

if((input < 1) || (input > 9) || configuration[input-1] != NONE) cout << "Incorrect input!" << endl;
else break;
}
return input;
}*/

bool Game::setMove(int location, symbols symbol){
	if((location >= 1) && (location <= 9) && ((symbol == O) || (symbol == X)) && (configuration[location-1] == NONE)){
		configuration[location-1] = symbol;
		return true;
	}
	else{
		return false;
	}
}

void Game::checkWinState(){
	bool win = false;
	int configurationMask;
	symbols currentSymbol;

	if(gameState == PLAYER_ONE_TURN){
		currentSymbol = Player1->getSymbol();
	}
	else{
		currentSymbol = Player2->getSymbol();
	}

	configurationMask = getConfigurationMask(currentSymbol);

	for(int i = 0 ; i < 8; i++){
		if((configurationMask & WinMasks[i]) == WinMasks[i]){
			win = true;
			break;
		}
	}

	if(win){
		if(gameState == PLAYER_ONE_TURN) gameState = PLAYER_ONE_WIN;
		else if(gameState == PLAYER_TWO_TURN) gameState = PLAYER_TWO_WIN;
	}
	else if(turnCount >= 9) gameState = DRAW;
}

Game :: ~Game(){
	delete[] Player1;
	delete[] Player2;
}