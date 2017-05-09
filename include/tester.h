#ifndef TESTER_H
#define TESTER_H

#include <iostream>
#include <cmath>
#include <stdlib.h>     /* srand, rand */
#include <time.h> //time
#include <fstream> //write to file
#include <ctime>//for millis
#include <chrono>//for millis http://en.cppreference.com/w/cpp/chrono/duration
#include "codesys.h"

class Tester {

public:
	//Code parameters k, n, t, r
	const int m_k;//Size of information symbols
	const int m_n;//Length of codeword
	const int m_t;//Size of a request
	const int m_r;//The maximum number of symbols in codeword to be used for the recovery of one symbol in request.
	const int maxUsable;// Nr of times one symbol in codeword can be used. Default 1.

	Tester(int nk);
	Tester(int nk, int nn, int nt);
	Tester(int nk, int nn, int nt, int nr);
	Tester(int nk, int nn, int nt, int nr, int usable);
	~Tester() {		
		delete m_Y;
		delete m_givenData;
	}

	int **m_genMatrix;
	int *m_givenData;
	int *m_Y; //!< Generated Y array (from givendata and generator matrix)

	
	int** getGen();
	int* getY();
	
	void setGivenData(int(*given));
	void setGen(int** matrix);
	void displayGenMatrix();

	/**
	* Check if given answer is correct.
	* @param answer - given answer (from generator matrix and y array)
	* @param request - given request
	* @param rsize - request size
	* @return - true, if answer is correct. Otherwise false.
	*/
	bool autoCheck(int* answer, int* request, int rsize);


	/**
	* Runs all possible requests for current generator matrix using multicombinations.
	* @param method - chosen search method 0 - usual recursive, 1- recursive + presearch, 2 - recursive multicol
	* @return - true, if all tests pass - matrix is code. Oterwise false.
	*/
	bool runAllTests();


    /**
	* Runs all possible requests for current generator matrix with chosen fillmethod.
	* @param method - chosen search method 0 - usual recursive, 1- recursive + presearch, 2 - recursive multicol
	* @return - true, if all tests pass - matrix is code. Oterwise false.
	*/
	bool runAllTestsForPIRCode();

	/**
	* Copy the content of one array to another.
	*/
	void copyArray(int* fromA, int* toA, int len);

	/**
	* Runs the genMatrix and generateYArray.
	*/
	void init();

private:

	/**
	* Algorithm from http://www.martinbroadhurst.com/combinations.html , last check 19.03.2017
	* The algorithm has been modified.
	*/

	bool nextCombination(int many, int outOf, int* writeTo);


	/**
	* The original method was taken from: http://www.martinbroadhurst.com/multicombinations.html
	* It was modified to fit this model.
	* The function generates arrays of k elements out of total n possible elements by the laws of multicombinations. That means, combinations with repetition without permutations.
	* @param ar - array to operate on. The current state is increased.
	* @return - bool value, true, if a new combination was generated, false if the array was not changed.
	* prev k=b, n=a, because k and n are important class variables.
	*/
	bool next_multicombination(int* ar, int a, int b);

	/**
	* Grenerates the y array. Prequest - given data and generatormatrix are set.
	*/
	void generateYArray();
	/**
	* Generates a generator matrix (systematic)
	*/
	void genMatrix();

	int* copyCols(int* chosen);

};

#endif