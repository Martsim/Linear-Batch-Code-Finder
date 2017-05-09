#include <iostream>
//#include <fstream>
#include "tester.h"
#include "codesys.h"
#include "codefinder.h"
#include "matrixcontainer.h"

using namespace std;

int main (int argc, char* argv[]) {
	
	
	int progType = 0; //0  search all Batch codes; 1 Choose cols and search all batch codes; 2 Search  all PIR codes


	int k = 3;
	int n = 6;
	int t = 3;
	int r = 2;
	bool systematic = false;
	int usablenroftimes = 1;	


	if (argc > 1) {
		int tyIN = atoi(argv[1]);
		if (tyIN >= 0 && tyIN < 3 ) {
			progType = tyIN;
		} else {
                        cout << "Invalid program type! Continue with 0 - seeking batch codes\n";
		}
		
		if (argc > 2) {			
			int kIN = atoi(argv[2]);
			if (kIN > 0) {			
				k = kIN;
			} else {
				cout << "Invalid k, continues with k = 2\n";
				k = 2;
			}
		
			if (argc > 3) {				
				int nIN = atoi(argv[3]);				
				if (nIN > 0) {			
					n = nIN;
				} else {
					cout << "Invalid n, continues with n = 3\n";
					n = 3;
				}
			
				if (argc > 4) {					
					int tIN = atoi(argv[4]);
					if (tIN > 0) {			
						t = tIN;
					} else {
						cout << "Invalid t, continues with t = 2\n";
						t = 2;
					}
					
					if (argc > 5) {
						int rIN = atoi(argv[5]);
						if (rIN > 0) {			
							r = rIN;
						} else {
							cout << "Invalid r, continues with r = 2\n";
							r = 2;
						}
						

						if (argc > 6) {
							int systIN = atoi(argv[6]);
							systematic = (systIN == 1);

							if (argc > 7) {
								int nrtimesusableIN = atoi(argv[7]);
								if (nrtimesusableIN > 0) {
									usablenroftimes = nrtimesusableIN;
								} else {
									cout << "Invalid amount for nr of times usable, continues with 1\n";
									usablenroftimes = 1;
								}

							}
						}
					}
				}
			}
		}
	}
	else {
		/*for (int ik = 3; ik < 4; ik++) {
			for (int in = 6; in < 10; ik++) {
				for (int it = 2; it < 5; it++) {
					CodeFinder cf(ik, in, it);
					cf.findCodes();

				}
			}
		}*/
		int ik = 4;
		int in = 7;
		int it = 3;
		while (ik < 10 && in < 50) {
			for (int i = 0; i < 3; i++) {
				CodeFinder cf(ik, in + i, it);
				cf.findCodes();

			}

			ik += 1;
			in += 1;
		}

		return 0;//EXIT_SUCCESS;
	}
	
	cout << "Searching all " << (progType == 0? "Batch codes" : (progType == 1 ? "Batch codes with given columns" : "PIR codes")) << " where n = " << n << ", t = " << t << ", r = " << r << ( systematic ? " systematic " : "" ) << ", usable " << usablenroftimes << " times.\n";
	CodeFinder cf(k, n, t, r, systematic, usablenroftimes);
	
	if (progType == 2) {
		cf.setCodeType(1);
		cf.findCodes();
	} else if (progType == 1) {
		cf.findCodesSelect(); //Systematic doesn't matter
	}
	else {
		cf.findCodes();
	}
	
	return 0;//EXIT_SUCCESS;
}
