#include "Includes.h"

/*class Player{
private:
	bool isAI;
	symbols symbol;
	int OneMoveFromWinMasks[24];
	int WinningMove[24];
public:
	Player();
	Player(bool AI, symbols symb);
	int getMove(int configurationMaskSelf, int configurationMaskOpponent);
	symbols getSymbol();
};*/

class Player{
protected:
	symbols symbol;
public:
	Player(symbols symb);
	virtual int getMove(int configurationMaskSelf, int configurationMaskOpponent);
	symbols getSymbol();
};

/*class ComputerPlayer : public Player
{
private:
	int OneMoveFromWinMasks[24];
	int WinningMove[24];

public:
	ComputerPlayer(symbols symb);
	int getMove(int configurationMaskSelf, int configurationMaskOpponent);
}*/