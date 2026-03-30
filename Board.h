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

struct Move {
	short source;
	short dest;
};

class Board {
private:	
	// Bitboards
	unsigned long long Bitboards[9];
	char BitboardIndex[64]; // array with size of real board and values = bitboards enums to quickly take any value from this array and tell which piece is placed on this square

	enum BitboardsEnum { // paired with bitboardIndex
		Void = -1,
		Existence = 0,
		White = 1,
		Black = 2,
		Pawn = 3,
		Knight = 4,
		Bishop = 5,
		Rook = 6,
		Queen = 7,
		King = 8
	};

	char pieceLetters[9]{'P','N','B','R','Q','K'}; // used to fastly place some pieces on the board (if black pieces used with tolower()) always -3 this array

	// Magic Bitboards
	//inline static unsigned long long MagicRook[64][4096];
	//inline static unsigned long long MagicBishop[64][4096];

	// Main Rules
	bool moveTurn = true; // true - white turn, false - black turn
	bool CastlingRights[4]{1,1,1,1}; // 0 - white short, 1 - white long, 2 - black short, 3 - black long 
	char epRow = -1, epCol = -1; // en passant cords

public:
	void drawBoard();
	void drawBitBoard(unsigned long long bitBoard);
	void initStartPos();
	void setTestPos();
	unsigned long long genKnightMoves(char absPos);
	unsigned long long genKingMoves(char absPos);
	unsigned long long genSinglePawnMoves(char absPos);
	unsigned long long genDoublePawnMoves(char absPos);
	unsigned long long genPawnMoves(char absPos);
	unsigned long long RookRaycasting(char absPos, unsigned long long artificialBitboard);
	unsigned long long BishopRaycasting(char absPos, unsigned long long artificialBitboard);
	unsigned long long QueenRaycasting(char absPos, unsigned long long artificialBitboard);
	void MakeMove(int source, int destination);
	
	// TODO: MagicBitboards after Minimal Vital Product
	//unsigned long long RookBlockerMask(char absPos);
	//unsigned long long BishopBlockerMask(char absPos);
	//unsigned long long enumerateAllSubsetsOfTheBitboardUniverse(unsigned long long mask, unsigned long long **MagicBitboard);

	Board() { initStartPos(); }
};