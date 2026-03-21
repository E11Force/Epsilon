#include <iostream>
#include <vector>
#include <cctype>

using namespace std;

class ChessBoard
{
private:
	char Board[8][8];
	bool MoveTurn = true; // true for white, false for black
	bool Castling[4] = { true, true, true, true }; // 1,1,1,1 (left part for white, right for black, first for short, second for long)
	short epRow = -1, epCol = -1; // En passant target square cords (that just jumped over)

public:
	void returnMove(vector<int> Move)
	{
		Board[Move[0]][Move[1]] = Board[Move[2]][Move[3]];
		Board[Move[2]][Move[3]] = Move[4];
		MoveTurn = !MoveTurn;
	}

	void makeMove(vector<int> Move)
	{
		Board[Move[2]][Move[3]] = Board[Move[0]][Move[1]];
		Board[Move[0]][Move[1]] = '.';
		MoveTurn = !MoveTurn;
	}

	bool isAccessible(char targetPiece)
	{
		switch (MoveTurn)
		{
		case true:
			return (islower(targetPiece) || targetPiece == '.');
		case false:
			return (isupper(targetPiece) || targetPiece == '.');
		}
	}

	void boardPrint()
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
				cout << Board[i][j] << " ";
			}
			cout << endl;
		}
		cout << endl;
	}

	void setBoardStartPos()
	{
		for (int i = 0; i < 8; i++) // Empty place setup
		{
			for (int j = 0; j < 8; j++)
			{
				Board[i][j] = '.';
			}
		}

		for (int i = 0; i < 8; i++) // Pawns Setup
		{
			Board[1][i] = 'p';
			Board[6][i] = 'P';
		}

		for (int i = 0; i < 8; i++) // Pieces Setup
		{
			Board[0][i] = "rnbqkbnr"[i];
			Board[7][i] = "RNBQKBNR"[i];
		}
	}

	vector<vector<int>> generateMoves()
	{
		int knightRow[8] = { -1,1,2,2,1,-1,-2,-2 };
		int knightCol[8] = { -2,-2,-1,1,2,2,1,-1 };

		vector<vector<int>> Moves;

		for (int i = 0; i < 8; i++)
		{
			for (int j = 0; j < 8; j++)
			{
				if (Board[i][j] == (MoveTurn ? 'N' : 'n'))
				{
					for (int k = 0; k < 8; k++)
					{
						int newRow = (i + knightRow[k]);
						int newCol = (j + knightCol[k]);

						if ((newRow < 8 && newRow >= 0) && (newCol < 8 && newCol >= 0) && isAccessible(Board[newRow][newCol]))
						{
							vector<int> Move = { i, j, newRow, newCol, Board[newRow][newCol]};
							Moves.push_back(Move);
						}
					}
				}
			}
		}
		return Moves;
	}
};


int main()
{
	ChessBoard BoardStruct;
	BoardStruct.setBoardStartPos();
	BoardStruct.boardPrint();
	vector<vector<int>> Moves = BoardStruct.generateMoves();

	for (int i = 0; i < Moves.size(); i++)
	{
		BoardStruct.makeMove(Moves[i]);
		BoardStruct.boardPrint();
		BoardStruct.returnMove(Moves[i]);
	}
}