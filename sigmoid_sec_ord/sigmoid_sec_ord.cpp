#include "sigmoid_sec_ord.h"
#include <time.h>
#include <iostream>
#define _USE_MATH_DEFINES
#include <math.h>
#include <stdio.h>
#include <string.h>
#include "../modules/modules.h"
#include <fstream>
#include <algorithm>

#define SAMPLES_COUNT	(32)
#define INPUTS_COUNT	2
#define OUTPUTS_COUNT	1
#define MODULES_COUNT	24	// 2xInput, 19x8ANDOR

REGISTER_TYPE ** pInputs;
REGISTER_TYPE ** pOutputs;

//int nActIn[INPUTS_COUNT];
int nActTestSet = 0;
int nTestSetsCount = SAMPLES_COUNT;
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
	pOutputs = new REGISTER_TYPE*[SAMPLES_COUNT];

	for (int i = 0; i < SAMPLES_COUNT; i++)
	{
		pInputs[i] = new REGISTER_TYPE[INPUTS_COUNT];
		pInputs[i][0] = i*0x8;
		pInputs[i][1] = 0x40;

		pOutputs[i] = new REGISTER_TYPE[OUTPUTS_COUNT];
		double dOut = 1.0 / (1 + pow(M_E, -i*0.125));
		/*UINT nOut = 0;
		for (int j = 0; j < 8; j++)
		{
			double dDiv = pow(2, 1 - j);
			nOut += (int) (dOut / dDiv);
			nOut <<= 1;
			dOut -= (int) (dOut / dDiv);
		}*/

		pOutputs[i][0] = (REGISTER_TYPE) (dOut * 64);
	}
}

void test(double fCross, double fMut)
{
	srand((UINT) time(NULL));
	CModule * pModules[MODULES_COUNT];

	GenerateIOs(pInputs, pOutputs, nTestSetsCount);

	for (int i = 0; i < INPUTS_COUNT; i++) {
		pModules[i] = new CSigmoidInputModule(pInputs, INPUTS_COUNT, i, nTestSetsCount, &nActTestSet);
	}

	for (int i = 0; i < 4; i++)
		pModules[INPUTS_COUNT + i] = new CMultiplier(6);

	for (int i = 0; i < 4; i++)
		pModules[INPUTS_COUNT + 4 + i] = new CBitShifter();

	for (int i = 0; i < 4; i++)
		pModules[INPUTS_COUNT + 8 + i] = new CAdder();

	for (int i = 0; i < (MODULES_COUNT - INPUTS_COUNT - 12); i++)
		pModules[INPUTS_COUNT + 12 + i] = new CAndOr(5);

	CEnvironment * pEnv = new CEnvironment(NULL, NULL, 1, 1, NULL);
	
	BYTE instModAllIn[] = {0x00, 0x00, 0x00, 0x03};

	BYTE iosModAllIn[] = {0x00, 0x01, 0x02, 0x03};

	//BYTE instOneAnd[] = { 0x00, 0x00, 0x00, 0x20 };
	//BYTE iosOneAnd[] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xC2, 0xFF, 0xFF };

	BYTE instModAll[] = {0x00, 0xFF, 0xFF, 0xFF};

	BYTE iosModAll[] = {0xFF, 0xFF, 0xFF, 0xFF, // Input modules
		0xFF, 0xFF, 0xFF, // Multiplers
		0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF, // Bit-shifters
		0xFF, 0xFF, 0xFF, 0xFF, 
		0xFF, 0xFF, 0xFF, 0xFF, 
		0xFF, 0xFF, 0xFF, 0xFF, 
		0xFF, 0xFF, 0xFF, 0xFF, // Adders
		0xFF, 0xFF, 0xFF, 0xFF, 
		0xFF, 0xFF, 0xFF, 0xFF, 
		0xFF, 0xFF, 0xFF, 0xFF, 
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xC2, 0xFF, 0xFF, // AndOrs
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xC2, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xC2, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xC2, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xC2, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xC2, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xC2, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xC2, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xC2, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xC2, 0xFF, 0xFF/*,
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
#define INSTR_COUNT (25)

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
									
									/*new CInstruction(4, instOneAnd, false, std::string("ONE"), 1, iosOneAnd, 8),
									new CInstruction(4, instOneAnd, false, std::string("ONE"), 1, iosOneAnd, 8),
									new CInstruction(4, instOneAnd, false, std::string("ONE"), 1, iosOneAnd, 8),
									new CInstruction(4, instOneAnd, false, std::string("ONE"), 1, iosOneAnd, 8),
									new CInstruction(4, instOneAnd, false, std::string("ONE"), 1, iosOneAnd, 8),*/
									new CInstruction(4, instModAll, false, std::string("ALL"), 1, iosModAll, 128),
									new CInstruction(4, instModAll, false, std::string("ALL"), 1, iosModAll, 128),
									new CInstruction(4, instModAll, false, std::string("ALL"), 1, iosModAll, 128),
									new CInstruction(4, instModAll, false, std::string("ALL"), 1, iosModAll, 128),
									new CInstruction(4, instModAll, false, std::string("ALL"), 1, iosModAll, 128),
									new CInstruction(4, instModAll, false, std::string("ALL"), 1, iosModAll, 128),

									new CInstruction(4, instModAllIn, false, std::string("ALL_IN"), 1, iosModAllIn, 4),
									new CInstruction(4, instModAllIn, false, std::string("ALL_IN"), 1, iosModAllIn, 4)
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

		strEvolutionParams evoParams(50, 1000000, 0.1, 0.7,/*fCross, 0.1*prob,*/
			1);
		evoParams.strMutParams.fProbs[infoMutationReg] = 0;
		evoParams.strMutParams.fProbs[infoMutationModule] = 0;

		evoParams.strSimulParams.nMaxLogicTime = 300;
				
		strIndividualParams indivParams(10,5);
		CSigmoidSecOrdFramework * pFramework = new CSigmoidSecOrdFramework(pArch, pEnv, evoParams, indivParams);

		/*CSigmoidIndiv tmpIndiv(pArch, pEnv, pFramework, &indivParams);
		std::ifstream inFile("d:\\EvoHSC\\x64\\Debug\\indiv_out_32AAB040.xml");
		std::stringstream buffer;
		buffer << inFile.rdbuf();
		std::string sXml = buffer.str();
		inFile.close();
		tmpIndiv.Parse(sXml);
		tmpIndiv.EvaluateFitness(evoParams.strSimulParams);*/

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

double CSigmoidSecOrdIndiv::EvaluateOutputs(CEnvironment * pEnv)
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

	for (std::vector<REGISTER_TYPE>::iterator itOut = vecOuts.begin(); itOut != vecOuts.end(); itOut++)
	{
		REGISTER_TYPE out = pOutputs[nActTestSet][0];

		if (*itOut == out)
			dFitness += (100.0 / SAMPLES_COUNT);
		else
		{
			double dDiff = abs(*itOut - out);
			dFitness += (100.0 / SAMPLES_COUNT) / (1 + dDiff);
		}
	}

	return dFitness;
}

int CSigmoidSecOrdIndiv::PregenerateProgram()
{
	return 0;
	BYTE pAllIn[] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09 };
	BYTE * pIOs = new BYTE[10];
	memcpy(pIOs, pAllIn, 10);
	AppendInstrBlock(INSTR_COUNT - 1, 0, pIOs);
	
	return 1;
}

bool CSigmoidSecOrdFramework::EvaluateStopCondition(UINT nGeneration)
{
	static bool bPartialPrinted;
	if (nGeneration == 1)
		bPartialPrinted = false;

	for (UINT nBest = 0; nBest < m_strEvoParams.nEliteSize; nBest++) {
		//if (nGeneration == 1)
		//	m_pBestIndivs[nBest]->EvaluateFitness(m_strEvoParams.strSimulParams);
		double fitness = m_pBestIndivs[nBest]->GetFitness(eResultsFitness);
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
		else
		{
			if (nGeneration % 10000 == 0)
			{
				m_strEvoParams.strSimulParams.bPrintInfo = true;
				m_strEvoParams.strSimulParams.bPrintDebug = true;
				m_pBestIndivs[nBest]->EvaluateFitness(m_strEvoParams.strSimulParams);
				m_strEvoParams.strSimulParams.bPrintDebug = false;
				m_strEvoParams.strSimulParams.bPrintInfo = false;
			}
		}
	}
	
	return false;
}

void CSigmoidSecOrdFramework::ResetAfterEval()
{
}

bool CSigmoidSecOrdFramework::IsNextInputSet()
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
