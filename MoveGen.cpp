#include <vector>
#include "Board.h"

unsigned long long Board::genKnightMoves(char absPos) { // all legal knight moves on bitboard
	unsigned long long moveBitboard = 0; // place to save all our moves
	unsigned long long currentBitboard = moveTurn ? Bitboards[White] : Bitboards[Black];
	unsigned long long bufferBitboard = (1ULL << absPos);

	moveBitboard |= ((bufferBitboard << 17) & notAMask) | ((bufferBitboard << 10) & notABMask) | ((bufferBitboard >> 6) & notABMask) | ((bufferBitboard >> 15) & notAMask) | ((bufferBitboard >> 17) & notHMask) | ((bufferBitboard >> 10) & notGHMask) | ((bufferBitboard << 6) & notGHMask) | ((bufferBitboard << 15) & notHMask);
	moveBitboard &= ~currentBitboard; // we dont eat our color pieces!! (moveBitboard &= ~Bitboards[White] for ex)
	return moveBitboard;
}

unsigned long long Board::genKingMoves(char absPos) { // generate available legal king moves on bitboard
	unsigned long long moveBitboard = 0;
	unsigned long long currentBitboard = moveTurn ? Bitboards[White] : Bitboards[Black]; // define who is moving Implemented DRY
	unsigned long long bufferBitboard = (1ULL << absPos);

	moveBitboard |= ((bufferBitboard << 8) | ((bufferBitboard << 9) & notAMask) | ((bufferBitboard << 1) & notAMask) | ((bufferBitboard >> 7) & notAMask) | (bufferBitboard >> 8) | ((bufferBitboard >> 9) & notHMask) | ((bufferBitboard >> 1) & notHMask) | ((bufferBitboard << 7) & notHMask));
	moveBitboard &= ~currentBitboard;
	return moveBitboard;
}

unsigned long long Board::genSinglePawnMoves(char absPos) { // single move (+8)
	unsigned long long moveBitboard = 0;
	unsigned long long bufferBitboard = (1ULL << absPos);

	if (moveTurn == true) {
		moveBitboard |= (bufferBitboard << 8);
		moveBitboard &= ~Bitboards[Existence]; // we cant push our pawn if there is someone standing furter us
		return moveBitboard;
	}
	else {
		moveBitboard |= (bufferBitboard >> 8);
		moveBitboard &= ~Bitboards[Existence];
		return moveBitboard;
	}
}

unsigned long long Board::genDoublePawnMoves(char absPos) {
	unsigned long long moveBitboard = genSinglePawnMoves(absPos);

	if (moveTurn == true) {
		moveBitboard |= ((moveBitboard << 8) & ~Bitboards[Existence] & Rank4Mask); // remove those who didn't reached rank4 because they didn't maked a double push
		return moveBitboard;
	}
	else {
		moveBitboard |= ((moveBitboard >> 8) & ~Bitboards[Existence] & Rank5Mask);
		return moveBitboard;
	}
}

unsigned long long Board::genPawnMoves(char absPos) { // here we gen attacks for pawns
	unsigned long long moveBitboard = genDoublePawnMoves(absPos);
	unsigned long long bufferBitboard = (1ULL << absPos);

	if (moveTurn == true) {

		moveBitboard |= ((((bufferBitboard << 7) & notHMask) | ((bufferBitboard << 9) & notAMask)) & Bitboards[Black]);
		return moveBitboard;
	}
	else {
		moveBitboard |= ((((bufferBitboard >> 9) & notHMask) | ((bufferBitboard >> 7) & notAMask)) & Bitboards[White]);
		return moveBitboard;
	}
}

unsigned long long Board::RookRaycasting(char absPos, unsigned long long artificialBitboard) { // shoots the ray in all 4 directions from abs position
	unsigned long long moveBitboard = 0;													   // THE ROOOOOOOOOOOOOOOK!!! RAYCASTING!!!!!
	unsigned long long raycastBuffer = (1ULL << absPos); // put this in position of piece

	while ((raycastBuffer & notHMask) != 0)
	{
		raycastBuffer = raycastBuffer << 1;
		moveBitboard |= raycastBuffer;
		if ((raycastBuffer & ~artificialBitboard) == 0) { break; } // if there is someone standing
	}

	raycastBuffer = (1ULL << absPos);

	while ((raycastBuffer & notAMask) != 0)
	{
		raycastBuffer = raycastBuffer >> 1;
		moveBitboard |= raycastBuffer;
		if ((raycastBuffer & ~artificialBitboard) == 0) { break; }
	}

	raycastBuffer = (1ULL << absPos);

	while ((raycastBuffer & not8Mask) != 0)
	{
		raycastBuffer = raycastBuffer << 8;
		moveBitboard |= raycastBuffer;
		if ((raycastBuffer & ~artificialBitboard) == 0) { break; }
	}

	raycastBuffer = (1ULL << absPos);

	while ((raycastBuffer & not1Mask) != 0)
	{
		raycastBuffer = raycastBuffer >> 8;
		moveBitboard |= raycastBuffer;
		if ((raycastBuffer & ~artificialBitboard) == 0) { break; }
	}

	return moveBitboard;
}

unsigned long long Board::BishopRaycasting(char absPos, unsigned long long artificialBitboard) {
	unsigned long long moveBitboard = 0;
	unsigned long long raycastBuffer = (1ULL << absPos);

	while ((raycastBuffer & notHMask) != 0 && (raycastBuffer & not8Mask) != 0)
	{
		raycastBuffer = raycastBuffer << 9;
		moveBitboard |= raycastBuffer;
		if ((raycastBuffer & ~artificialBitboard) == 0) { break; }
	}

	raycastBuffer = (1ULL << absPos);

	while ((raycastBuffer & notAMask) != 0 && (raycastBuffer & not8Mask) != 0)
	{
		raycastBuffer = raycastBuffer << 7;
		moveBitboard |= raycastBuffer;
		if ((raycastBuffer & ~artificialBitboard) == 0) { break; }
	}

	raycastBuffer = (1ULL << absPos);

	while ((raycastBuffer & not1Mask) != 0 && (raycastBuffer & notHMask) != 0)
	{
		raycastBuffer = raycastBuffer >> 7;
		moveBitboard |= raycastBuffer;
		if ((raycastBuffer & ~artificialBitboard) == 0) { break; }
	}

	raycastBuffer = (1ULL << absPos);

	while ((raycastBuffer & not1Mask) != 0 && (raycastBuffer & notAMask) != 0)
	{
		raycastBuffer = raycastBuffer >> 9;
		moveBitboard |= raycastBuffer;
		if ((raycastBuffer & ~artificialBitboard) == 0) { break; }
	}

	return moveBitboard;
}

unsigned long long Board::QueenRaycasting(char absPos, unsigned long long artificialBitboard) { // the easiest raycasting i've ever made!
	return BishopRaycasting(absPos, artificialBitboard) | RookRaycasting(absPos, artificialBitboard);
}

vector<Move> Board::GenerateAllMoves() {
	vector<Move> LegalMoves;
	unsigned long long currentBitboard = moveTurn ? Bitboards[White] : Bitboards[Black];
	unsigned long long BitboardMask = moveTurn ? Bitboards[White] : Bitboards[Black];
	unsigned long long bufferBitboard = 0;

	while (currentBitboard != 0) {
		unsigned long index;
		_BitScanForward64(&index, currentBitboard); // getting first bit of mask
		char pieceID = BitboardIndex[index];
		if (pieceID != Void) {
			if (pieceID == Pawn) { bufferBitboard = genPawnMoves(index); }
			else if (pieceID == Rook) { bufferBitboard = RookRaycasting(index, Bitboards[Existence]) & ~BitboardMask; }
			else if (pieceID == Bishop) { bufferBitboard = BishopRaycasting(index, Bitboards[Existence]) & ~BitboardMask; }
			else if (pieceID == Queen) { bufferBitboard = QueenRaycasting(index, Bitboards[Existence]) & ~BitboardMask; }
			else if (pieceID == Knight) { bufferBitboard = genKnightMoves(index); }
			else if (pieceID == King) { bufferBitboard = genKingMoves(index); }
		}

		while (bufferBitboard != 0) {
			unsigned long moveIndx;
			_BitScanForward64(&moveIndx, bufferBitboard);
			Move move; move.dest = moveIndx; move.source = index; move.capturedPiece = BitboardIndex[moveIndx];
			LegalMoves.push_back(move);
			bufferBitboard &= bufferBitboard - 1;
		}
		currentBitboard &= currentBitboard - 1;
	}
	return LegalMoves;
}

bool Board::isSquareAttacked(char absPos) { // making all attack masks from our position to check if there is someone attacking us (reverse attack)
	unsigned long long squareMask = (1ULL << absPos);
	unsigned long long currentBoard = moveTurn ? Bitboards[Black] : Bitboards[White];
	unsigned long long knightBuffer = genKnightMoves(absPos);
	if ((knightBuffer & (currentBoard & Bitboards[Knight])) != 0) { return true; }
	unsigned long long rookBuffer = RookRaycasting(absPos, Bitboards[Existence]);
	if ((rookBuffer & (currentBoard & Bitboards[Rook])) != 0 || (rookBuffer & (currentBoard & Bitboards[Queen])) != 0) { return true; }
	unsigned long long bishopBuffer = BishopRaycasting(absPos, Bitboards[Existence]);
	if ((bishopBuffer & (currentBoard & Bitboards[Bishop])) != 0 || (bishopBuffer & (currentBoard & Bitboards[Queen])) != 0) { return true; }
	unsigned long long kingBuffer = genKingMoves(absPos);
	if ((kingBuffer & (currentBoard & Bitboards[King])) != 0) { return true; }
	if (moveTurn == true) {
		if (((squareMask << 7) & (currentBoard & Bitboards[Pawn]) & notHMask) != 0 || ((squareMask << 9) & (currentBoard & Bitboards[Pawn]) & notAMask) != 0) { return true; }
	}
	else if (moveTurn == false) {
		if (((squareMask >> 7) & (currentBoard & Bitboards[Pawn]) & notAMask) != 0 || ((squareMask >> 9) & (currentBoard & Bitboards[Pawn]) & notHMask) != 0) { return true; }
	}
	return false;
}

vector<Move> Board::GenerateLegalMoves() {
	vector<Move> LegalMoves;
	vector<Move> pseudoMoves = GenerateAllMoves();

	for (int i = 0; i < pseudoMoves.size(); i++) {
		MakeMove(pseudoMoves[i].source, pseudoMoves[i].dest);
		moveTurn = !moveTurn;
		unsigned long long currentBitboard = moveTurn ? Bitboards[White] : Bitboards[Black];
		unsigned long kingIndx;
		_BitScanForward64(&kingIndx, Bitboards[King] & currentBitboard);
		if (isSquareAttacked(kingIndx) == false) { LegalMoves.push_back(pseudoMoves[i]); }
		moveTurn = !moveTurn;
		UnmakeMove(pseudoMoves[i].source, pseudoMoves[i].dest, pseudoMoves[i].capturedPiece);
		
	}
	
	return LegalMoves;
}

// TODO: MagicBitboards after Minimal Vital Product
//unsigned long long Board::RookBlockerMask(char absPos) { // we dont actually need corners to make magic bitboards (save some time on that)
//	unsigned long long raycastBuffer = RookRaycasting(absPos, 0ULL);
//
//	if ((absPos / 8) != 0) {
//		raycastBuffer &= not1Mask;
//	}
//	if ((absPos / 8) != 7) {
//		raycastBuffer &= not8Mask;
//	}
//	if ((absPos % 8) != 0) {
//		raycastBuffer &= notAMask;
//	}
//	if ((absPos % 8) != 7) {
//		raycastBuffer &= notHMask;
//	}
//	return raycastBuffer;
//}
//
//unsigned long long Board::BishopBlockerMask(char absPos) {
//	unsigned long long raycastBuffer = BishopRaycasting(absPos, 0ULL);
//	return raycastBuffer &= not1Mask & not8Mask & notAMask & notHMask;
//}
//
//unsigned long long Board::enumerateAllSubsetsOfTheBitboardUniverse(unsigned long long mask, unsigned long long **MagicBitboard) { // took name of function from https://www.chessprogramming.org/Traversing_Subsets_of_a_Set what a beast???
//	unsigned long long subset = mask;
//	MagicBitboard[0][0] = subset;
//
//	int counter = 1;
//
//	while (true)
//	{
//		subset = (subset - 1) & mask;
//		MagicBitboard[0][counter] = subset;
//		counter++;
//		if (subset == 0) { break; }
//	}
//
//	return subset;
//}
