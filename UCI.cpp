#include <string>
#include <iostream>
#include <sstream>
#include "Board.h"

using namespace std;

int ParseSquare(string square) {
	return square[0] - 'a' + (square[1] - '1') * 8; // getting from e2e4 real value for our bitboards
}

void ParseMove(string moveString, Board& board) { // slicing them in two to make a move
	int source = ParseSquare(moveString.substr(0, 2));
	int dest = ParseSquare(moveString.substr(2, 2));
	board.MakeMove(source,dest);
}

void InitUCI(Board& board) // UCI console, nothing special
{
	string command;

	while (true) {
		getline(cin, command);

		if (command == "uci") { cout << "uciok" << endl; }
		else if (command == "quit") { exit(0); }
		else if (command == "isready") { cout << "readyok" << endl; }
		else if (command.substr(0, 8) == "position") {
			board.initStartPos(); // reset the board
			size_t movesIndex = command.find("moves"); // find index of word moves
			if (movesIndex != string::npos) { // npos - "no position", means that returned value from find (if there is really nothing) is npos
				stringstream moves(command.substr(movesIndex + 6)); // moves - 5 ' ' - 1 5+1=6 wow such a hard math
				string move;
				while (moves >> move) { // get moves by one >:)
					ParseMove(move, board);
				}
			}
		}
		else if (command.substr(0, 2) == "go") {
			cout << "bestmove e2e4" << endl;
		}
	}
}