#ifndef CODESYS_H
#define CODESYS_H

#include <iostream>
#include "searchdata.h"
#include <cmath>
//#include <vector>

class Codesys {

public:
	const int m_k;//rows
	const int m_n; //columns
	const int m_T; //Size of request
	const int m_r;//cols per x in request
	const int maxUsable; // Nr of times one column can be used. Default 1.

	int **m_genMatrix; //Generator matrix
	int *m_Y;// Codeword calculated from given symbols and generator matrix.
	int *m_request;// A request.
	

	//Constructors
	Codesys(int nx, int ny);
	Codesys(int nx, int ny, int nt);
	Codesys(int nx, int ny, int nt, int nr);
	Codesys(int nx, int ny, int nt, int nr, int nUsable);

	
	//Callable functions for filling requests	
	int* fillRequestWithSearchRandC();  // Creates combinations of columns to fill the search
	
	//Class initialization functions
	void setGenerator(int(**gen));
	void setRequest(int(*request));
	void setY(int(*ay));
	void showY();

	
private:
	
	/**
	* Helper function. Decides if n columns equal out if one row is ignored.
	* @param columns - list of indexes of columns.
	* @param ignore - index of ignored row.
	* @param nrOfColumns - nr of columns to be compared.
	* @return - True, if they equal out. Otherwise, false.
	*/
	bool listOfColsEqualOut(int* columns, int ignore, int nrOfCols);

	
	//Helper functions for callable fill functions

	/**
	* Fill the search (recursive)(generating combinations)
	* @param step - step of recursion/depth(pos at request)
	* @param sD - given SearchData object. Contains answer and indexes of used columns
	* @return SearchData object, containing answer and indexes of used columns
	*/
	SearchData* recursFillWithRandC(int step, SearchData* sD);

	// Algorithm from http://www.martinbroadhurst.com/combinations.html , last check 19.03.2017
	bool nextCombination(int many, int outOf, int* writeTo);
};

#endif