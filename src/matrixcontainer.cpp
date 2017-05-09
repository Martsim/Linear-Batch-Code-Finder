#include "matrixcontainer.h"

using namespace std;

MatrixContainer::MatrixContainer(int* matCols):
	matrixCols(matCols)
{
}


int* MatrixContainer::getMatrixCols() {
	return matrixCols;
}