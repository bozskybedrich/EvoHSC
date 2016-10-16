// Goldschmidt.cpp : Defines the entry point for the console application.
//
#include "Parity.h"
#include <time.h>
#include <iostream>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include "../maximum/MaxInputModule.h"
#include "XorModule.h"

#define SAMPLES_COUNT	8
#define TEST_SETS_COUNT	8//256

REGISTER_TYPE ** pInputs;
REGISTER_TYPE pParity[TEST_SETS_COUNT];
int nActIn = 0;
int nActTestSet = 0;

void test(double fCross, double fMut);

int main(int argc, char* argv[])
{
	if (argc == 3) {
		double fCross = atof(argv[1]);
		double fMut = atof(argv[2]);
		test(fCross, fMut);
	}
	else
		test(0.1, 0.7);

	//system("PAUSE");
#ifdef _DEBUG
	_CrtDumpMemoryLeaks();
#endif

	return 0;
}

void test(double fCross, double fMut)
{
	srand((UINT) time(NULL));
	CModule * pModules[2*SAMPLES_COUNT];

	pInputs = new REGISTER_TYPE *[TEST_SETS_COUNT];
	for (int nSet = 0; nSet < TEST_SETS_COUNT; nSet++)
		pInputs[nSet] = new REGISTER_TYPE[SAMPLES_COUNT];
	
	pInputs[0][0] = 0xFFFFFFFF;
	pInputs[1][0] = 0xFFFFFFFF;
	pInputs[2][0] = 0xFFFFFFFF;
	pInputs[3][0] = 0xFFFFFFFF;
	pInputs[4][0] = 0;
	pInputs[5][0] = 0;
	pInputs[6][0] = 0;
	pInputs[7][0] = 0;

	pInputs[0][1] = 0xFFFFFFFF;
	pInputs[1][1] = 0xFFFFFFFF;
	pInputs[2][1] = 0;
	pInputs[3][1] = 0;
	pInputs[4][1] = 0xFFFFFFFF;
	pInputs[5][1] = 0xFFFFFFFF;
	pInputs[6][1] = 0;
	pInputs[7][1] = 0;

	pInputs[0][2] = 0xFFFFFFFF;
	pInputs[1][2] = 0;
	pInputs[2][2] = 0xFFFFFFFF;
	pInputs[3][2] = 0;
	pInputs[4][2] = 0xFFFFFFFF;
	pInputs[5][2] = 0;
	pInputs[6][2] = 0xFFFFFFFF;
	pInputs[7][2] = 0;

	pInputs[0][3] = 0xFFFF0000;
	pInputs[1][3] = 0xFFFF0000;
	pInputs[2][3] = 0xFFFF0000;
	pInputs[3][3] = 0xFFFF0000;
	pInputs[4][3] = 0xFFFF0000;
	pInputs[5][3] = 0xFFFF0000;
	pInputs[6][3] = 0xFFFF0000;
	pInputs[7][3] = 0xFFFF0000;

	pInputs[0][4] = 0xFF00FF00;
	pInputs[1][4] = 0xFF00FF00;
	pInputs[2][4] = 0xFF00FF00;
	pInputs[3][4] = 0xFF00FF00;
	pInputs[4][4] = 0xFF00FF00;
	pInputs[5][4] = 0xFF00FF00;
	pInputs[6][4] = 0xFF00FF00;
	pInputs[7][4] = 0xFF00FF00;

	pInputs[0][5] = 0xF0F0F0F0;
	pInputs[1][5] = 0xF0F0F0F0;
	pInputs[2][5] = 0xF0F0F0F0;
	pInputs[3][5] = 0xF0F0F0F0;
	pInputs[4][5] = 0xF0F0F0F0;
	pInputs[5][5] = 0xF0F0F0F0;
	pInputs[6][5] = 0xF0F0F0F0;
	pInputs[7][5] = 0xF0F0F0F0;

	pInputs[0][6] = 0xCCCCCCCC;
	pInputs[1][6] = 0xCCCCCCCC;
	pInputs[2][6] = 0xCCCCCCCC;
	pInputs[3][6] = 0xCCCCCCCC;
	pInputs[4][6] = 0xCCCCCCCC;
	pInputs[5][6] = 0xCCCCCCCC;
	pInputs[6][6] = 0xCCCCCCCC;
	pInputs[7][6] = 0xCCCCCCCC;

	pInputs[0][7] = 0xAAAAAAAA;
	pInputs[1][7] = 0xAAAAAAAA;
	pInputs[2][7] = 0xAAAAAAAA;
	pInputs[3][7] = 0xAAAAAAAA;
	pInputs[4][7] = 0xAAAAAAAA;
	pInputs[5][7] = 0xAAAAAAAA;
	pInputs[6][7] = 0xAAAAAAAA;
	pInputs[7][7] = 0xAAAAAAAA;

	for (int nSet = 0; nSet < TEST_SETS_COUNT; nSet++) {
		for (int i = 0; i < SAMPLES_COUNT; i++) {
			pParity[nSet] = pParity[nSet] ^ pInputs[nSet][i];
		}
	}

	/*pInputs = new REGISTER_TYPE *[TEST_SETS_COUNT];
	for (int nSet = 0; nSet < TEST_SETS_COUNT; nSet++) {
		pInputs[nSet] = new REGISTER_TYPE[SAMPLES_COUNT];

		std::cout << "Input #" << nSet << " ";
		pParity[nSet] = 0;
		UINT nCopy = nSet;

		for (int i = 0; i < SAMPLES_COUNT; i++) {
			pInputs[nSet][i] = nCopy & 1;
			pParity[nSet] = (bool) pParity[nSet] ^ (bool) pInputs[nSet][i];

			nCopy >>= 1;

			std::cout << pInputs[nSet][i] << " ";
		}
		std::cout << " Parity: " << pParity[nSet] << std::endl;
	}*/

	for (int i = 0; i < SAMPLES_COUNT; i++) {
		CMaxInputModule * pInMod = new CMaxInputModule(pInputs, SAMPLES_COUNT, &nActIn, TEST_SETS_COUNT, &nActTestSet);
		pInMod->SetDefZero(false);
		pInMod->SetDefSign(true);
		pModules[i] = pInMod;
	}

	for (int i = 0; i < SAMPLES_COUNT; i++) {
		CXorModule * pXorMod = new CXorModule();
		pModules[SAMPLES_COUNT + i] = pXorMod;
	}

	CEnvironment * pEnv = new CEnvironment(NULL, NULL, 1, 1, NULL);
	
	BYTE instLOAD1[] = {0x90, 0x00, 0x00, 0x00};
	BYTE instLOAD2[] = {0x91, 0x00, 0x00, 0x00};
	BYTE instLOAD3[] = {0x92, 0x00, 0x00, 0x00};
	BYTE instLOAD4[] = {0x93, 0x00, 0x00, 0x00};
	
	BYTE instModAll[] = {0x00, 0x00, 0xFF, 0xFF};
	BYTE iosModAll[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
		0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF
	};

	BYTE instMod1_9[] = {0x00, 0x00, 0x01, 0x01};
	BYTE iosMod1_9[] = {0xFF, 0xFF, 0xFF, 0xFF };
	
	// MOV instructions
	BYTE instMOVr1[] = {0x80, 0x00, 0x00, 0x00};
	BYTE instMOVr2[] = {0x81, 0x00, 0x00, 0x00};
	BYTE instMOVr3[] = {0x82, 0x00, 0x00, 0x00};
	BYTE instMOVr4[] = {0x83, 0x00, 0x00, 0x00};

	BYTE instJMP[] = {0x10, 0x00, 0x00, 0x00};
	BYTE instJZ1[] = {0x20, 0x00, 0x00, 0x01}; // JUMP if module 1 has zero flag
	BYTE instJS1[] = {0x20, 0x00, 0x00, 0x01}; // JUMP if module 1 has sign flag
	BYTE instJNZ1[] = {0x40, 0x00, 0x00, 0x01}; // JUMP if module 1 doesn't have zero flag
	BYTE instJNS1[] = {0x50, 0x00, 0x00, 0x01}; // JUMP if module 1 doesn't have sign flag
	BYTE instJZ2[] = {0x20, 0x00, 0x00, 0x02}; // JUMP if module 2 has zero flag
	BYTE instJS2[] = {0x20, 0x00, 0x00, 0x02}; // JUMP if module 2 has sign flag
	BYTE instJNZ2[] = {0x40, 0x00, 0x00, 0x02}; // JUMP if module 2 doesn't have zero flag
	BYTE instJNS2[] = {0x50, 0x00, 0x00, 0x02}; // JUMP if module 2 doesn't have zero flag
	BYTE instJZ3[] = {0x20, 0x00, 0x00, 0x04}; // JUMP if module 3 has zero flag
	BYTE instJS3[] = {0x20, 0x00, 0x00, 0x04}; // JUMP if module 3 has sign flag
	BYTE instJNZ3[] = {0x40, 0x00, 0x00, 0x04}; // JUMP if module 3 doesn't have zero flag
	BYTE instJNS3[] = {0x50, 0x00, 0x00, 0x04}; // JUMP if module 3 doesn't have zero flag
	BYTE instJZ4[] = {0x20, 0x00, 0x00, 0x08}; // JUMP if module 4 has zero flag
	BYTE instJS4[] = {0x20, 0x00, 0x00, 0x08}; // JUMP if module 4 has sign flag
	BYTE instJNZ4[] = {0x40, 0x00, 0x00, 0x08}; // JUMP if module 4 doesn't have zero flag
	BYTE instJNS4[] = {0x50, 0x00, 0x00, 0x08}; // JUMP if module 4 doesn't have zero flag

	BYTE instrOUTr4[] = {0xC3, 0x00, 0x00, 0x00};
	BYTE instrOUTr3[] = {0xC2, 0x00, 0x00, 0x00};
	BYTE instrOUTr2[] = {0xC1, 0x00, 0x00, 0x00};
	BYTE instrOUTr1[] = {0xC0, 0x00, 0x00, 0x00};
	BYTE instrNOP[] = { 0x00, 0x00, 0x00, 0x00 };
	BYTE instrRST[] = { 0xFF, 0xFF, 0xFF, 0xFF };
#define INSTR_COUNT 31

	CInstruction* arrInstrSet[INSTR_COUNT] = { 
									new CInstruction((UINT) 4, (BYTE*) instLOAD1, true, std::string("LOADr1")),
									new CInstruction(4, instLOAD2, true, std::string("LOADr2")),
									new CInstruction(4, instLOAD3, true, std::string("LOADr3")),
									new CInstruction(4, instLOAD4, true, std::string("LOADr4")),

									new CInstruction(4, instMOVr1, true, std::string("MOVr1")),
									new CInstruction(4, instMOVr2, true, std::string("MOVr2")),
									new CInstruction(4, instMOVr3, true, std::string("MOVr3")),
									new CInstruction(4, instMOVr4, true, std::string("MOVr3")),

									new CInstruction(4, instJZ1, true, std::string("JZ1")),
									new CInstruction(4, instJZ2, true, std::string("JZ2")),
									new CInstruction(4, instJZ3, true, std::string("JZ3")),
									new CInstruction(4, instJZ4, true, std::string("JZ4")),
									new CInstruction(4, instJNZ1, true, std::string("JNZ1")),
									new CInstruction(4, instJNZ2, true, std::string("JNZ2")),
									new CInstruction(4, instJNZ3, true, std::string("JNZ3")),
									new CInstruction(4, instJNZ4, true, std::string("JNZ4")),
									new CInstruction(4, instJS1, true, std::string("JS1")),
									new CInstruction(4, instJS2, true, std::string("JS2")),
									new CInstruction(4, instJS3, true, std::string("JS3")),
									new CInstruction(4, instJS4, true, std::string("JS4")),
									new CInstruction(4, instJNS1, true, std::string("JNS1")),
									new CInstruction(4, instJNS2, true, std::string("JNS2")),
									new CInstruction(4, instJNS3, true, std::string("JNS3")),
									new CInstruction(4, instJNS4, true, std::string("JNS4")),
						
									new CInstruction(4, instrOUTr1, true, std::string("OUTr1")),
									new CInstruction(4, instrOUTr2, true, std::string("OUTr2")),
									new CInstruction(4, instrOUTr3, true, std::string("OUTr3")),
									new CInstruction(4, instrOUTr4, true, std::string("OUTr4")),

									new CInstruction(4, instrNOP, false, std::string("NOP")),
									new CInstruction(4, instrRST, false, std::string("RST")),
									
									new CInstruction(4, instModAll, false, std::string("MOD_ALL"), 1, iosModAll, 32)
									//new CInstruction(4, instMod1_9, false, std::string("MOD_1_9"), 1, iosMod1_9, 5)
	};
	
	strArchitectureParams archParams;
	archParams.nMaxModulesCount = 4;
	archParams.nMaxRegistersCount = 4;
	archParams.pAllowedInstructions = arrInstrSet;
	archParams.pAllowedModules = pModules;
	
	bool bDynamic = true;
	for (double probCros = 0; probCros <= 1.0; probCros+=0.1) {
		for (double probMut = 0; probMut <= 1.0; probMut+=0.1) {
			for (UINT i = 0; i < 50; i++) {
				CArchitecture * pArch = new CArchitecture(4, (REGISTER_TYPE) 0xFFFFFFFF, 2*SAMPLES_COUNT, pModules, INSTR_COUNT, arrInstrSet, &archParams, bDynamic);
				strArchitectureSettings settings(3);
				pArch->SetArchSettings(&settings);

				strEvolutionParams evoParams(50, 20000, 0.05, 0.7,/*fCross, 0.1*prob,*/
					1);
				evoParams.strMutParams.fProbs[infoMutationReg] = 0;
				//evoParams.strMutParams.fProbs[infoMutationModule] = 0;

				evoParams.strSimulParams.nMaxLogicTime = 500;
				
				strIndividualParams indivParams(10,10);
				CParityFramework * pFramework = new CParityFramework(pArch, pEnv, evoParams, indivParams);
				pFramework->StartEvolution();
				delete pFramework;
				delete pArch;
			}
		}
	}
	
	delete pEnv;

	for (int i = 0; i < SAMPLES_COUNT; i++) {
		delete pModules[i];
	}
	
	for (UINT i = 0; i < INSTR_COUNT; i++)
		delete arrInstrSet[i];

	//tmpArch.ExecuteProgram((void*) program, sizeof(program), &env);
	return;
}

double CParityIndiv::EvaluateOutputs(CEnvironment * pEnv)
{
	double dFitness = 0;
	CDynamicTimeline * pTimeline = pEnv->GetOutput(0);
	if (pTimeline) {
		strDynamicTimeVal * pVal = pTimeline->GetFirst();
		if (pVal) {
			if (pVal->val == 0)
				return 0;

			REGISTER_TYPE nRes = pVal->val ^ pParity[nActTestSet];
			for (int i = 0; i < 8*sizeof(REGISTER_TYPE); i++) {
				if ((nRes & 1) == 0)
					dFitness += (100.0 / TEST_SETS_COUNT / (8*sizeof(REGISTER_TYPE)));

				nRes >>= 1;
			}
		}
	}

	return dFitness;
}

void CParityIndiv::GenerateRandomProgram()
{
	CIndividual::GenerateRandomProgram();
	return;

	/*m_pIndivParams->nMaxProgramSize--;
	__super::GenerateRandomProgram();
	AppendInstrBlock(8, -20, NULL);
	m_pIndivParams->nMaxProgramSize++;
	return;*/

	
	// Smallest
	BYTE modIos[] = { 0xFF, 0x00, 0x04, 0x00 };
	BYTE * compIOs = new BYTE[4];
	memcpy(compIOs, modIos, 4);
	AppendInstrBlock(30, 0, compIOs);

	AppendInstrBlock(20, -1, NULL);
	AppendInstrBlock(24, 0, NULL);

	// Fastest
	/*BYTE modIos[] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
		0x04, 0x05, 0xFF, 0xFF,
		0x08, 0x09, 0xFF, 0xFF,
		0x0C, 0x0D, 0xFF, 0xFF,
		0x10, 0x11, 0xFF, 0xFF,
		0x14, 0x15, 0xFF, 0xFF,
		0x18, 0x19, 0xFF, 0xFF,
		0x1C, 0x1D, 0xFF, 0x00
	};

	BYTE * compIOs = new BYTE[40];
	memcpy(compIOs, modIos, 5);
	AppendInstrBlock(30, 0, compIOs);

	AppendInstrBlock(24, 0, NULL);
	AppendInstrBlock(28, 0, NULL);*/

	AppendInstrBlock(28, 0, NULL);
	AppendInstrBlock(28, 0, NULL);
	AppendInstrBlock(28, 0, NULL);
	AppendInstrBlock(28, 0, NULL);
	AppendInstrBlock(28, 0, NULL);
	AppendInstrBlock(28, 0, NULL);
	AppendInstrBlock(28, 0, NULL);

	return;
}

void CParityIndiv::GenerateRandomHWTopology()
{
	CIndividual::GenerateRandomHWTopology();
	return;

	UINT nModCount = m_pArch->GetModulesCount();
	m_pArchSettings = new strArchitectureSettings(nModCount);

	for (UINT nActMod = 0; nActMod < nModCount; nActMod++) {
		m_pArchSettings->pModulesOrder[nActMod] = nActMod;
	}
}

void CParityIndiv::PrintProgram()
{
	for (UINT m = 0; m < m_pArch->GetModulesCount(); m++) {
		printf("mod%d: %d\n", m, m_pArchSettings->pModulesOrder[m]);
	}

	for (UINT i = 0; i < m_pArch->GetRegistersCount(); i++)
		printf("reg%d: %X\n", i, m_pArch->GetRegisterMask(i));

	for (CInstrBlock * pActInstr = m_pFirstInstr; pActInstr != NULL; pActInstr = pActInstr->GetNext()) {
		for (UINT nActInstr = 0; nActInstr < pActInstr->GetInstrCount(); nActInstr++) {
			UINT nInstrNo = pActInstr->GetInstrNo(nActInstr);
			CInstruction * pInstr = m_pArch->GetInstruction(nInstrNo);
			if (pInstr->IsParametric())
#ifndef FP_REGISTER_TYPE
				printf("%s %d", pInstr->GetDescription().c_str(), pActInstr->GetParam(nActInstr));
#else
				printf("%s %f", pInstr->GetDescription().c_str(), pActInstr->GetParam(nActInstr));
#endif
			else
				printf("%s", pInstr->GetDescription().c_str());
			
			if (pInstr->GetInsOuts() != NULL) {
				BYTE * pIOs = pActInstr->GetInstrIOs(nActInstr);
				BYTE * pInstrIOs = pInstr->GetInsOuts();
				for (UINT nIO = 0; nIO < pInstr->GetInsOutsCount(); nIO++) {
					if (pInstrIOs[nIO] != 0xFF && ((pInstrIOs[nIO] & 0x80) != 0) && ((pInstrIOs[nIO] & 0xC0) != 0xC0))
						printf(" %X", pInstrIOs[nIO]);
					else
						printf(" %X", pIOs[nIO]);
				}
				printf("\n");
			}
			else
				printf("\n");
		}
	}
}

double CParityIndiv::EvaluateSpeed(UINT nExecTime, UINT nMaxTime)
{
	double dFitness = 0;
	CDynamicTimeline * pTimeline = m_pEnv->GetOutput(0);
	if (pTimeline) {
		strDynamicTimeVal * pVal = pTimeline->GetFirst();
		if (pVal)
			dFitness = 1 - ((double) pVal->time / nMaxTime);
	}

	return dFitness;
}

bool CParityFramework::EvaluateStopCondition(UINT nGeneration)
{
	static bool bPartialPrinted;
	if (nGeneration == 1)
		bPartialPrinted = false;

	for (UINT nBest = 0; nBest < m_strEvoParams.nEliteSize; nBest++) {
		//if (nGeneration == 1)
		//	m_pBestIndivs[nBest]->EvaluateFitness(m_strEvoParams.strSimulParams);
		if (m_pBestIndivs[nBest]->GetFitness(eResultsFitness) >= 90) {
			if (m_pBestIndivs[nBest]->GetFitness(eAreaFitness) > 4500) {
				if (m_bPrintInfo) {
					m_pBestIndivs[nBest]->PrintProgram();
					m_strEvoParams.strSimulParams.bPrintInfo = true;
					m_pBestIndivs[nBest]->EvaluateFitness(m_strEvoParams.strSimulParams);
					m_strEvoParams.strSimulParams.bPrintInfo = false;
					system("PAUSE");
				}
			
				return true;
			}
		}
	}
	
	return false;
}

void CParityFramework::ResetAfterEval()
{
	nActIn = 0;
}

bool CParityFramework::IsNextInputSet()
{
	if (nActTestSet < (TEST_SETS_COUNT - 1)) {
		nActTestSet++;
		return true;
	}
	else {
		nActTestSet = 0;
		return false;
	}
}
