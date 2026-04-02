/*
	Epsilon - Chess Engine
	Copyright (C) 2026 Fozik

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU Affero General Public License as published
	by the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Affero General Public License for more details.

	You should have received a copy of the GNU Affero General Public License
	along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#pragma once
#include <cstdint>
#include <array>
#include <iostream>

// Namespaces
using Bitboard = uint_fast64_t; // uint64_t / uint_fast64_t
using Square = uint_fast8_t; // uint8_t / uint_fast8_t

// structures
struct Move {
	Square source;
	Square destination;
	Square capturedPiece;
};


// Constexpr Masks
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
	std::array<Bitboard, 7> byTypeBB; // array with all piece bitboards (uint_fast64_t is more performant than simple uint64_t or ULL) ex. Pawns Rooks..
	std::array<Bitboard, 2> byColorBB; // array with color bitboards (White/Black)
	std::array<Square, 64> byIndexBB; // represents the board where each square has it own value ex. Void, Pawn, Rook.. etc
	std::array<char, 7> byCharBB = {'.','P','N','B','R','Q','K'};

	// Enums
	enum typeBitboard {
		Void = 0,
		Pawn,
		Knight,
		Bishop,
		Rook,
		Queen,
		King
	};
	
	enum colorBitboard {
		Black,
		White
	};

	// Basic Rules
	bool moveTurn = true;

public:
	// board functions
	void initStartPos();
	void drawBitboard(Bitboard anyBitboard);
	void drawBoard();
	void makeMove(Square sourceIndex, Square destinationIndex);
	void unmakeMove(Square sourceIndex, Square destinationIndex, Square capturedPiece);

	// move generation functions
	Bitboard generateKnightMoves(Square index);
	Bitboard generateKingMoves(Square index);
	Bitboard generatePawnMoves(Square index);
	Bitboard RookRaycasting(Square index);
	Bitboard BishopRaycasting(Square index);
	Bitboard QueenRaycasting(Square index);

	Board() {
		initStartPos();
		drawBoard();
	} // on board create
};