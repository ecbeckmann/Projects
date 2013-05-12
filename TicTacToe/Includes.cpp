#include "Includes.h"


int mask(int position)
{
	// position is 1 - 9
	return 1 << (9-position);
}