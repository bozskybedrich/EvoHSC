// Goldschmidt.cpp : Defines the entry point for the console application.
//
#include "Goldschmidt.h"
#include <time.h>
#include <iostream>
#include <math.h>
#include <stdio.h>
#include <string.h>

#define SAMPLES_COUNT	10

int nInputNum[2] = { 0, 0 };
int nInputsProcessed[2] = { 0, 0 };
REGISTER_TYPE inputs[2][SAMPLES_COUNT];
REGISTER_TYPE tmpOuts[2][SAMPLES_COUNT];

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

REGISTER_TYPE inCallback(UINT nIndex, UINT time)
{
	if (nIndex > 1)
		nIndex = 0;

	nInputsProcessed[nIndex]++;
	return tmpOuts[nIndex][nInputNum[nIndex] / SAMPLES_COUNT];
}

void outCallback(UINT nIndex, UINT nTime, REGISTER_TYPE val)
{
	if (nIndex > 1)
		nIndex = 0;

	if (nInputNum[nIndex] < (SAMPLES_COUNT*SAMPLES_COUNT) - 1) {
		tmpOuts[nIndex][nInputNum[nIndex] / SAMPLES_COUNT] = val;
		nInputNum[nIndex]++;
	}
}

void ResetAfterEval()
{
	for (int nNum = 0; nNum < 2; nNum++) {
		for (int i = 0; i < 10; i++) {
			tmpOuts[nNum][i] = inputs[nNum][i];
		}

		nInputsProcessed[nNum] = 0;
		nInputNum[nNum] = 0;
	}
}

void test(double fCross, double fMut)
{
	srand((UINT) time(NULL));
	for (int i = 0; i < 10; i++) {
		inputs[0][i] = (rand() % 2000) / 100.0 + 20.0;
		inputs[1][i] = (rand() % 198 + 1) / 1000.0;// + 0.8;

		std::cout << "Input #" << i << " " << inputs[0][i] << " " << inputs[1][i] << std::endl;
	}

	CAdder * pAddMod1 = new CAdder();
	CAdder * pAddMod2 = new CAdder();
	CMultiplier * pMultMod1 = new CMultiplier();
	CMultiplier * pMultMod2 = new CMultiplier();
	
	CEnvironment * pEnv = new CEnvironment(inCallback, outCallback, 2, 2, ResetAfterEval);
	
	BYTE instLOAD1[] = {0x90, 0x00, 0x00, 0x00};
	BYTE instLOAD2[] = {0x91, 0x00, 0x00, 0x00};
	BYTE instLOAD3[] = {0x92, 0x00, 0x00, 0x00};
	BYTE instLOAD4[] = {0x93, 0x00, 0x00, 0x00};
	
	BYTE instMod1[] = {0x00, 0x00, 0x00, 0x01};
	BYTE instMod2[] = {0x00, 0x00, 0x00, 0x02};
	BYTE instMod3[] = {0x00, 0x00, 0x00, 0x04};
	BYTE instMod4[] = {0x00, 0x00, 0x00, 0x08};
	BYTE instMod1_2[] = {0x00, 0x00, 0x00, 0x03};
	BYTE instMod1_3[] = {0x00, 0x00, 0x00, 0x05};
	BYTE instMod1_4[] = {0x00, 0x00, 0x00, 0x09};
	BYTE instMod2_3[] = {0x00, 0x00, 0x00, 0x06};
	BYTE instMod2_4[] = {0x00, 0x00, 0x00, 0x0A};
	BYTE instMod3_4[] = {0x00, 0x00, 0x00, 0x0C};
	BYTE instMod1_2_3[] = {0x00, 0x00, 0x00, 0x07};
	BYTE instMod1_2_4[] = {0x00, 0x00, 0x00, 0x0B};
	BYTE instMod1_3_4[] = {0x00, 0x00, 0x00, 0x0D};
	BYTE instMod2_3_4[] = {0x00, 0x00, 0x00, 0x0E};
	BYTE instModAll[] = {0x00, 0x00, 0x00, 0x0F};
	BYTE iosModAdd[] = {0xFF, 0xFF, 0xC4, 0xFF};
	BYTE iosModMul[] = {0xFF, 0xFF, 0xFF};
	BYTE iosModAddAdd[] = {0xFF, 0xFF, 0xC4, 0xFF, 0xFF, 0xFF, 0xC4, 0xFF};
	BYTE iosModAddMul[] = {0xFF, 0xFF, 0xC4, 0xFF, 0xFF, 0xFF, 0xFF};
	BYTE iosModMulMul[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
	BYTE iosModAddAddMul[] = {0xFF, 0xFF, 0xC4, 0xFF, 0xFF, 0xFF, 0xC4, 0xFF, 0xFF, 0xFF, 0xFF};
	BYTE iosModAddMulMul[] = {0xFF, 0xFF, 0xC4, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
	BYTE iosModAll[] = {0xFF, 0xFF, 0xC4, 0xFF, 0xFF, 0xFF, 0xC4, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
	
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

	BYTE instrINr1[] = {0xA0, 0x00, 0x00, 0x00};
	BYTE instrINr2[] = {0xA1, 0x00, 0x00, 0x00};
	BYTE instrINr3[] = {0xA2, 0x00, 0x00, 0x00};
	BYTE instrINr4[] = {0xA3, 0x00, 0x00, 0x00};
	BYTE instrOUTr4[] = {0xC3, 0x00, 0x00, 0x00};
	BYTE instrOUTr3[] = {0xC2, 0x00, 0x00, 0x00};
	BYTE instrOUTr2[] = {0xC1, 0x00, 0x00, 0x00};
	BYTE instrOUTr1[] = {0xC0, 0x00, 0x00, 0x00};
	BYTE instrNOP[] = { 0x00, 0x00, 0x00, 0x00 };
	BYTE instrRST[] = { 0xFF, 0xFF, 0xFF, 0xFF };
#define INSTR_COUNT 49

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

									new CInstruction(4, instrINr1, true, std::string("INr1")),
									new CInstruction(4, instrINr2, true, std::string("INr2")),
									new CInstruction(4, instrINr3, true, std::string("INr3")),
									new CInstruction(4, instrINr4, true, std::string("INr4")),
									
									new CInstruction(4, instrOUTr1, true, std::string("OUTr1")),
									new CInstruction(4, instrOUTr2, true, std::string("OUTr2")),
									new CInstruction(4, instrOUTr3, true, std::string("OUTr3")),
									new CInstruction(4, instrOUTr4, true, std::string("OUTr4")),

									new CInstruction(4, instrNOP, false, std::string("NOP")),
									new CInstruction(4, instrRST, false, std::string("RST")),
									
									new CInstruction(4, instMod1, false, std::string("MOD1"), 1, iosModAdd, 4),
									new CInstruction(4, instMod2, false, std::string("MOD2"), 1, iosModAdd, 4),
									new CInstruction(4, instMod3, false, std::string("MOD3"), 1, iosModMul, 3),
									new CInstruction(4, instMod4, false, std::string("MOD4"), 1, iosModMul, 3),
									new CInstruction(4, instMod1_2, false, std::string("MOD1_2"), 1, iosModAddAdd, 8),
									new CInstruction(4, instMod1_3, false, std::string("MOD1_3"), 1, iosModAddMul, 7),
									new CInstruction(4, instMod1_4, false, std::string("MOD1_4"), 1, iosModAddMul, 7),
									new CInstruction(4, instMod2_3, false, std::string("MOD2_3"), 1, iosModAddMul, 7),
									new CInstruction(4, instMod2_4, false, std::string("MOD2_4"), 1, iosModAddMul, 7),
									new CInstruction(4, instMod3_4, false, std::string("MOD3_4"), 1, iosModMulMul, 6),
									new CInstruction(4, instMod1_2_3, false, std::string("MOD1_2_3"), 1, iosModAddAddMul, 11),
									new CInstruction(4, instMod1_2_4, false, std::string("MOD1_2_4"), 1, iosModAddAddMul, 11),
									new CInstruction(4, instMod1_3_4, false, std::string("MOD1_3_4"), 1, iosModAddMulMul, 11),
									new CInstruction(4, instMod2_3_4, false, std::string("MOD2_3_4"), 1, iosModAddMulMul, 10),
									new CInstruction(4, instModAll, false, std::string("MOD_ALL"), 1, iosModAll, 14)
	};
	
	CModule * pModules[] = { pAddMod1, pAddMod2, pMultMod1, pMultMod2 };
	strArchitectureParams archParams;
	archParams.nMaxModulesCount = 4;
	archParams.nMaxRegistersCount = 4;
	archParams.pAllowedInstructions = arrInstrSet;
	archParams.pAllowedModules = pModules;
	
	bool bDynamic = true;
	for (double probCros = 0; probCros <= 1.0; probCros+=0.1) {
		for (double probMut = 0; probMut <= 1.0; probMut+=0.1) {
			for (UINT i = 0; i < 50; i++) {
				CArchitecture * pArch = new CArchitecture(4, (REGISTER_TYPE) 0xFFFFFFFF, 4, pModules, INSTR_COUNT, arrInstrSet, &archParams, bDynamic);
				strArchitectureSettings settings(3);
				pArch->SetArchSettings(&settings);

				strEvolutionParams evoParams(50, 100000, 0.05, 0.7,/*fCross, 0.1*prob,*/
					1);
				evoParams.strMutParams.fProbs[infoMutationReg] = 0;
				evoParams.strMutParams.fProbs[infoMutationModule] = 0;

				evoParams.strSimulParams.nMaxLogicTime = 15000;
				
				strIndividualParams indivParams(10,10);
				CGoldschmidtFramework * pFramework = new CGoldschmidtFramework(pArch, pEnv, evoParams, indivParams);
				pFramework->StartEvolution();
				delete pFramework;
				delete pArch;
			}
		}
	}
	
	delete pEnv;

	delete pAddMod1;
	delete pAddMod2;
	delete pMultMod1;
	delete pMultMod2;
	
	for (UINT i = 0; i < INSTR_COUNT; i++)
		delete arrInstrSet[i];

	//tmpArch.ExecuteProgram((void*) program, sizeof(program), &env);
	return;
}

double CGoldschmidtIndiv::EvaluateOutputs(CEnvironment * pEnv)
{
	REGISTER_TYPE expectedOrder[10];
	for (int i = 0; i < 10; i++)
		expectedOrder[i] = inputs[0][i] / inputs[1][i];

	UINT nOutsCount[2];

	double dFitness = 0;
	double totalFitness[2];
	
	for (int nOut = 0; nOut < 2; nOut++) {
		totalFitness[nOut] = -1;
		REGISTER_TYPE pLastOut = 0;

		CDynamicTimeline * pTimeline = pEnv->GetOutput(nOut);
		int i = 0;
		if (pTimeline->GetFirst())
			totalFitness[nOut] = 0;

		nOutsCount[nOut] = 0;

		for (strDynamicTimeVal * pActOut = pTimeline->GetFirst(); pActOut && i < 100; i++) {
			nOutsCount[nOut]++;
			if (i % 10 == 0)
				pLastOut = inputs[0][i / 10];

			REGISTER_TYPE dActDiff = (REGISTER_TYPE) fabs(pActOut->val - expectedOrder[i / 10]);
			REGISTER_TYPE dLastDiff = (REGISTER_TYPE) fabs(pLastOut - expectedOrder[i / 10]);
		
			if (dLastDiff > 0)
				dFitness = (dActDiff / dLastDiff);
			else
				dFitness = 0;
		
			pLastOut = pActOut->val;

			totalFitness[nOut] += dFitness;

			if (pActOut->pSucc)
				pActOut = pActOut->pSucc;
			else
				nOutsCount[nOut]--;
		}

		if (totalFitness[nOut] > 1)
			totalFitness[nOut] = 1000*(1 / totalFitness[nOut]);
		else if (totalFitness[nOut] = -1)
			totalFitness[nOut] = 0;
		else
			totalFitness[nOut] = 100;
	}

	/*if (nOutsCount[0] < 100 || nOutsCount[1] < 100 || nInputsProcessed[0] < 99 || nInputsProcessed[1] < 99) {
		totalFitness[0] = 0;
		totalFitness[1] = 0;
	}*/

	pEnv->ResetAfterEval();
	double dFinalFitness = totalFitness[0] > totalFitness[1] ? totalFitness[0] : totalFitness[1];
	
	return dFinalFitness;
}

void CGoldschmidtIndiv::GenerateRandomProgram()
{
	CIndividual::GenerateRandomProgram();
	return;

	/*m_pIndivParams->nMaxProgramSize--;
	__super::GenerateRandomProgram();
	AppendInstrBlock(8, -20, NULL);
	m_pIndivParams->nMaxProgramSize++;
	return;*/

	AppendInstrBlock(24, 0, NULL);
	AppendInstrBlock(25, 1, NULL);
	AppendInstrBlock(2, 0, NULL);

	BYTE incIos[] = { 0x02, 0xFF, 0x82, 0xFF, 0x04, 0xFF, 0x82, 0x02 };
	BYTE * compIOs = new BYTE[8];
	memcpy(compIOs, incIos, 8);
	AppendInstrBlock(38, 0, compIOs);

	BYTE mulIos[] = { 0x02, 0x01, 0x81, 0xFF, 0x04, 0x00, 0x00, 0x04, 0x01, 0x01 };
	compIOs = new BYTE[11];
	memcpy(compIOs, mulIos, 11);
	AppendInstrBlock(46, 0, compIOs);

	AppendInstrBlock(28, 0, NULL);
	AppendInstrBlock(29, 1, NULL);

	AppendInstrBlock(12, -20, NULL);
	
	AppendInstrBlock(32, 0, NULL);
	AppendInstrBlock(32, 0, NULL);
	AppendInstrBlock(32, 0, NULL);
	AppendInstrBlock(32, 0, NULL);
	AppendInstrBlock(32, 0, NULL);
	AppendInstrBlock(32, 0, NULL);
	AppendInstrBlock(32, 0, NULL);

	return;
}

void CGoldschmidtIndiv::GenerateRandomHWTopology()
{
	CIndividual::GenerateRandomHWTopology();
	return;

	UINT nModCount = m_pArch->GetModulesCount();
	m_pArchSettings = new strArchitectureSettings(nModCount);

	for (UINT nActMod = 0; nActMod < nModCount; nActMod++) {
		m_pArchSettings->pModulesOrder[nActMod] = nActMod;
	}
}

void CGoldschmidtIndiv::PrintProgram()
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

bool CGoldschmidtFramework::EvaluateStopCondition(UINT nGeneration)
{
	static bool bPartialPrinted;
	if (nGeneration == 1)
		bPartialPrinted = false;

	for (UINT nBest = 0; nBest < m_strEvoParams.nEliteSize; nBest++) {
		//if (nGeneration == 1)
		//	m_pBestIndivs[nBest]->EvaluateFitness(m_strEvoParams.strSimulParams);
		if (m_pBestIndivs[nBest]->GetFitness(eResultsFitness) >= 15) {
			if (m_bPrintInfo) {
				m_pBestIndivs[nBest]->PrintProgram();
				m_strEvoParams.strSimulParams.bPrintInfo = true;
				m_pBestIndivs[nBest]->EvaluateFitness(m_strEvoParams.strSimulParams);
				m_strEvoParams.strSimulParams.bPrintInfo = false;
				system("PAUSE");
			}
			//if (m_pBestIndivs[nBest]->GetFitness().dAreaFitness >= 182)
				//m_pBestIndivs[nBest]->PrintProgram();
				//system("PAUSE");
				return true;
			/*else if (!bPartialPrinted) {
				printf("Partial solution on %dth generation\n", nGeneration);
				bPartialPrinted = true;
			}*/
		}
	}
	
	return false;
}
