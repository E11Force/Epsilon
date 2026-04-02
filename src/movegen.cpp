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

#include <vector>
#include <intrin.h>
#include "board.hpp"

// Knight
Bitboard Board::generateKnightMoves(Square index) {
	Bitboard IndexMask = (1ULL << index);
	Bitboard KnightMoves;
	Bitboard colorBitboard = moveTurn ? byColorBB[White] : byColorBB[Black];
	KnightMoves = ((IndexMask << 15) & notfileH) | ((IndexMask << 17) & notfileA) | ((IndexMask << 10) & notfileAB) | ((IndexMask >> 6) & notfileAB) | ((IndexMask >> 15) & notfileA) | ((IndexMask >> 17) & notfileH) | ((IndexMask >> 10) & notfileGH) | ((IndexMask << 6) & notfileGH);
	KnightMoves &= ~colorBitboard;
	return KnightMoves;
}

// King
Bitboard Board::generateKingMoves(Square index) {
	Bitboard IndexMask = (1ULL << index);
	Bitboard KingMoves;
	Bitboard colorBitboard = moveTurn ? byColorBB[White] : byColorBB[Black];
	KingMoves = ((IndexMask << 8)) | ((IndexMask << 9) & notfileA) | ((IndexMask << 1) & notfileA) | ((IndexMask >> 7) & notfileA) | ((IndexMask >> 8)) | ((IndexMask >> 9) & notfileH) | ((IndexMask >> 1) & notfileH) | ((IndexMask << 7) & notfileH);
	KingMoves &= ~colorBitboard;
	return KingMoves;
}

// Pawn
Bitboard Board::generatePawnMoves(Square index) {
	Bitboard IndexMask = (1ULL << index);
	Bitboard PawnMoves = 0;
	Bitboard colorBitboard = moveTurn ? byColorBB[White] : byColorBB[Black];
	Bitboard existenceBitboard = byColorBB[White] | byColorBB[Black];
	if (moveTurn == true) {
		PawnMoves |= ((IndexMask << 8) & ~existenceBitboard);
		PawnMoves |= ((PawnMoves << 8) & ~existenceBitboard & rank4);
		PawnMoves |= (((IndexMask << 7) & notfileH & byColorBB[Black]) | ((IndexMask << 9) & notfileA & byColorBB[Black]));
	}
	else if (moveTurn == false) {
		PawnMoves |= ((IndexMask >> 8) & ~existenceBitboard);
		PawnMoves |= ((PawnMoves >> 8) & ~existenceBitboard & rank5);
		PawnMoves |= (((IndexMask >> 7) & notfileA & byColorBB[White]) | ((IndexMask >> 9) & notfileH & byColorBB[White]));
	}
	return PawnMoves;
}

// Rook
Bitboard Board::RookRaycasting(Square index) {
	Bitboard IndexMask(1ULL << index);
	Bitboard RookMoves = 0;
	Bitboard enemyBitboard = moveTurn ? byColorBB[Black] : byColorBB[White];
	Bitboard currentBitboard = moveTurn ? byColorBB[White] : byColorBB[Black];

	while ((IndexMask & notRank1) != 0) {
		IndexMask = IndexMask >> 8;
		if ((IndexMask & ~currentBitboard) == 0) { break; } // if he got into our piece
		RookMoves |= IndexMask;
		if ((IndexMask & ~enemyBitboard) == 0) { break; }
	}
	IndexMask = (1ULL << index);
	while ((IndexMask & notRank8) != 0) {
		IndexMask = IndexMask << 8;
		if ((IndexMask & ~currentBitboard) == 0) { break; }
		RookMoves |= IndexMask;
		if ((IndexMask & ~enemyBitboard) == 0) { break; }
	}
	IndexMask = (1ULL << index);
	while ((IndexMask & notfileA) != 0) {
		IndexMask = IndexMask >> 1;
		if ((IndexMask & ~currentBitboard) == 0) { break; }
		RookMoves |= IndexMask;
		if ((IndexMask & ~enemyBitboard) == 0) { break; }
	}
	IndexMask = (1ULL << index);
	while ((IndexMask & notfileH) != 0) {
		IndexMask = IndexMask << 1;
		if ((IndexMask & ~currentBitboard) == 0) { break; }
		RookMoves |= IndexMask;
		if ((IndexMask & ~enemyBitboard) == 0) { break; }
	}

	//RookMoves = RookMoves & ~currentBitboard;

	return RookMoves;
}

// Bishop
Bitboard Board::BishopRaycasting(Square index) {
	Bitboard IndexMask(1ULL << index);
	Bitboard BishopMoves = 0;
	Bitboard enemyBitboard = moveTurn ? byColorBB[Black] : byColorBB[White];
	Bitboard currentBitboard = moveTurn ? byColorBB[White] : byColorBB[Black];

	while ((IndexMask & notRank8 & notfileH) != 0) {
		IndexMask = IndexMask << 9;
		if ((IndexMask & ~currentBitboard) == 0) { break; }
		BishopMoves |= IndexMask;
		if ((IndexMask & ~enemyBitboard) == 0) { break; }
	}
	IndexMask = (1ULL << index);
	while ((IndexMask & notRank1 & notfileH) != 0) {
		IndexMask = IndexMask >> 7;
		if ((IndexMask & ~currentBitboard) == 0) { break; }
		BishopMoves |= IndexMask;
		if ((IndexMask & ~enemyBitboard) == 0) { break; }
	}
	IndexMask = (1ULL << index);
	while ((IndexMask & notfileA & notRank1) != 0) {
		IndexMask = IndexMask >> 9;
		if ((IndexMask & ~currentBitboard) == 0) { break; }
		BishopMoves |= IndexMask;
		if ((IndexMask & ~enemyBitboard) == 0) { break; }
	}
	IndexMask = (1ULL << index);
	while ((IndexMask & notfileA & notRank8) != 0) {
		IndexMask = IndexMask << 7;
		if ((IndexMask & ~currentBitboard) == 0) { break; }
		BishopMoves |= IndexMask;
		if ((IndexMask & ~enemyBitboard) == 0) { break; }
	}

	//BishopMoves = BishopMoves & ~currentBitboard;

	return BishopMoves;
}

// Queen
Bitboard Board::QueenRaycasting(Square index) {
	return RookRaycasting(index) | BishopRaycasting(index);
}

std::vector<Move> Board::GeneratePseudoLegalMoves(Bitboard anyBoard) {
	std::vector<Move> PseudoLegalMoves;
	Bitboard byColorBufferBB = anyBoard;
	Bitboard bufferBitboard = 0;

	while (byColorBufferBB != 0) {
		unsigned long index = 0;
		_BitScanForward64(&index, byColorBufferBB);

		if (byIndexBB[index] == Pawn) { bufferBitboard = generatePawnMoves(index); }
		else if (byIndexBB[index] == Knight) { bufferBitboard = generateKnightMoves(index); }
		else if (byIndexBB[index] == Bishop) { bufferBitboard = BishopRaycasting(index); }
		else if (byIndexBB[index] == Rook) { bufferBitboard = RookRaycasting(index); }
		else if (byIndexBB[index] == Queen) { bufferBitboard = QueenRaycasting(index); }
		else if (byIndexBB[index] == King) { bufferBitboard = generateKingMoves(index); }

		while (bufferBitboard != 0) {
			unsigned long bufferIndex = 0;
			_BitScanForward64(&bufferIndex, bufferBitboard);
			Move pseudoMove; pseudoMove.source = index; pseudoMove.destination = bufferIndex; pseudoMove.capturedPiece = byIndexBB[bufferIndex];
			PseudoLegalMoves.push_back(pseudoMove);
			bufferBitboard &= bufferBitboard - 1;
		}
		byColorBufferBB &= byColorBufferBB - 1;
	}
	return PseudoLegalMoves;
}

Bitboard Board::Perft(int depth) {
	std::vector<Move> perftMoves;
	Bitboard nodes = 0;

	if (depth == 0) { return 1ULL; }

	perftMoves = GeneratePseudoLegalMoves(byColorBB[moveTurn]);
	for (int i = 0; i < perftMoves.size(); i++) {
		makeMove(perftMoves[i]);
		nodes += Perft(depth - 1);
		unmakeMove(perftMoves[i]);
	}
	return nodes;
}