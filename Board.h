#pragma once
#include <iostream>

using namespace std;

class Board {
private:	
	unsigned long long existenceBitboard = 0; // 1 - есть фигура, 0 - нет фигуры
	unsigned long long whiteBitboard = 0; // 1 - белая фигура, 0 - не белая фигура
	unsigned long long blackBitboard = 0; // 1 - чёрная, 0 - не чёрная фигура
	unsigned long long pawnBitboard = 0; // 1 - пешка, 0 - не пешка
	unsigned long long knightBitboard = 0; // 1 - конь, 0 - не конь
	unsigned long long bishopBitboard = 0; // 1 - слон, 0 - не слон
	unsigned long long rookBitboard = 0; // 1 - ладья, 0 - не ладья
	unsigned long long queenBitboard = 0; // 1 - ферзь, 0 - не ферзь
	unsigned long long kingBitboard = 0; // 1 - король, 0 - не король

	bool moveTurn; // true - ход белых, false - ход черных
	bool CastlingRights[4]{1,1,1,1}; // 0 - белая короткая, 1 - белая длинная, 2 - черная короткая, 3 - черная длинная 
	char epRow = -1, epCol = -1; // координаты для en passant

public:
	Board(unsigned long long existenceBitboard)
	{
		this->existenceBitboard = existenceBitboard;
	}

	void drawBitboard()
	{
		for (int i = 7; i >= 0; i--) // white is down the screen
		{
			for (int j = 0; j < 8; j++)
			{
				unsigned long long absPosition = (1ULL << (i * 8 + j)); // absolute position from 2d -> 1d

				if (whiteBitboard & absPosition)
				{
					if (pawnBitboard & absPosition)
					{
						cout << "P ";
					}
					else if (rookBitboard & absPosition)
					{
						cout << "R ";
					}
					else if (bishopBitboard & absPosition)
					{
						cout << "B ";
					}
					else if (knightBitboard & absPosition)
					{
						cout << "N ";
					}
					else if (queenBitboard & absPosition)
					{
						cout << "Q ";
					}
					else if (kingBitboard & absPosition)
					{
						cout << "K ";
					}
				}
				else if (blackBitboard & absPosition) // black squares
				{
					if (pawnBitboard & absPosition)
					{
						cout << "p ";
					}
					else if (rookBitboard & absPosition)
					{
						cout << "r ";
					}
					else if (bishopBitboard & absPosition)
					{
						cout << "b ";
					}
					else if (knightBitboard & absPosition)
					{
						cout << "n ";
					}
					else if (queenBitboard & absPosition)
					{
						cout << "q ";
					}
					else if (kingBitboard & absPosition)
					{
						cout << "k ";
					}
				}
				else // nullptr
				{
					cout << "* ";
				}
			}
			cout << endl;
		}
	}

	void initStartPos() {
		pawnBitboard |=			(0xFFULL << 8) | (0xFFULL << 48);
		rookBitboard |=			(0x81ULL) | (0x81ULL << 56);
		bishopBitboard |=		(0x24ULL) | (0x24ULL << 56);
		knightBitboard |=		(0x42ULL) | (0x42ULL << 56);
		queenBitboard |=		(0x8ULL) | (0x8ULL << 56);
		kingBitboard |=			(0x10ULL) | (0x10ULL << 56);
		whiteBitboard |=		(0xFFULL) | (0xFFULL << 8);
		blackBitboard |=		(0xFFULL << 48) | (0xFFULL << 56);
		existenceBitboard |=	(whiteBitboard | blackBitboard);
	}
};