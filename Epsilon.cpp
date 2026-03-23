#include <iostream>
#include "Board.h"

using namespace std;

int main()
{
	unsigned long long testBoard = 0;

	Board board(testBoard);
	board.initStartPos();
	board.drawBitboard();

}