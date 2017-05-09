#include "codesys.h"

using namespace std;
Codesys::Codesys(int nx, int ny):
	m_k(nx), m_n(ny), m_T(nx), m_r(2), maxUsable(1)
{
}
Codesys::Codesys(int nx, int ny, int nt) :
	m_k(nx), m_n(ny), m_T(nt), m_r(2), maxUsable(1)
{
}


Codesys::Codesys(int nx, int ny, int nt, int nr) :
	m_k(nx), m_n(ny), m_T(nt), m_r(nr), maxUsable(1)
{
}


Codesys::Codesys(int nx, int ny, int nt, int nr, int nUsable) :
	m_k(nx), m_n(ny), m_T(nt), m_r(nr), maxUsable(nUsable)
{
}

void Codesys::setGenerator(int (**gen)) {
	m_genMatrix = gen;
}
void Codesys::setRequest(int(*request)) {
	m_request = request;
}
void Codesys::setY(int(*aY)) {
	m_Y = aY;
}
void Codesys::showY() {
	std::cout << "Y ARRAY\n";
	for (int i = 0; i < m_n; i++) {
		std::cout << " " << m_Y[i] << " ";
	}
	std::cout << "\n";
}

//List of colun nr-s
bool Codesys::listOfColsEqualOut(int* columns, int ignore, int nrOfCols) {
	
	for (int row = 0; row < m_k; row++) {
		
		int sumInRow = 0;
		for (int col = 0; col < nrOfCols; col++) {
			sumInRow += (m_genMatrix)[row][columns[col]];
		}
		if (row != ignore && sumInRow % 2 != 0) {
			return false;
		}
		if (row == ignore && sumInRow % 2 != 1) {
			return false;
		}
	}
	
	return true;
}

/** Recursively with combinations
*/
int* Codesys::fillRequestWithSearchRandC() {
	int *answer = new int[m_T];
	int *used = new int[m_n];
	for (int i = 0; i < m_n; i++) {
		(used)[i] = 0;
	}
	for (int i = 0; i < m_T; i++) {
		(answer)[i] = 2;
	}
	SearchData* initial = new SearchData(used, answer);
	initial = recursFillWithRandC(1, initial);
	
	answer = initial->getAnswer();
	delete used;
	delete initial;

	return answer;

}


SearchData* Codesys::recursFillWithRandC(int step, SearchData* sD) {

	int rd = ((m_request)[step - 1]) - 1;//The number requested
	//cout << "step " << step << " seeking " << rd + 1 << ".\ncombsize1 ";

	//combination of size 1
	for (int ii = 0; ii < m_n; ii++) {
		if (sD->used[ii] < maxUsable && (m_genMatrix)[rd][ii] == 1) {
			int weight = 0;
			for (int rrow = 0; rrow < m_k; rrow++) {
				if ((m_genMatrix)[rrow][ii] == 1) {
					weight += 1;
				}
			}
			if (weight == 1) {

				sD->answer[step - 1] = (m_Y)[ii];
				sD->used[ii] += 1;
				//cout << "used" << ii <<" .\n";
				if (step == m_T) {
					//cout << "Request filled\n";
					sD->setFilled();
					return sD;
				}
				else {

					sD = recursFillWithRandC(step + 1, sD);

					if (sD->isFilled()) {
						return sD;
					}
					sD->answer[step - 1] = 2;
					sD->used[ii] -= 1;
				}

				break;//only need to test one of similar columns
			}
		}
	}

	if (m_r == 1) {
		return sD;
	}

	//Next we take combinations up to m_r out of n
	for (int combSize = 2; combSize <= m_r; combSize++) {
		
		int* combMaker = new int[combSize];//indexes of m_r columns. First of all combSize
		for (int ii = 0; ii < combSize; ii++) {
			combMaker[ii] = ii;
		}

        //For every combination of selected columns: checks if they are usable and if they can answer the sought bit.
		do { 
			bool fail = false;
			bool hasNecessary = false;
			for (int ii = 0; ii < combSize; ii++) {

				if (sD->used[combMaker[ii]] >= maxUsable) {
					fail = true;
					
					break;
				}
				if ((m_genMatrix)[rd][combMaker[ii]] == 1) {
					hasNecessary = true;
				}

			}
			if (fail || !hasNecessary) {
				continue;
			}
			else {
				
				if (!listOfColsEqualOut(combMaker, rd, combSize)) {
					continue;
				}
				
				int summer = 0;
				for (int ii = 0; ii < combSize; ii++) {
					summer += (m_Y)[combMaker[ii]];
					sD->used[combMaker[ii]] += 1;
				}

				sD->answer[step - 1] = summer % 2;				

				//Was the request filled?
				if (step == m_T) {
					sD->setFilled();
					return sD;
				}
				else {

					sD = recursFillWithRandC(step + 1, sD);

					if (sD->isFilled()) {						
						return sD;

					}
					// Recreate the state that existed before picking columns at this step.
					sD->answer[step - 1] = 2;
					for (int ii = 0; ii < combSize; ii++) {						
						sD->used[combMaker[ii]] -= 1;
					}

				}

			}
		} while (nextCombination(combSize, m_n, combMaker)); //As long as there is a next combination

		delete combMaker;
	}
	return sD;

}

// Algorithm from http://www.martinbroadhurst.com/combinations.html , last check 19.03.2017
bool Codesys::nextCombination(int many, int outOf, int* writeTo) {

	bool finished = false;
	bool changed = false;
	int i;

	if (many == outOf) {
		bool prevContained = false;
		for (int i = 0; i < many; i++) {
			if (writeTo[i] == i) {
				prevContained = true;
			}
			else {
				prevContained = false;
			}
			writeTo[i] = i;
		}
		if (prevContained) {
			return false;
		}
		else {
			return false;
		}
	}

	if (many > 0) {
		for (i = many - 1; !finished && !changed; i--) {
			if (writeTo[i] < (outOf - 1) - (many - 1) + i) {

				writeTo[i]++;
				if (i < many - 1) {
					int j;
					for (j = i + 1; j < many; j++) {
						writeTo[j] = writeTo[j - 1] + 1;
					}
				}
				changed = true;
			}
			finished = i == 0;
		}

	}
	return changed; //returns if there was a new combination
}
