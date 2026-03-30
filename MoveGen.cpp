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

vector<Move> GenerateAllMoves() {

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
