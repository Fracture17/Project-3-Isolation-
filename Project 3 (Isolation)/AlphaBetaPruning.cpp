#include "stdafx.h"
#include "AlphaBetaPruning.h"
#include <immintrin.h>
#include <algorithm>
#include <iostream>
#include <numeric>
#include <random>
#include <tuple>

string AlphaBetaPruning::GetPosString(char pos) {
	string x = { (char) pos / 8 + 'A', (char) pos % 8 + '1' };
	return x;
}

void AlphaBetaPruning::PlayGame() {
	u64 timeLimit;
	cout << "Time Limit: ";
	cin >> timeLimit;
	cout << "Computer First?(T/F) ";
	string in;
	cin >> in;
	bool isMyTurn = (in[0] | 0x20) == 't';

	AlphaBetaPruning A(isMyTurn, timeLimit);
	if(isMyTurn) {
		A.MakeMove(BestMove1);
		A.OpponentMove();
		A.MakeMove(BestMoves3[A.theirPos]);
	}
	else {
		A.OpponentMove();
		A.MakeMove(BestMoves2[A.theirPos]);
	}
	while(!A.hasLost) {
		A.OpponentMove();
		A.IterativeDeepening(10);
	}
}

void CheckDirection(char start, char direction, char end, unsigned long long &moves, int times) {
	while ((start += direction) != end && times--) {
		if (direction < 0) {
			moves |= moves >> -direction;
		}
		else {
			moves |= moves << direction;
		}
	}
}

u64 GetAdjacent(int times, char p) {
	u64 moves = 1ull << p;
	CheckDirection(p, -8, Endings[p][0], moves, times);
	CheckDirection(p, 1, Endings[p][2], moves, times);
	CheckDirection(p, 8, Endings[p][4], moves, times);
	CheckDirection(p, -1, Endings[p][6], moves, times);
	return moves ^ (1ull << p);
}

vector<u64> SetMask() {
	vector<u64> result;
	for (int i = 0; i < 64; i++) {
		result.push_back(GetAdjacent(1, i));
	}
	return result;
}

int AlphaBetaPruning::IterativeDeepening(int startingDepth) {
	startTime = Clock::now();
	int d = startingDepth;
	do {
		StartSearch(d);
		d++;
	} while ((Clock::now() - startTime).count() < timeLimit * 0.8 && d < 50);
	MakeMove();
	cout << "Total Time: " << (Clock::now() - startTime).count() / 100000000 << endl;
	cout << "Depth: " << d - 1 << endl;
	cout << "Nodes: " << numNodes << endl;
	cout << "Score: " << get<0>(ImmediateMoves.back()) << endl;
	ImmediateMoves.clear();
	return d;
}

AlphaBetaPruning::AlphaBetaPruning(bool isMyTurn, u64 timeLimit)
{
	this->timeLimit = timeLimit * 900000000;
	isFirst = isMyTurn;
	if(isMyTurn) {
		myPos = 0;
		theirPos = 63;
	}
	else {
		myPos = 63;
		theirPos = 0;
	}
	board = 0x8000000000000001;
}

int AlphaBetaPruning::EvaluateMe() {
	int myMoves = _mm_popcnt_u64(GetMoveBits(myPos));
	int theirMoves = _mm_popcnt_u64(GetMoveBits(theirPos));

	if (myMoves == 0) {
		myMoves = -1000000;
	}
	if (theirMoves == 0) {
		theirMoves = -200000;
	}
	
	return myMoves - (theirMoves << 1);
}

int AlphaBetaPruning::EvaluateThem() {
	int myMoves = _mm_popcnt_u64(GetMoveBits(myPos));
	int theirMoves = _mm_popcnt_u64(GetMoveBits(theirPos));

	if (myMoves == 0) {
		myMoves = -1000000;
	}
	if (theirMoves == 0) {
		theirMoves = -1000000;
	}
	
	return myMoves - (theirMoves << 1);
}

void AlphaBetaPruning::SortMyMoves(int numMoves) {
	if (depth > 4) {
		sort(MoveList[depth], MoveList[depth] + numMoves, [&](char p1, char p2) {
			NextState(p1);
			int myMoves = _mm_popcnt_u64(GetMoveBits(p1));
			int theirMoves = _mm_popcnt_u64(GetMoveBits(theirPos));

			if (myMoves == 0) {
				myMoves = -1000000;
			}
			if (theirMoves == 0) {
				theirMoves = -200000;
			}
			NextState(p1);

			NextState(p2);
			int myMoves2 = _mm_popcnt_u64(GetMoveBits(p2));
			int theirMoves2 = _mm_popcnt_u64(GetMoveBits(theirPos));

			if (myMoves2 == 0) {
				myMoves2 = -1000000;
			}
			if (theirMoves2 == 0) {
				theirMoves2 = -200000;
			}
			NextState(p2);

			return (myMoves - theirMoves * 3) < (myMoves2 - theirMoves2 * 3);
		});
	}
}

void AlphaBetaPruning::SortTheirMoves(int numMoves) {
	if (depth > 4) {
		sort(MoveList[depth], MoveList[depth] + numMoves, [&](char p1, char p2) {
			NextState(p1);
			int myMoves = _mm_popcnt_u64(GetMoveBits(p1));
			int theirMoves = _mm_popcnt_u64(GetMoveBits(theirPos));

			if (myMoves == 0) {
				myMoves = -1000000;
			}
			if (theirMoves == 0) {
				theirMoves = -1000000;
			}
			NextState(p1);

			NextState(p2);
			int myMoves2 = _mm_popcnt_u64(GetMoveBits(p2));
			int theirMoves2 = _mm_popcnt_u64(GetMoveBits(theirPos));

			if (myMoves2 == 0) {
				myMoves2 = -1000000;
			}
			if (theirMoves2 == 0) {
				theirMoves2 = -1000000;
			}
			NextState(p2);

			return (myMoves - theirMoves * 3) > (myMoves2 - theirMoves2 * 3);
		});
	}
}

void AlphaBetaPruning::StartSearch(int maxDepth) {
	depth = maxDepth;
	numNodes = 1;

	int numMoves;
	if(ImmediateMoves.size() == 0) {
		numMoves = GetMyMoves();
	}
	else {
		numMoves = ImmediateMoves.size();
		for(int i = 0; i < ImmediateMoves.size(); i++) {
			MoveList[depth][i] = get<1>(ImmediateMoves[i]);
		}
	}
	ImmediateMoves.clear();

	if(numMoves == 0) {
		hasLost = true;
		return;
	}

	int alpha = INT32_MIN;
	char tempPos = myPos;
	while(numMoves--) {
		myPos = MoveList[depth][numMoves];
		NextState(myPos);
		tempValue = ABPruningThem(alpha, INT32_MAX);
		NextState(myPos);
		ImmediateMoves.emplace_back(make_pair(tempValue, myPos));
		if (tempValue > alpha) {
			alpha = tempValue;
		}
		if((Clock::now() - startTime).count() > timeLimit) {
			break;
		}
	}
	myPos = tempPos;

	sort(ImmediateMoves.begin(), ImmediateMoves.end());
}

int AlphaBetaPruning::ABPruningMe(int alpha, int beta) {
	numNodes++;
	if (depth > 1) {
		depth--;
		int numMoves = GetMyMoves();
		if(numMoves == 0) {
			depth++;
			return -1000000;
		}
		char const tempPos = myPos;
		while (numMoves--) {
			myPos = MoveList[depth][numMoves];
			NextState(myPos);
			tempValue = ABPruningThem(alpha, beta);
			NextState(myPos);
			if (tempValue > alpha) {
				alpha = tempValue;
				if (beta <= tempValue) {
					depth++;
					myPos = tempPos;
					return INT32_MAX;
				}
			}
		}
		depth++;
		myPos = tempPos;
		return alpha;
	}
	return EvaluateMe();
}

int AlphaBetaPruning::ABPruningThem(int alpha, int beta) {
	numNodes++;
	if (depth > 1) {
		depth--;
		int numMoves = GetTheirMoves();
		if (numMoves == 0) {
			depth++;
			return 1000000;
		}
		char const tempPos = theirPos;
		while (numMoves--) {
			theirPos = MoveList[depth][numMoves];
			NextState(theirPos);
			tempValue = ABPruningMe(alpha, beta);
			NextState(theirPos);
			if (tempValue < beta) {
				beta = tempValue;
				if (tempValue <= alpha) {
					depth++;
					theirPos = tempPos;
					return INT32_MIN;
				}
			}
		}
		depth++;
		theirPos = tempPos;
		return beta;
	}
	return EvaluateThem();
}

int AlphaBetaPruning::GetMyMoves() {
	u64 moveBits = GetMoveBits(myPos);
	int numMoves = GetMoveList(moveBits, MoveList[depth]);
	SortMyMoves(numMoves);
	return numMoves;
}

int AlphaBetaPruning::GetTheirMoves() {
	u64 moveBits = GetMoveBits(theirPos);
	int numMoves = GetMoveList(moveBits, MoveList[depth]);
	SortTheirMoves(numMoves);
	return numMoves;
}

int AlphaBetaPruning::GetMoveList(u64 moveBits, char Frontier[]) {
	u64 b = 1;
	int cnt = 0;
	for (int i = 0; i < 64; i++, b <<= 1) {
		if (moveBits & b) {
			Frontier[cnt] = i;
			cnt++;
		}
	}

	return cnt;
}

u64 AlphaBetaPruning::GetMoveBits(char pos) {
	u64 moveMask = GetLineMoves(pos);
	moveMask |= GetDiagonalMoves(pos, DIAGONAL_MASKS[pos]);
	moveMask |= GetDiagonalMoves(pos, ANTI_DIAGONAL_MASKS[pos]);
	return moveMask;
}

u64 AlphaBetaPruning::GetLineMoves(char pos) {
	u64 Hmask = HORIZONTAL_MASKS[pos];
	u64 p = 1ull << pos; //get player bit
	u64 b = board & Hmask; //clear bits that can't be reached

	u64 h = p - 1; //get mask of bits less than player
	h &= b;
	h |= h >> 1;
	h |= h >> 2;
	h |= h >> 4;
	h++;
	h = b - h;
	h ^= b;
	h &= Hmask;
	h &= ~board;


	u64 Vmask = VERTICAL_MASKS[pos];
	b = board & Vmask;
	u64 v = p - 1;
	v &= b;
	v |= v >> 1;
	v |= v >> 2;
	v |= v >> 4;
	v |= v >> 8;
	v |= v >> 16;
	v |= v >> 32;
	v++;
	v = b - v;
	v &= Vmask;
	v &= ~board;
	
	return v | h;
}

u64 AlphaBetaPruning::GetDiagonalMoves(char pos, u64 mask) {
	u64 p = 1ull << pos;
	u64 f = (board & mask);
	u64 r = _byteswap_uint64(f);
	f -= p;
	r -= _byteswap_uint64(p);
	f ^= _byteswap_uint64(r);
	return (f & mask) & ~board;
}

void AlphaBetaPruning::NextState(char pos) {
	board ^= 1ull << pos;
}

u64 GetDirectionMask(char r, char c, char rd, char cd) {
	u64 mask = 1ull << (r * 8 + c);
	while (r >= 0 && c >= 0 && r < 8 && c < 8) {
		mask ^= 1ull << (r * 8 + c);
		r += rd;
		c += cd;
	}
	return mask;
}

vector<u64> GetMoveMask(int type) {
	static vector<char> xDirs = { -1, 1, 0, 0, -1, 1, -1, 1 };
	static vector<char> yDirs = { 0, 0, 1, -1, -1, 1, 1, -1 };
	vector<u64> result;
	for (int r = 0; r < 8; r++) {
		for (int c = 0; c < 8; c++) {
			result.push_back(GetDirectionMask(r, c, yDirs[type], xDirs[type]) | GetDirectionMask(r, c, yDirs[type + 1], xDirs[type + 1]));
		}
	}
	return result;
}

void AlphaBetaPruning::PrintBoard() {
	cout << "  1 2 3 4 5 6 7 8" << endl;
	for (int i = 0, b = 0; i < 8; i++) {
		cout << (char) ('A' + i) << " ";
		for (int j = 0; j < 8; j++, b++) {
			if (b == myPos) {
				cout << "X ";
			}
			else if (b == theirPos) {
				cout << "O ";
			}
			else {
				cout << (board & (1ull << b) ? "# " : "- ");
			}
		}
		cout << endl;
	}
	cout << endl;
}

void AlphaBetaPruning::MakeMove(char nextPos) {
	myPos = nextPos;
	NextState(myPos);
	MoveHistory.push_back(myPos);
	PrintMoveHistory();
	PrintBoard();
	cout << "Computer's move is: " << GetPosString(myPos) << endl << endl;
}

char AlphaBetaPruning::MakeMove() {
	static random_device rd;
	static mt19937 rng(rd());
	static uniform_real<> perecnt(0, 1);
	char nextPos = get<1>(ImmediateMoves.back());
	if(perecnt(rng) < 0.01) {
		double x = get<0>(ImmediateMoves.back()) - get<0>(ImmediateMoves[ImmediateMoves.size() - 2]);
		x = 1 / x;
		if(perecnt(rng) < x) {
			nextPos = get<0>(ImmediateMoves[ImmediateMoves.size() - 2]);
		}
	}

	myPos = nextPos;
	NextState(myPos);
	MoveHistory.push_back(myPos);
	PrintMoveHistory();
	PrintBoard();
	cout << "Computer's move is: " << GetPosString(myPos) << endl << endl;
	return myPos;
}

bool AlphaBetaPruning::OpponentMove(char nextPos) {
	if((1ull << nextPos) & GetMoveBits(theirPos)) {
		theirPos = nextPos;
		NextState(nextPos);
		MoveHistory.push_back(theirPos);
		return true;
	}
	cout << "Invalid move" << endl;
	return false;
}

void AlphaBetaPruning::OpponentMove() {
	bool good;
	do {
		cout << "Enter opponent's move: ";
		string move;
		cin >> move;
		good = OpponentMove(((move[0] | 0x20) - 'a') * 8 + move[1] - '1');
	} while (!good);
	PrintBoard();
}

void AlphaBetaPruning::PrintMoveHistory() {
	if(isFirst) {
		cout << "Computer vs. Opponent" << endl;
	}
	else {
		cout << "Opponent vs. Computer" << endl;
	}
	for(int i = 0; i < MoveHistory.size(); i++) {
		if(i & 1) {
			cout << "   " << GetPosString(MoveHistory[i]) << endl;
		}
		else {
			cout << (char) (i / 2 + '1') << ". " << GetPosString(MoveHistory[i]);
		}
	}
	cout << endl;
}