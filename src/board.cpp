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
	byTypeBB[Pawn] =	0xFF00000000FF00ULL;
	byTypeBB[King] =	0x1000000000000010ULL;
	byTypeBB[Queen] =	0x800000000000008ULL;
	byTypeBB[Bishop] =	0x2400000000000024ULL;
	byTypeBB[Knight] =	0x4200000000000042ULL;
	byTypeBB[Rook] =	(0x8100000000000081);
	
	// Colors
	byColorBB[White] =	0xffff;
	byColorBB[Black] =	0xffff000000000000;

	// Index
	for (int i = 7; i >= 0; i--) {
		for (int j = 0; j < 8; j++) {
			Square index = (i * 8 + j);
			Bitboard indexMask = (1ULL << index);
			if (byTypeBB[Pawn] & indexMask) { byIndexBB[index] = Pawn; byRights[index] = 15; }
			else if (byTypeBB[King] & indexMask) { 
				byIndexBB[index] = King; 
				if (byColorBB[White] & byTypeBB[King] & indexMask) { byRights[index] = 12; }
				else if (byColorBB[Black] & byTypeBB[King]) { byRights[index] = 3; }
			}
			else if (byTypeBB[Queen] & indexMask) { byIndexBB[index] = Queen; byRights[index] = 15; }
			else if (byTypeBB[Bishop] & indexMask) { byIndexBB[index] = Bishop; byRights[index] = 15; }
			else if (byTypeBB[Knight] & indexMask) { byIndexBB[index] = Knight; byRights[index] = 15; }
			else if (byTypeBB[Rook] & indexMask) { 
				byIndexBB[index] = Rook;
				if (~notfileA & byTypeBB[Rook] & byColorBB[White] & indexMask) { byRights[index] = 13; }
				else if (~notfileA & byTypeBB[Rook] & byColorBB[Black] & indexMask) { byRights[index] = 7; }
				else if (~notfileH & byTypeBB[Rook] & byColorBB[White] & indexMask) { byRights[index] = 14; }
				else if (~notfileH & byTypeBB[Rook] & byColorBB[Black] & indexMask) { byRights[index] = 11; }
			}
			else { byIndexBB[index] = Null; byRights[index] = 15; }
		}
	}


}

bool Board::isCastling(Move& move) {
	if (byIndexBB[move.source] == King && abs(move.destination - move.source) == 2) { return true; }
	else { return false; }
}

void Board::makeMove(Move& move) {
	Bitboard sourceMask = (1ULL << move.source);
	Bitboard destinationMask = (1ULL << move.destination);

	if (isCastling(move) == true) { 
		switch (move.destination) {
		case 6: {
			Bitboard sourceMask = (1ULL << 7);
			Bitboard destinationMask = (1ULL << 5);
			byTypeBB[Rook] = (byTypeBB[Rook] & ~sourceMask) | destinationMask;
			byColorBB[moveTurn] = (byColorBB[moveTurn] & ~sourceMask) | destinationMask;
			byIndexBB[7] = Null; byIndexBB[5] = Rook;
			break;
		}
		case 2: {
			Bitboard sourceMask = (1ULL << 0);
			Bitboard destinationMask = (1ULL << 3);
			byTypeBB[Rook] = (byTypeBB[Rook] & ~sourceMask) | destinationMask;
			byColorBB[moveTurn] = (byColorBB[moveTurn] & ~sourceMask) | destinationMask;
			byIndexBB[0] = Null; byIndexBB[3] = Rook;
			break;
		}
		case 62: {
			Bitboard sourceMask = (1ULL << 63);
			Bitboard destinationMask = (1ULL << 61);
			byTypeBB[Rook] = (byTypeBB[Rook] & ~sourceMask) | destinationMask;
			byColorBB[moveTurn] = (byColorBB[moveTurn] & ~sourceMask) | destinationMask;
			byIndexBB[63] = Null; byIndexBB[61] = Rook;
			break;
		}
		case 58: {
			Bitboard sourceMask = (1ULL << 56);
			Bitboard destinationMask = (1ULL << 59);
			byTypeBB[Rook] = (byTypeBB[Rook] & ~sourceMask) | destinationMask;
			byColorBB[moveTurn] = (byColorBB[moveTurn] & ~sourceMask) | destinationMask;
			byIndexBB[56] = Null; byIndexBB[59] = Rook;
			break;
		}
		}
	}
	

	castleRights = castleRights & byRights[move.source] & byRights[move.destination];
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

	if (isCastling(move) == true) {
		switch (move.destination) {
		case 6: {
			Bitboard sourceMask = (1ULL << 7);
			Bitboard destinationMask = (1ULL << 5);
			byTypeBB[Rook] = (byTypeBB[Rook] & ~destinationMask) | sourceMask;
			byColorBB[moveTurn] = (byColorBB[moveTurn] & ~destinationMask) | sourceMask;
			byIndexBB[7] = Rook; byIndexBB[5] = Null;
			break;
		}
		case 2: {
			Bitboard sourceMask = (1ULL << 0);
			Bitboard destinationMask = (1ULL << 3);
			byTypeBB[Rook] = (byTypeBB[Rook] & ~destinationMask) | sourceMask;
			byColorBB[moveTurn] = (byColorBB[moveTurn] & ~destinationMask) | sourceMask;
			byIndexBB[0] = Rook; byIndexBB[3] = Null;
			break;
		}
		case 62: {
			Bitboard sourceMask = (1ULL << 63);
			Bitboard destinationMask = (1ULL << 61);
			byTypeBB[Rook] = (byTypeBB[Rook] & ~destinationMask) | sourceMask;
			byColorBB[moveTurn] = (byColorBB[moveTurn] & ~destinationMask) | sourceMask;
			byIndexBB[63] = Rook; byIndexBB[61] = Null;
			break;
		}
		case 58: {
			Bitboard sourceMask = (1ULL << 56);
			Bitboard destinationMask = (1ULL << 59);
			byTypeBB[Rook] = (byTypeBB[Rook] & ~destinationMask) | sourceMask;
			byColorBB[moveTurn] = (byColorBB[moveTurn] & ~destinationMask) | sourceMask;
			byIndexBB[56] = Rook; byIndexBB[59] = Null;
			break;
		}
		}
	}

	castleRights = move.buffer_castleRights;
	if (byIndexBB[move.destination] != Null) { byTypeBB[byIndexBB[move.destination]] = byTypeBB[byIndexBB[move.destination]] & ~destinationMask | sourceMask; }
	byTypeBB[move.capturedPiece] |= destinationMask;
	byIndexBB[move.source] = byIndexBB[move.destination];
	byIndexBB[move.destination] = move.capturedPiece;
	byColorBB[moveTurn] = byColorBB[moveTurn] & ~destinationMask | sourceMask;
	if (byIndexBB[move.destination] != Null) { byColorBB[!moveTurn] = byColorBB[!moveTurn] | destinationMask; }
}