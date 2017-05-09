#ifndef SEARCHDATA_H
#define SEARCHDATA_H

//#include <iostream>
//#include <vector>


class SearchData {

public:
	int *used;
	int *answer;
	bool filled;
	
	//std::vector<int, vector<int>> actionsTaken;//empty
	SearchData(int *pUsed, int *pAnsw) {
		filled = false;
		used = pUsed;
		answer = pAnsw;
	};

	~SearchData() {
		//delete answer;
		//delete used;
	}

	/**
	* Check if the request is filled
	* @return - true, if request is set to filled, false otherwise.
	*/
	bool isFilled() { return filled; };
	
	/**
	* Set the request filled
	*/
	void setFilled() { filled = true; };

	/**
	* Getter for answer
	* @return - pointer to array of answer.
	*/
	int* getAnswer() { return answer; };
	
};

#endif