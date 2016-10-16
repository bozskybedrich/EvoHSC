#include "symreg.h"

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
	//system("PAUSE");
#ifdef _DEBUG
	_CrtDumpMemoryLeaks();
#endif
}

REGISTER_TYPE inCallback(UINT nIndex, UINT time)
{
	REGISTER_TYPE inputs[10] = { 4, 5, 6, 7, 8, 9, 10, 11, 12, 13 };
	return inputs[nInputNum];
}

void outCallback(UINT nIndex, UINT nTime, REGISTER_TYPE val)
{
	if (nInputNum < 9)
		nInputNum++;
}

void test(double fCross)
{
	CAdder * pAddMod1 = new CAdder();
	CMultiplier * pMultMod1 = new CMultiplier();
	
	char xml[] = "\
		<environment>\
  <inputs>\
    <input index=\"0\">\
      <change time=\"0\" value=\"2\" />\
    </input>\
    <input index=\"3\">\
      <change time=\"0\" value=\"2\" />\
    </input>\
  </inputs>\
  <outputs>\
    <output index=\"0\">\
      <change time=\"330\" value=\"34\">\
    </output>\
  </outputs>\
</environment>";

	//CEnvironment * pEnv = new CEnvironment(xml);
	CEnvironment * pEnv = new CEnvironment(inCallback, outCallback, 1, 1);
	
	//CDynamicTimeline * pOutputs = new CDynamicTimeline[env.GetOutputsCount()];
	BYTE instLOAD1[] = {0x90, 0x00, 0x00, 0x00};
	BYTE instLOAD2[] = {0x91, 0x00, 0x00, 0x00};
	BYTE instLOAD3[] = {0x92, 0x00, 0x00, 0x00};
	///////////////////////////////////////
	// ADD instructions
	///////////////////////////////////////
	// Module 1
	BYTE instADD[] = {0x00, 0x00, 0x00, 0x01};
	BYTE iosADD[] = {0xFF, 0xFF, 0x80, 0xFF}; // ADD r1,r2 -> r1
	BYTE instSUB[] = {0x00, 0x00, 0x00, 0x01};
	BYTE iosSUB[] = {0xFF, 0xFF, 0x81, 0xFF}; // ADD r1,r2 -> r1
	BYTE instMUL[] = {0x00, 0x00, 0x00, 0x02};
	BYTE iosMUL[] = {0xFF, 0xFF, 0xFF}; // ADD r1,r2 -> r1
	/*BYTE instADD2[] = {0x00, 0x00, 0x00, 0x01};
	BYTE iosADD2[] = {0xC0, 0xC1, 0x80, 0xC1}; // ADD r1,r2 -> r2
	BYTE instADD3[] = {0x00, 0x00, 0x00, 0x01};
	BYTE iosADD3[] = {0xC0, 0xC1, 0x80, 0xC2}; // ADD r1,r2 -> r3
	BYTE instADD4[] = {0x00, 0x00, 0x00, 0x01};
	BYTE iosADD4[] = {0xC0, 0xC2, 0x80, 0xC0}; // ADD r1,r3 -> r1
	BYTE instADD5[] = {0x00, 0x00, 0x00, 0x01};
	BYTE iosADD5[] = {0xC0, 0xC2, 0x80, 0xC1}; // ADD r1,r3 -> r2
	BYTE instADD6[] = {0x00, 0x00, 0x00, 0x01};
	BYTE iosADD6[] = {0xC0, 0xC2, 0x80, 0xC2}; // ADD r1,r3 -> r3
	BYTE instADD7[] = {0x00, 0x00, 0x00, 0x01};
	BYTE iosADD7[] = {0xC1, 0xC2, 0x80, 0xC0}; // ADD r2,r3 -> r1
	BYTE instADD8[] = {0x00, 0x00, 0x00, 0x01};
	BYTE iosADD8[] = {0xC1, 0xC2, 0x80, 0xC1}; // ADD r2,r3 -> r2
	BYTE instADD9[] = {0x00, 0x00, 0x00, 0x01};
	BYTE iosADD9[] = {0xC1, 0xC2, 0x80, 0xC2}; // ADD r2,r3 -> r3*/

	// Module 2
	BYTE instADDb[] = {0x00, 0x00, 0x00, 0x02};
	BYTE iosADDb[] = {0xFF, 0xFF, 0x80, 0xFF}; // ADD r1,r2 -> r1
	/*BYTE instADDb2[] = {0x00, 0x00, 0x00, 0x02};
	BYTE iosADDb2[] = {0xC0, 0xC1, 0x80, 0xC1}; // ADD r1,r2 -> r2
	BYTE instADDb3[] = {0x00, 0x00, 0x00, 0x02};
	BYTE iosADDb3[] = {0xC0, 0xC1, 0x80, 0xC2}; // ADD r1,r2 -> r3
	BYTE instADDb4[] = {0x00, 0x00, 0x00, 0x02};
	BYTE iosADDb4[] = {0xC0, 0xC2, 0x80, 0xC0}; // ADD r1,r3 -> r1
	BYTE instADDb5[] = {0x00, 0x00, 0x00, 0x02};
	BYTE iosADDb5[] = {0xC0, 0xC2, 0x80, 0xC1}; // ADD r1,r3 -> r2
	BYTE instADDb6[] = {0x00, 0x00, 0x00, 0x02};
	BYTE iosADDb6[] = {0xC0, 0xC2, 0x80, 0xC2}; // ADD r1,r3 -> r3
	BYTE instADDb7[] = {0x00, 0x00, 0x00, 0x02};
	BYTE iosADDb7[] = {0xC1, 0xC2, 0x80, 0xC0}; // ADD r2,r3 -> r1
	BYTE instADDb8[] = {0x00, 0x00, 0x00, 0x02};
	BYTE iosADDb8[] = {0xC1, 0xC2, 0x80, 0xC1}; // ADD r2,r3 -> r2
	BYTE instADDb9[] = {0x00, 0x00, 0x00, 0x02};
	BYTE iosADDb9[] = {0xC1, 0xC2, 0x80, 0xC2}; // ADD r2,r3 -> r3*/

	// MOV instructions
	BYTE instMOVr1[] = {0x80, 0x00, 0x00, 0x00};
	BYTE instMOVr2[] = {0x81, 0x00, 0x00, 0x00};
	BYTE instMOVr3[] = {0x82, 0x00, 0x00, 0x00};

	BYTE instJMP[] = {0x10, 0x00, 0x00, 0x00};
	BYTE instJZ[] = {0x20, 0x00, 0x00, 0x01}; // JUMP if module 1 has zero flag
	BYTE instJNS[] = {0x50, 0x00, 0x00, 0x01}; // JUMP if module 1 doesn't have zero flag
	BYTE instDEC[] = {0x00, 0x00, 0x00, 0x01};
	BYTE iosDEC[] = {0xC0, 0xC0, 0x83, 0xC0}; // DEC r1
	BYTE instrINr1[] = {0xA0, 0x00, 0x00, 0x00};
	BYTE instrINr2[] = {0xA1, 0x00, 0x00, 0x00};
	BYTE instrOUTr3[] = {0xC2, 0x00, 0x00, 0x00};
	BYTE instrOUTr2[] = {0xC1, 0x00, 0x00, 0x00};
	BYTE instrOUTr1[] = {0xC0, 0x00, 0x00, 0x00};
	BYTE instrNOP[] = { 0x00, 0x00, 0x00, 0x00 };
	BYTE instrRST[] = { 0xFF, 0xFF, 0xFF, 0xFF };
#define INSTR_COUNT 16

	CInstruction* arrInstrSet[INSTR_COUNT] = { new CInstruction(4, instLOAD1, true, std::string("LOAD1")),
									new CInstruction(4, instLOAD2, true, std::string("LOAD2")),
									new CInstruction(4, instLOAD3, true, std::string("LOAD3")),
									new CInstruction(8, instADD, false, std::string("ADD"), 1, iosADD, 4),
									new CInstruction(8, instSUB, false, std::string("SUB"), 1, iosSUB, 4),
									new CInstruction(7, instMUL, false, std::string("MUL"), 1, iosMUL, 3),
									/*new CInstruction(8, instADD2, false, std::string(), 1, iosADD2, 4),
									new CInstruction(8, instADD3, false, std::string(), 1, iosADD3, 4),
									new CInstruction(8, instADD4, false, std::string(), 1, iosADD4, 4),
									new CInstruction(8, instADD5, false, std::string(), 1, iosADD5, 4),
									new CInstruction(8, instADD6, false, std::string(), 1, iosADD6, 4),
									new CInstruction(8, instADD7, false, std::string(), 1, iosADD7, 4),
									new CInstruction(8, instADD8, false, std::string(), 1, iosADD8, 4),
									new CInstruction(8, instADD9, false, std::string(), 1, iosADD9, 4),*/
									//new CInstruction(8, instADDb, false, std::string(), 1, iosADDb, 4),
									/*new CInstruction(8, instADDb2, false, std::string(), 1, iosADDb2, 4),
									new CInstruction(8, instADDb3, false, std::string(), 1, iosADDb3, 4),
									new CInstruction(8, instADDb4, false, std::string(), 1, iosADDb4, 4),
									new CInstruction(8, instADDb5, false, std::string(), 1, iosADDb5, 4),
									new CInstruction(8, instADDb6, false, std::string(), 1, iosADDb6, 4),
									new CInstruction(8, instADDb7, false, std::string(), 1, iosADDb7, 4),
									new CInstruction(8, instADDb8, false, std::string(), 1, iosADDb8, 4),
									new CInstruction(8, instADDb9, false, std::string(), 1, iosADDb9, 4),*/
									new CInstruction(4, instMOVr1, true, std::string("MOVr1")),
									new CInstruction(4, instMOVr2, true, std::string("MOVr2")),
									new CInstruction(4, instMOVr3, true, std::string("MOVr3")),
									new CInstruction(4, instrOUTr1, true, std::string("OUTr1")),
									new CInstruction(4, instrOUTr2, true, std::string("OUTr2")),
									new CInstruction(4, instJMP, true, std::string("JMP")),
									new CInstruction(4, instrINr1, true, std::string("INr1")),
									new CInstruction(4, instrINr2, true, std::string("INr2")),
									new CInstruction(4, instrNOP, false, std::string("NOP")),
									new CInstruction(4, instrRST, false, std::string("RST"))/*,
									CInstruction(8, instDEC, false),
									//CInstruction(4, instJZ, true),
									//CInstruction(4, instJNS, true),
									CInstruction(4, instLOAD3, true),
									CInstruction(4, instMOVr1, true),
									CInstruction(4, instrINr1, true),
									CInstruction(4, instrINr2, true),
									CInstruction(4, instrOUTr3, true)*/
	};
	
	
	CModule * pModules[] = { pAddMod1, pMultMod1 };
	strArchitectureParams archParams;
	archParams.nMaxModulesCount = 2;
	archParams.nMaxRegistersCount = 3;
	archParams.pAllowedInstructions = arrInstrSet;
	archParams.pAllowedModules = pModules;

	bool bDynamic = true;
	//for (UINT prob = 0; prob < 11; prob++) {
		for (UINT i = 0; i < 50; i++) {
			CArchitecture * pArch = new CArchitecture(3, (REGISTER_TYPE) 0xFFFFFFFF, 2, pModules, INSTR_COUNT, arrInstrSet, &archParams, bDynamic);
			strEvolutionParams evoParams(50, 20000, 0, 0.7,/*fCross, 0.1*prob,*/
				1);
			evoParams.strMutParams.fProbs[infoMutationReg] = 0;
			evoParams.strMutParams.fProbs[infoMutationModule] = 0;

			evoParams.strSimulParams.nMaxLogicTime = 1200;
			strIndividualParams indivParams(15,15);
			CSymRegFramework * pFramework = new CSymRegFramework(pArch, pEnv, evoParams, indivParams);
			pFramework->StartEvolution();
			delete pFramework;
			delete pArch;
		}
	//}
	
	delete pEnv;

	delete pAddMod1;
	delete pMultMod1;
	
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

double CSymRegIndiv::EvaluateOutputs(CEnvironment * pEnv)
{
	// y = x^4 - x^3 - 2x^2 + 3x + 4
	int nExpectedOrder[10] = { 3600, 14400, 44100, 112896, 254016, 518400, 980100, 1742400, 2944656, 4769856 }; // x^6-2*x^4+x^2
	//int nExpectedOrder[10] = { 340, 780, 1554, 2800, 4680, 7380, 11110, 16104, 22620, 30940 }; // x^4+x^3+x^2+x
	//int nExpectedOrder[10] = { 4, 2, 22, 112, 344, 814, 1642, 2972, 4972, 7834 };
	//int nExpectedOrder[10] = { 1, 16, 81, 256, 625, 1296, 2401, 4096, 6561, 10000 };
	//int nExpectedOrder[10] = { 1, 4, 9, 16, 25, 36, 49, 64, 81, 100 };
	double dFitness = 0;
	nInputNum = 0;
	
	CDynamicTimeline * pTimeline = pEnv->GetOutput(0);
	strDynamicTimeVal * pActOut = pTimeline->GetFirst();
	for (UINT nActInd = 0; nActInd < 11 && pActOut; nActInd++, pActOut = pActOut->pSucc) {
		if (pActOut->val == nExpectedOrder[nActInd])
			dFitness += 1;
		else {
			double dSquareDiff = abs(pActOut->val*pActOut->val - nExpectedOrder[nActInd]*nExpectedOrder[nActInd]);
			if (dSquareDiff > 0)
				dFitness += 1.0 / dSquareDiff;
		}
	}

	if (dFitness == 0)
		return dFitness;
	else if (dFitness == 1)
		return dFitness;
	else if (dFitness == 2)
		return dFitness;
	else if (dFitness == 3)
		return dFitness;
	else if (dFitness == 4)
		return dFitness;
	else if (dFitness > 8)
		return dFitness;

	nInputNum = 0;
	return dFitness;
}

void CSymRegIndiv::PrintProgram()
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

bool CSymRegFramework::EvaluateStopCondition(UINT nGeneration)
{
	static bool bPartialPrinted;
	if (nGeneration == 1)
		bPartialPrinted = false;

	for (UINT nBest = 0; nBest < m_strEvoParams.nEliteSize; nBest++) {
		if (nGeneration == 1)
			m_pBestIndivs[nBest]->EvaluateFitness(m_strEvoParams.strSimulParams);
		if (m_pBestIndivs[nBest]->GetFitness().dResultsFitness > 9) {
			if (m_bPrintInfo) {
				m_pBestIndivs[nBest]->PrintProgram();
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
