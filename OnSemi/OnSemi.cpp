// Goldschmidt.cpp : Defines the entry point for the console application.
//
#include "OnSemi.h"
#include <time.h>
#include <iostream>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include "modules.h"
#include "OnSemiInputGenerator.h"

#define SAMPLES_COUNT	8
#define INPUTS_COUNT	4
#define CONSTANTS_COUNT	19
#define MODULES_COUNT	9	// 4 inputs, 3 shifters, 1 adder, 1 multiplier

REGISTER_TYPE ** pInputs;
REGISTER_TYPE ** pOutputs;

int nActIn[4];
int nActTestSet = 0;
int nTestSetsCount;
bool bDetailedOut = false;

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
	CModule * pModules[MODULES_COUNT];

	GenerateIOs(0, pInputs, pOutputs, &nTestSetsCount);

	/*for (int i = 0; i < nTestSetsCount; i++) {
		for (int j = 0; j < INPUTS_COUNT; j++) {
			std::cout << pInputs[i][j] << "\t";
		}
		std::cout << pOutputs[i][0] << std::endl;
	}*/

	for (int i = 0; i < INPUTS_COUNT; i++) {
		nActIn[i] = i;
		pModules[i] = new CMaxInputModule(pInputs, INPUTS_COUNT, &nActIn[i], nTestSetsCount, &nActTestSet);
	}

	/*for (int i = 0; i < nTestSetsCount; i++) {
		for (int j = 0; j < INPUTS_COUNT; j++) {
			REGISTER_TYPE out;
			pModules[j]->Execute(NULL, &out);
			std::cout << out << "\t";
			//nActIn[j] = j;
		}
		std::cout << pOutputs[i][0] << std::endl;
		nActTestSet++;
	}

	return;*/
	
	for (int i = INPUTS_COUNT; i < INPUTS_COUNT + 3; i++)
		pModules[i] = new CBitShifter();

	pModules[INPUTS_COUNT + 3] = new CAdder();
	pModules[INPUTS_COUNT + 4] = new CMultiplier();

	CEnvironment * pEnv = new CEnvironment(NULL, NULL, 1, 1, NULL);
	
	BYTE instLOAD1[] = {0x90, 0x00, 0x00, 0x00};
	BYTE instLOAD2[] = {0x91, 0x00, 0x00, 0x00};
	BYTE instLOAD3[] = {0x92, 0x00, 0x00, 0x00};
	BYTE instLOAD4[] = {0x93, 0x00, 0x00, 0x00};
	BYTE instLOAD5[] = {0x94, 0x00, 0x00, 0x00};
	BYTE instLOAD6[] = {0x95, 0x00, 0x00, 0x00};

	BYTE instMod1[] = {0x00, 0x00, 0x00, 0x01};
	BYTE instMod2[] = {0x00, 0x00, 0x00, 0x02};
	BYTE instMod3[] = {0x00, 0x00, 0x00, 0x04};
	BYTE instMod4[] = {0x00, 0x00, 0x00, 0x08};
	BYTE instMod5[] = {0x00, 0x00, 0x00, 0x10};
	BYTE instMod6[] = {0x00, 0x00, 0x00, 0x20};
	BYTE instMod7[] = {0x00, 0x00, 0x00, 0x40};
	BYTE instMod8[] = {0x00, 0x00, 0x00, 0x80};
	BYTE instMod9[] = {0x00, 0x00, 0x01, 0x00};
	BYTE instMod128[] = {0x00, 0x00, 0x00, 0x83};
	BYTE instMod1234[] = {0x00, 0x00, 0x00, 0x0F};

	BYTE instModAll[] = {0x00, 0x00, 0xFF, 0xFF};

	BYTE iosMod8[] = {0xFF, 0xFF, 0xC4, 0xFF};
	BYTE iosMod128[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xC4, 0xFF};
	BYTE iosMod1234[] = {0x00, 0x01, 0x02, 0x03};
	BYTE iosModAll[] = {0xFF, 0xFF, 0xFF, 0xFF, // Input modules outputs
		0xFF, 0xFF, 0xFF, 0xFF, // 3 shifters - 3 inputs, 1 output each
		0xFF, 0xFF, 0xFF, 0xFF, 
		0xFF, 0xFF, 0xFF, 0xFF, 
		0xFF, 0xFF, 0xC4, 0xFF, // Adder
		0xFF, 0xFF, 0xFF		// Multiplier
	};

	// MOV instructions
	BYTE instMOVr1[] = {0x80, 0x00, 0x00, 0x00};
	BYTE instMOVr2[] = {0x81, 0x00, 0x00, 0x00};
	BYTE instMOVr3[] = {0x82, 0x00, 0x00, 0x00};
	BYTE instMOVr4[] = {0x83, 0x00, 0x00, 0x00};
	BYTE instMOVr5[] = {0x84, 0x00, 0x00, 0x00};
	BYTE instMOVr6[] = {0x85, 0x00, 0x00, 0x00};

	BYTE instJMP[] = {0x10, 0x00, 0x00, 0x00};
	BYTE instJZ5[] = {0x20, 0x00, 0x00, 0x05}; // JUMP if module 5 (shifter) has zero flag
	BYTE instJS5[] = {0x30, 0x00, 0x00, 0x05}; // JUMP if module 5 has sign flag
	BYTE instJNZ5[] = {0x40, 0x00, 0x00, 0x05}; // JUMP if module 5 doesn't have zero flag
	BYTE instJNS5[] = {0x50, 0x00, 0x00, 0x05}; // JUMP if module 5 doesn't have sign flag
	BYTE instJZ6[] = {0x20, 0x00, 0x00, 0x06}; // JUMP if module 6 (shifter) has zero flag
	BYTE instJS6[] = {0x30, 0x00, 0x00, 0x06}; // JUMP if module 6 has sign flag
	BYTE instJNZ6[] = {0x40, 0x00, 0x00, 0x06}; // JUMP if module 6 doesn't have zero flag
	BYTE instJNS6[] = {0x50, 0x00, 0x00, 0x06}; // JUMP if module 6 doesn't have sign flag
	BYTE instJZ7[] = {0x20, 0x00, 0x00, 0x07}; // JUMP if module 7 (shifter) has zero flag
	BYTE instJS7[] = {0x30, 0x00, 0x00, 0x07}; // JUMP if module 7 has sign flag
	BYTE instJNZ7[] = {0x40, 0x00, 0x00, 0x07}; // JUMP if module 7 doesn't have zero flag
	BYTE instJNS7[] = {0x50, 0x00, 0x00, 0x07}; // JUMP if module 7 doesn't have sign flag
	BYTE instJZ8[] = {0x20, 0x00, 0x00, 0x08}; // JUMP if module 8 (shifter) has zero flag
	BYTE instJS8[] = {0x30, 0x00, 0x00, 0x08}; // JUMP if module 8 has sign flag
	BYTE instJNZ8[] = {0x40, 0x00, 0x00, 0x08}; // JUMP if module 8 doesn't have zero flag
	BYTE instJNS8[] = {0x50, 0x00, 0x00, 0x08}; // JUMP if module 8 doesn't have sign flag
	BYTE instJZ9[] = {0x20, 0x00, 0x00, 0x09}; // JUMP if module 9 (shifter) has zero flag
	BYTE instJS9[] = {0x30, 0x00, 0x00, 0x09}; // JUMP if module 9 has sign flag
	BYTE instJNZ9[] = {0x40, 0x00, 0x00, 0x09}; // JUMP if module 9 doesn't have zero flag
	BYTE instJNS9[] = {0x50, 0x00, 0x00, 0x09}; // JUMP if module 9 doesn't have sign flag

	BYTE instrOUTr1[] = {0xC0, 0x00, 0x00, 0x00};
	BYTE instrOUTr2[] = {0xC1, 0x00, 0x00, 0x00};
	BYTE instrOUTr3[] = {0xC2, 0x00, 0x00, 0x00};
	BYTE instrOUTr4[] = {0xC3, 0x00, 0x00, 0x00};
	BYTE instrOUTr5[] = {0xC4, 0x00, 0x00, 0x00};
	BYTE instrOUTr6[] = {0xC5, 0x00, 0x00, 0x00};

	BYTE instrNOP[] = { 0x00, 0x00, 0x00, 0x00 };
	BYTE instrRST[] = { 0xFF, 0xFF, 0xFF, 0xFF };
#define INSTR_COUNT (53)

	CInstruction* arrInstrSet[INSTR_COUNT] = { 
									new CInstruction((UINT) 4, (BYTE*) instLOAD1, true, std::string("LOADr1")),
									new CInstruction(4, instLOAD2, true, std::string("LOADr2")),
									new CInstruction(4, instLOAD3, true, std::string("LOADr3")),
									new CInstruction(4, instLOAD4, true, std::string("LOADr4")),
									new CInstruction(4, instLOAD5, true, std::string("LOADr5")),
									new CInstruction(4, instLOAD6, true, std::string("LOADr6")),

									new CInstruction(4, instMOVr1, true, std::string("MOVr1")),
									new CInstruction(4, instMOVr2, true, std::string("MOVr2")),
									new CInstruction(4, instMOVr3, true, std::string("MOVr3")),
									new CInstruction(4, instMOVr4, true, std::string("MOVr4")),
									new CInstruction(4, instMOVr5, true, std::string("MOVr5")),
									new CInstruction(4, instMOVr6, true, std::string("MOVr6")),

									new CInstruction(4, instJMP, true, std::string("JMP")),
									new CInstruction(4, instJZ5, true, std::string("JZ5")),
									new CInstruction(4, instJZ6, true, std::string("JZ6")),
									new CInstruction(4, instJZ7, true, std::string("JZ7")),
									new CInstruction(4, instJZ8, true, std::string("JZ8")),
									new CInstruction(4, instJZ9, true, std::string("JZ9")),
									new CInstruction(4, instJNZ5, true, std::string("JNZ5")),
									new CInstruction(4, instJNZ6, true, std::string("JNZ6")),
									new CInstruction(4, instJNZ7, true, std::string("JNZ7")),
									new CInstruction(4, instJNZ8, true, std::string("JNZ8")),
									new CInstruction(4, instJNZ9, true, std::string("JNZ9")),
									new CInstruction(4, instJS5, true, std::string("JS5")),
									new CInstruction(4, instJS6, true, std::string("JS6")),
									new CInstruction(4, instJS7, true, std::string("JS7")),
									new CInstruction(4, instJS8, true, std::string("JS8")),
									new CInstruction(4, instJS9, true, std::string("JS9")),
									new CInstruction(4, instJNS5, true, std::string("JNS5")),
									new CInstruction(4, instJNS6, true, std::string("JNS6")),
									new CInstruction(4, instJNS7, true, std::string("JNS7")),
									new CInstruction(4, instJNS8, true, std::string("JNS8")),
									new CInstruction(4, instJNS9, true, std::string("JNS9")),
						
									new CInstruction(4, instrOUTr1, true, std::string("OUTr1")),
									new CInstruction(4, instrOUTr2, true, std::string("OUTr2")),
									new CInstruction(4, instrOUTr3, true, std::string("OUTr3")),
									new CInstruction(4, instrOUTr4, true, std::string("OUTr4")),
									new CInstruction(4, instrOUTr5, true, std::string("OUTr5")),
									new CInstruction(4, instrOUTr6, true, std::string("OUTr6")),

									new CInstruction(4, instrNOP, false, std::string("NOP")),
									new CInstruction(4, instrRST, false, std::string("RST")),
									
									new CInstruction(4, instMod1, false, std::string("MOD1"), 1, iosModAll, 1),
									new CInstruction(4, instMod2, false, std::string("MOD2"), 1, iosModAll, 1),
									new CInstruction(4, instMod3, false, std::string("MOD3"), 1, iosModAll, 1),
									new CInstruction(4, instMod4, false, std::string("MOD4"), 1, iosModAll, 1),
									new CInstruction(4, instMod5, false, std::string("MOD5"), 1, iosModAll, 4),
									new CInstruction(4, instMod6, false, std::string("MOD6"), 1, iosModAll, 4),
									new CInstruction(4, instMod7, false, std::string("MOD7"), 1, iosModAll, 4),
									new CInstruction(4, instMod8, false, std::string("MOD8"), 1, iosMod8, 4),
									new CInstruction(4, instMod9, false, std::string("MOD9"), 1, iosModAll, 3),

									new CInstruction(4, instMod128, false, std::string("MOD128"), 1, iosMod128, 5),
									new CInstruction(4, instMod1234, false, std::string("MOD1234"), 1, iosMod1234, 4),

									new CInstruction(4, instModAll, false, std::string("MOD_ALL"), 1, iosModAll, 23)
									//new CInstruction(4, instMod1_9, false, std::string("MOD_1_9"), 1, iosMod1_9, 5)
	};
	
	strArchitectureParams archParams;
	archParams.nMaxModulesCount = MODULES_COUNT;
	archParams.nMaxRegistersCount = 6;
	archParams.pAllowedInstructions = arrInstrSet;
	archParams.pAllowedModules = pModules;
	
	bool bDynamic = true;
	for (UINT i = 0; i < 50; i++) {
		CArchitecture * pArch = new CArchitecture(6, (REGISTER_TYPE) 0xFFFFFFFF, MODULES_COUNT, pModules, INSTR_COUNT, arrInstrSet, &archParams, bDynamic);
		strArchitectureSettings settings(MODULES_COUNT);
		pArch->SetArchSettings(&settings);

		strEvolutionParams evoParams(10, 500000, 0.1, 0.7,/*fCross, 0.1*prob,*/
			1);
		evoParams.strMutParams.fProbs[infoMutationReg] = 0;
		evoParams.strMutParams.fProbs[infoMutationModule] = 0;

		evoParams.strSimulParams.nMaxLogicTime = 200;
				
		strIndividualParams indivParams(15,5);
		CMaximumFramework * pFramework = new CMaximumFramework(pArch, pEnv, evoParams, indivParams);
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

	//tmpArch.ExecuteProgram((void*) program, sizeof(program), &env);
	return;
}

double COnSemiIndiv::EvaluateOutputs(CEnvironment * pEnv)
{
	double dFitness = 0;
	CDynamicTimeline * pTimeline = pEnv->GetOutput(0);
	if (pTimeline) {
		strDynamicTimeVal * pVal = pTimeline->GetFirst();
		if (pVal) {
			//dFitness = pVal->val / (double) (nActTestSet + 1);
			dFitness = (100.0 / nTestSetsCount) * (1 / pow((double) (1 + abs(pVal->val - pOutputs[nActTestSet][0])), 2));
			//dFitness = (pVal->val == pOutputs[nActTestSet][0]) ? 1 : 0;
			if (bDetailedOut)
				std::cerr << pVal->val << std::endl;
		}
	}

	return dFitness;
}

void COnSemiIndiv::GenerateRandomProgram()
{
	CIndividual::GenerateRandomProgram();
	return;
}

int COnSemiIndiv::PregenerateProgram()
{
	BYTE * pIOs = new BYTE[4];
	pIOs[0] = 0x00;
	pIOs[1] = 0x01;
	pIOs[2] = 0x02;
	pIOs[3] = 0x03;
	AppendInstrBlock(INSTR_COUNT - 1, 0, pIOs);
/*
	pIOs = new BYTE[1];
	pIOs[0] = 0x01;
	AppendInstrBlock(INSTR_COUNT - 11, 0, pIOs);

	pIOs = new BYTE[1];
	pIOs[0] = 0x02;
	AppendInstrBlock(INSTR_COUNT - 11, 0, pIOs);

	pIOs = new BYTE[1];
	pIOs[0] = 0x03;
	AppendInstrBlock(INSTR_COUNT - 11, 0, pIOs);

	pIOs = new BYTE[4];
	pIOs[0] = 0x01;
	pIOs[1] = 0x00;
	pIOs[2] = 0x80;
	pIOs[3] = 0x00;
	AppendInstrBlock(INSTR_COUNT - 4, 0, pIOs);

	AppendInstrBlock(33, 0, NULL);*/

	return 1;
}

void COnSemiIndiv::GenerateRandomHWTopology()
{
	CIndividual::GenerateRandomHWTopology();
	return;
}

void COnSemiIndiv::PrintProgram()
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

double COnSemiIndiv::EvaluateSpeed(UINT nExecTime, UINT nMaxTime)
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

bool CMaximumFramework::EvaluateStopCondition(UINT nGeneration)
{
	static bool bPartialPrinted;
	if (nGeneration == 1)
		bPartialPrinted = false;

	for (UINT nBest = 0; nBest < m_strEvoParams.nEliteSize; nBest++) {
		//if (nGeneration == 1)
		//	m_pBestIndivs[nBest]->EvaluateFitness(m_strEvoParams.strSimulParams);
		if (m_pBestIndivs[nBest]->GetFitness(eResultsFitness) >= 90) {
			if (true) {
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

void CMaximumFramework::ResetAfterEval()
{
	for (int i = 0; i < 4; ++i)
		nActIn[i] = i;
}

bool CMaximumFramework::IsNextInputSet()
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
