#include "sqrt.h"
#include <ctime>

#ifdef _DEBUG
	#define new MYDEBUG_NEW
#endif

int nInputNum = 0;

void test(double fCross);

int main(int argc, char *argv[]) {
	if (argc == 2) {
		double fCross = atof(argv[1]);
		test(fCross);
	}
	else
		test(0.1);
	//system("PAUSE");
#ifdef _DEBUG
	_CrtDumpMemoryLeaks();
#endif
}

REGISTER_TYPE inCallback(UINT nIndex, UINT time)
{
	REGISTER_TYPE inputs[5] = { 12, 15, 14, 11, 17 };
	return inputs[nInputNum];
}

void outCallback(UINT nIndex, UINT nTime, REGISTER_TYPE val)
{
	if (nInputNum < 4)
		nInputNum++;
}

void test(double fCross)
{
	CAdder * pAddMod1 = new CAdder();
	CAdder * pAddMod2 = new CAdder();
	CAdder * pAddMod3 = new CAdder();
	CMultiplier * pMultMod2 = new CMultiplier();
	
	char xml[] = "\
		<environment>\
  <inputs>\
    <input index=\"0\">\
      <change time=\"0\" value=\"17\" />\
    </input>\
  </inputs>\
  <outputs>\
    <output index=\"0\">\
      <change time=\"0\" value=\"289\">\
    </output>\
  </outputs>\
</environment>";

	
	//CDynamicTimeline * pOutputs = new CDynamicTimeline[env.GetOutputsCount()];

	BYTE instLOAD1[] = {0x90, 0x00, 0x00, 0x00};
	BYTE instLOAD2[] = {0x91, 0x00, 0x00, 0x00};
	BYTE instLOAD3[] = {0x92, 0x00, 0x00, 0x00};
	///////////////////////////////////////
	// ADD instructions
	///////////////////////////////////////
	// Module 1
	BYTE instADD[] = {0x00, 0x00, 0x00, 0x01};
	BYTE iosADD[] = {0xFF, 0xFF, 0x80, 0xFF}; // ADD instr
	BYTE instSUB[] = {0x00, 0x00, 0x00, 0x01};
	BYTE iosSUB[] = {0xFF, 0xFF, 0x81, 0xFF}; // SUB instr
	BYTE instINC[] = {0x00, 0x00, 0x00, 0x01};
	BYTE iosINC[] = {0xFF, 0x80, 0x82, 0xFF}; // INC instr
	BYTE instDEC[] = {0x00, 0x00, 0x00, 0x01};
	BYTE iosDEC[] = {0xFF, 0x80, 0x83, 0xFF}; // DEC isntr

	// Module 2
	BYTE instMUL[] = {0x00, 0x00, 0x00, 0x02};
	BYTE iosMUL[] = {0xFF, 0xFF, 0xFF}; // ADD r1,r2 -> r1

	// MOV instructions
	BYTE instMOVr1[] = {0x80, 0x00, 0x00, 0x00};
	BYTE instMOVr2[] = {0x81, 0x00, 0x00, 0x00};
	BYTE instMOVr3[] = {0x82, 0x00, 0x00, 0x00};

	BYTE instJMP[] = {0x10, 0x00, 0x00, 0x00};
	BYTE instJZ[] = {0x20, 0x00, 0x00, 0x01}; // JUMP if module 1 has zero flag
	BYTE instJZ2[] = {0x20, 0x00, 0x00, 0x02}; // JUMP if module 2 has zero flag
	BYTE instJNZ[] = {0x40, 0x00, 0x00, 0x01}; // JUMP if module 1 doesn't have zero flag
	BYTE instJNZ2[] = {0x40, 0x00, 0x00, 0x02}; // JUMP if module 2 doesn't have zero flag
	BYTE instJNS[] = {0x50, 0x00, 0x00, 0x01}; // JUMP if module 1 doesn't have zero flag
	BYTE instINr1[] = {0xA0, 0x00, 0x00, 0x00};
	BYTE instINr2[] = {0xA1, 0x00, 0x00, 0x00};
	BYTE instINr3[] = {0xA2, 0x00, 0x00, 0x00};
	BYTE instrOUTr3[] = {0xC2, 0x00, 0x00, 0x00};
	BYTE instrOUTr2[] = {0xC1, 0x00, 0x00, 0x00};
	BYTE instrOUTr1[] = {0xC0, 0x00, 0x00, 0x00};
	BYTE instrNOP[] = { 0x00, 0x00, 0x00, 0x00 };
	BYTE instrRST[] = { 0xFF, 0xFF, 0xFF, 0xFF };
#define INSTR_COUNT 22

	CInstruction* arrInstrSet[INSTR_COUNT] = { new CInstruction(4, instLOAD1, true, std::string("LOAD r1, ")),
									new CInstruction(4, instLOAD2, true, std::string("LOAD r2, ")),
									new CInstruction(4, instLOAD3, true, std::string("LOAD r3, ")),
									new CInstruction(8, instADD, false, std::string("ADD "), 1, iosADD, 4),
									new CInstruction(8, instSUB, false, std::string("SUB "), 1, iosSUB, 4),
									//new CInstruction(8, instINC, false, std::string("INC "), 1, iosINC, 4),
									new CInstruction(8, instDEC, false, std::string("DEC "), 1, iosDEC, 4),
									new CInstruction(7, instMUL, false, std::string("MUL "), 1, iosMUL, 3),
									/*new CInstruction(8, instADDb, false, std::string("ADDb r1, r2 -> r1")),
									new CInstruction(8, instADDb2, false, std::string("ADDb r1, r2 -> r2")),
									new CInstruction(8, instADDb3, false, std::string("ADDb r1, r2 -> r3")),
									new CInstruction(8, instADDb4, false, std::string("ADDb r1, r3 -> r1")),
									new CInstruction(8, instADDb5, false, std::string("ADDb r1, r3 -> r2")),
									new CInstruction(8, instADDb6, false, std::string("ADDb r1, r3 -> r3")),
									new CInstruction(8, instADDb7, false, std::string("ADDb r2, r3 -> r1")),
									new CInstruction(8, instADDb8, false, std::string("ADDb r2, r3 -> r2")),
									new CInstruction(8, instADDb9, false, std::string("ADDb r2, r3 -> r3")), //20*/
									new CInstruction(4, instMOVr1, true, std::string("MOV r1, r")),
									new CInstruction(4, instMOVr2, true, std::string("MOV r2, r")),
									new CInstruction(4, instMOVr3, true, std::string("MOV r3, r")),
									new CInstruction(4, instrOUTr1, true, std::string("OUT r1 -> out")),
									new CInstruction(4, instrOUTr2, true, std::string("OUT r2 -> out")),
									/*new CInstruction(8, instDEC1b, false, std::string("DECb r1")),
									new CInstruction(8, instDEC2b, false, std::string("DECb r2")),
									new CInstruction(8, instDEC3b, false, std::string("DECb r3")),*/
									new CInstruction(4, instINr1, true, std::string("IN r1 <- in")),
									new CInstruction(4, instINr2, true, std::string("IN r2 <- in")),
									new CInstruction(4, instINr3, true, std::string("IN r3 <- in")),
									//new CInstruction(4, instJMP, true, std::string("JMP ")),
									new CInstruction(4, instJZ, true, std::string("JZ ")),
									new CInstruction(4, instJZ2, true, std::string("JZ2 ")),
									//new CInstruction(4, instJZ2, true, std::string("JZb ")),
									new CInstruction(4, instrOUTr3, true, std::string("OUT r3 -> out")),
									new CInstruction(4, instJNZ, true, std::string("JNZ ")),
									new CInstruction(4, instJNZ2, true, std::string("JNZ2 ")),
									new CInstruction(4, instrNOP, false, std::string("NOP")),
									new CInstruction(4, instrRST, false, std::string("RST"))/*
									new CInstruction(4, instJNZ2, true, std::string("JNZb ")),
									CInstruction(8, instDEC, false),
									//CInstruction(4, instJZ, true),
									//CInstruction(4, instJNS, true),
									CInstruction(4, instLOAD3, true),
									CInstruction(4, instMOVr1, true),
									CInstruction(4, instrINr1, true),
									CInstruction(4, instrINr2, true),
									CInstruction(4, instrOUTr3, true)*/
	};
	
	CModule * pModules[] = { pAddMod1, pAddMod2, pAddMod3, pMultMod2 };
	strArchitectureParams archParams;
	archParams.nMaxModulesCount = 4;
	archParams.nMaxRegistersCount = 3;
	archParams.pAllowedInstructions = arrInstrSet;
	archParams.pAllowedModules = pModules;
	CEnvironment * pEnv = new CEnvironment(inCallback, outCallback, 1, 1);

	bool bDynamic = false;
	srand((UINT) time(NULL));
	//for (UINT prob = 8; prob < 11; prob++) {
		for (UINT i = 0; i < 100; i++) {
			CArchitecture * pArch = new CArchitecture(3, (REGISTER_TYPE) 0xFFFFFFFF, 4, pModules, INSTR_COUNT, arrInstrSet, &archParams, bDynamic);
			pArch->SetModuleUsed(1, false);
			int nPopSize = 5;
			strEvolutionParams evoParams(nPopSize, 1000000 / nPopSize, 0, 0.7, 1);
			evoParams.strSimulParams.nMaxLogicTime = 8000;
			strIndividualParams indivParams(15,30);
			evoParams.strMutParams.fProbs[infoMutationReg] = 0;
			evoParams.strMutParams.fProbs[infoMutationModule] = 0;
			evoParams.fCrossoverProbability = 0;
			CSqrtFramework * pFramework = new CSqrtFramework(pArch, pEnv, evoParams, indivParams, false);
			pFramework->StartEvolution();
			delete pFramework;
			delete pArch;
		}
	//}
	
	delete pEnv;

	delete pAddMod1;
	delete pMultMod2;
	
	for (UINT i = 0; i < INSTR_COUNT; i++)
		delete arrInstrSet[i];
	
	

	/*CIndividual indiv(&tmpArch, &env);
	indiv.GenerateNewProgram(2000);
	indiv.EvaluateFitness();*/

	/*CIndividual indiv(&tmpArch, &env);
	indiv.GenerateRandomProgram(indivParams);
	indiv.AppendInstrBlock(7, 2);	// LOAD r3, 2
	indiv.AppendInstrBlock(1, 4);	// LOAD r2, 4
	indiv.AppendInstrBlock(2);		// ADD r2, r3 -> r3
	indiv.AppendInstrBlock(11, 0);	// OUT r3 -> o0
	indiv.AppendInstrBlock(3, -2);
	indiv.EvaluateFitness();*/

	BYTE program[] = {	/*0x00, 0x04, 0x00, 0x00, 0x00,*/ // load r1, 4
						0x09, 0x00, 0x00, 0x00, 0x00, // IN r1
						0x04, 0x00, 0x00, 0x83, 0x00,
						/*0x01, 0x02, 0x00, 0x00, 0x00,*/ // load r2, 2
						0x0A, 0x03, 0x00, 0x00, 0x00, // IN r2
						0x07, 0x00, 0x00, 0x00, 0x00, // load r3, 0
						0x02, /*0x07, 0x00, 0x00, 0x00,*/ // instADD
						/*0x03, 0x05, 0x00, 0x00, 0x00, // JMP to DEC
						0x00, 0x04, 0x00, 0x00, 0x00,*/
						0x04, 0x00, 0x00, 0x83, 0x00, // DEC r1
						0x06, 0xF5, 0xFF, 0xFF, 0xFF,
						0x0B, 0x00, 0x00, 0x00, 0x00, // OUT r3
						0x03, 0xD7, 0xFF, 0xFF, 0xFF
						//0x08, 0x02, 0x00, 0x00, 0x00
	};

	//tmpArch.ExecuteProgram((void*) program, sizeof(program), &env);
	return;
}

double CSqrtIndiv::EvaluateOutputs(CEnvironment * pEnv)
{
	int nInputs[5] = { 12, 15, 14, 11, 17 };
	int nExpectedOrder[5] = { 144, 225, 196, 121, 289 };
	bool bFound[5] = { false, false, false, false, false };
	double dFitness = 0;
	nInputNum = 0;
	
	CDynamicTimeline * pTimeline = pEnv->GetOutput(0);
	strDynamicTimeVal * pActOut = pTimeline->GetFirst();
	if (!pActOut)
		return dFitness;

	strDynamicTimeVal * pTempOut = pActOut;
	UINT nOutCount = 0;
	while (pTempOut) {
		nOutCount++;
		if (nOutCount > 30)
			break;
		pTempOut = pTempOut->pSucc;
	}
	if (nOutCount > 30 || pActOut->time < 400)
		return dFitness;

	dFitness = 0.5;
	UINT i=0;
	
	while (pActOut) {
		for (UINT j = 0; j < 5; j++) {
			REGISTER_TYPE diff = abs(nExpectedOrder[j] - pActOut->val);
			if (diff == 0 && !bFound[j]) {
				if (j == i)
					dFitness += 1;
				else
					dFitness += 0.1;
				bFound[j] = true;
				break;
			}
			else if (diff != 0) {
				dFitness += (0.01/(diff*diff));
			}
			/*else if (diff < 10) {
				dFitness += (0.1 - diff / (double) 100);
				break;
			}*/
		}
		pActOut = pActOut->pSucc;
		i++;
	}

	nInputNum = 0;
	return dFitness;
}

void CSqrtIndiv::PrintProgram()
{
	for (UINT i = 0; i < m_pArch->GetRegistersCount(); i++)
		printf("reg%d: %X\n", i, m_pArch->GetRegisterMask(i));

	for (CInstrBlock * pActInstr = m_pFirstInstr; pActInstr != NULL; pActInstr = pActInstr->GetNext()) {
		for (UINT nActInstr = 0; nActInstr < pActInstr->GetInstrCount(); nActInstr++) {
			UINT nInstrNo = pActInstr->GetInstrNo(nActInstr);
			CInstruction * pInstr = m_pArch->GetInstruction(nInstrNo);
			if (pInstr->IsParametric())
				printf("%s %d", pInstr->GetDescription().c_str(), pActInstr->GetParam(nActInstr));
			else
				printf("%s", pInstr->GetDescription().c_str());
			
			if (pInstr->GetInsOuts() != NULL) {
				BYTE * pIOs = pActInstr->GetInstrIOs(nActInstr);
				BYTE * pInstrIOs = pInstr->GetInsOuts();
				for (UINT nIO = 0; nIO < pInstr->GetInsOutsCount(); nIO++) {
					if (pInstrIOs[nIO] != 0xFF && ((pInstrIOs[nIO] & 0x80) != 0))
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

bool CSqrtFramework::EvaluateStopCondition(UINT nGeneration)
{
	for (UINT nBest = 0; nBest < m_strEvoParams.nEliteSize; nBest++) {
		if (m_pBestIndivs[nBest]->GetFitness().dResultsFitness > 5) {
			if (m_bPrintInfo) {
				m_pBestIndivs[nBest]->PrintProgram();
				system("PAUSE");
				m_pBestIndivs[0]->EvaluateFitness(m_strEvoParams.strSimulParams);
			}
			return true;
		}
	}
	
	return false;
}
