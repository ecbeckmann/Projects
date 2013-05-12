#include "Player.h"

class ComputerPlayer : public Player
{
private:
	/*int OneMoveFromWinMasks[24];
	int WinningMove[24];*/

public:
	ComputerPlayer(symbols symb);
	int getMove(int configurationMaskSelf, int configurationMaskOpponent);
};