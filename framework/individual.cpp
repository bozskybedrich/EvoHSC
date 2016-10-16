#include "individual.h"
#include <assert.h>
#include <time.h>
#include <stdlib.h>
#include "framework_typedefs.h"
#include "framework.h"
#include <map>
#include <iostream>
#include <vector>

extern int nActTestSet;
#ifdef _DEBUG
	#define new MYDEBUG_NEW
#endif

// Copy constructor
CIndividual::CIndividual(const CIndividual & source)
{
	m_pEnv = source.m_pEnv;
	m_pFramework = source.m_pFramework;

	m_pIndivParams = source.m_pIndivParams;

	if (source.m_pArch->IsDynamic())
		m_pArch = source.m_pArch ? new CArchitecture(*source.m_pArch) : NULL;
	else
		m_pArch = source.m_pArch;

	m_pArchParams = source.m_pArchParams;
	m_pArchSettings = new strArchitectureSettings(*source.m_pArchSettings);
	m_pFirstInstr = m_pLastInstr = NULL;

	m_nInstrCount = source.m_nInstrCount;
	for (CInstrBlock * pActInstr = source.m_pFirstInstr; pActInstr; pActInstr = pActInstr->GetNext())
		AppendInstrBlock(pActInstr);

	m_mapFitness = source.m_mapFitness;
}


CIndividual::CIndividual(CArchitecture * pArch, CEnvironment * pEnv, CFramework * pFramework, strIndividualParams * pIndivParams)
	: m_pEnv(pEnv), m_pFramework(pFramework), m_pIndivParams(pIndivParams), m_pFirstInstr(NULL), m_pLastInstr(NULL), m_pArchParams(NULL)
{
	assert(pArch);
	assert(pEnv);

	if (pArch->IsDynamic())
		m_pArch = new CArchitecture(*pArch);
	else
		m_pArch = pArch;
}

CIndividual::~CIndividual()
{
	CInstrBlock * pActInstr = m_pFirstInstr, * pNextInstr;
	while (pActInstr) {
		pNextInstr = pActInstr->GetNext();
		delete pActInstr;
		pActInstr = pNextInstr;
	}

	if (m_pArch->IsDynamic())
		delete m_pArch;

	if (m_pArchSettings)
		delete m_pArchSettings;
}

double CIndividual::EvaluateFitness(strSimulationParams & simParams)
{
	UINT nExecTime;
	m_pArch->SetArchSettings(m_pArchSettings);
	m_mapFitness[eResultsFitness] = 0;
	m_mapFitness[eSpeedFitness] = 0;
	m_mapFitness[eAreaFitness] = 0;
	m_mapFitness[ePriceFitness] = 0;

	assert(nActTestSet == 0);

	do {
		m_pArch->ExecuteProgram(m_pFirstInstr, m_pEnv, simParams.nMaxLogicTime, nExecTime);
		m_mapFitness[eResultsFitness] += EvaluateOutputs(m_pEnv);
		m_mapFitness[eSpeedFitness] += EvaluateSpeed(nExecTime, simParams.nMaxLogicTime);
		EvaluateHW();
		if (simParams.bPrintInfo)
			m_pEnv->PrintOutputs();
		m_pEnv->ResetRealOutputs();
		m_pArch->ResetRegisters();
		m_pArch->ResetModules();
		m_pFramework->ResetAfterEval();
	} while (m_pFramework->IsNextInputSet());

	return 0;
}

double CIndividual::EvaluateOutputs(CEnvironment * pEnv)
{
	return 0;
}

double CIndividual::EvaluateSpeed(UINT nExecTime, UINT nMaxLogicTime)
{
	if (nMaxLogicTime > 0)
		return 1 - ((double) nExecTime / nMaxLogicTime);
	else {
		assert(false);
		return 0;
	}
}

double CIndividual::GetFitness(const enumFitnessType eType) const
{
	std::map<enumFitnessType, double>::const_iterator itFound = m_mapFitness.find(eType);
	if (itFound != m_mapFitness.end())
		return itFound->second;
	else {
		//assert(false);
		return 0;
	}
}

void CIndividual::MakeMutation(strMutationParams & mutParams)
{
	m_info.Reset();

	// HW mutation
	double fRand = rand() / (double) RAND_MAX;
	if (fRand < mutParams.fProbs[infoMutationModule]) {
		// Modules change
		m_info.m_arrOpUsed[infoMutationModule] = true;

		UINT nModInd = rand() % m_pArch->GetModulesCount();
		if (m_pArch->IsModuleUsed(nModInd))
			m_pArch->SetModuleUsed(nModInd, false);
		else
			m_pArch->SetModuleUsed(nModInd, true);
	}
	
	fRand = rand() / (double) RAND_MAX;
	if (fRand < mutParams.fProbs[infoMutationReg]) {
		// Registers change
		m_info.m_arrOpUsed[infoMutationReg] = true;

		UINT nRegNum = rand() % m_pArch->GetRegistersCount();
		REGISTER_TYPE nMask = m_pArch->GetRegisterMask(nRegNum);
		UINT nMaxOffset = rand() % (sizeof(REGISTER_TYPE)*8 / 2);
		int nOffset = GAUSS(nMaxOffset);
		if (nOffset == 0)
			nOffset = (rand() / (double) RAND_MAX < 0.5) ? 1 : -1;

#ifndef FP_REGISTER_TYPE
		for (; nOffset != 0; nOffset > 0 ? nOffset-- : nOffset++) {
			if (nOffset > 0) {
				nMask >>= 1;
				nMask = nMask & ~(1 << (sizeof(REGISTER_TYPE)*8 - 1));
			}
			else {
				nMask <<= 1;
				nMask = nMask | 1;
			}
		}
#endif

		m_pArch->SetRegisterMask(nRegNum, nMask);
	}

	fRand = rand() / (double) RAND_MAX;
	if (fRand < mutParams.fProbs[infoMutationInstrSet]) {
		// Instruction set change
		m_info.m_arrOpUsed[infoMutationInstrSet] = true;
	}

	// SW mutation
	fRand = rand() / (double) RAND_MAX;
	UINT nInstrNo = rand() % m_pIndivParams->nMaxProgramSize;
	if (nInstrNo == 0)
		nInstrNo++;

	CInstrBlock * pBlock = NULL;
	for (CInstrBlock * pActBlock = m_pFirstInstr; pActBlock && nInstrNo > 0; pActBlock = pActBlock->GetNext(), nInstrNo--)
		pBlock = pActBlock;
	assert(pBlock);

	fRand = rand() / (double) RAND_MAX;
	if (fRand < mutParams.fProbs[infoMutationInstrType]) {
		// Instruction change
		m_info.m_arrOpUsed[infoMutationInstrType] = true;

		UINT nInstrIndex = rand() % pBlock->GetInstrCount();
		pBlock->SetInstrNo(nInstrIndex, rand() % m_pArch->GetInstructionCount());
		CInstruction * pInstr = m_pArch->GetInstruction(pBlock->GetInstrNo(nInstrIndex));
		INSTR_TYPE eType = pInstr->GetType();
		REGISTER_TYPE newParam = MutateRandomParam(pBlock->GetParam(nInstrIndex), eType);
		pBlock->SetParam(nInstrIndex, newParam);

		BYTE * pIOs = GenerateRandomIOs(pInstr);
		pBlock->ReplaceInstrIOs(nInstrIndex, pIOs);
	}
	
	fRand = rand() / (double) RAND_MAX;
	if (fRand < mutParams.fProbs[infoMutationInstrParam]) {
		// Instruction parameter change
		m_info.m_arrOpUsed[infoMutationInstrParam] = true;
		
		UINT nInstrIndex = rand() % pBlock->GetInstrCount();
		CInstruction * pInstr = m_pArch->GetInstruction(pBlock->GetInstrNo(nInstrIndex));
		INSTR_TYPE eType = pInstr->GetType();
		REGISTER_TYPE newParam = MutateRandomParam(pBlock->GetParam(nInstrIndex), eType);
		pBlock->SetParam(nInstrIndex, newParam);
	}
	
	fRand = rand() / (double) RAND_MAX;
	if (fRand < mutParams.fProbs[infoMutationInstrIOs]) {
		// Instruction IOs change
		UINT nInstrIndex = rand() % pBlock->GetInstrCount();
		CInstruction * pInstr = m_pArch->GetInstruction(pBlock->GetInstrNo(nInstrIndex));
		BYTE * pInstrIOs = pInstr->GetInsOuts();
		
		std::vector<UINT> vecModifiable;
		for (UINT nActIO = 0; nActIO < pInstr->GetInsOutsCount(); nActIO++) {
			if (pInstrIOs[nActIO] == 0xFF || ((pInstrIOs[nActIO] & 0xC0) == 0xC0))
				vecModifiable.push_back(nActIO);
		}

		if (vecModifiable.size() > 0) {
			m_info.m_arrOpUsed[infoMutationInstrIOs] = true;
			UINT nIOIndex = vecModifiable.at(rand() % vecModifiable.size());
			if (pInstrIOs[nIOIndex] == 0xFF)
				pBlock->SetIO(nInstrIndex, nIOIndex, rand() % (m_pArch->GetMaxPossibleInputs() + 1) - 1);
			else
				pBlock->SetIO(nInstrIndex, nIOIndex, rand() % (pInstrIOs[nIOIndex] & 0x3F));
		}
	}

	fRand = rand() / (double) RAND_MAX;
	if (fRand < mutParams.fProbs[infoMutationInstrPos]) {
		// Instruction position change
		m_info.m_arrOpUsed[infoMutationInstrPos] = true;

		UINT nOffset = GAUSS(m_pIndivParams->nMaxProgramSize);
		if (nOffset == 0)
			nOffset = (rand() / (double) RAND_MAX < 0.5 ? 1 : -1);
			
		CInstrBlock * pTmpBlock = pBlock;
		CInstrBlock * pTargetBlock;
		for (; pTmpBlock && nOffset != 0; nOffset = (nOffset > 0 ? nOffset - 1 : nOffset + 1)) {
			pTargetBlock = pTmpBlock;
			pTmpBlock = (nOffset > 0) ? pTmpBlock->GetNext() : pTmpBlock->GetPrev();
		}
			
		if (pBlock && pTargetBlock && pBlock != pTargetBlock) {
			SpliceOutInstrBlock(pBlock);
			InsertAfter(pBlock, pTargetBlock);
		}
	}

	fRand = rand() / (double) RAND_MAX;
	if (fRand < mutParams.fProbs[infoMutationModulesMask]) {
		int nMod = rand() % ((sizeof(DWORD) - 1) * 8);
		DWORD nModMask = pBlock->GetModulesMask();
		// Invert the module mask bit
		if ((nModMask & (1 << nMod)) != 0) {
			nModMask = nModMask & (~(1 << nMod));
		}
		else {
			nModMask = nModMask | (1 << nMod);
		}
	}
}

inline REGISTER_TYPE CIndividual::OffsetParam(REGISTER_TYPE origParam, REGISTER_TYPE maxDelta, REGISTER_TYPE minVal, REGISTER_TYPE maxVal)
{
	REGISTER_TYPE newParam;
	REGISTER_TYPE nDelta;
	nDelta = GAUSS(maxDelta);
	newParam = origParam + nDelta;
	if (newParam > maxVal)
		newParam = maxVal;
	else if (newParam < minVal)
		newParam = minVal;
	
	return newParam;
}

inline REGISTER_TYPE CIndividual::MutateRandomParam(REGISTER_TYPE origParam, INSTR_TYPE eType)
{
	REGISTER_TYPE nDelta;
	REGISTER_TYPE newParam;
	switch (eType) {
		case INST_JMP: {
			REGISTER_TYPE nMaxOffset = m_pIndivParams->nMaxJmpOffset;
			newParam = OffsetParam(origParam, nMaxOffset, -nMaxOffset, nMaxOffset); }
			break;
		case INST_IN: {
			UINT nInpCount = m_pEnv->GetInputsCount();
			newParam = OffsetParam(origParam, nInpCount, 0, nInpCount - 1); }
			break;
		case INST_OUT: {
			UINT nOutCount = m_pEnv->GetOutputsCount(); 
			newParam = OffsetParam(origParam, nOutCount, 0, nOutCount - 1); }
			break;
		case INST_MOV: {
			UINT nRegCount = m_pArch->GetRegistersCount();
			newParam = OffsetParam(origParam, nRegCount, 0, nRegCount - 1); }
			break;
		case INST_LOAD:
		case INST_OTHER: {
			UINT nMaxParam = m_pIndivParams->nMaxRandParam;
			newParam = OffsetParam(origParam, nMaxParam, -nMaxParam, nMaxParam); }
			break;
		default:
			assert(false);
			break;
	}

	return newParam;
}

void CIndividual::GenerateRandomProgram()
{
	/*BYTE * addIOs = new BYTE[4];
	addIOs[0] = 2;
	addIOs[1] = 1;
	addIOs[2] = 0;
	addIOs[3] = 1;
	BYTE * decIOs = new BYTE[4];
	decIOs[0] = 0;
	decIOs[1] = 0;
	decIOs[2] = 3;
	decIOs[3] = 0;
	AppendInstrBlock(12, 0, NULL);
	AppendInstrBlock(8, 0, NULL);
	AppendInstrBlock(9, 0, NULL);
	AppendInstrBlock(5, 0, decIOs);
	AppendInstrBlock(3, 0, addIOs);
	AppendInstrBlock(5, 0, decIOs);
	AppendInstrBlock(18, -2, NULL);
	AppendInstrBlock(11, 0, NULL);
	AppendInstrBlock(21, 0, NULL);
	AppendInstrBlock(18, -9, NULL);
	return;*/

	/*BYTE * mulIOs = new BYTE[3];
	mulIOs[0] = 0;
	mulIOs[1] = 0;
	mulIOs[2] = 0;
	AppendInstrBlock(11, 0, NULL);
	AppendInstrBlock(4, 0, mulIOs);
	AppendInstrBlock(8, 0, NULL);
	AppendInstrBlock(10, -10, NULL);
	return;*/
	/*AppendInstrBlock(32, 0); // IN r1, 0
	AppendInstrBlock(22, 0); // MOV r1, r2
	AppendInstrBlock(2, 0); // LOAD r3, 0
	AppendInstrBlock(11, 2); // ADD r2, r3
	AppendInstrBlock(26, 0); // DEC r1
	AppendInstrBlock(39, -2); // JNZ -2
	AppendInstrBlock(38, 0); // OUT r0
	AppendInstrBlock(35, -7); // OUT r0
	return;*/

	m_nInstrCount = m_pArch->GetInstructionCount();
	UINT nOutCount = 0;
	UINT nJmpCount = 0;
	UINT nProgLength = PregenerateProgram();
	for (; nProgLength < m_pIndivParams->nMaxProgramSize; nProgLength++) {
		BYTE nInstrNum = rand() % m_nInstrCount;
		CInstruction * pInstr = m_pArch->GetInstruction(nInstrNum);
		REGISTER_TYPE nParam;
		INSTR_TYPE eType = pInstr->GetType();
		switch (eType) {
		case INST_JMP:
			nParam = (REGISTER_TYPE) (rand() % (2 * m_pIndivParams->nMaxJmpOffset)) - m_pIndivParams->nMaxJmpOffset;
			break;
		case INST_IN:
			nParam = rand() % m_pEnv->GetInputsCount();
			break;
		case INST_OUT:
			nParam = rand() % m_pEnv->GetOutputsCount();
			break;
		case INST_MOV:
			nParam = rand() % m_pArch->GetRegistersCount();
			break;
		case INST_LOAD:
		case INST_OTHER:
			nParam = (REGISTER_TYPE) (rand() % (2 * m_pIndivParams->nMaxRandParam)) - m_pIndivParams->nMaxRandParam;
			break;
		default:
			assert(false);
			break;
		}

		BYTE * pIOs = GenerateRandomIOs(nInstrNum);
		DWORD nModulesMask = GenerateRandomModulesMask();
		AppendInstrBlock(nInstrNum, nParam, pIOs, nModulesMask);
	}

	if (nProgLength < m_pIndivParams->nMaxProgramSize)
		assert(false);
	/*CInstrBlock * pNewLast = m_pLastInstr->GetPrev();
	pNewLast->SetNext(NULL);
	delete m_pLastInstr;
	m_pLastInstr = pNewLast;
	AppendInstrBlock(6, -10, NULL);*/
}

int CIndividual::PregenerateProgram()
{
	return 0;
}

void CIndividual::GenerateRandomHWTopology()
{
	assert(m_pArch);
	
	UINT nModCount = m_pArch->GetModulesCount();
	m_pArchSettings = new strArchitectureSettings(nModCount);

	for (UINT nActMod = 0; nActMod < nModCount; nActMod++) {
		m_pArchSettings->pModulesOrder[nActMod] = -1;
		m_pArchSettings->pModulesInverseOrder[nActMod] = 0;
	}

	// Filling the modules order array
	for (UINT nActMod = 0; nActMod < nModCount; nActMod++) {
		UINT nRandPos = rand() % nModCount;
		
		while (m_pArchSettings->pModulesOrder[nRandPos] != -1)
			nRandPos = (nRandPos + 1) % nModCount;

		m_pArchSettings->pModulesOrder[nRandPos] = nActMod;
	}

	// Filling the inverse modules order array
	for (UINT nActOrd = 0; nActOrd < nModCount; nActOrd++) {
		for (UINT nPrec = 0; nPrec < nActOrd; nPrec++)
			if (m_pArchSettings->pModulesOrder[nPrec] > m_pArchSettings->pModulesOrder[nActOrd])
				m_pArchSettings->pModulesInverseOrder[m_pArchSettings->pModulesOrder[nActOrd]]++;
	}

	assert(m_pArchSettings->pModulesOrder[0] != m_pArchSettings->pModulesOrder[1]);
}

void CIndividual::UpdateHWTopology()
{
	assert(m_pArch);
	UINT nModCount = m_pArch->GetModulesCount();
	UINT * pPos = new UINT[nModCount];

	for (int i = nModCount - 1; i >= 0; i--) {
		for (UINT m = i + 1; m < nModCount; m++) {
			if (pPos[m] >= m_pArchSettings->pModulesInverseOrder[i] + 1)
				pPos[m]++;
		}
		pPos[i] = m_pArchSettings->pModulesInverseOrder[i] + 1;
	}
	
	for (UINT i = 0; i < nModCount; i++) {
		m_pArchSettings->pModulesOrder[pPos[i] - 1] = i;
	}

#ifdef _DEBUG
	for (int j = 1; j < nModCount; j++)
		assert(m_pArchSettings->pModulesOrder[j - 1] != m_pArchSettings->pModulesOrder[j]);
#endif

	delete[] pPos;
}

BYTE * CIndividual::GenerateRandomIOs(CInstruction * pInstr)
{
	BYTE * pIOs = NULL;
	UINT nIOsCount = pInstr->GetInsOutsCount();
	if (nIOsCount > 0) {
		pIOs = new BYTE[nIOsCount];
		BYTE * pInstrIOs = pInstr->GetInsOuts();

		for (UINT nActIO = 0; nActIO < nIOsCount; nActIO++) {
			if ((pInstrIOs[nActIO] & 0xC0) == 0xC0) {
				if (pInstrIOs[nActIO] == 0xFF)
					pIOs[nActIO] = rand() % (m_pArch->GetMaxPossibleInputs() + 1) - 1;
				else
					pIOs[nActIO] = (rand() % (pInstrIOs[nActIO] & 0x3F)) | 0x80;
			}
			else if ((pInstrIOs[nActIO] & 0x80) != 0) {
				pIOs[nActIO] = pInstrIOs[nActIO];
			}
			else {
				pIOs[nActIO] = pInstrIOs[nActIO];
				/*assert(pInstrIOs[nActIO]);
				if (pInstrIOs[nActIO] == 0) {
					pIOs[nActIO] = 0;
				}
				else
					pIOs[nActIO] = rand() % pInstrIOs[nActIO];*/
			}
		}
	}

	return pIOs;
}

DWORD CIndividual::GenerateRandomModulesMask()
{
	DWORD nModulesMask = 0;

	for (int i = 0; i < sizeof(DWORD) - 1; i++) { // Just 3 bytes as the fourth byte is the instruction header
		BYTE nRandByte = rand() % 0xFF;
		nModulesMask = nModulesMask << 8;
		nModulesMask = nModulesMask | nRandByte;
	}

	return nModulesMask;
}

CInstrBlock * CIndividual::AppendInstrBlock(CInstrBlock * pSourceBlock)
{
	CInstrBlock * pInserted = new CInstrBlock(pSourceBlock, m_pLastInstr, m_pArch);
	if (!m_pFirstInstr)
		m_pFirstInstr = m_pLastInstr = pInserted;
	else
		m_pLastInstr = pInserted;

	return pInserted;
}

CInstrBlock * CIndividual::AppendInstrBlock(UINT nInstrNo, REGISTER_TYPE param, BYTE * pIOs, DWORD nModulesMask)
{
	CInstrBlock * pInserted = new CInstrBlock(nInstrNo, param, m_pLastInstr, pIOs, nModulesMask);
	if (!m_pFirstInstr)
		m_pFirstInstr = m_pLastInstr = pInserted;
	else
		m_pLastInstr = pInserted;

	return pInserted;
}

CInstrBlock * CIndividual::SpliceOutInstrBlock(CInstrBlock * pBlock)
{
	CInstrBlock * pPrev = pBlock->GetPrev();
	CInstrBlock * pNext = pBlock->GetNext();
	
	if (m_pFirstInstr == pBlock)
		m_pFirstInstr = pNext;
	if (m_pLastInstr == pBlock)
		m_pLastInstr = pPrev;

	if (pPrev)
		pPrev->SetNext(pNext);
	if (pNext)
		pNext->SetPrev(pPrev);

	pBlock->SetPrev(NULL);
	pBlock->SetNext(NULL);

	return pBlock;
}

CInstrBlock * CIndividual::InsertAfter(CInstrBlock * pBlockToInsert, CInstrBlock * pInsertAfter)
{
	assert(pBlockToInsert);
	assert(pInsertAfter);

	if (pInsertAfter == m_pLastInstr)
		m_pLastInstr = pBlockToInsert;

	CInstrBlock * pNext = pInsertAfter->ReplaceNext(pBlockToInsert);
	pBlockToInsert->SetNext(pNext);
	CInstrBlock * pPrev = NULL;
	if (pNext)
		pPrev = pNext->ReplacePrev(pBlockToInsert);
	else
		pPrev = pInsertAfter;
	pBlockToInsert->SetPrev(pPrev);

	return pBlockToInsert;
}

CIndividual * CIndividual::Clone()
{
	return new CIndividual(*this);
}

void CIndividual::EvaluateHW()
{
	double dUsedArea = 0, dUsedPrice = 0;
	double dTotalArea = 0, dTotalPrice = 0;
	double dActArea = 0, dActPrice = 0;

	for (UINT i = 0; i < m_pArch->GetModulesCount(); i++) {
		CModule * pModule = m_pArch->GetModule(i);
		dActArea = pModule->GetArea();
		dActPrice = pModule->GetPrice();

		if (m_pArch->IsModuleUsed(i)) {
			dUsedArea += dActArea;
			dUsedPrice += dActPrice;
		}
		dTotalArea += dActArea;
		dTotalPrice += dActPrice;
	}

	m_mapFitness[eAreaFitness] += (dTotalArea - dUsedArea);
	m_mapFitness[ePriceFitness] += (dTotalPrice - dUsedPrice);

#ifndef FP_REGISTER_TYPE
	/*REGISTER_TYPE nRegDefMask = m_pArch->GetDefRegMask();
	for (UINT i = 0; i < m_pArch->GetRegistersCount(); i++) {
		REGISTER_TYPE nRegMask = m_pArch->GetRegisterMask(i);
		nRegMask = nRegDefMask & (~nRegMask);
		for (UINT bit = 0; bit < sizeof(REGISTER_TYPE)*8; bit++) {
			if (nRegMask & (1 << bit)) {
				m_strFitness.dAreaFitness++;
				m_strFitness.dPriceFitness++;
			}
		}
	}*/
#endif
}

CInstrBlock * CIndividual::GetInstrBlockByPos(UINT nPos)
{
	CInstrBlock * pBlock = m_pFirstInstr;

	for (UINT nActPos = 0; nActPos < nPos; nActPos++) {
		if (pBlock == m_pLastInstr)
			break;

		CInstrBlock * pBlockNext = pBlock->GetNext();
		if (!pBlockNext)
			assert(false);
		pBlock = pBlock->GetNext();
	}

	return pBlock;
}
