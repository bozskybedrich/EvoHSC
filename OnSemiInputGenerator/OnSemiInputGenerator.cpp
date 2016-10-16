#include <iostream>
#include <stdio.h>
#include <set>
#include "OnSemiInputGenerator.h"
#include <math.h>

void GenerateIOs(int nStage, REGISTER_TYPE ** &pInputs, REGISTER_TYPE ** &pOutputs, int * pTestSetsCount)
{
	switch (nStage) {
	case 0:
		std::set<int> setIn12;
		for (int i = 2000; i < 15000; i+=12000) {
			if (i == 14000)
				i = 13824;
			setIn12.insert(i);
			setIn12.insert(-i);
		}

		int nTestsCount = setIn12.size()*(setIn12.size() - 1)*2*2;
		pInputs = new REGISTER_TYPE*[nTestsCount];
		pOutputs = new REGISTER_TYPE*[nTestsCount];
		*pTestSetsCount = nTestsCount;

		for (int i = 0; i < nTestsCount; i++) {
			pInputs[i] = new REGISTER_TYPE[4];
			pOutputs[i] = new REGISTER_TYPE[1];
		}

		int i = 0;
		for (std::set<int>::iterator it1 = setIn12.begin(); it1 != setIn12.end(); it1++) {
			for (std::set<int>::iterator it2 = setIn12.begin(); it2 != setIn12.end(); it2++) {
				if (*it1 == *it2)
					continue;

				for (int sh1 = 2; sh1 <= 4; sh1 += 2) {
					for (int sh2 = 6; sh2 <= 8; sh2 += 2) {
						if (sh1 == (sh2 - 2))
							continue;

						pInputs[i][0] = *it1;
						pInputs[i][1] = *it2;
						pInputs[i][2] = sh1;
						pInputs[i][3] = sh2;
						//pOutputs[i][0] = (*it2-*it1);// / 2 * 8 * (int) pow((double) 2, sh1) / (int) pow((double) 2, sh2);
						pOutputs[i][0] = (*it2-*it1) / 2 * 8 * (int) pow((double) 2, sh1) / (int) pow((double) 2, sh2);
						++i;
					}
				}
			}
		}
		break;
	}
}