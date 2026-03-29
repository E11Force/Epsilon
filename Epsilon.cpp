#include <iostream>
#include "Board.h" // Board class with majority of variables

using namespace std;

#pragma region DebugUI

void Board::drawBoard() {
	for (int i = 7; i >= 0; i--) // white is down the screen
	{
		for (int j = 0; j < 8; j++)
		{
			char absPosition = (i * 8 + j); // absolute position from 2d -> 1d
			unsigned long long absPosMask = (1ULL << absPosition);
			char pieceID = BitboardIndex[absPosition];

			if (pieceID != Void) {
				if ((absPosMask & Bitboards[White])) {
					cout << pieceLetters[pieceID - 3] << ' ';
				}
				else
				{
					cout << char(tolower(pieceLetters[pieceID - 3])) << ' ';
				}
			}
			else // Void
			{
				cout << ". ";
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
	Bitboards[Existence] = 0;
	Bitboards[White] = 0;
	Bitboards[Black] = 0;
	Bitboards[Pawn] = 0;
	Bitboards[Knight] = 0;
	Bitboards[Bishop] = 0;
	Bitboards[Rook] = 0;
	Bitboards[Queen] = 0;
	Bitboards[King] = 0;

	//Bitboards[King] |= (1ULL << 34);
	//Bitboards[White] |= (1ULL << 34);
	//drawBitBoard(genKingMoves());
	//initStartPos();
}

#pragma endregion

unsigned long long Board::genKnightMoves() { // all legal knight moves on bitboard
	unsigned long long moveBitboard = 0;
	unsigned long long currentBitboard = moveTurn ? Bitboards[White] : Bitboards[Black];
	unsigned long long bufferBitboard = (Bitboards[Knight] & currentBitboard);

	moveBitboard |= ((bufferBitboard << 17) & notAMask) | ((bufferBitboard << 10) & notABMask) | ((bufferBitboard >> 6) & notABMask) | ((bufferBitboard >> 15) & notAMask) | ((bufferBitboard >> 17) & notHMask) | ((bufferBitboard >> 10) & notGHMask) | ((bufferBitboard << 6) & notGHMask) | ((bufferBitboard << 15) & notHMask);
	moveBitboard &= ~currentBitboard;
	return moveBitboard;
}

unsigned long long Board::genKingMoves() { // generate available legal king moves on bitboard
	unsigned long long moveBitboard = 0;
	unsigned long long currentBitboard = moveTurn ? Bitboards[White] : Bitboards[Black]; // define who is moving Implemented DRY
	unsigned long long bufferBitboard = (Bitboards[King] & currentBitboard);

	moveBitboard |= ((bufferBitboard << 8) | ((bufferBitboard << 9) & notAMask) | ((bufferBitboard << 1) & notAMask) | ((bufferBitboard >> 7) & notAMask) | (bufferBitboard >> 8) | ((bufferBitboard >> 9) & notHMask) | ((bufferBitboard >> 1) & notHMask) | ((bufferBitboard << 7) & notHMask));
	moveBitboard &= ~currentBitboard;
	return moveBitboard;
}

unsigned long long Board::genSinglePawnMoves() { // single move (+8)
	unsigned long long moveBitboard = 0;

	if (moveTurn == true) {
		unsigned long long bufferBitboard = (Bitboards[Pawn] & Bitboards[White]);
		moveBitboard |= (bufferBitboard << 8);
		moveBitboard &= ~Bitboards[Existence];
		return moveBitboard;
	}
	else {
		unsigned long long bufferBitboard = (Bitboards[Pawn] & Bitboards[Black]);
		moveBitboard |= (bufferBitboard >> 8);
		moveBitboard &= ~Bitboards[Existence];
		return moveBitboard;
	}
}

unsigned long long Board::genDoublePawnMoves() {
	unsigned long long moveBitboard = genSinglePawnMoves();

	if (moveTurn == true) {
		moveBitboard |= ((moveBitboard << 8) & ~Bitboards[Existence] & Rank4Mask);
		return moveBitboard;
	}
	else {
		moveBitboard |= ((moveBitboard >> 8) & ~Bitboards[Existence] & Rank5Mask);
		return moveBitboard;
	}
}

unsigned long long Board::genPawnMoves() {
	unsigned long long moveBitboard = genDoublePawnMoves();

	if (moveTurn == true) {
		unsigned long long bufferBitboard = (Bitboards[Pawn] & Bitboards[White]);
		moveBitboard |= ((((bufferBitboard << 7) & notHMask) | ((bufferBitboard << 9) & notAMask)) & Bitboards[Black]);
		return moveBitboard;
	}
	else {
		unsigned long long bufferBitboard = (Bitboards[Pawn] & Bitboards[Black]);
		moveBitboard |= ((((bufferBitboard >> 9) & notHMask) | ((bufferBitboard >> 7) & notAMask)) & Bitboards[White]);
		return moveBitboard;
	}
}

unsigned long long Board::RookRaycasting(char absPos, unsigned long long artificialBitboard) { // shoots the ray in all 4 directions from abs position
	unsigned long long moveBitboard = 0;													   // THE ROOOOOOOOOOOOOOOK!!! RAYCASTING!!!!!
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

unsigned long long Board::QueenRaycasting(char absPos, unsigned long long artificialBitboard) { // the easiest raycasting i've ever made!
	return BishopRaycasting(absPos, artificialBitboard) | RookRaycasting(absPos, artificialBitboard);
}

// TODO: MagicBitboards after Minimal Vital Product
//unsigned long long Board::RookBlockerMask(char absPos) { // we dont actually need corners to make magic bitboards (save some time on that)
//	unsigned long long raycastBuffer = RookRaycasting(absPos, 0ULL);
//
//	if ((absPos / 8) != 0) {
//		raycastBuffer &= not1Mask;
//	}
//	if ((absPos / 8) != 7) {
//		raycastBuffer &= not8Mask;
//	}
//	if ((absPos % 8) != 0) {
//		raycastBuffer &= notAMask;
//	}
//	if ((absPos % 8) != 7) {
//		raycastBuffer &= notHMask;
//	}
//	return raycastBuffer;
//}
//
//unsigned long long Board::BishopBlockerMask(char absPos) {
//	unsigned long long raycastBuffer = BishopRaycasting(absPos, 0ULL);
//	return raycastBuffer &= not1Mask & not8Mask & notAMask & notHMask;
//}
//
//unsigned long long Board::enumerateAllSubsetsOfTheBitboardUniverse(unsigned long long mask, unsigned long long **MagicBitboard) { // took name of function from https://www.chessprogramming.org/Traversing_Subsets_of_a_Set what a beast???
//	unsigned long long subset = mask;
//	MagicBitboard[0][0] = subset;
//
//	int counter = 1;
//
//	while (true)
//	{
//		subset = (subset - 1) & mask;
//		MagicBitboard[0][counter] = subset;
//		counter++;
//		if (subset == 0) { break; }
//	}
//
//	return subset;
//}

void Board::initStartPos() {
	Bitboards[Pawn] =		 (0xFFULL << 8) | (0xFFULL << 48);
	Bitboards[Rook] =		 (0x81ULL) | (0x81ULL << 56);
	Bitboards[Bishop] =	 (0x24ULL) | (0x24ULL << 56);
	Bitboards[Knight] =	 (0x42ULL) | (0x42ULL << 56);
	Bitboards[Queen] =		 (0x8ULL) | (0x8ULL << 56);
	Bitboards[King] =		 (0x10ULL) | (0x10ULL << 56);
	Bitboards[White] =		 (0xFFULL) | (0xFFULL << 8);
	Bitboards[Black] =		 (0xFFULL << 48) | (0xFFULL << 56);
	Bitboards[Existence] =  (Bitboards[White] | Bitboards[Black]);

	for (int i = 7; i >= 0; i--) { // I'LL MAKE THIS ONE FREAKIN TIME FOR OPTIMIZATION CODE FURTHER
		for (int j = 0; j < 8; j++) {
			char absValue = (i * 8 + j);
			unsigned long long absPosition = (1ULL << absValue); // absolute position from 2d -> 1d

			if (Bitboards[Pawn] & absPosition) {
				BitboardIndex[absValue] = Pawn;
			}
			else if (Bitboards[Rook] & absPosition) {
				BitboardIndex[absValue] = Rook;
			}
			else if (Bitboards[Bishop] & absPosition) {
				BitboardIndex[absValue] = Bishop;
			}
			else if (Bitboards[Knight] & absPosition) {
				BitboardIndex[absValue] = Knight;
			}
			else if (Bitboards[Queen] & absPosition) {
				BitboardIndex[absValue] = Queen;
			}
			else if (Bitboards[King] & absPosition) {
				BitboardIndex[absValue] = King;
			}
			else {
				BitboardIndex[absValue] = Void;
			}
		}
	}
}

void Board::MakeMove(int source, int destination) {
	unsigned long long sourceBitboard = (1ULL << source); // same energy as down comment
	unsigned long long destinationBitboard = (1ULL << destination); // 1 on its place in this empty bitboard
	char myColor = moveTurn ? White : Black; // whos going to move???
	char enemyColor = moveTurn ? Black : White;
	char movingID = BitboardIndex[source];
	char destinationID = BitboardIndex[destination]; // getting specific piece ex. Pawn

	if (movingID == Void) { return; } // protection from dumbasses

	if (destinationID != Void) {
		Bitboards[destinationID] &= ~destinationBitboard; // Pawn is being erased from her own board // Bitboards[Pawn] &= ~destinationBitboard
		Bitboards[enemyColor] &= ~destinationBitboard; // still deleting the pawn but now from her color bitboard
	}
	Bitboards[movingID] &= ~sourceBitboard; // deleting the piece that attacked this poor pawn outa his source value on bitboard
	Bitboards[myColor] &= ~sourceBitboard;
	Bitboards[movingID] |= destinationBitboard; // place attacking piece on place of died pawn
	Bitboards[myColor] |= destinationBitboard;
	BitboardIndex[source] = Void;
	BitboardIndex[destination] = movingID;
	Bitboards[Existence] = Bitboards[White] | Bitboards[Black];
	moveTurn = !moveTurn;

	// Rest in piece little pawn 28.03.2026 - 29.03.2026 10:39:04 
}

int main() {
	Board ChessBoard;
	ChessBoard.drawBoard();
	ChessBoard.drawBitBoard(ChessBoard.QueenRaycasting(28,0));
}