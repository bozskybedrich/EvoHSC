#include "sigmoid.h"

#include <time.h>
#include <iostream>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include "../modules/modules.h"
#include <fstream>
#include <algorithm>

#define SAMPLES_COUNT	(1)
#define INPUTS_COUNT	5
#define OUTPUTS_COUNT	5
#define MODULES_COUNT	24	// 5xInput, 19x8ANDOR

REGISTER_TYPE ** pInputs;
REGISTER_TYPE ** pOutputs;

//int nActIn[INPUTS_COUNT];
int nActTestSet = 0;
int nTestSetsCount = 1;
bool bDetailedOut = false;

void test(double fCross, double fMut);

int main(int argc, char* argv[])
{
	//AddPepperAndSalt(std::string("C:\\vutbr\\lena64.bmp"), std::string("C:\\vutbr\\lena_salt_64.bmp"), 0.025, 0.025);

	if (argc == 3) {
		double fCross = atof(argv[1]);
		double fMut = atof(argv[2]);
		test(fCross, fMut);
	}
	else
		test(0.01, 0.1);

	//system("PAUSE");
#ifdef _DEBUG
	_CrtDumpMemoryLeaks();
#endif

	return 0;
}

void GenerateIOs(REGISTER_TYPE ** &pInputs, REGISTER_TYPE ** &pOutputs, int & nSamplesCount)
{
	pInputs = new REGISTER_TYPE*[SAMPLES_COUNT];
	pInputs[0] = new REGISTER_TYPE[INPUTS_COUNT];
	pInputs[0][0] = 0xFFFF0000;
	pInputs[0][1] = 0xFF00FF00;
	pInputs[0][2] = 0xF0F0F0F0;
	pInputs[0][3] = 0xCCCCCCCC;
	pInputs[0][4] = 0xAAAAAAAA;

	pOutputs = new REGISTER_TYPE*[SAMPLES_COUNT];
	pOutputs[0] = new REGISTER_TYPE[OUTPUTS_COUNT];
	pOutputs[0][0] = 0xFFFFFE00;
	pOutputs[0][1] = 0xFFFF01E0;
	pOutputs[0][2] = 0xFFC0F198;
	pOutputs[0][3] = 0xF038C954;
	pOutputs[0][4] = 0xE36ACFE;
}

void test(double fCross, double fMut)
{
	srand((UINT) time(NULL));
	CModule * pModules[MODULES_COUNT];

	GenerateIOs(pInputs, pOutputs, nTestSetsCount);

	for (int i = 0; i < INPUTS_COUNT; i++) {
		pModules[i] = new CSigmoidInputModule(pInputs, INPUTS_COUNT, i, nTestSetsCount, &nActTestSet);
	}

	for (int i = 0; i < (MODULES_COUNT - INPUTS_COUNT); i++)
		pModules[INPUTS_COUNT + i] = new CAndOr(4);

	CEnvironment * pEnv = new CEnvironment(NULL, NULL, 1, 1, NULL);
	
	BYTE instModAll[] = {0x00, 0x00, 0x00, 0x7F};

	BYTE iosModAll[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // Input modules
		0xFF, 0xFF, 0xFF, 0xFF, 0xC2, 0xFF, //ANDOR modules (9 ins - 8 inputs, 1 fn select, 1 out)
		0xFF, 0xFF, 0xFF, 0xFF, 0xC2, 0xFF/*,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xC2, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xC2, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xC2, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xC2, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xC2, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xC2, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xC2, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xC2, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xC2, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xC2, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xC2, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xC2, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xC2, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xC2, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xC2, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xC2, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xC2, 0xFF*/
	};
	
	// MOV instructions
	BYTE instMOVr1[] = {0x80, 0x00, 0x00, 0x00};
	BYTE instMOVr2[] = {0x81, 0x00, 0x00, 0x00};
	BYTE instMOVr3[] = {0x82, 0x00, 0x00, 0x00};
	BYTE instMOVr4[] = {0x83, 0x00, 0x00, 0x00};
	BYTE instMOVr5[] = {0x84, 0x00, 0x00, 0x00};
	BYTE instMOVr6[] = {0x85, 0x00, 0x00, 0x00};

	BYTE instrOUTr1[] = {0xC0, 0x00, 0x00, 0x00};
	BYTE instrOUTr2[] = {0xC1, 0x00, 0x00, 0x00};
	BYTE instrOUTr3[] = {0xC2, 0x00, 0x00, 0x00};
	BYTE instrOUTr4[] = {0xC3, 0x00, 0x00, 0x00};
	BYTE instrOUTr5[] = {0xC4, 0x00, 0x00, 0x00};
	BYTE instrOUTr6[] = {0xC5, 0x00, 0x00, 0x00};
	BYTE instrOUTr7[] = {0xC6, 0x00, 0x00, 0x00};
	BYTE instrOUTr8[] = {0xC7, 0x00, 0x00, 0x00};
	BYTE instrOUTr9[] = {0xC8, 0x00, 0x00, 0x00};

	BYTE instrNOP[] = { 0x00, 0x00, 0x00, 0x00 };
	BYTE instrRST[] = { 0xFF, 0xFF, 0xFF, 0xFF };
#define INSTR_COUNT (28)

	CInstruction* arrInstrSet[INSTR_COUNT] = { 
									new CInstruction(4, instMOVr1, true, std::string("MOVr1")),
									new CInstruction(4, instMOVr2, true, std::string("MOVr2")),
									new CInstruction(4, instMOVr3, true, std::string("MOVr3")),
									new CInstruction(4, instMOVr4, true, std::string("MOVr4")),
									new CInstruction(4, instMOVr5, true, std::string("MOVr5")),
									new CInstruction(4, instMOVr6, true, std::string("MOVr6")),

									new CInstruction(4, instrOUTr1, true, std::string("OUTr0")),
									new CInstruction(4, instrOUTr2, true, std::string("OUTr1")),
									new CInstruction(4, instrOUTr3, true, std::string("OUTr2")),
									new CInstruction(4, instrOUTr4, true, std::string("OUTr3")),
									new CInstruction(4, instrOUTr5, true, std::string("OUTr4")),
									new CInstruction(4, instrOUTr6, true, std::string("OUTr5")),
									new CInstruction(4, instrOUTr7, true, std::string("OUTr6")),
									new CInstruction(4, instrOUTr8, true, std::string("OUTr7")),
									new CInstruction(4, instrOUTr9, true, std::string("OUTr8")),

									new CInstruction(4, instrNOP, false, std::string("NOP")),
									new CInstruction(4, instrRST, false, std::string("RST")),
									
									new CInstruction(4, instModAll, false, std::string("ALL"), 1, iosModAll, 17),
									new CInstruction(4, instModAll, false, std::string("ALL"), 1, iosModAll, 17),
									new CInstruction(4, instModAll, false, std::string("ALL"), 1, iosModAll, 17),
									new CInstruction(4, instModAll, false, std::string("ALL"), 1, iosModAll, 17),
									new CInstruction(4, instModAll, false, std::string("ALL"), 1, iosModAll, 17),
									new CInstruction(4, instModAll, false, std::string("ALL"), 1, iosModAll, 17),
									new CInstruction(4, instModAll, false, std::string("ALL"), 1, iosModAll, 17),
									new CInstruction(4, instModAll, false, std::string("ALL"), 1, iosModAll, 17),
									new CInstruction(4, instModAll, false, std::string("ALL"), 1, iosModAll, 17),
									new CInstruction(4, instModAll, false, std::string("ALL"), 1, iosModAll, 17),
									new CInstruction(4, instModAll, false, std::string("ALL"), 1, iosModAll, 17)
	};
	
	strArchitectureParams archParams;
	archParams.nMaxModulesCount = MODULES_COUNT;
	archParams.nMaxRegistersCount = 10;
	archParams.pAllowedInstructions = arrInstrSet;
	archParams.pAllowedModules = pModules;
	
	bool bDynamic = true;
	for (UINT i = 0; i < 50; i++) {
		CArchitecture * pArch = new CArchitecture(10, (REGISTER_TYPE) 0xFFFFFFFF, MODULES_COUNT, pModules, INSTR_COUNT, arrInstrSet, &archParams, bDynamic);
		strArchitectureSettings settings(MODULES_COUNT);
		pArch->SetArchSettings(&settings);

		strEvolutionParams evoParams(50, 1000000, 0.01, 0.7,/*fCross, 0.1*prob,*/
			1);
		evoParams.strMutParams.fProbs[infoMutationReg] = 0;
		evoParams.strMutParams.fProbs[infoMutationModule] = 0;

		evoParams.strSimulParams.nMaxLogicTime = 200;
				
		strIndividualParams indivParams(10,5);
		CSigmoidFramework * pFramework = new CSigmoidFramework(pArch, pEnv, evoParams, indivParams);
		pFramework->StartEvolution();
		delete pFramework;
		delete pArch;
	}
	
	delete pEnv;

	for (int i = 0; i < MODULES_COUNT; i++) {
		delete pModules[i];
	}
	
	for (UINT i = 0; i < INSTR_COUNT; i++)
		delete arrInstrSet[i];

	delete[] pInputs;
	delete[] pOutputs;

	//tmpArch.ExecuteProgram((void*) program, sizeof(program), &env);
	return;
}

double CSigmoidIndiv::EvaluateOutputs(CEnvironment * pEnv)
{
	double dFitness = 0;
	std::vector<REGISTER_TYPE> vecOuts;
	CDynamicTimeline * pTimeline = pEnv->GetOutput(0);
	if (pTimeline) {
		int nActOut = 0;
		for (strDynamicTimeVal * pVal = pTimeline->GetFirst(); pVal && nActOut < 1/*OUTPUTS_COUNT*/; pVal = pVal->pSucc, nActOut++) {
			vecOuts.push_back(pVal->val);
		}
	}

	std::sort(vecOuts.rbegin(), vecOuts.rend());

	for (std::vector<REGISTER_TYPE>::iterator itOut = vecOuts.begin(); itOut != vecOuts.end(); itOut++)
	{
		if (*itOut == pOutputs[0][3/*itOut - vecOuts.begin()*/])
			dFitness += 100.0;
		else
		{
			REGISTER_TYPE masked = (*itOut ^ pOutputs[0][3/*itOut - vecOuts.begin()*/]);
			int nCorrectBits = 0;
			int nBitWidth = sizeof(REGISTER_TYPE)*8;

			for (int i = 0; i < nBitWidth; i++)
			{
				if ((masked & 0x1) == 0)
					nCorrectBits++;

				masked >>= 1;
			}

			if (nCorrectBits > 28)
				dFitness += 10.0;
			else
				dFitness += (20.0 / OUTPUTS_COUNT / (nBitWidth * nBitWidth) * (nCorrectBits * nCorrectBits));
		}
	}

	return dFitness;
}

bool CSigmoidFramework::EvaluateStopCondition(UINT nGeneration)
{
	static bool bPartialPrinted;
	if (nGeneration == 1)
		bPartialPrinted = false;

	for (UINT nBest = 0; nBest < m_strEvoParams.nEliteSize; nBest++) {
		//if (nGeneration == 1)
		//	m_pBestIndivs[nBest]->EvaluateFitness(m_strEvoParams.strSimulParams);
		if (m_pBestIndivs[nBest]->GetFitness(eResultsFitness) >= 98) {
			if (true) {
				if (m_bPrintInfo) {
					m_pBestIndivs[nBest]->PrintProgram();
					m_strEvoParams.strSimulParams.bPrintInfo = false;
					m_pBestIndivs[nBest]->EvaluateFitness(m_strEvoParams.strSimulParams);
					m_strEvoParams.strSimulParams.bPrintInfo = false;
					char outFile[50];
					sprintf(outFile, "indiv_out_%X.xml", clock());
					std::ofstream outIndiv(outFile, std::ofstream::binary);
					std::string sXml = m_pBestIndivs[nBest]->Serialize();
					outIndiv.write(sXml.c_str(), sXml.length());
					outIndiv.close();
					//system("PAUSE");
				}
			
				return true;
			}
		}
	}
	
	return false;
}

void CSigmoidFramework::ResetAfterEval()
{
}

bool CSigmoidFramework::IsNextInputSet()
{
	if (nActTestSet < (nTestSetsCount - 1)) {
		nActTestSet++;
		return true;
	}
	else {
		nActTestSet = 0;
		return false;
	}
}
