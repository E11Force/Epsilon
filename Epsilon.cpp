#include <iostream>
#include "Board.h" // Board class with majority of variables

using namespace std;

#pragma region DebugUI

void Board::drawBoard() {
	for (int i = 7; i >= 0; i--) // white is down the screen
	{
		for (int j = 0; j < 8; j++)
		{
			unsigned long long absPosition = (1ULL << (i * 8 + j)); // absolute position from 2d -> 1d

			if (whiteBitboard & absPosition) // white squares
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
	cout << endl;
}

void Board::drawBitBoard(unsigned long long bitBoard)
{
	for (int i = 7; i >= 0; i--) {
		for (int j = 0; j < 8; j++) {
			unsigned long long absPosition = (1ULL << (i * 8 + j));
			if (bitBoard & absPosition)
			{
				cout << "1 ";
			}
			else
			{
				cout << ". ";
			}
		}
		cout << endl;
	}
	cout << endl;
}

#pragma endregion

unsigned long long Board::genKnightMoves() { // all legal knight moves on bitboard
	
	unsigned long long moveBitboard = 0;

	if (moveTurn == true) { // White
		unsigned long long bufferBitboard = (knightBitboard & whiteBitboard); // getting all white knights on the board
		moveBitboard |= ((bufferBitboard << 17) & notAMask) | ((bufferBitboard << 10) & notABMask) | ((bufferBitboard >> 6) & notABMask) | ((bufferBitboard >> 15) & notAMask) | ((bufferBitboard >> 17) & notHMask) | ((bufferBitboard >> 10) & notGHMask) | ((bufferBitboard << 6) & notGHMask) | ((bufferBitboard << 15) & notHMask);
		moveBitboard &= ~whiteBitboard;
		return moveBitboard;
	}
	else { // Black
		unsigned long long bufferBitboard = (knightBitboard & blackBitboard);
		moveBitboard |= ((bufferBitboard << 17) & notAMask) | ((bufferBitboard << 10) & notABMask) | ((bufferBitboard >> 6) & notABMask) | ((bufferBitboard >> 15) & notAMask) | ((bufferBitboard >> 17) & notHMask) | ((bufferBitboard >> 10) & notGHMask) | ((bufferBitboard << 6) & notGHMask) | ((bufferBitboard << 15) & notHMask);
		moveBitboard &= ~blackBitboard;
		return moveBitboard;
	}
}

void Board::initStartPos() {
	pawnBitboard |=		 (0xFFULL << 8) | (0xFFULL << 48);
	rookBitboard |=		 (0x81ULL) | (0x81ULL << 56);
	bishopBitboard |=	 (0x24ULL) | (0x24ULL << 56);
	knightBitboard |=	 (0x42ULL) | (0x42ULL << 56);
	queenBitboard |=	 (0x8ULL) | (0x8ULL << 56);
	kingBitboard |=		 (0x10ULL) | (0x10ULL << 56);
	whiteBitboard |=	 (0xFFULL) | (0xFFULL << 8);
	blackBitboard |=	 (0xFFULL << 48) | (0xFFULL << 56);
	existenceBitboard |= (whiteBitboard | blackBitboard);
}

int main() {
	Board ChessBoard;
	ChessBoard.drawBoard();
	ChessBoard.drawBitBoard(ChessBoard.genKnightMoves());
}