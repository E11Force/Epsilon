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

void Board::setTestPos()
{
	existenceBitboard = 0;
	whiteBitboard = 0;
	blackBitboard = 0;
	pawnBitboard = 0;
	knightBitboard = 0;
	bishopBitboard = 0;
	rookBitboard = 0;
	queenBitboard = 0;
	kingBitboard = 0;

	//kingBitboard |= (1ULL << 34);
	//whiteBitboard |= (1ULL << 34);
	//drawBitBoard(genKingMoves());
	//initStartPos();
}

#pragma endregion

unsigned long long Board::genKnightMoves() { // all legal knight moves on bitboard
	unsigned long long moveBitboard = 0;
	unsigned long long currentBitboard = moveTurn ? whiteBitboard : blackBitboard;
	unsigned long long bufferBitboard = (knightBitboard & currentBitboard);

	moveBitboard |= ((bufferBitboard << 17) & notAMask) | ((bufferBitboard << 10) & notABMask) | ((bufferBitboard >> 6) & notABMask) | ((bufferBitboard >> 15) & notAMask) | ((bufferBitboard >> 17) & notHMask) | ((bufferBitboard >> 10) & notGHMask) | ((bufferBitboard << 6) & notGHMask) | ((bufferBitboard << 15) & notHMask);
	moveBitboard &= ~currentBitboard;
	return moveBitboard;
}

unsigned long long Board::genKingMoves() { // generate available legal king moves on bitboard
	unsigned long long moveBitboard = 0;
	unsigned long long currentBitboard = moveTurn ? whiteBitboard : blackBitboard; // define who is moving Implemented DRY
	unsigned long long bufferBitboard = (kingBitboard & currentBitboard);

	moveBitboard |= ((bufferBitboard << 8) | ((bufferBitboard << 9) & notAMask) | ((bufferBitboard << 1) & notAMask) | ((bufferBitboard >> 7) & notAMask) | (bufferBitboard >> 8) | ((bufferBitboard >> 9) & notHMask) | ((bufferBitboard >> 1) & notHMask) | ((bufferBitboard << 7) & notHMask));
	moveBitboard &= ~currentBitboard;
	return moveBitboard;
}

unsigned long long Board::genSinglePawnMoves() { // single move (+8)
	unsigned long long moveBitboard = 0;

	if (moveTurn == true) {
		unsigned long long bufferBitboard = (pawnBitboard & whiteBitboard);
		moveBitboard |= (bufferBitboard << 8);
		moveBitboard &= ~existenceBitboard;
		return moveBitboard;
	}
	else {
		unsigned long long bufferBitboard = (pawnBitboard & blackBitboard);
		moveBitboard |= (bufferBitboard >> 8);
		moveBitboard &= ~existenceBitboard;
		return moveBitboard;
	}
}

unsigned long long Board::genDoublePawnMoves() {
	unsigned long long moveBitboard = genSinglePawnMoves();

	if (moveTurn == true) {
		moveBitboard |= ((moveBitboard << 8) & ~existenceBitboard & Rank4Mask);
		return moveBitboard;
	}
	else {
		moveBitboard |= ((moveBitboard >> 8) & ~existenceBitboard & Rank5Mask);
		return moveBitboard;
	}
}

unsigned long long Board::genPawnMoves() {
	unsigned long long moveBitboard = genDoublePawnMoves();

	if (moveTurn == true) {
		unsigned long long bufferBitboard = (pawnBitboard & whiteBitboard);
		moveBitboard |= ((((bufferBitboard << 7) & notHMask) | ((bufferBitboard << 9) & notAMask)) & blackBitboard);
		return moveBitboard;
	}
	else {
		unsigned long long bufferBitboard = (pawnBitboard & blackBitboard);
		moveBitboard |= ((((bufferBitboard >> 9) & notHMask) | ((bufferBitboard >> 7) & notAMask)) & whiteBitboard);
		return moveBitboard;
	}
}

unsigned long long Board::RookRaycasting(char absPos, unsigned long long artificialBitboard) { // shoots the ray in all 4 directions from abs position
	unsigned long long moveBitboard = 0;
	unsigned long long raycastBuffer = (1ULL << absPos);

	while ((raycastBuffer & notHMask) != 0)
	{
		raycastBuffer = raycastBuffer << 1;
		moveBitboard |= raycastBuffer;
		if ((raycastBuffer & ~artificialBitboard) == 0) { break; }
	}

	raycastBuffer = (1ULL << absPos);

	while ((raycastBuffer & notAMask) != 0)
	{
		raycastBuffer = raycastBuffer >> 1;
		moveBitboard |= raycastBuffer;
		if ((raycastBuffer & ~artificialBitboard) == 0) { break; }
	}

	raycastBuffer = (1ULL << absPos);

	while ((raycastBuffer & not8Mask) != 0)
	{
		raycastBuffer = raycastBuffer << 8;
		moveBitboard |= raycastBuffer;
		if ((raycastBuffer & ~artificialBitboard) == 0) { break; }
	}

	raycastBuffer = (1ULL << absPos);

	while ((raycastBuffer & not1Mask) != 0)
	{
		raycastBuffer = raycastBuffer >> 8;
		moveBitboard |= raycastBuffer;
		if ((raycastBuffer & ~artificialBitboard) == 0) { break; }
	}

	return moveBitboard;
}

unsigned long long Board::BishopRaycasting(char absPos, unsigned long long artificialBitboard) {
	unsigned long long moveBitboard = 0;
	unsigned long long raycastBuffer = (1ULL << absPos);

	while ((raycastBuffer & notHMask) != 0 && (raycastBuffer & not8Mask) != 0)
	{
		raycastBuffer = raycastBuffer << 9;
		moveBitboard |= raycastBuffer;
		if ((raycastBuffer & ~artificialBitboard) == 0) { break; }
	}

	raycastBuffer = (1ULL << absPos);

	while ((raycastBuffer & notAMask) != 0 && (raycastBuffer & not8Mask) != 0)
	{
		raycastBuffer = raycastBuffer << 7;
		moveBitboard |= raycastBuffer;
		if ((raycastBuffer & ~artificialBitboard) == 0) { break; }
	}

	raycastBuffer = (1ULL << absPos);

	while ((raycastBuffer & not1Mask) != 0 && (raycastBuffer & notHMask) != 0)
	{
		raycastBuffer = raycastBuffer >> 7;
		moveBitboard |= raycastBuffer;
		if ((raycastBuffer & ~artificialBitboard) == 0) { break; }
	}

	raycastBuffer = (1ULL << absPos);

	while ((raycastBuffer & not1Mask) != 0 && (raycastBuffer & notAMask) != 0)
	{
		raycastBuffer = raycastBuffer >> 9;
		moveBitboard |= raycastBuffer;
		if ((raycastBuffer & ~artificialBitboard) == 0) { break; }
	}

	return moveBitboard;
}

unsigned long long Board::RookBlockerMask(char absPos) { // we dont actually need corners to make magic bitboards (save some time on that)
	unsigned long long raycastBuffer = RookRaycasting(absPos, 0ULL);

	if ((absPos / 8) != 0) {
		raycastBuffer &= not1Mask;
	}
	if ((absPos / 8) != 7) {
		raycastBuffer &= not8Mask;
	}
	if ((absPos % 8) != 0) {
		raycastBuffer &= notAMask;
	}
	if ((absPos % 8) != 7) {
		raycastBuffer &= notHMask;
	}
	return raycastBuffer;
}

unsigned long long Board::BishopBlockerMask(char absPos) {
	unsigned long long raycastBuffer = BishopRaycasting(absPos, 0ULL);
	return raycastBuffer &= not1Mask & not8Mask & notAMask & notHMask;
}

void Board::initStartPos() {
	pawnBitboard =		 (0xFFULL << 8) | (0xFFULL << 48);
	rookBitboard =		 (0x81ULL) | (0x81ULL << 56);
	bishopBitboard =	 (0x24ULL) | (0x24ULL << 56);
	knightBitboard =	 (0x42ULL) | (0x42ULL << 56);
	queenBitboard =		 (0x8ULL) | (0x8ULL << 56);
	kingBitboard =		 (0x10ULL) | (0x10ULL << 56);
	whiteBitboard =		 (0xFFULL) | (0xFFULL << 8);
	blackBitboard =		 (0xFFULL << 48) | (0xFFULL << 56);
	existenceBitboard =  (whiteBitboard | blackBitboard); 
}

int main() {
	Board ChessBoard;
	ChessBoard.drawBoard();
	ChessBoard.drawBitBoard(ChessBoard.BishopBlockerMask(28));
}