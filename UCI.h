#pragma once
#include <string>
using namespace std;
int ParseSquare(string square);
void ParseMove(string moveString, Board& board);
void InitUCI(Board& board);
string SquareToUCI(int index);