#include <iostream>
#include "Board.h"

using namespace std;

void Board::drawBoard() {
	for (int i = 7; i >= 0; i--) // white is down the screen
	{
		for (int j = 0; j < 8; j++)
		{
			char absPosition = (i * 8 + j); // absolute position from 2d -> 1d
			unsigned long long absPosMask = (1ULL << absPosition);
			char pieceID = BitboardIndex[absPosition]; // get id by abspos of piece

			if (pieceID != Void) { // if square not empty
				if ((absPosMask & Bitboards[White])) { // if the piece is white
					cout << pieceLetters[pieceID - 3] << ' '; // fetch letter from our array
				}
				else
				{
					cout << char(tolower(pieceLetters[pieceID - 3])) << ' '; // same as previous but lower it
				}
			}
			else // Void
			{
				cout << ". "; // its empty square
			}
		}
		cout << endl;
	}
	cout << endl;
}

void Board::drawBitBoard(unsigned long long bitBoard)
{
	for (int i = 7; i >= 0; i--) { //upside-down 
		for (int j = 0; j < 8; j++) { // every single square
			unsigned long long absPosition = (1ULL << (i * 8 + j));
			if (bitBoard & absPosition) // simpliest logic you can ever have
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

void Board::initStartPos() {
	moveTurn = true;
	CastlingRights[0] = 1; CastlingRights[1] = 1; CastlingRights[2] = 1; CastlingRights[3] = 1;
	epRow = -1; epCol = -1;
	Bitboards[Pawn] = (0xFFULL << 8) | (0xFFULL << 48);
	Bitboards[Rook] = (0x81ULL) | (0x81ULL << 56);
	Bitboards[Bishop] = (0x24ULL) | (0x24ULL << 56);
	Bitboards[Knight] = (0x42ULL) | (0x42ULL << 56);
	Bitboards[Queen] = (0x8ULL) | (0x8ULL << 56);
	Bitboards[King] = (0x10ULL) | (0x10ULL << 56);
	Bitboards[White] = (0xFFULL) | (0xFFULL << 8);
	Bitboards[Black] = (0xFFULL << 48) | (0xFFULL << 56);
	Bitboards[Existence] = (Bitboards[White] | Bitboards[Black]);

	for (int i = 7; i >= 0; i--) { // I'LL MAKE THIS ONE FREAKIN TIME FOR OPTIMIZATION CODE FURTHER
		for (int j = 0; j < 8; j++) {
			char absValue = (i * 8 + j);
			unsigned long long absPosition = (1ULL << absValue); // absolute position from 2d -> 1d (its more mask tbh (i need to rewrite my comments))

			if (Bitboards[Pawn] & absPosition) {
				BitboardIndex[absValue] = Pawn; // here we are filling up our BitboardIndex array with values..
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

void Board::MakeMove(char source, char destination) {
	unsigned long long sourceBitboard = (1ULL << source); // same energy as down comment
	unsigned long long destinationBitboard = (1ULL << destination); // mask of destBitboard
	char myColor = moveTurn ? White : Black; // whos going to move???
	char enemyColor = moveTurn ? Black : White; // anddd for black
	char movingID = BitboardIndex[source]; // down comment
	char destinationID = BitboardIndex[destination]; // getting specific piece ex. Pawn

	if (movingID == Void) { return; } // protection from dumbasses (we cant move void)

	if (destinationID != Void) { // if there is someone standing
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

void Board::UnmakeMove(char source, char destination, char capturedPiece) {
	moveTurn = !moveTurn;
	char myColor = moveTurn ? White : Black;
	char enemyColor = moveTurn ? Black : White;
	unsigned long long sourceMask = (1ULL << source);
	unsigned long long destMask = (1ULL << destination);
	BitboardIndex[source] = BitboardIndex[destination];
	BitboardIndex[destination] = capturedPiece;
	Bitboards[BitboardIndex[source]] &= ~destMask;
	Bitboards[BitboardIndex[source]] |= sourceMask;
	if (capturedPiece != Void) { Bitboards[capturedPiece] |= destMask; }
	Bitboards[myColor] |= sourceMask;
	Bitboards[myColor] &= ~destMask;
	if (capturedPiece != Void) { Bitboards[enemyColor] |= destMask; }
	Bitboards[Existence] = Bitboards[White] | Bitboards[Black];
}