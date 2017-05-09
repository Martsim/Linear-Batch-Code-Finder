#ifndef CODEFINDER_H
#define CODEFINDER_H

#include "codesys.h"
#include "tester.h"
#include "matrixcontainer.h"

#include <iostream>
#include <cmath>
#include <stdlib.h>     /* srand, rand */
#include <time.h> //time
#include <fstream> //write to file
#include <ctime>//for millis
#include <chrono>//for millis http://en.cppreference.com/w/cpp/chrono/duration
#include <string>
#include <vector>

using namespace std;

/* - Find codes with given parameters
*/
class CodeFinder {

public:
	//const int size_x;
	//const int size_y;
	
	int k;
	int n;
	int t;
	int r;//size_t as in nr of cols per x
	Tester* testR;
	bool onlySystematic;
	int maxUsable;
    int findingCodeType = 0; //Default for Batch code.1 = PIR

	/**Filename for log*/
	std::string* filename = new std::string("empty.txt");

	/**
	* Constructor for codefinder
	* @param nk - parameter k
	* @param nt - parameter n
	* @param nn - parameter t
	*/
	CodeFinder(int nk, int nn, int nt);

	/**
	* Constructor for codefinder
	* @param nk - parameter k
	* @param nt - parameter n
	* @param nn - parameter t
	* @param nr - parameter r
	*/
	CodeFinder(int nk, int nn, int nt, int nr);

	/**
	* Constructor for codefinder
	* @param nk - parameter k
	* @param nt - parameter n
	* @param nn - parameter t
	* @param nr - parameter r
	* @param systematic - find only systematic codes
	*/
	CodeFinder(int nk, int nn, int nt, int nr, bool systematic);

	/**
	* Constructor for codefinder
	* @param nk - parameter k
	* @param nt - parameter n
	* @param nn - parameter t
	* @param nr - parameter r
	* @param systematic - find only systematic codes
	* @param usable - nr of times one column can be accessed
	*/
	CodeFinder(int nk, int nn, int nt, int nr, bool systematic, int usable);


	~CodeFinder();


	void setCodeType(int type);

	/**
	* Find all possible codes for set parameters with the user selecting the first columns.
	*/
	void findCodesSelect();

	/**
	* Find all possible codes for set parameters.
	*/
	void findCodes();

	//Functions for generating all possible matrices.
	void genMatrix(int *chosenCols, int col);

	
	
protected:
	//list of codes, list of not codes
	
	/**
	* The original method was taken from: http://www.martinbroadhurst.com/multicombinations.html
	* It was modified to fit this model.
	* The function generates arrays of k elements out of total n possible elements by the laws of multicombinations. That means, combinations with repetition without permutations.
	* @param ar - array to operate on. The current state is increased.
	* @return - bool value, true, if a new combination was generated, false if the array was not changed.
	* prev k=b, n=a, because k and n are important class variables.
	*/
	bool next_multicombination(int* ar, int a, int b);

	int lenOfLOC;
	int inLOC;
	int lenOfLONC;
	int inLONC;
	MatrixContainer **listOfCodes;// = new MatrixContainer[5];//Check if constructs the vector
	MatrixContainer **listOfNotCodes;// = new MatrixContainer[5];
	
	int** allCols;//By columns
	int nrOfAllCols;

private:

	/**
	* Increas the arraysize of container arrays.
	*/
	void increaseArraySize();

	/**
	* Checks the list of saved matrixes and compares the given matrix to all of the previous ones.
	* @param matrix - matrix to be tested against.
	*/
	bool noPrevious(int *chosenCols);

	/**
	* Checks if one matrix could be a permutation of anothers rows.
	*/
	bool twoMatrixesAreOfSameClass(int* mat1cols, int* mat2cols);

	/**
	* Checks the matrix for Hamilton weight. Uses previously calculated values.
	*/
	bool isPossibleCode(int *chosenCols);


	/**
	* Checks the matrix for Hamilton weight. Calculates the value.
	*/
	bool isPossibleCode2(int *chosenCols);

	/**
	* Test if the matrix is a code.
	*/
	void testMatrix(int *chosenCols);


	int* rowWeights;//current rowweights. Updated every step easier checking.


	/** Generate all possible columns - saved to class variable
	*/
	void generateAllPossibleColumns();
	
	
	/** Recursively generate all possible columns
	* Helper for generateAllPossibleColumns()
	*/
	int colRecFill(int row, int* preFill, int mTh, int maxM);


	/** Copy the columns into a new array for storage.
	*/
	int* copyCols(int* chosen);


	/**
	* @param i - the index of row, at wihich the 1 should be.
	* @return The index of a column with weight 1, and 1 at given row
	*/
	int findW1AtRowx(int i);
};

#endif