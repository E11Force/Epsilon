#include <iostream>
#include <vector>
#include <cctype>

using namespace std;

struct BoardState
{
	char Board[8][8];
	bool MoveTurn = true; // true for white, false for black
	bool Castling[4] = { true, true, true, true }; // 1,1,1,1 (left part for white, right for black, first for short, second for long)
	short epRow = -1, epCol = -1; // En passant target square cords (that just jumped over)
};

vector<int> generateMoves(BoardState& BoardStruct)
{
	int knightRow[8] = { -1,1,2,2,1,-1,-2,-2 };
	int knightCol[8] = { -2,-2,-1,1,2,2,1,-1 };

	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (BoardStruct.Board[i][j] == 'N')
			{
				for (int k = 0; k < 8; k++)
				{
					int newRow = (i + knightRow[k]);
					int newCol = (j + knightCol[k]);

					if ((newRow < 8 && newRow >= 0) && (newCol < 8 && newCol >= 0) && ((islower(BoardStruct.Board[newRow][newCol])) || (BoardStruct.Board[newRow][newCol] == '.')))
					{
						cout << newRow << ' ' << newCol << endl;
						vector<int> Move = { i, j, newRow, newCol };
					}
				}
				cout << endl;
			}
		}
	}
	return vector<int>(0,0);
}

void boardPrint(BoardState& BoardStruct)
{
	cout << "   ";

	for (int i = 0; i < 8; i++)
	{
		cout << i << ' ';
	}

	cout << endl << endl;

	for (int i = 0; i < 8; i++)
	{
		cout << i << "  ";
		for (int j = 0; j < 8; j++)
		{
			cout << BoardStruct.Board[i][j] << " ";
		}
		cout << endl;
	}
	cout << endl;
}

void setBoardStartPos(BoardState& BoardStruct)
{
	for (int i = 0; i < 8; i++) // Empty place setup
	{
		for (int j = 0; j < 8; j++)
		{
			BoardStruct.Board[i][j] = '.';
		}
	}

	for (int i = 0; i < 8; i++) // Pawns Setup
	{
		BoardStruct.Board[1][i] = 'p';
		BoardStruct.Board[6][i] = 'P';
	}

	for (int i = 0; i < 8; i++) // Pieces Setup
	{
		BoardStruct.Board[0][i] = "rnbqkbnr"[i];
		BoardStruct.Board[7][i] = "RNBQKBNR"[i];
	}
}

int main()
{
	BoardState BoardStruct;
	setBoardStartPos(BoardStruct);
	boardPrint(BoardStruct);
	generateMoves(BoardStruct);
}