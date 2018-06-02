#include <iostream>
#include <fstream>
using namespace std;

class ThinningSkeleton
{
public:
	int numRows;
	int numCols;
	int minVal;
	int maxVal;
	bool changeFlag;
	int cycleCount;
	int* neighborAry;
	int** firstAry;
	int** secondAry;

	ThinningSkeleton(int row, int col, int min, int max)
	{
		numRows = row;
		numCols = col;
		minVal = min;
		maxVal = max;
		changeFlag = true;
		cycleCount = 0;

		neighborAry = new int[9];
		firstAry = new int*[numRows + 2];
		secondAry = new int*[numRows + 2];
		for(int i = 0; i < numRows + 2; i++)
		{
			firstAry[i] = new int[numCols + 2];
			secondAry[i] = new int[numCols + 2];
		}

		for(int i = 0; i < 9; i++)
		{
			neighborAry[i] = 0;
		}

		for(int i = 0; i < numRows + 2; i++)
		{
			for(int j = 0; j < numCols + 2; j++)
			{
				firstAry[i][j] = 0;
				secondAry[i][j] = 0;
			}
		}
	}

	~ThinningSkeleton()
	{
		for(int i = 0; i < numRows+2; ++i)
		{
			delete[] firstAry[i];
			delete[] secondAry[i];
		}
		delete[] firstAry;
		delete[] secondAry;
		delete[] neighborAry;
	}


	void zeroFramed()
	{
		for(int col = 0; col < numCols + 2; col++)
		{
			firstAry[0][col] = 0;
			firstAry[numRows + 1][col] = 0;
			secondAry[0][col] = 0;
			secondAry[numRows + 1][col] = 0;
		}

		for(int row = 0; row < numRows + 2; row++)
		{
			firstAry[row][0] = 0;
			firstAry[row][numCols + 1] = 0;
			secondAry[row][0] = 0;
			secondAry[row][numCols + 1] = 0;
		}

	}

	void copyAry()
	{
		for(int i = 0; i <= numRows + 1; i++)
		{
			for(int j = 0; j <= numCols + 1; j++)
			{
				firstAry[i][j] = secondAry[i][j];
			}
		}
	}

	void loadNeighbor(int i, int j)
	{
		int counter = 0;
		for(int z = i - 1; z <= i + 1; z++)
		{
			for(int k = j - 1; k <= j + 1; k++)
			{
				neighborAry[counter] = firstAry[z][k];
				counter++;
			}
		}
	}


	bool checkCC(int i, int j)
	{
		if(firstAry[i - 1][j] == 0 && firstAry[i][j - 1] == 0 && firstAry[i][j + 1] == 0 && firstAry[i + 1][j] == 0)
		{
			return false;
		}
		else if((firstAry[i - 1][j] == 0 && firstAry[i + 1][j] == 0 ) || (firstAry[i][j - 1] == 0 && firstAry[i][j + 1] == 0))
		{
			return false;
		}
		else
		{
			int countZero = 0;

			if(firstAry[i - 1][j] == 0)
			{
				countZero++;
			}

			if(firstAry[i][j - 1] == 0)
			{
				countZero++;
			}

			if(firstAry[i + 1][j] == 0)
			{
				countZero++;
			}

			if(firstAry[i][j + 1] == 0)
			{
				countZero++;
			}

			if(countZero == 3)
			{
				return false;
			}

			if(countZero == 2)
			{
				if(firstAry[i - 1][j] == 0 && firstAry[i][j + 1] == 0)
				{
					if(firstAry[i - 1][j + 1] != 0)
					{
						return false;
					}
				}

				if(firstAry[i][j + 1] == 0 && firstAry[i + 1][j] == 0)
				{
					if(firstAry[i + 1][j + 1] != 0)
					{
						return false;
					}
				}

				if(firstAry[i + 1][j] == 0 && firstAry[i][j - 1] == 0)
				{
					if(firstAry[i + 1][j - 1] != 0)
					{
						return false;
					}
				}

				if(firstAry[i][j - 1] == 0 && firstAry[i - 1][j] == 0)
				{
					if(firstAry[i - 1][j - 1] != 0)
					{
						return false;
					}
				}
			}
		}
		return true;
	}

	void doThinning(int i, int j)
	{
		loadNeighbor(i, j);

		int countNeighbor = 0;
		for(int i = 0; i < 9; i++)
		{
			if(i != 4)
			{
				if(neighborAry[i] > 0)
				{
					countNeighbor++;
				}
			} 

		}

		if(countNeighbor >= 4 && checkCC(i, j) == true)
		{
				secondAry[i][j] = 0;
				changeFlag = true;
		}
		else
		{
			secondAry[i][j] = 1;
		}
	}

	void NorthThinning(ofstream &outFile)
	{
		outFile << "North thinning" << endl;
		for(int i = 1; i < numRows + 1; i++)
		{
			for(int j = 1; j < numCols + 1; j++)
			{
				if(firstAry[i][j] > 0 && firstAry[i - 1][j] == 0)
				{
					doThinning(i, j);
				}
				else
				{
					secondAry[i][j] = firstAry[i][j];
				}
			}
		}
	}

	void SouthThinning(ofstream &outFile)
	{
		outFile << "South thinning" << endl;
		for(int i = 1; i < numRows + 1; i++)
		{
			for(int j = 1; j < numCols + 1; j++)
			{
				if(firstAry[i][j] > 0 && firstAry[i + 1][j] == 0)
				{
					doThinning(i, j);
				}
				else
				{
					secondAry[i][j] = firstAry[i][j];
				}
			}
		}
	}

	void EastThinning(ofstream &outFile)
	{
		outFile << "East thinning" << endl;
		for(int i = 1; i < numRows + 1; i++)
		{
			for(int j = 1; j < numCols + 1; j++)
			{
				if(firstAry[i][j] > 0 && firstAry[i][j + 1] == 0)
				{
					doThinning(i, j);
				}
				else
				{
					secondAry[i][j] = firstAry[i][j];
				}
			}
		}
	}

	void WestThinning(ofstream &outFile)
	{
		outFile << "West thinning" << endl;
		for(int i = 1; i < numRows + 1; i++)
		{
			for(int j = 1; j < numCols + 1; j++)
			{
				if(firstAry[i][j] > 0 && firstAry[i][j - 1] == 0)
				{
					doThinning(i, j);
				}
				else
				{
					secondAry[i][j] = firstAry[i][j];
				}
			}
		}
//		outFile << endl;
	}

	void prettyPrint(ofstream &outFile)
	{
		for(int i = 1; i < numRows + 1; i++)
		{
			for(int j = 1; j < numCols + 1; j++)
			{
				if(firstAry[i][j] == 0)
				{
					outFile <<  "  ";
				}
				else
				{
					outFile << firstAry[i][j] << " ";
				}
			}
			outFile << endl;
		}
		outFile << endl;
	}
};

int main(int argc, char **argv)
{
	ifstream inFile;
	ofstream outFile1;
	ofstream outFile2;

	if (!inFile)
	{
		cout << "ERROR to open file";
		exit(1);
	}

	inFile.open(argv[1]);
	outFile1.open(argv[2]);
	outFile2.open(argv[3]);

	int row, col, min, max;

	inFile >> row >> col >> min >> max;
	ThinningSkeleton skeleton(row, col, min, max);

	skeleton.zeroFramed();
	int value;
	while(!inFile.eof())
	{
		for(int i = 1; i < skeleton.numRows + 1; i++)
		{
			for(int j = 1; j < skeleton.numCols + 1; j++)
			{
				inFile >> value;
				skeleton.firstAry[i][j] = value;
			}
		}
	}

	while(skeleton.changeFlag == true)
	{
		if(skeleton.cycleCount == 0 || skeleton.cycleCount == 2 || skeleton.cycleCount == 4)
		{
			outFile2 << "cycleCount = " << skeleton.cycleCount << endl;
			skeleton.prettyPrint(outFile2);
		}

		skeleton.changeFlag = false;
		skeleton.cycleCount++;

		skeleton.NorthThinning(outFile2);
		skeleton.copyAry();

		skeleton.SouthThinning(outFile2);
		skeleton.copyAry();

		skeleton.WestThinning(outFile2);
		skeleton.copyAry();

		skeleton.EastThinning(outFile2);
		skeleton.copyAry();

	}
	outFile2 << "cycleCount = " << skeleton.cycleCount << endl;
	skeleton.prettyPrint(outFile2);
	outFile1 << skeleton.numRows << " " << skeleton.numCols << " " << skeleton.minVal << " " << skeleton.maxVal << endl;
	skeleton.prettyPrint(outFile1);

	inFile.close();
	outFile1.close();
	outFile2.close();

	return 0;
}
