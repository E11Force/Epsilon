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

#include "board.hpp"
#include <cstdint>
#include <iostream>


// Board debug functions
void Board::drawBoard() { // LERF Mapping
	for (int i = 7; i >= 0; i--) {
		for (int j = 0; j < 8; j++) {
			Square index = (i * 8 + j);
			Bitboard indexMask = (1ULL << index);
			if (byColorBB[White] & indexMask) { std::cout << byCharBB[byIndexBB[index]] << ' '; }
			else if (byColorBB[Black] & indexMask) { std::cout << (char)tolower(byCharBB[byIndexBB[index]]) << ' '; }
			else { std::cout << ". "; }
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

void Board::drawBitboard(Bitboard anyBitboard) {
	for (int i = 7; i >= 0; i--) {
		for (int j = 0; j < 8; j++) {
			Bitboard indexMask = (1ULL << (i * 8 + j));
			if (anyBitboard & indexMask) { std::cout << "\033[1;32m1 \033[0m"; }
			else { std::cout << "\033[1;37m. \033[0m"; }
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

// Board manipulation functions
void Board::initStartPos() {
	// Pieces
	byTypeBB[Pawn] =	0xFF00000000FF00;
	byTypeBB[King] =	0x1000000000000010;
	byTypeBB[Queen] =	0x800000000000008;
	byTypeBB[Bishop] =	0x2400000000000024;
	byTypeBB[Knight] =	0x4200000000000042;
	byTypeBB[Rook] =	0x8100000000000081;
	
	// Colors
	byColorBB[White] =	0xffff;
	byColorBB[Black] =	0xffff000000000000;

	// Index
	for (int i = 7; i >= 0; i--) {
		for (int j = 0; j < 8; j++) {
			Square index = (i * 8 + j);
			Bitboard indexMask = (1ULL << index);
			if (byTypeBB[Pawn] & indexMask) { byIndexBB[index] = Pawn; }
			else if (byTypeBB[King] & indexMask) { byIndexBB[index] = King; }
			else if (byTypeBB[Queen] & indexMask) { byIndexBB[index] = Queen; }
			else if (byTypeBB[Bishop] & indexMask) { byIndexBB[index] = Bishop; }
			else if (byTypeBB[Knight] & indexMask) { byIndexBB[index] = Knight; }
			else if (byTypeBB[Rook] & indexMask) { byIndexBB[index] = Rook; }
			else { byIndexBB[index] = Null; }
		}
	}
}

void Board::makeMove(Move& move) {
	Bitboard sourceMask = (1ULL << move.source);
	Bitboard destinationMask = (1ULL << move.destination);

	if (byIndexBB[move.destination] != Null) { byTypeBB[byIndexBB[move.destination]] = byTypeBB[byIndexBB[move.destination]] & ~destinationMask; }
	byTypeBB[byIndexBB[move.source]] = byTypeBB[byIndexBB[move.source]] & ~sourceMask | destinationMask;
	byIndexBB[move.destination] = byIndexBB[move.source];
	byIndexBB[move.source] = Null;
	byColorBB[moveTurn] = byColorBB[moveTurn] & ~sourceMask | destinationMask;
	byColorBB[!moveTurn] = byColorBB[!moveTurn] & ~destinationMask;
	moveTurn = !moveTurn;
}

void Board::unmakeMove(Move& move) {
	moveTurn = !moveTurn;
	Bitboard sourceMask = (1ULL << move.source);
	Bitboard destinationMask = (1ULL << move.destination);

	if (byIndexBB[move.destination] != Null) { byTypeBB[byIndexBB[move.destination]] = byTypeBB[byIndexBB[move.destination]] & ~destinationMask | sourceMask; }
	byTypeBB[move.capturedPiece] |= destinationMask;
	byIndexBB[move.source] = byIndexBB[move.destination];
	byIndexBB[move.destination] = move.capturedPiece;
	byColorBB[moveTurn] = byColorBB[moveTurn] & ~destinationMask | sourceMask;
	if (byIndexBB[move.destination] != Null) { byColorBB[!moveTurn] = byColorBB[!moveTurn] | destinationMask; }
}