//The Game class controls input and output streams, and also keeps track of the current state of the game and passes off turns.


#include "ComputerPlayer.h"


class Game
{
private:
	symbols configuration[9];
	Player* Player1;
	Player* Player2;
	enum gameStates{PLAYER_ONE_TURN, PLAYER_TWO_TURN, PLAYER_ONE_WIN, PLAYER_TWO_WIN, DRAW} gameState;
	int turnCount;
	int getConfigurationMask(symbols symbol);
	void draw();
	void checkWinState();
	bool setMove(int location, symbols symbol);	
	//int WinMasks[8];

public:
	Game();
	void play();
	~Game();
};