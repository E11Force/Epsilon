#pragma once
#include <cstdint>
#include <array>
#include <iostream>

using Bitboard = uint_fast64_t;

// Masks
constexpr Bitboard notfileA =	(0xfefefefefefefefe);
constexpr Bitboard notfileAB =	(0xfcfcfcfcfcfcfcfc);
constexpr Bitboard notfileH =	(0x7f7f7f7f7f7f7f7f);
constexpr Bitboard notfileGH =	(0x3f3f3f3f3f3f3f3f);
constexpr Bitboard notRank1 =	(0xffffffffffffff00);
constexpr Bitboard notRank8 =	(0xffffffffffffff);
constexpr Bitboard rank4 =		(0xff000000);
constexpr Bitboard rank5 =		(0xff00000000);

class Board { // LERF Mapping
protected:
	// Bitboards
	std::array<Bitboard, 6> byTypeBB; // array with all piece bitboards (uint_fast64_t is more performant than simple uint64_t or ULL) ex. Pawns Rooks..
	std::array<Bitboard, 2> byColorBB; // array with color bitboards (White/Black)
	std::array<char, 64> byIndexBB; // represents the board where each square has it own value ex. Void, Pawn, Rook.. etc
	std::array<char, 6> byCharBB = {'P','N','B','R','Q','K'};

	// Enums
	enum typeBitboard {
		Null = -1,
		Pawn,
		Knight,
		Bishop,
		Rook,
		Queen,
		King
	};
	
	enum colorBitboard {
		White,
		Black
	};

	// Basic Rules
	bool moveTurn = true;

public:
	// board functions
	void initStartPos();
	void drawBitboard(Bitboard anyBitboard);
	void drawBoard();

	// move generation functions
	Bitboard generateKnightMoves(char index);
	Bitboard generateKingMoves(char index);
	Bitboard generatePawnMoves(char index);
	Bitboard RookRaycasting(char index);
	Bitboard BishopRaycasting(char index);
	Bitboard QueenRaycasting(char index);

	Board() {
		initStartPos(); 
		drawBitboard(QueenRaycasting(21));
		drawBitboard(byColorBB[Black]);
	} // on create object
};