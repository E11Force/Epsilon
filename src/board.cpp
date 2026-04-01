#include "board.hpp"
#include <cstdint>
#include <iostream>

void Board::drawBoard() { // LERF Mapping
	for (int i = 7; i >= 0; i--) {
		for (int j = 0; j < 8; j++) {
			char index = (i * 8 + j);
			Bitboard indexMask = (1ULL << index);
			if (byColorBB[White] & indexMask) { std::cout << byCharBB[byIndexBB[index]] << ' '; }
			else if (byColorBB[Black] & indexMask) { std::cout << (char)tolower(byCharBB[byIndexBB[index]]) << ' '; }
			else { std::cout << ". "; }
		}
		std::cout << std::endl;
	}
}

void Board::drawBitboard(Bitboard anyBitboard) {
	for (int i = 7; i >= 0; i--) {
		for (int j = 0; j < 8; j++) {
			Bitboard indexMask = (1ULL << (i * 8 + j));
			if (anyBitboard & indexMask) { std::cout << "1 "; }
			else { std::cout << ". "; }
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

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
			char index = (i * 8 + j);
			Bitboard indexMask = (1ULL << index);
			if (byTypeBB[Pawn] & indexMask) { byIndexBB[index] = Pawn; }
			else if (byTypeBB[King] & indexMask) { byIndexBB[index] = King; }
			else if (byTypeBB[Queen] & indexMask) { byIndexBB[index] = Queen; }
			else if (byTypeBB[Bishop] & indexMask) { byIndexBB[index] = Bishop; }
			else if (byTypeBB[Knight] & indexMask)
			{ 
				byIndexBB[index] = Knight; 
			}
			else if (byTypeBB[Rook] & indexMask) { byIndexBB[index] = Rook; }
			else { byIndexBB[index] = Null; }
		}
	}
}