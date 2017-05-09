#ifndef MATRIXCONTAINER_H
#define MATRIXCONTAINER_H


class MatrixContainer {

public:
	MatrixContainer(int* matCols);
	//MatrixContainer(const MatrixContainer &obj);

	int* getMatrixCols();
	
	//Must be called when deleting the final vector containing the codes.
	void deleteMatrix() {
		delete matrixCols;
	}
private:
	int *matrixCols;
};

#endif