#pragma once

using namespace std;

// initiating constexpr masks
constexpr unsigned long long notAMask = (0xFEFEFEFEFEFEFEFEULL);
constexpr unsigned long long notHMask = (0x7F7F7F7F7F7F7F7FULL);
constexpr unsigned long long not1Mask = (0xFFFFFFFFFFFFFF00ULL);
constexpr unsigned long long not8Mask = (0xFFFFFFFFFFFFFFULL);
constexpr unsigned long long notABMask = (0xFCFCFCFCFCFCFCFCULL);
constexpr unsigned long long notGHMask = (0x3F3F3F3F3F3F3F3FULL);
constexpr unsigned long long Rank4Mask = (0xFF000000ULL);
constexpr unsigned long long Rank5Mask = (0xFF00000000ULL);


class Board {
private:	
	// Bitboards
	unsigned long long existenceBitboard = 0; // 1 - piece, 0 - empty
	unsigned long long whiteBitboard = 0; // 1 - white piece, 0 - not white piece
	unsigned long long blackBitboard = 0; // 1 - black piece, 0 - not black piece
	unsigned long long pawnBitboard = 0; // 1 - pawn, 0 - not pawn
	unsigned long long knightBitboard = 0; // 1 - knight, 0 - not knight
	unsigned long long bishopBitboard = 0; // 1 - bishop, 0 - not bishop
	unsigned long long rookBitboard = 0; // 1 - THE ROOOOK!, 0 - not the rook
	unsigned long long queenBitboard = 0; // 1 - queen, 0 - not queen
	unsigned long long kingBitboard = 0; // 1 - king, 0 - not king

	// Magic Bitboards

	inline static unsigned long long MagicRook[64][4096];
	inline static unsigned long long MagicBishop[64][4096];

	// Main Rules
	bool moveTurn = true; // true - white turn, false - black turn
	bool CastlingRights[4]{1,1,1,1}; // 0 - white short, 1 - white long, 2 - black short, 3 - black long 
	char epRow = -1, epCol = -1; // en passant cords

public:
	void drawBoard();
	void drawBitBoard(unsigned long long bitBoard);
	void initStartPos();
	void setTestPos();
	unsigned long long genKnightMoves();
	unsigned long long genKingMoves();
	unsigned long long genSinglePawnMoves();
	unsigned long long genDoublePawnMoves();
	unsigned long long genPawnMoves();
	unsigned long long RookRaycasting(char absPos, unsigned long long artificialBitboard);
	unsigned long long BishopRaycasting(char absPos, unsigned long long artificialBitboard);
	unsigned long long RookBlockerMask(char absPos);
	unsigned long long BishopBlockerMask(char absPos);

	Board() { initStartPos(); }
};