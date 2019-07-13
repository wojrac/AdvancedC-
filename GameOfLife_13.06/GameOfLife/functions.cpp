#include"functions.h"
#include<omp.h>
void printBoard(vector<vector<bool>>& vectorToDisp, CircleShape shapeAlive, CircleShape shapeDead, int sizFunX, int sizFunY, RenderWindow &winFun, int cntBtn)
{
	for (int i = sizFunY - 1; i >= 0; i--)
	{
		for (int j = 0; j < sizFunX; j++)
		{
			if (vectorToDisp[cntBtn - 1][j + sizFunX*i] == true)
			{
				winFun.draw(shapeAlive);
				shapeAlive.setPosition(20 * j, 20 * i);
			}
			else
			{
				winFun.draw(shapeDead);
				shapeDead.setPosition(20 * j, 20 * i);
			}
		}
	}
}
void solver(shared_ptr<Cell> board, Cell leftTop[], Cell rightTop[], Cell leftDown[], Cell rightDown[], unique_ptr<unique_ptr<Cell[]>[]>& topRow, unique_ptr<unique_ptr<Cell[]>[]>& downRow,
	unique_ptr<unique_ptr<Cell[]>[]>& leftColumn, unique_ptr<unique_ptr<Cell[]>[]>& rightColumn, unique_ptr<unique_ptr<Cell[]>[]>& middle, unique_ptr<int[]>& withSum, unique_ptr<int[]>& tempArray,
	int sX, int sY, vector<bool>& mem1D, vector<vector<bool>>& mem2D, bool isMEM, vector<bool>& noCHMem1D, vector<vector<bool>>& noCHMem2D, int& cntOfPlayed,bool isNCH)
{
	int NumberOfCellInRow = sX - 2;
	int NumberOfCellInColumn = sY - 2;
	unique_ptr<Cell[]>                   smartPtr1D;
	unique_ptr<Cell[]>                   DownRow1D;
	unique_ptr<Cell[]>					 Left1D;
	unique_ptr<Cell[]>                   Right1D;

	topRow = make_unique< unique_ptr<Cell[]>[] >(NumberOfCellInRow);
	downRow = make_unique< unique_ptr<Cell[]>[] >(NumberOfCellInRow);
	leftColumn = make_unique< unique_ptr<Cell[]>[] >(NumberOfCellInColumn);
	rightColumn = make_unique< unique_ptr<Cell[]>[] >(NumberOfCellInColumn);
	for (int i = 0; i < NumberOfCellInRow; i++)
	{
		smartPtr1D = make_unique<Cell[]>(5);
		DownRow1D = make_unique<Cell[]>(5);
		for (int j = 0; j < 5; j++)
		{
			//smartPtr1D[j] = myBoard.get()[j];
			//dataCounter++;
		}
		topRow[i] = move(smartPtr1D);
		downRow[i] = move(DownRow1D);
	}
	for (int i = 0; i < NumberOfCellInColumn; i++)
	{
		Left1D = make_unique<Cell[]>(8);
		Right1D = make_unique<Cell[]>(8);
		leftColumn[i] = move(Left1D);
		rightColumn[i] = move(Right1D);
	}
	int NumberOfCellInTheMiddle = (sX - 2)*(sY - 2);

	unique_ptr<Cell[]> middle1D;
	middle = make_unique< unique_ptr<Cell[]>[] >(NumberOfCellInTheMiddle);
	for (int i = 0; i < NumberOfCellInTheMiddle; i++)
	{
		middle1D = make_unique<Cell[]>(8);

		middle[i] = move(middle1D);
	}
	tempArray = make_unique<int[]>(NumberOfCellInTheMiddle);
	withSum = make_unique<int[]>(sX*sY);
	int rightDownId = sX - 1;
	int rightTopId = sX*sY - 1;
	int leftTopId = rightTopId - sX + 1;

		
		leftDown[0] = board.get()[1];
		leftDown[1] = board.get()[sX];
		leftDown[2] = board.get()[sX + 1];
		leftDown[3] = board.get()[rightDownId];
		leftDown[4] = board.get()[rightDownId + sX];

		rightDown[0] = board.get()[rightDownId - 1];
		rightDown[1] = board.get()[rightDownId + sX];
		rightDown[2] = board.get()[rightDownId + sX - 1];
		rightDown[3] = board.get()[0];
		rightDown[4] = board.get()[sX];

		rightTop[0] = board.get()[rightTopId - 1];
		rightTop[1] = board.get()[rightTopId - sX];
		rightTop[2] = board.get()[rightTopId - 1 - sX];
		rightTop[3] = board.get()[leftTopId];
		rightTop[4] = board.get()[leftTopId - sX];

		leftTop[0] = board.get()[leftTopId + 1];
		leftTop[1] = board.get()[leftTopId - sX];
		leftTop[2] = board.get()[leftTopId + 1 - sX];
		leftTop[3] = board.get()[rightTopId];
		leftTop[4] = board.get()[rightTopId - sX];
	
	//CZAS NA PASY
	//najpierw musze wyznaczyc ilosc komorek w kolumnie
	for (int i = 0; i < NumberOfCellInRow; i++)
	{
		topRow[i][0] = board.get()[(leftTopId + i + 1) - 1];
		topRow[i][1] = board.get()[(leftTopId + i + 1) + 1];
		topRow[i][2] = board.get()[(leftTopId + i + 1) - sX - 1];
		topRow[i][3] = board.get()[(leftTopId + i + 1) - sX];
		topRow[i][4] = board.get()[(leftTopId + i + 1) - sX + 1];

		downRow[i][0] = board.get()[(i + 1) - 1];
		downRow[i][1] = board.get()[(i + 1) + 1];
		downRow[i][2] = board.get()[(i + 1) + sX + 1];
		downRow[i][3] = board.get()[(i + 1) + sX];
		downRow[i][4] = board.get()[(i + 1) + sX - 1];
	}  //chyba dziala :)

	for (int i = 0; i < NumberOfCellInColumn; i++)
	{
		leftColumn[i][0] = board.get()[(sX*i + sX) + sX];
		leftColumn[i][1] = board.get()[(sX*i + sX) + sX + 1];
		leftColumn[i][2] = board.get()[(sX*i + sX) + 1];
		leftColumn[i][3] = board.get()[(sX*i + sX) - sX + 1];
		leftColumn[i][4] = board.get()[(sX*i + sX) - sX];
		//nowe
		leftColumn[i][5] = board.get()[rightDownId + sX*i + sX];
		leftColumn[i][6] = board.get()[(rightDownId + sX*(i + 1)) + sX];
		leftColumn[i][7] = board.get()[(rightDownId + sX*(i + 1)) - sX];

		rightColumn[i][0] = board.get()[(rightDownId + sX*(i + 1)) + sX];
		rightColumn[i][1] = board.get()[(rightDownId + sX*(i + 1)) + sX - 1];
		rightColumn[i][2] = board.get()[(rightDownId + sX*(i + 1)) - 1];
		rightColumn[i][3] = board.get()[(rightDownId + sX*(i + 1)) - sX - 1];
		rightColumn[i][4] = board.get()[(rightDownId + sX*(i + 1)) - sX];
		//nowe
		rightColumn[i][5] = board.get()[i*sX + sX];
		rightColumn[i][6] = board.get()[(sX*i + sX) + sX];
		rightColumn[i][7] = board.get()[(sX*i + sX) - sX];
	}//wyglada ze dziala
	for (int i = 0, l = 0; l < sY - 2; i += sX, l++)
	{
		for (int j = sX + 1 + i, k = 0; k < sX - 2; j++, k++)
		{
			tempArray[k + l*(sX - 2)] = j;
		}
	}
	for (int i = 0; i < NumberOfCellInTheMiddle; i++)
	{

		middle[i][0] = board.get()[tempArray[i] - sX - 1];
		middle[i][1] = board.get()[tempArray[i] - sX];
		middle[i][2] = board.get()[tempArray[i] - sX + 1];
		middle[i][3] = board.get()[tempArray[i] + 1];
		middle[i][4] = board.get()[tempArray[i] + sX + 1];
		middle[i][5] = board.get()[tempArray[i] + sX];
		middle[i][6] = board.get()[tempArray[i] + sX - 1];
		middle[i][7] = board.get()[tempArray[i] - 1];
	}
	withSum[0] = leftDown[0].IsAlive() + leftDown[1].IsAlive() + leftDown[2].IsAlive() + leftDown[3].IsAlive() + leftDown[4].IsAlive();
	withSum[rightDownId] = rightDown[0].IsAlive() + rightDown[1].IsAlive() + rightDown[2].IsAlive() + rightDown[3].IsAlive() + rightDown[4].IsAlive();
	withSum[rightTopId] = rightTop[0].IsAlive() + rightTop[1].IsAlive() + rightTop[2].IsAlive() + rightTop[3].IsAlive() + rightTop[4].IsAlive();
	withSum[leftTopId] = leftTop[0].IsAlive() + leftTop[1].IsAlive() + leftTop[2].IsAlive() + leftTop[3].IsAlive() + leftTop[4].IsAlive();  //oki to chyba dziala
	for (int i = 1, j = 0; j < NumberOfCellInRow; i++, j++)
	{
		withSum[i] = downRow[j][0].IsAlive() + downRow[j][1].IsAlive() + downRow[j][2].IsAlive() + downRow[j][3].IsAlive() + downRow[j][4].IsAlive();
	}
	for (int i = leftTopId + 1, j = 0; j < NumberOfCellInRow; i++, j++)
	{
		withSum[i] = topRow[j][0].IsAlive() + topRow[j][1].IsAlive() + topRow[j][2].IsAlive() + topRow[j][3].IsAlive() + topRow[j][4].IsAlive();
	}
	for (int i = sX, j = 0; j < NumberOfCellInColumn; i += sX, j++)
	{
		withSum[i] = leftColumn[j][0].IsAlive() + leftColumn[j][1].IsAlive() + leftColumn[j][2].IsAlive() + leftColumn[j][3].IsAlive() + leftColumn[j][4].IsAlive() + leftColumn[j][5].IsAlive() + leftColumn[j][6].IsAlive() + leftColumn[j][7].IsAlive();
	}
	for (int i = rightDownId + sX, j = 0; j < NumberOfCellInColumn; i += sX, j++)
	{
		withSum[i] = rightColumn[j][0].IsAlive() + rightColumn[j][1].IsAlive() + rightColumn[j][2].IsAlive() + rightColumn[j][3].IsAlive() + rightColumn[j][4].IsAlive() + rightColumn[j][5].IsAlive() + rightColumn[j][6].IsAlive() + rightColumn[j][7].IsAlive();
	}
	for (int j = 0; j < (sX - 2)*(sY - 2); j++)
	{
		withSum[tempArray[j]] = middle[j][0].IsAlive() + middle[j][1].IsAlive() + middle[j][2].IsAlive() + middle[j][3].IsAlive() + middle[j][4].IsAlive() + middle[j][5].IsAlive() + middle[j][6].IsAlive() + middle[j][7].IsAlive();
	}
	if (isMEM)
	{
		for (int j = 0; j < sX*sY; j++)
		{
			mem1D.push_back(board.get()[j].IsAlive());
		}
		mem2D.push_back(mem1D);
		mem1D.clear();
	}
	if (isNCH)
	{
		for (int i = 0; i < sX*sY; i++)
		{
			noCHMem1D.push_back(board.get()[i].IsAlive());
		}
		noCHMem2D.push_back(noCHMem1D);
		noCHMem1D.clear();
	}
	//TERAZ WARUNKI
	for (int i = 0; i < sX*sY; i++)
	{
		if (!(board.get()[i].IsAlive()))
		{
			if (withSum[i] == 3)
			{
				board.get()[i].setCellAlive();
			}
		}
		else
		{
			if (withSum[i] < 2 || withSum[i] > 3)
			{
				board.get()[i].setCellDead();
			}
		}
	}
}