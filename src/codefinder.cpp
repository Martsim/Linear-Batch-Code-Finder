#include "codefinder.h"

using namespace std;

CodeFinder::CodeFinder(int nk, int nn, int nt):
	k (nk), n(nn), t(nt), r(2), onlySystematic(false), maxUsable(1)
{	
	delete filename;
	filename = new string("logs\\" + to_string(k) + 'k' + to_string(n) + 'n' + to_string(t) + 't' + to_string(r) + "r_codes.txt");

	//Create initial arrays.
	listOfCodes = new MatrixContainer *[5];
	listOfNotCodes = new MatrixContainer *[5];
	lenOfLOC = 5;
	inLOC = 0;
	lenOfLONC = 5;
	inLONC = 0;

	generateAllPossibleColumns();

	testR = new Tester(k, n, t, r, maxUsable);
}

CodeFinder::CodeFinder(int nk, int nn, int nt, int nr) :
	k(nk), n(nn), t(nt), r(nr), onlySystematic(false), maxUsable(1)
{
	delete filename;
	filename = new string("logs\\" + to_string(k) + 'k' + to_string(n) + 'n' + to_string(t) + 't' + to_string(r) + "r_codes.txt");

	//Create initial arrays.
	listOfCodes = new MatrixContainer *[5];
	listOfNotCodes = new MatrixContainer *[5];
	lenOfLOC = 5;
	inLOC = 0;
	lenOfLONC = 5;
	inLONC = 0;

	generateAllPossibleColumns();

	testR = new Tester(k, n, t, r, maxUsable);
}

CodeFinder::CodeFinder(int nk, int nn, int nt, int nr, bool systematic) :
	k(nk), n(nn), t(nt), r(nr), onlySystematic(systematic), maxUsable(1)
{
	delete filename;
	filename = new string("logs\\" + to_string(k) + 'k' + to_string(n) + 'n' + to_string(t) + 't' + to_string(r) + "r_" + to_string(systematic) + "sys_codes.txt");

	//Create initial arrays.
	listOfCodes = new MatrixContainer *[5];
	listOfNotCodes = new MatrixContainer *[5];
	lenOfLOC = 5;
	inLOC = 0;
	lenOfLONC = 5;
	inLONC = 0;

	generateAllPossibleColumns();

	testR = new Tester(k, n, t, r, maxUsable);	
}


CodeFinder::CodeFinder(int nk, int nn, int nt, int nr, bool systematic, int usable) :
	k(nk), n(nn), t(nt), r(nr), onlySystematic(systematic), maxUsable(usable)
{
	delete filename;
	filename = new string("logs\\" + to_string(k) + 'k' + to_string(n) + 'n' + to_string(t) + 't' + to_string(r) + "r_" + to_string(systematic) + "sys_" + to_string(maxUsable) + "times_codes.txt");

	//Create initial arrays.
	listOfCodes = new MatrixContainer *[5];
	listOfNotCodes = new MatrixContainer *[5];
	lenOfLOC = 5;
	inLOC = 0;
	lenOfLONC = 5;
	inLONC = 0;
	
	generateAllPossibleColumns();

	testR = new Tester(k, n, t, r, maxUsable);
}


CodeFinder::~CodeFinder() {
	
		//Delete saved filename
		delete filename;

		//Delete saved list of codes
		if (inLOC == 0) {
			delete listOfCodes;
		}
		else {
			for (int i = 0; i < inLOC; i++) {
				listOfCodes[i]->deleteMatrix();
				delete listOfCodes[i];
			}
			delete listOfCodes;
		}

		//Delete saved list of non codes
		if (inLONC == 0) {
			delete listOfNotCodes;
		}
		else {
			for (int i = 0; i < inLONC; i++) {
				listOfNotCodes[i]->deleteMatrix();
				delete listOfNotCodes[i];
			}
			delete listOfNotCodes;
		}

		//Delete all columns generated
		for (int i = 0; i < nrOfAllCols; i++) {
			delete allCols[i];
			
		}
		delete allCols;

		delete testR;
}


void CodeFinder::setCodeType(int type) {
	findingCodeType = type;
}


void CodeFinder::testMatrix(int *chosenCols) {

	//Generate a real matrix
	int** matrix = new int *[k];//has k rows and n cols
	for (int i = 0; i < k; i++) {
		matrix[i] = new int[n];
	}

	for (int i = 0; i < n; i++) {  //Look through all chosen cols
		int col = chosenCols[i];
		for (int j = 0; j < k; j++) {
			(matrix)[j][i] = allCols[col][j]; //from col viewpoint to row first view
		}
	}

	testR->setGen(matrix);
	//auto startA = std::chrono::steady_clock::now();

	bool b = false;
	if (findingCodeType == 0) {
		b = testR->runAllTests();
	} else {
		b = testR->runAllTestsForPIRCode();
	}

	//auto endA = std::chrono::steady_clock::now();
	//auto elapsedA = std::chrono::duration_cast<std::chrono::milliseconds>(endA - startA);
	//std::cout << "Running all tests took " << elapsedA.count() << "seconds \nResults was " << b << "\n";
	if (b) {
		ofstream myfile;
		std::cout << "code" << "\n";
		myfile.open(filename->c_str(), std::ios_base::app);
		
		//myfile.open("k3n6t3_codes.txt", std::ios_base::app);
		if (findingCodeType == 1) {
			myfile << "\nPIR CODE\n";
		} else {
			myfile << "\nBatch CODE\n";
		}
		
		for (int row = 0; row < k; row++) {
			for (int c = 0; c < n; c++) {
				myfile << matrix[row][c] << " ";
			}
			myfile <<"\n";
		}
		myfile << "\n---------\n";
		myfile.close();

		//After writing to file, add matrix to list

		int* goodCols = copyCols(chosenCols);
		if (inLOC == lenOfLOC) {
			increaseArraySize();
		}
		listOfCodes[inLOC] = new MatrixContainer(goodCols);
		inLOC += 1;
	}
	else {
		int* badCols = copyCols(chosenCols);
		if (inLONC == lenOfLONC) {
			increaseArraySize();
		}
		listOfNotCodes[inLONC] = new MatrixContainer(badCols);
		inLONC += 1;
	}

	//delete test matrix
	testR->setGen(nullptr);
	for (int i = 0; i < k; i++) {
		delete matrix[i];
	}
	delete matrix;

}


bool CodeFinder::isPossibleCode(int *chosenCols) {
	//every row must contain at least t entrys to satisfy request of size t
	//first find rowweights	
	for (int row = 0; row < k; row++) {
		if (rowWeights[row] * maxUsable < t) {
			return false;
		}
	}
	return true;
}

bool CodeFinder::isPossibleCode2(int *chosenCols) {
	//every row must contain at least t entrys to satisfy request of size t
	//first find rowweights	
	for (int row = 0; row < k; row++) {
		int summer = 0;
		for (int col = 0; col < n; col++) {
			if (allCols[chosenCols[col]][row] == 1) {
				summer += 1;
			}
		}
		if (summer * maxUsable < t) {
			return false;
		}
	}
	return true;
}

void CodeFinder::increaseArraySize() {
	if (inLOC == lenOfLOC) {
		lenOfLOC += 5;
		MatrixContainer **listOfCodes2 = new MatrixContainer *[lenOfLOC];
		for (int i = 0; i < lenOfLOC - 5; i++ ) {
			listOfCodes2[i] = listOfCodes[i];
		}
		delete listOfCodes;
		listOfCodes = listOfCodes2;
	}
	if (inLONC == lenOfLONC) {
		lenOfLONC += 5;
		MatrixContainer **listOfNotCodes2 = new MatrixContainer *[lenOfLONC];
		for (int i = 0; i < lenOfLONC - 5; i++) {
			listOfNotCodes2[i] = listOfNotCodes[i];
		}
		delete listOfNotCodes;
		listOfNotCodes = listOfNotCodes2;
	}
}


bool CodeFinder::twoMatrixesAreOfSameClass(int* mat1cols, int* mat2cols) {
	int* usedCol = new int [n];//n cols in matrix
	for (int i = 0; i < n; i++) {
		usedCol[i] = 0;
	}
	//Check through the columns of one matrix
	for (int ccol = 0; ccol < n; ccol++) {
		int lookFor = mat1cols[ccol];
		bool found = false;
		//Find same column in other matrix
		for (int ccol2 = 0; ccol2 < n; ccol2++) {
			if (usedCol[ccol2] == 0 && mat2cols[ccol2] == lookFor) {
				usedCol[ccol2] = 1;
				found = true;
				break;
			}
		}
		if (!found) {
			delete usedCol;
			return false;
		}
	}
	
	delete usedCol;
	return true;
}


bool CodeFinder::noPrevious(int *chosenCols) {
	if (inLOC > 0) {
		//std::cout << "inLOC greater than 0. testing. size= " << inLOC << "\n";
		for (int i = 0; i < inLOC; i++) {
			if (twoMatrixesAreOfSameClass(chosenCols, listOfCodes[i]->getMatrixCols())) {
				//std::cout << "LOC contains similar\n";
				return false;
			}
		}
	}
	
	if (inLONC > 0) {
		//std::cout << "inLONC greater than 0. testing. size= " << inLONC << "\n";
		for (int i = 0; i < inLONC; i++) {
			if (twoMatrixesAreOfSameClass(chosenCols, listOfNotCodes[i]->getMatrixCols())) {
				//std::cout << "LONC contains similar\n";
				return false;
			}
		}
	}
	//std::cout << "No previous similar matrix found\n";
	return true;
}


void CodeFinder::generateAllPossibleColumns() {
	nrOfAllCols = pow(2, k) - 1;  //nr of Possible cols, 2^k - 1(the all zero col)
	
	allCols = new int *[nrOfAllCols];
	
	for (int i = 0; i < nrOfAllCols; i++) {
		allCols[i] = new int[k];  //new col
	}

	//The filling part
	int* prefill = new int[k];
	for (int i = 0; i < k; i++) {
		prefill[i] = 0;
	}
	colRecFill(0, prefill, 0, nrOfAllCols);

	delete prefill;  //After all columns generated

	//showing by 10
	cout << "Displaying all possible columns\nFirst 10\n";
	if (nrOfAllCols > 10) {
		int first = 0;
		int last = 9;
		int nextlast = 9;
		while (last < nrOfAllCols - 1) {
			last = nextlast;
			cout << "\nCols " << first << " until" << last << "\n";
			for (int roo = 0; roo < k; roo++) {
				for (int col = first; col <= last; col++) {
					cout << allCols[col][roo] << " ";
				}
				cout << "\n";
			}
			first = last + 1;
			if (last != nrOfAllCols - 1) {
				if (last + 10 < nrOfAllCols - 1) {
					nextlast += 10;
				}
				else {
					nextlast = nrOfAllCols - 1;
				}
			}
			cout << "\n";
		}
	}
	else {

		for (int roo = 0; roo < k; roo++) {
			for (int col = 0; col < nrOfAllCols; col++) {
				cout << allCols[col][roo] << " ";
			}
			cout << "\n";
		}
	}
	//cout << "\nThe end\n";


}


int CodeFinder::colRecFill(int row, int* preFill, int mTh, int maxM) {
	if (row == k) {
		//Copy columns
		for (int cc = 0; cc < k; cc++) {
			allCols[mTh][cc] = preFill[cc];
		}
		//Return index of next empty column
		return mTh + 1;

	} else {
		//if the filed contains more elements, this step could be done with for loop
		preFill[row] = 1;
		int nV = colRecFill(row + 1, preFill, mTh, maxM);  //nv, new value of next empty column
		if (nV == maxM) {
			return nV;
		}
		preFill[row] = 0;
		int nV2 = colRecFill(row + 1, preFill, nV, maxM);

		return nV2;
	}
}


void CodeFinder::findCodesSelect() {
	int* chosenColumns = new int[n]; //Will choose n columns

	for (int i = 0; i < n; i++) {
		chosenColumns[i] = 0;
	}
	
	rowWeights = new int[k];
	for (int j = 0; j < k; j++) {
		rowWeights[j] = 0;
	}

    //Ask user how many columns they want to enter
	int columnsin = 0;
	cout << "Enter the amount of columns You wish to choose: ";
	do {
		cin >> columnsin;
	} while (cin.fail());
	cout << "I shall now ask You to enter the indexes of " << columnsin << " columns, with first one being 0.\n";
	for (int asking = 0; asking < columnsin; asking++) {
		int colnr;
		do {
			cin >> colnr;
		} while (cin.fail());
		int thisin = colnr;
		chosenColumns[asking] = thisin;
		for (int rrow = 0; rrow < k; rrow++) {
			if (allCols[thisin][rrow] == 1) {
				rowWeights[rrow] += 1;
			}
		}
	}
	cout << "You have chosen the following columns: ";
	for (int asking = 0; asking < columnsin; asking++) {
		cout << chosenColumns[asking] << " ";
	}
	cout << "\n";


	auto start = std::chrono::steady_clock::now();

	start = std::chrono::steady_clock::now();
	try {
		genMatrix(chosenColumns, columnsin);
	}
	catch (int e) {
		cout << "Exception happened in genMatrix\nOkay?";
		char a;
		cin >> a;
	}
	

	auto end = std::chrono::steady_clock::now();
	auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(end - start);

	delete chosenColumns;
	delete rowWeights;

	std::cout << "The program has reached the end after " << elapsed.count() << "seconds \n";

	ofstream myfile;
	//std::cout << "Writing Code to file" << "\n";
	myfile.open(filename->c_str(), std::ios_base::app);
	myfile << "\nRunning all tests on this combination took " << elapsed.count() << "seconds \n";
	myfile << "Size of found codes is " << inLOC << ". Size of saved non code representatives " << inLONC << "\n";
	myfile.close();

}


void CodeFinder::genMatrix(int *chosenCols, int col) {

	if (col >= n) {
		if (isPossibleCode(chosenCols) && noPrevious(chosenCols)) {
			testMatrix(chosenCols);
		}

	}
	else {

		for (int ccol = 0; ccol < nrOfAllCols; ccol++) {
			chosenCols[col] = ccol;

			//Calc new rowweight
			for (int rrow = 0; rrow < k; rrow++) {
				if (allCols[ccol][rrow] == 1) {
					rowWeights[rrow] += 1;
				}
			}

			genMatrix(chosenCols, col + 1);

			//reverse the changes to rowweights
			for (int rrow = 0; rrow < k; rrow++) {
				if (allCols[ccol][rrow] == 1) {
					rowWeights[rrow] -= 1;
				}
			}

		} //Main col cycle
	}
}


void CodeFinder::findCodes() {
	int* chosenColumns = new int[n]; //Will be choosing n columns

	for (int i = 0; i < n; i++) {
		chosenColumns[i] = 0;
	}

	rowWeights = new int[k];
	for (int j = 0; j < k; j++) {
		rowWeights[j] = 0;
	}

	auto start = std::chrono::steady_clock::now();

	if (onlySystematic) {
		//systematic part of the matrix
		for (int i = 0; i < k; i++) {
			chosenColumns[i] = findW1AtRowx(i);
		}

		for (int i = 0; i < k; i++) {
			(rowWeights)[i] = 1;
		}

	}
	start = std::chrono::steady_clock::now();
	
	//generation begins
	do {
		
		if (isPossibleCode2(chosenColumns) && noPrevious(chosenColumns)) {
			//cout << "Found a possible code\n";
			testMatrix(chosenColumns);
		}

	} while (next_multicombination(chosenColumns, nrOfAllCols, n));

	//generation ends
	auto end = std::chrono::steady_clock::now();
	auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(end - start);

	delete chosenColumns;
	delete rowWeights;

	std::cout << "The program, that was finding " << (findingCodeType == 1 ? "PIR codes" : "Batch codes") <<  " has reached the end after " << elapsed.count() << "seconds \n";

	ofstream myfile;
	myfile.open(filename->c_str(), std::ios_base::app);
	myfile << "\nRunning all tests for " << (findingCodeType == 1 ? "PIR CODE" : "Batch CODE") << " on this combination took " << elapsed.count() << "seconds \n";
	myfile << "Size of found generator matrix representatives is " << inLOC << ". Size of saved non-code generator matrix representatives " << inLONC << "\n";
	myfile.close();

}

//n - size of set, b - choose b elements
bool CodeFinder::next_multicombination(int* ar, int a, int b)
{
	bool changed = false;
	int i;
	if (!onlySystematic) {
		for (i = b - 1; i >= 0 && !changed; i--) {
			if (ar[i] < a - 1) {
				/* Increment this element */
				ar[i]++;
				if (i < b - 1) {
					/* Make the elements after it the same */
					int j;
					for (j = i + 1; j < b; j++) {
						ar[j] = ar[j - 1];
					}
				}
				changed = true;
			}
		}
	}
	else {
		for (i = b - 1; i >= k && !changed; i--) {
			if (ar[i] < a - 1) {
				/* Increment this element */
				ar[i]++;
				if (i < b - 1) {
					/* Make the elements after it the same */
					int j;
					for (j = i + 1; j < b; j++) {
						ar[j] = ar[j - 1];
					}
				}
				changed = true;
			}
		}
	}
	if (!changed) {
		return false;//ends
	}
	return changed;
}


int* CodeFinder::copyCols(int* chosen) {
	int* generated = new int[n];

	for (int i = 0; i < n; i++) {
		generated[i] = chosen[i];
	}
	return generated;
}


int CodeFinder::findW1AtRowx(int i) {
	int xx = -1;

	for (int ccol = 0; ccol < nrOfAllCols; ccol++) {
		if (allCols[ccol][i] == 1) {
			int sum = 0;
			for (int rrow = 0; rrow < k; rrow++) {
				if (allCols[ccol][rrow] == 1) {
					sum += 1;
				}
			}
			if (sum == 1) {
				return ccol;
			}
		}
	}

	return xx;
}

