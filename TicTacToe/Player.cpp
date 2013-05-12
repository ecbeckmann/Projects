#include "Player.h"
#include <iostream>
#include <stdlib.h>
#include <string>

using namespace std;

/*Player::Player(){
}
Player::Player(bool AI, symbols symb) : isAI(AI), symbol(symb) {
OneMoveFromWinMasks[0] = mask(2) + mask(3);
WinningMove[0] = 1;
OneMoveFromWinMasks[1] = mask(1) + mask(3);
WinningMove[1] = 2;
OneMoveFromWinMasks[2] = mask(1) + mask(2);
WinningMove[2] = 3;
OneMoveFromWinMasks[3] = mask(5) + mask(6);
WinningMove[3] = 4;
OneMoveFromWinMasks[4] = mask(4) + mask(6);
WinningMove[4] = 5;
OneMoveFromWinMasks[5] = mask(4) + mask(5);
WinningMove[5] = 6;
OneMoveFromWinMasks[6] = mask(8) + mask(9);
WinningMove[6] = 7;
OneMoveFromWinMasks[7] = mask(7) + mask(9);
WinningMove[7] = 8;
OneMoveFromWinMasks[8] = mask(7) + mask(8);
WinningMove[8] = 9;
OneMoveFromWinMasks[9] = mask(4) + mask(7);
WinningMove[9] = 1;
OneMoveFromWinMasks[10] = mask(1) + mask(7);
WinningMove[10] = 4;
OneMoveFromWinMasks[11] = mask(1) + mask(4) ;
WinningMove[11] = 7;
OneMoveFromWinMasks[12] = mask(5) + mask(8);
WinningMove[12] = 2;
OneMoveFromWinMasks[13] = mask(2) + mask(8);
WinningMove[13] = 5;
OneMoveFromWinMasks[14] = mask(2) + mask(5);
WinningMove[14] = 8;
OneMoveFromWinMasks[15] = mask(6) + mask(9);
WinningMove[15] = 3;
OneMoveFromWinMasks[16] = mask(3) + mask(9);
WinningMove[16] = 6;
OneMoveFromWinMasks[17] = mask(3) + mask(6);
WinningMove[17] = 9;
OneMoveFromWinMasks[18] = mask(5) + mask(9);
WinningMove[18] = 1;
OneMoveFromWinMasks[19] = mask(1) + mask(9);
WinningMove[19] = 5;
OneMoveFromWinMasks[20] = mask(1) + mask(5);
WinningMove[20] = 9;
OneMoveFromWinMasks[21] = mask(5) + mask(7);
WinningMove[21] = 3;
OneMoveFromWinMasks[22] = mask(3) + mask(7);
WinningMove[22] = 5;
OneMoveFromWinMasks[23] = mask(3) + mask(5);
WinningMove[23] = 7;
}

int Player::getMove(int configurationMaskSelf, int configurationMaskOpponent){

if(isAI){
//check if the player is one move away from winning
for(int i = 0 ; i < 24 ; i++){
if((configurationMaskSelf & OneMoveFromWinMasks[i]) == OneMoveFromWinMasks[i]){ //is the current configuration one move away from winning?
if(((configurationMaskSelf | configurationMaskOpponent) & mask(WinningMove[i])) != mask(WinningMove[i])){ //is the winning move position unoccupied?
return WinningMove[i];
}
}
}
for(int i = 0 ; i < 24 ; i++){
if((configurationMaskOpponent & OneMoveFromWinMasks[i]) == OneMoveFromWinMasks[i]){ //is the current configuration one move away from winning?
if(((configurationMaskOpponent | configurationMaskSelf) & mask(WinningMove[i])) != mask(WinningMove[i])){ //is the winning move position unoccupied?
return WinningMove[i];
}
}
}
//otherwise just return a random space
while(true){
int move = 0;
move = (rand() % 9) + 1;
if(((configurationMaskOpponent | configurationMaskSelf) & mask(move)) != mask(move)) return move;
}
return 0;
}
else{
int location = 0;
string locationString;

cout << "Input move (1-9):";

getline(cin, locationString);

if(locationString.length() == 1){
location = locationString.at(0) - '0';
}

return location;
}
return 0;

}

symbols Player::getSymbol(){
return symbol;
}*/

Player::Player(symbols symb) : symbol(symb) {
}

int Player::getMove(int configurationMaskSelf, int configurationMaskOpponent){
	int location = 0;
	string locationString;

	cout << "Input move (1-9):";

	getline(cin, locationString);

	if(locationString.length() == 1){
		location = locationString.at(0) - '0';
	}

	return location;

}

symbols Player::getSymbol(){
	return symbol;
}

/*ComputerPlayer::ComputerPlayer(symbols symb) : Player(symb){
OneMoveFromWinMasks[0] = mask(2) + mask(3);
WinningMove[0] = 1;
OneMoveFromWinMasks[1] = mask(1) + mask(3);
WinningMove[1] = 2;
OneMoveFromWinMasks[2] = mask(1) + mask(2);
WinningMove[2] = 3;
OneMoveFromWinMasks[3] = mask(5) + mask(6);
WinningMove[3] = 4;
OneMoveFromWinMasks[4] = mask(4) + mask(6);
WinningMove[4] = 5;
OneMoveFromWinMasks[5] = mask(4) + mask(5);
WinningMove[5] = 6;
OneMoveFromWinMasks[6] = mask(8) + mask(9);
WinningMove[6] = 7;
OneMoveFromWinMasks[7] = mask(7) + mask(9);
WinningMove[7] = 8;
OneMoveFromWinMasks[8] = mask(7) + mask(8);
WinningMove[8] = 9;
OneMoveFromWinMasks[9] = mask(4) + mask(7);
WinningMove[9] = 1;
OneMoveFromWinMasks[10] = mask(1) + mask(7);
WinningMove[10] = 4;
OneMoveFromWinMasks[11] = mask(1) + mask(4) ;
WinningMove[11] = 7;
OneMoveFromWinMasks[12] = mask(5) + mask(8);
WinningMove[12] = 2;
OneMoveFromWinMasks[13] = mask(2) + mask(8);
WinningMove[13] = 5;
OneMoveFromWinMasks[14] = mask(2) + mask(5);
WinningMove[14] = 8;
OneMoveFromWinMasks[15] = mask(6) + mask(9);
WinningMove[15] = 3;
OneMoveFromWinMasks[16] = mask(3) + mask(9);
WinningMove[16] = 6;
OneMoveFromWinMasks[17] = mask(3) + mask(6);
WinningMove[17] = 9;
OneMoveFromWinMasks[18] = mask(5) + mask(9);
WinningMove[18] = 1;
OneMoveFromWinMasks[19] = mask(1) + mask(9);
WinningMove[19] = 5;
OneMoveFromWinMasks[20] = mask(1) + mask(5);
WinningMove[20] = 9;
OneMoveFromWinMasks[21] = mask(5) + mask(7);
WinningMove[21] = 3;
OneMoveFromWinMasks[22] = mask(3) + mask(7);
WinningMove[22] = 5;
OneMoveFromWinMasks[23] = mask(3) + mask(5);
WinningMove[23] = 7;
}

int ComputerPlayer::getMove(int configurationMaskSelf, int configurationMaskOpponent){
//check if the player is one move away from winning
for(int i = 0 ; i < 24 ; i++){
if((configurationMaskSelf & OneMoveFromWinMasks[i]) == OneMoveFromWinMasks[i]){ //is the current configuration one move away from winning?
if(((configurationMaskSelf | configurationMaskOpponent) & mask(WinningMove[i])) != mask(WinningMove[i])){ //is the winning move position unoccupied?
return WinningMove[i];
}
}
}
for(int i = 0 ; i < 24 ; i++){
if((configurationMaskOpponent & OneMoveFromWinMasks[i]) == OneMoveFromWinMasks[i]){ //is the current configuration one move away from winning?
if(((configurationMaskOpponent | configurationMaskSelf) & mask(WinningMove[i])) != mask(WinningMove[i])){ //is the winning move position unoccupied?
return WinningMove[i];
}
}
}
//otherwise just return a random space
while(true){
int move = 0;
move = (rand() % 9) + 1;
if(((configurationMaskOpponent | configurationMaskSelf) & mask(move)) != mask(move)) return move;
}
return 0;
}*/