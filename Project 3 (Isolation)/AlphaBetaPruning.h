#pragma once

#include <vector>
#include <chrono>
#include <string>

typedef std::chrono::high_resolution_clock Clock;
typedef unsigned long long u64;

using namespace std;

static char Endings[64][8] = { { -8, -7, 8, 72, 64, 7, -1, -9 },{ -7, -6, 8, 64, 65, 15, -1, -8 },{ -6, -5, 8, 56, 66, 23, -1, -7 },{ -5, -4, 8, 48, 67, 31, -1, -6 },{ -4, -3, 8, 40, 68, 39, -1, -5 },{ -3, -2, 8, 32, 69, 47, -1, -4 },{ -2, -1, 8, 24, 70, 55, -1, -3 },{ -1, 0, 8, 16, 71, 63, -1, -2 },{ -8, -6, 16, 71, 64, 15, 7, -1 },{ -7, -5, 16, 72, 65, 23, 7, -9 },{ -6, -4, 16, 64, 66, 31, 7, -8 },{ -5, -3, 16, 56, 67, 39, 7, -7 },{ -4, -2, 16, 48, 68, 47, 7, -6 },{ -3, -1, 16, 40, 69, 55, 7, -5 },{ -2, 0, 16, 32, 70, 63, 7, -4 },{ -1, 8, 16, 24, 71, 64, 7, -3 },{ -8, -5, 24, 70, 64, 23, 15, 7 },{ -7, -4, 24, 71, 65, 31, 15, -1 },{ -6, -3, 24, 72, 66, 39, 15, -9 },{ -5, -2, 24, 64, 67, 47, 15, -8 },{ -4, -1, 24, 56, 68, 55, 15, -7 },{ -3, 0, 24, 48, 69, 63, 15, -6 },{ -2, 8, 24, 40, 70, 64, 15, -5 },{ -1, 16, 24, 32, 71, 65, 15, -4 },{ -8, -4, 32, 69, 64, 31, 23, 15 },{ -7, -3, 32, 70, 65, 39, 23, 7 },{ -6, -2, 32, 71, 66, 47, 23, -1 },{ -5, -1, 32, 72, 67, 55, 23, -9 },{ -4, 0, 32, 64, 68, 63, 23, -8 },{ -3, 8, 32, 56, 69, 64, 23, -7 },{ -2, 16, 32, 48, 70, 65, 23, -6 },{ -1, 24, 32, 40, 71, 66, 23, -5 },{ -8, -3, 40, 68, 64, 39, 31, 23 },{ -7, -2, 40, 69, 65, 47, 31, 15 },{ -6, -1, 40, 70, 66, 55, 31, 7 },{ -5, 0, 40, 71, 67, 63, 31, -1 },{ -4, 8, 40, 72, 68, 64, 31, -9 },{ -3, 16, 40, 64, 69, 65, 31, -8 },{ -2, 24, 40, 56, 70, 66, 31, -7 },{ -1, 32, 40, 48, 71, 67, 31, -6 },{ -8, -2, 48, 67, 64, 47, 39, 31 },{ -7, -1, 48, 68, 65, 55, 39, 23 },{ -6, 0, 48, 69, 66, 63, 39, 15 },{ -5, 8, 48, 70, 67, 64, 39, 7 },{ -4, 16, 48, 71, 68, 65, 39, -1 },{ -3, 24, 48, 72, 69, 66, 39, -9 },{ -2, 32, 48, 64, 70, 67, 39, -8 },{ -1, 40, 48, 56, 71, 68, 39, -7 },{ -8, -1, 56, 66, 64, 55, 47, 39 },{ -7, 0, 56, 67, 65, 63, 47, 31 },{ -6, 8, 56, 68, 66, 64, 47, 23 },{ -5, 16, 56, 69, 67, 65, 47, 15 },{ -4, 24, 56, 70, 68, 66, 47, 7 },{ -3, 32, 56, 71, 69, 67, 47, -1 },{ -2, 40, 56, 72, 70, 68, 47, -9 },{ -1, 48, 56, 64, 71, 69, 47, -8 },{ -8, 0, 64, 65, 64, 63, 55, 47 },{ -7, 8, 64, 66, 65, 64, 55, 39 },{ -6, 16, 64, 67, 66, 65, 55, 31 },{ -5, 24, 64, 68, 67, 66, 55, 23 },{ -4, 32, 64, 69, 68, 67, 55, 15 },{ -3, 40, 64, 70, 69, 68, 55, 7 },{ -2, 48, 64, 71, 70, 69, 55, -1 },{ -1, 56, 64, 72, 71, 70, 55, -9 } };
static char Directions[] = { -8, -7, 1, 9, 8, 7, -1, -9 };
static char BestMove1 = 54;
static char BestMoves2[64] = { -1, 9, 18, 27, 36, 45, 27, 31, 9, 18, -1, -1, -1, -1, -1, -1, 18, -1, 27, -1, -1, -1, -1, -1, 27, -1, -1, 36, -1, -1, -1, -1, 36, -1, -1, -1, 45, -1, -1, -1, 45, -1, -1, -1, -1, 62, -1, -1, 27, -1, -1, -1, -1, -1, 61, -1, 59, -1, -1, -1, -1, -1, -1, -1 };
static char BestMoves3[64] = { -1, -1, -1, -1, -1, -1, -1, 45, -1, -1, -1, -1, -1, -1, -1, 22, -1, -1, -1, -1, -1, -1, -1, 22, -1, -1, -1, -1, -1, -1, -1, 30, -1, -1, -1, -1, -1, -1, -1, 38, -1, -1, -1, -1, -1, -1, -1, 45, -1, -1, -1, -1, -1, -1, -1, 45, 45, 50, 50, 51, 52, 45, 45, -1 };

vector<u64> GetMoveMask(int type);
vector<u64> SetMask();

class AlphaBetaPruning
{
	int depth;
	u64 board;
	char myPos;
	char theirPos;

	char MoveList[60][32];

	vector<tuple<int, char>> ImmediateMoves;

	vector<char> MoveHistory;

	bool isFirst;
	u64 timeLimit;
	Clock::time_point startTime;

	u64 numNodes;
	bool hasLost = false;

	vector<u64> HORIZONTAL_MASKS = GetMoveMask(0);
	vector<u64> VERTICAL_MASKS = GetMoveMask(2);
	vector<u64> DIAGONAL_MASKS = GetMoveMask(4);
	vector<u64> ANTI_DIAGONAL_MASKS = GetMoveMask(6);
	vector<u64> Adjacent1 = SetMask();

	int tempValue;
public:
	AlphaBetaPruning(bool isMyTurn, u64 timeLimit);

	void StartSearch(int maxDepth);

	int ABPruningMe(int alpha, int beta);
	int ABPruningThem(int alpha, int beta);

	int IterativeDeepening(int startingDepth);

	int GetMyMoves();
	int GetTheirMoves();

	u64 GetLineMoves(char pos);
	u64 GetDiagonalMoves(char pos, u64 mask);

	int GetMoveList(u64 moveBits, char Frontier[]);

	void NextState(char pos);

	int EvaluateMe();
	int EvaluateThem();

	u64 GetMoveBits(char pos);

	void PrintBoard();

	char MakeMove();
	void MakeMove(char nextPos);
	bool OpponentMove(char nextPos);
	void OpponentMove();

	void SortMyMoves(int numMoves);
	void SortTheirMoves(int numMoves);

	void PrintMoveHistory();

	static void PlayGame();
	static string GetPosString(char pos);
};

