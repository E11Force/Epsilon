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
#include <iostream>
#include <cstdint>
#include <array>
#include <vector>

// Namespaces
using Bitboard = uint_fast64_t; // uint64_t / uint_fast64_t
using Square = uint_fast8_t; // uint8_t / uint_fast8_t
using namespace std;

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
constexpr array<unsigned long long, 10> perftValues{ 1,20,400,8902,197281,4865609,119060324,3195901860,84998978956,2439530234167 };

class Board { // LERF Mapping
protected:
	// Bitboards
	array<Bitboard, 7> byTypeBB; // array with all piece bitboards (uint_fast64_t is more performant than simple uint64_t or ULL) ex. Pawns Rooks..
	array<Bitboard, 2> byColorBB; // array with color bitboards (White/Black)
	array<Square, 64> byIndexBB; // represents the board where each square has it own value ex. Null, Pawn, Rook.. etc
	array<char, 7> byCharBB = {'.','P','N','B','R','Q','K'};
	array<bool, 4> castleRights = { 1,1,1,1 };
	Move en_passantMove;

	// Enums
	enum typeBitboard {
		Null = 0,
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
	void makeMove(Move& move);
	void unmakeMove(Move& move);

	// move generation functions
	Bitboard generateKnightMoves(Square index);
	Bitboard generateKingMoves(Square index);
	Bitboard generatePawnMoves(Square index);
	Bitboard RookRaycasting(Square index);
	Bitboard BishopRaycasting(Square index);
	Bitboard QueenRaycasting(Square index);
	bool isSquareAttacked(Square index);
	vector<Move> GeneratePseudoLegalMoves(Bitboard anyBitboard);
	vector<Move> GenerateLegalMoves(Bitboard anyBitboard);

	// other
	Bitboard Perft(int depth);
	void perftStart(int depth);

	Board() {
		Move move1; move1.source = 4; move1.destination = 20; move1.capturedPiece = Null;
		Move move2; move2.source = 63; move2.destination = 36; move2.capturedPiece = Null;
		Move move3; move3.source = 0; move3.destination = 16; move2.capturedPiece = Null;
		initStartPos(); 
		perftStart(8); 
	} // on board create
};