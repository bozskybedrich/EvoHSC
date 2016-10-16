#include "HWtopology.h"
#include <limits>

#ifdef _DEBUG
	#define new MYDEBUG_NEW
#endif

int nInputNum = 0;
//REGISTER_TYPE inputs[10] = { 0.5, 0.55, 0.6, 0.65, 0.7, 0.75, 0.8, 0.85, 0.9, 0.95 };
REGISTER_TYPE inputs[10] = { 0.5, 0.875, 0.6, 0.65, 0.7, 0.75, 0.8, 0.85, 0.9, 0.95 };
REGISTER_TYPE tmpOuts[10] = { 0.5, 0.875, 0.6, 0.65, 0.7, 0.75, 0.8, 0.85, 0.9, 0.95 };

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
	return nIndex == 0 ? inputs[nInputNum / 10] : tmpOuts[nInputNum / 10];
}

void outCallback(UINT nIndex, UINT nTime, REGISTER_TYPE val)
{
	if (nInputNum < 99) {
		tmpOuts[nInputNum / 10] = val;
		nInputNum++;
	}
}

void test(double fCross)
{
	CAdder * pAddMod1 = new CAdder();
	CAdder * pAddMod2 = new CAdder();
	CMultiplier * pMultMod1 = new CMultiplier();
	
	char xml[] = "\
		<environment>\
  <inputs>\
    <input index=\"0\">\
      <change time=\"0\" value=\"1\" />\
    </input>\
	<input index=\"1\">\
      <change time=\"0\" value=\"2\" />\
    </input>\
	<input index=\"2\">\
      <change time=\"0\" value=\"3\" />\
    </input>\
    <input index=\"3\">\
      <change time=\"0\" value=\"4\" />\
    </input>\
  </inputs>\
  <outputs>\
    <output index=\"0\">\
      <change time=\"330\" value=\"34\">\
    </output>\
  </outputs>\
</environment>";

	//CEnvironment * pEnv = new CEnvironment(xml);
	CEnvironment * pEnv = new CEnvironment(inCallback, outCallback, 2, 1);
	
	//CDynamicTimeline * pOutputs = new CDynamicTimeline[env.GetOutputsCount()];
	BYTE instLOAD1[] = {0x90, 0x00, 0x00, 0x00};
	BYTE instLOAD2[] = {0x91, 0x00, 0x00, 0x00};
	BYTE instLOAD3[] = {0x92, 0x00, 0x00, 0x00};
	BYTE instLOAD4[] = {0x93, 0x00, 0x00, 0x00};
	///////////////////////////////////////
	// ADD instructions
	///////////////////////////////////////
	// Module 1
	//BYTE instADD[] = {0x00, 0x00, 0x00, 0x01};
	//BYTE iosADD[] = {0xFF, 0xFF, 0x80, 0xFF}; // ADD r1,r2 -> r1
	//BYTE instSUB[] = {0x00, 0x00, 0x00, 0x01};
	//BYTE iosSUB[] = {0xFF, 0xFF, 0x81, 0xFF}; // ADD r1,r2 -> r1
	//BYTE instMUL[] = {0x00, 0x00, 0x00, 0x02};
	//BYTE iosMUL[] = {0xFF, 0xFF, 0xFF}; // ADD r1,r2 -> r1
	BYTE instMod1[] = {0x00, 0x00, 0x00, 0x01};
	BYTE instMod2[] = {0x00, 0x00, 0x00, 0x02};
	BYTE instMod3[] = {0x00, 0x00, 0x00, 0x04};
	BYTE instMod1_2[] = {0x00, 0x00, 0x00, 0x03};
	BYTE instMod1_3[] = {0x00, 0x00, 0x00, 0x05};
	BYTE instMod2_3[] = {0x00, 0x00, 0x00, 0x06};
	BYTE instModAll[] = {0x00, 0x00, 0x00, 0x07};
	BYTE iosMod1[] = {0xFF, 0xFF, 0xC4, 0xFF};
	BYTE iosMod2[] = {0xFF, 0xFF, 0xC4, 0xFF};
	BYTE iosMod3[] = {0xFF, 0xFF, 0xFF};
	BYTE iosMod1_2[] = {0xFF, 0xFF, 0xC4, 0xFF, 0xFF, 0xFF, 0xC4, 0xFF};
	BYTE iosMod1_3[] = {0xFF, 0xFF, 0xC4, 0xFF, 0xFF, 0xFF, 0xFF};
	BYTE iosMod2_3[] = {0xFF, 0xFF, 0xC4, 0xFF, 0xFF, 0xFF, 0xFF};
	BYTE iosModAll[] = {0xFF, 0xFF, 0xC4, 0xFF, 0xFF, 0xFF, 0xC4, 0xFF, 0xFF, 0xFF, 0xFF}; // ADD r1,r2 -> r1
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
	//BYTE instADDb[] = {0x00, 0x00, 0x00, 0x02};
	//BYTE iosADDb[] = {0xFF, 0xFF, 0x80, 0xFF}; // ADD r1,r2 -> r1
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
	BYTE instJNZ[] = {0x40, 0x00, 0x00, 0x01}; // JUMP if module 1 has zero flag
	BYTE instJNS2[] = {0x50, 0x00, 0x00, 0x02}; // JUMP if module 2 doesn't have zero flag
	BYTE instJNS1[] = {0x50, 0x00, 0x00, 0x01}; // JUMP if module 1 doesn't have zero flag
	BYTE instJS[] = {0x30, 0x00, 0x00, 0x01}; // JUMP if module 1 doesn't have zero flag
	BYTE instDEC[] = {0x00, 0x00, 0x00, 0x01};
	BYTE iosDEC[] = {0xC0, 0xC0, 0x83, 0xC0}; // DEC r1
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
#define INSTR_COUNT 27

	CInstruction* arrInstrSet[INSTR_COUNT] = { 
									new CInstruction(4, instModAll, false, std::string("COMPLEX"), 1, iosModAll, 11),
									new CInstruction(4, instLOAD1, true, std::string("LOADr1")),
									new CInstruction(4, instLOAD4, true, std::string("LOADr4")),
									new CInstruction(4, instMOVr1, true, std::string("MOVr1")),
									new CInstruction(4, instMOVr2, true, std::string("MOVr2")),
									new CInstruction(4, instMOVr3, true, std::string("MOVr3")),
									new CInstruction(4, instrOUTr1, true, std::string("OUTr1")),
									new CInstruction(4, instrOUTr3, true, std::string("OUTr3")),
									new CInstruction(4, instJMP, true, std::string("JMP")),
									new CInstruction(4, instJZ, true, std::string("JZ")),
									new CInstruction(4, instJNZ, true, std::string("JNZ")),
									new CInstruction(4, instJNS2, true, std::string("JNS2")),
									new CInstruction(4, instrINr1, true, std::string("INr1")),
									new CInstruction(4, instrINr2, true, std::string("INr2")),
									new CInstruction(4, instrINr3, true, std::string("INr3")),
									new CInstruction(4, instrINr4, true, std::string("INr4")),
									new CInstruction(4, instrNOP, false, std::string("NOP")),
									new CInstruction(4, instrRST, false, std::string("RST")),
									new CInstruction(4, instJNS1, true, std::string("JNS1")),
									new CInstruction(4, instrOUTr2, true, std::string("OUTr2")),
									new CInstruction(4, instrOUTr4, true, std::string("OUTr4")),
									new CInstruction(4, instMod1, false, std::string("MOD1"), 1, iosMod1, 4),
									new CInstruction(4, instMod2, false, std::string("MOD2"), 1, iosMod2, 4),
									new CInstruction(4, instMod3, false, std::string("MOD3"), 1, iosMod3, 3),
									new CInstruction(4, instMod1_2, false, std::string("MOD1_2"), 1, iosMod1_2, 8),
									new CInstruction(4, instMod1_3, false, std::string("MOD1_3"), 1, iosMod1_3, 7),
									new CInstruction(4, instMod2_3, false, std::string("MOD2_3"), 1, iosMod2_3, 7),
	};
	
	CModule * pModules[] = { pAddMod1, pAddMod2, pMultMod1 };
	strArchitectureParams archParams;
	archParams.nMaxModulesCount = 3;
	archParams.nMaxRegistersCount = 4;
	archParams.pAllowedInstructions = arrInstrSet;
	archParams.pAllowedModules = pModules;

	//CArchitecture * pArch = new CArchitecture(4, (REGISTER_TYPE) 0xFFFFFFFF, 3, pModules, INSTR_COUNT, arrInstrSet, &archParams, true);
	/*strIndividualParams indivParams(15,15);
	CIndividual indiv(pArch, pEnv, &indivParams);
	strArchitectureSettings settings(3);
	for (int i = 0; i < 3; i++)
		settings.pModulesOrder[i] = i;*/
	//strArchitectureSettings settings(3);
	//indiv.SetArchitectureSettings(&settings);
	//pArch->SetArchSettings(&settings);
	/*pArch->SetArchSettings(indiv.GetArchSettings());
	strEvolutionParams evoParams(50, 20000, 0, 0.7, 1);
	//indiv.GenerateRandomProgram(indivParams);
	indiv.AppendInstrBlock(0, 1, NULL);	// LOAD r3, 2
	indiv.AppendInstrBlock(1, 2, NULL);	// LOAD r2, 4
	indiv.AppendInstrBlock(2, 3, NULL);		// ADD r2, r3 -> r3
	indiv.AppendInstrBlock(3, 4, NULL);		// ADD r2, r3 -> r3
	indiv.AppendInstrBlock(4, 0, iosCOMPLEX);		// ADD r2, r3 -> r3
	indiv.EvaluateFitness(evoParams.strSimulParams);

	return;*/

	bool bDynamic = true;
	//for (UINT prob = 0; prob < 11; prob++) {
		for (UINT i = 0; i < 50; i++) {
			CArchitecture * pArch = new CArchitecture(4, (REGISTER_TYPE) 0xFFFFFFFF, 3, pModules, INSTR_COUNT, arrInstrSet, &archParams, bDynamic);
			strArchitectureSettings settings(3);
			pArch->SetArchSettings(&settings);

			strEvolutionParams evoParams(20, 100000, 0.05, 0.7,/*fCross, 0.1*prob,*/
				1);
			evoParams.strMutParams.fProbs[infoMutationReg] = 0;
			evoParams.strMutParams.fProbs[infoMutationModule] = 0;

			evoParams.strSimulParams.nMaxLogicTime = 15000;
			strIndividualParams indivParams(15,15);
			CHWTopologyFramework * pFramework = new CHWTopologyFramework(pArch, pEnv, evoParams, indivParams);
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

double CHWTopologyIndiv::EvaluateOutputs(CEnvironment * pEnv)
{
	// y = x^4 - x^3 - 2x^2 + 3x + 4
	//int nExpectedOrder[10] = { 3600, 14400, 44100, 112896, 254016, 518400, 980100, 1742400, 2944656, 4769856 }; // x^6-2*x^4+x^2
	//int nExpectedOrder[10] = { 340, 780, 1554, 2800, 4680, 7380, 11110, 16104, 22620, 30940 }; // x^4+x^3+x^2+x
	//int nExpectedOrder[10] = { 4, 2, 22, 112, 344, 814, 1642, 2972, 4972, 7834 };
	//int nExpectedOrder[10] = { 1, 16, 81, 256, 625, 1296, 2401, 4096, 6561, 10000 };
	REGISTER_TYPE expectedOrder[10] = { 1 / 0.5, 1 / 0.55, 1 / 0.6, 1 / 0.65, 1 / 0.7, 1 / 0.75, 1 / 0.8, 1 / 0.85, 1 / 0.9, 1 / 0.95 };
	double dFitness = 0;
	nInputNum = 0;
	double totalFitness = -1;
	
	CDynamicTimeline * pTimeline = pEnv->GetOutput(0);
	int i = 0;
	if (pTimeline->GetFirst())
		totalFitness = 0;
	UINT nOutsCount = 0;
	REGISTER_TYPE pLastOut;
	for (strDynamicTimeVal * pActOut = pTimeline->GetFirst(); pActOut && i < 100; i++) {
		nOutsCount++;
		if (i % 10 == 0)
			pLastOut = inputs[i / 10];

		REGISTER_TYPE dActDiff = (REGISTER_TYPE) fabs(pActOut->val - expectedOrder[i / 10]);
		REGISTER_TYPE dLastDiff = (REGISTER_TYPE) fabs(pLastOut - expectedOrder[i / 10]);
		
		if (dLastDiff > 0)
			dFitness = (dActDiff / dLastDiff);
		else
			dFitness = 0;
		
		pLastOut = pActOut->val;

		totalFitness += dFitness;

		if (pActOut->pSucc)
			pActOut = pActOut->pSucc;
		else
			nOutsCount--;
	}

	if (totalFitness > 1)
		totalFitness = 1000*(1 / totalFitness);
	else if (totalFitness = -1)
		totalFitness = 0;
	else
		totalFitness = 100;

	if (nOutsCount < 100)
		totalFitness = 0;

	return totalFitness;
}

void CHWTopologyIndiv::GenerateRandomProgram()
{
	__super::GenerateRandomProgram();
	return;

	/*m_pIndivParams->nMaxProgramSize--;
	__super::GenerateRandomProgram();
	AppendInstrBlock(8, -20, NULL);
	m_pIndivParams->nMaxProgramSize++;
	return;*/

	AppendInstrBlock(1, 10, NULL);
	AppendInstrBlock(13, 0, NULL);
	AppendInstrBlock(14, 1, NULL);
	AppendInstrBlock(2, 1, NULL);

	BYTE * compIOs = new BYTE[11];
					// first mod output not used
	BYTE iosCOMPLEX[] = {0x00, 0x00, 0x80, 0xFF, 0x03, 0x04, 0x81, 0x03, 0x01, 0x02, 0xFF}; // Xi*D, 1-Xi*D
	memcpy(compIOs, iosCOMPLEX, 11);
	AppendInstrBlock(0, 0, compIOs);

	//AppendInstrBlock(16, 0, NULL);
	compIOs = new BYTE[11];
					// first mod output not used
	BYTE iosCOMPLEX2[] = {0x00, 0x00, 0x80, 0xFF, 0x02, 0x04, 0x80, 0x02, 0x02, 0x03, 0xFF}; // Xi*(1-Xi*D), Xi+Xi*(1-Xi*D)
	memcpy(compIOs, iosCOMPLEX2, 11);
	AppendInstrBlock(0, 0, compIOs);
	
	compIOs = new BYTE[11];
					// first mod output not used
	BYTE iosCOMPLEX3[] = {0x00, 0x00, 0x80, 0xFF, 0x00, 0x00, 0x83, 0x00, 0x02, 0x03, 0xFF};	// DEC r1
	memcpy(compIOs, iosCOMPLEX3, 11);
	AppendInstrBlock(0, 0, compIOs);

	/*AppendInstrBlock(11, -4, NULL);
	AppendInstrBlock(7, 0, NULL);
	AppendInstrBlock(8, -15, NULL);*/
	
	AppendInstrBlock(16, 0, NULL);
	AppendInstrBlock(7, 0, NULL);
	//AppendInstrBlock(16, 0, NULL);
	AppendInstrBlock(8, -15, NULL);

	AppendInstrBlock(16, 0, NULL);
	AppendInstrBlock(16, 0, NULL);
	AppendInstrBlock(16, 0, NULL);
	AppendInstrBlock(16, 0, NULL);
	AppendInstrBlock(16, 0, NULL);

	return;
}

void CHWTopologyIndiv::GenerateRandomHWTopology()
{
	__super::GenerateRandomHWTopology();
	return;

	UINT nModCount = m_pArch->GetModulesCount();
	m_pArchSettings = new strArchitectureSettings(nModCount);

	for (UINT nActMod = 0; nActMod < nModCount; nActMod++) {
		m_pArchSettings->pModulesOrder[nActMod] = 2 - nActMod;
	}
}

void CHWTopologyIndiv::PrintProgram()
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

bool CHWTopologyFramework::EvaluateStopCondition(UINT nGeneration)
{
	static bool bPartialPrinted;
	if (nGeneration == 1)
		bPartialPrinted = false;

	for (UINT nBest = 0; nBest < m_strEvoParams.nEliteSize; nBest++) {
		if (nGeneration == 1)
			m_pBestIndivs[nBest]->EvaluateFitness(m_strEvoParams.strSimulParams);
		if (m_pBestIndivs[nBest]->GetFitness(eResultsFitness) >= 50) {
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

void CHWTopologyFramework::ResetAfterEval()
{
	for (int i = 0; i < 10; i++)
		tmpOuts[i] = inputs[i];
}
