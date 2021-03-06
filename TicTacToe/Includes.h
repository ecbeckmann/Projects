enum symbols{X, O, NONE};
int mask(int position);
int getConfigurationMask(symbols symbol);

const int WinMasks[8] = {mask(1)+ mask(2) + mask(3),
	mask(4)+ mask(5) + mask(6),
	mask(7)+ mask(8) + mask(9),
	mask(1)+ mask(4) + mask(7),
	mask(2)+ mask(5) + mask(8),
	mask(3)+ mask(6) + mask(9),
	mask(1)+ mask(5) + mask(9),
	mask(3)+ mask(5) + mask(7)}; //This contains all the 8 possible ways of winning TicTacToe

const int OneMoveFromWinMasks[24] = {mask(2) + mask(3),
	mask(1) + mask(3),
	mask(1) + mask(2),
	mask(5) + mask(6),
	mask(4) + mask(6),
	mask(4) + mask(5),
	mask(8) + mask(9),
	mask(7) + mask(9),
	mask(7) + mask(8),
	mask(4) + mask(7),
	mask(1) + mask(7),
	mask(1) + mask(4),
	mask(5) + mask(8),
	mask(2) + mask(8),
	mask(2) + mask(5),
	mask(6) + mask(9),
	mask(3) + mask(9),
	mask(3) + mask(6),
	mask(5) + mask(9),
	mask(1) + mask(9),
	mask(1) + mask(5),
	mask(5) + mask(7),
	mask(3) + mask(7),
	mask(3) + mask(5)};

const int WinningMove[24] = {1,
	2,
	3,
	4,
	5,
	6,
	7,
	8,
	9,
	1,
	4,
	7,
	2,
	5,
	8,
	3,
	6,
	9,
	1,
	5,
	9,
	3,
	5,
	7};