#include "tester.h"

using namespace std;


Tester::Tester(int nk):
	m_k (nk), m_n(pow(2, nk) - 1), m_t(nk), m_r(2), maxUsable(1)
{
	int* gD = new int[nk];
	for (int i = 0; i < m_k; i++) {
		gD[i] = i % 2;
	}
	setGivenData(gD);
}


Tester::Tester(int nk, int nn, int nt):
	m_k (nk), m_n(nn), m_t(nt), m_r(2), maxUsable(1)
{
	int* gD = new int[nk];
	for (int i = 0; i < m_k; i++) {
		gD[i] = i % 2;
	}
	setGivenData(gD);
}


Tester::Tester(int nk, int nn, int nt, int nr) :
	m_k(nk), m_n(nn), m_t(nt), m_r(nr), maxUsable(1)
{
	int* gD = new int[nk];
	for (int i = 0; i < m_k; i++) {
		gD[i] = i % 2;
	}
	setGivenData(gD);
}


Tester::Tester(int nk, int nn, int nt, int nr, int usable) :
	m_k(nk), m_n(nn), m_t(nt), m_r(nr), maxUsable(usable)
{
	int* gD = new int[nk];
	for (int i = 0; i < m_k; i++) {
		gD[i] = i % 2;
	}
	setGivenData(gD);
}


void Tester::init() {
	genMatrix();
	generateYArray();
}


void Tester::generateYArray( ) {
	m_Y = new int[m_n];
	for (int c = 0; c < m_n; c++) {
		int a = 0;
		for (int r = 0; r < m_k; r++) {
			a += (m_genMatrix)[r][c] * (m_givenData)[r];
		}
		(m_Y)[c] = a % 2;
	}
}


void Tester::genMatrix() {
	//int **array;
	m_genMatrix = new int *[m_k];
	for (int i = 0; i <m_k; i++)
		m_genMatrix[i] = new int[m_n];
	
	
	for (int i = 0; i < m_k; i++) {
		for (int j = 0; j < m_n; j++) {
			(m_genMatrix)[i][j] = 0;
		}
	}

	//Fills in the first columns
	for (int j = 0; j < m_k; j++) {
		(m_genMatrix)[j][j] = 1;
	}

	int lookat = 0;
	int lookati = 0;

	for (int j = m_k; j < m_n; j++) {
		while (lookat < m_n) {
			//If we have exceeded the list, move on.
			if (lookati >= m_k) {
				lookati = 0;
				lookat++;
			}

			//Seek last known 1.
			int last1 = -1;
			int x = m_k - 1;
			while (x >= 0) {
				if ((m_genMatrix)[x][lookat] == 1) {
					last1 = x;
					if (lookati <= x) {
						lookati = x + 1;
					}

					break;
				}
				else {
					x--;
				}
			}

			//if 1 not found (adding firsts)
			if (last1 < 0) {
				//add 1 to cuurent lookat index, increase the index, move on				
				(m_genMatrix)[lookati][j] = 1;
				lookati++;
				break;
			}
			else if (last1 == m_k - 1) {
				//if last found 1 was in the last row, nothing to do, move on.
				lookat++;
				lookati = 0;
				continue;
			}
			else {
				// add 1
				for (int k = 0; k < lookati; k++) {
					//Copy the previous table
					(m_genMatrix)[k][j] = (m_genMatrix)[k][lookat];
				}
				(m_genMatrix)[lookati][j] = 1;
				lookati += 1;
				break;
			}
		}

	}

	//Have added all possibilities
}


void Tester::setGivenData(int(*given)) {
	m_givenData = given;
}


void Tester::displayGenMatrix() {
	std::cout << "\nGENERATOR MATRIX" << "\n";
	for (int i = 0; i < m_k; i++) {
		for (int j = 0; j < m_n; j++) {
			std::cout << " " << (m_genMatrix)[i][j] << " ";
		}
		std::cout << "\n";
	}
}


int** Tester::getGen() {
	return m_genMatrix;
}


void Tester::setGen(int** matrix) {
	m_genMatrix = matrix;
}


int* Tester::getY() {
	return m_Y;
}


bool Tester::autoCheck(int* answer, int* request, int rsize) {
	int i = 0;
	while (i < rsize) {
		if (answer[i] != m_givenData[request[i] - 1]) return false;
		i++;
	}
	return true;
}


void Tester::copyArray(int* fromA, int* toA, int len) {
	for (int i = 0; i < len; i++) {
		toA[i] = fromA[i];
	}
}


// Algorithm from http://www.martinbroadhurst.com/combinations.html , last check 19.03.2017
bool Tester::nextCombination(int many, int outOf, int* writeTo) {
	if (writeTo[0] == writeTo[1]) {
		for (int i = 0; i < many; i++) {
			writeTo[i] = i;
		}
		return true;
	}
	
	bool finished = false;
	bool changed = false;
	int i;

	if (many == outOf) {
		for (int i = 0; i < many; i++) {
			writeTo[i] = i;
		}
		return false;
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


bool Tester::runAllTestsForPIRCode() {
	generateYArray();
	
	for (int xk = 0; xk < m_k; xk++) {
		int* request = new int[m_t];
		for (int ii = 0; ii < m_t; ii++) {
			request[ii] = xk + 1;
		}

		Codesys cS(m_k, m_n, m_t, m_r, maxUsable);
		cS.setGenerator(this->getGen());
		cS.setRequest(request);

		cS.setY(this->getY());
		int* answer = cS.fillRequestWithSearchRandC();
		

		bool b = autoCheck(answer, request, m_t);
		
		delete answer;
		delete request;

		if (!b) {
			return false;
		}

	}
	return true;
}


bool Tester::runAllTests() {
	generateYArray();

	int* request = new int[m_t];
	for (int ii = 0; ii < m_t; ii++) {
		request[ii] = 1;
	}

	do {
		Codesys cS(m_k, m_n, m_t, m_r, maxUsable);
		cS.setGenerator(this->getGen());
		cS.setRequest(request);
		cS.setY(this->getY());

		int* answer = cS.fillRequestWithSearchRandC();

		if (!autoCheck(answer, request, m_t)) {
			delete answer;
			delete request;
			return false;
		}
		else {
			delete answer;
		}
		cS.setRequest(nullptr);

	} while (next_multicombination(request, m_k + 1, m_t));

	delete request;
	return true;
}

//n - size of set, b - choose b elements
bool Tester::next_multicombination(int* ar, int a, int b)
{
	bool changed = false;
	int i;

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

	return changed;
}