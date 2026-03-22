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
		int rookRow[4] = { -1,0,1,0 };
		int rookCol[4] = { 0,-1,0,1 };
		int bishopRow[4] = { -1, 1, 1, -1 };
		int bishopCol[4] = { -1, -1, 1, 1 };
		int queenRow[8] = { -1, 0, 1, 1, 1, 0, -1, -1 };
		int queenCol[8] = { -1, -1, -1, 0, 1, 1, 1, 0 };

		vector<vector<int>> Moves;

		for (int i = 0; i < 8; i++)
		{
			for (int j = 0; j < 8; j++)
			{
				if (Board[i][j] == (MoveTurn ? 'N' : 'n')) // knight
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
				else if (Board[i][j] == (MoveTurn ? 'R' : 'r')) // THE ROOOOOOOKK!!!!!
				{
					for (int k = 0; k < 4; k++)
					{
						int newRow = (i + rookRow[k]);
						int newCol = (j + rookCol[k]);

						while ((newRow < 8 && newRow >= 0) && (newCol < 8 && newCol >= 0))
						{
							if (Board[newRow][newCol] == '.')
							{
								vector<int> Move = { i, j, newRow, newCol, Board[newRow][newCol] };
								Moves.push_back(Move);
								newRow += rookRow[k];
								newCol += rookCol[k];

							}
							else if (isAccessible(Board[newRow][newCol]))
							{
								vector<int> Move = { i, j, newRow, newCol, Board[newRow][newCol] };
								Moves.push_back(Move);
								break;
							}
							else
							{
								break;
							}
						}
					}
				}
				else if (Board[i][j] == (MoveTurn ? 'B' : 'b')) // bishop
				{
					for (int k = 0; k < 4; k++)
					{
						int newRow = (i + bishopRow[k]);
						int newCol = (j + bishopCol[k]);

						while ((newRow < 8 && newRow >= 0) && (newCol < 8 && newCol >= 0))
						{
							if (Board[newRow][newCol] == '.')
							{
								vector<int> Move = { i, j, newRow, newCol, Board[newRow][newCol] };
								Moves.push_back(Move);
								newRow += bishopRow[k];
								newCol += bishopCol[k];

							}
							else if (isAccessible(Board[newRow][newCol]))
							{
								vector<int> Move = { i, j, newRow, newCol, Board[newRow][newCol] };
								Moves.push_back(Move);
								break;
							}
							else
							{
								break;
							}
						}
					}
				}
				else if (Board[i][j] == (MoveTurn ? 'Q' : 'q')) // queen
				{
					for (int k = 0; k < 8; k++)
					{
						int newRow = (i + queenRow[k]);
						int newCol = (j + queenCol[k]);

						while ((newRow < 8 && newRow >= 0) && (newCol < 8 && newCol >= 0))
						{
							if (Board[newRow][newCol] == '.')
							{
								vector<int> Move = { i, j, newRow, newCol, Board[newRow][newCol] };
								Moves.push_back(Move);
								newRow += queenRow[k];
								newCol += queenCol[k];

							}
							else if (isAccessible(Board[newRow][newCol]))
							{
								vector<int> Move = { i, j, newRow, newCol, Board[newRow][newCol] };
								Moves.push_back(Move);
								break;
							}
							else
							{
								break;
							}
						}
					}
				}
				else if (Board[i][j] == (MoveTurn ? 'K' : 'k')) // king
				{
					for (int k = 0; k < 8; k++)
					{
						int newRow = (i + queenRow[k]);
						int newCol = (j + queenCol[k]);

						if ((newRow < 8 && newRow >= 0) && (newCol < 8 && newCol >= 0) && isAccessible(Board[newRow][newCol]))
						{
							vector<int> Move = { i, j, newRow, newCol, Board[newRow][newCol] };
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