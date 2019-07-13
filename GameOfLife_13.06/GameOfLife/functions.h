#pragma once
#include"TextField.h"
#include"Cell.h"
using namespace std;
using namespace sf;


void printBoard(vector<vector<bool>>& vectorToDisp, CircleShape shapeAlive, CircleShape shapeDead, int sizFunX, int sizFunY, RenderWindow &winFun, int cntBtn);

void solver(shared_ptr<Cell> board, Cell leftTop[], Cell rightTop[], Cell leftDown[], Cell rightDown[], unique_ptr<unique_ptr<Cell[]>[]>& topRow, unique_ptr<unique_ptr<Cell[]>[]>& downRow,
	unique_ptr<unique_ptr<Cell[]>[]>& leftColumn, unique_ptr<unique_ptr<Cell[]>[]>& rightColumn, unique_ptr<unique_ptr<Cell[]>[]>& middle, unique_ptr<int[]>& withSum, unique_ptr<int[]>& tempArray,
	int sX, int sY, vector<bool>& mem1D, vector<vector<bool>>& mem2D, bool isMEM, vector<bool>& noChMem1D, vector<vector<bool>>& noCHMem2D,int& cntOfPlayed,bool isNCh);

