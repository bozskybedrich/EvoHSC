#include "architecture.h"
#include "errorstatus.h"
#include "instrdecoder.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>
//#include <crtdbg.h>

#ifdef _DEBUG
	#define new MYDEBUG_NEW
#endif

CInstruction::CInstruction(UINT nSize, BYTE * pDecoded, bool bNeedParameter, const std::string & sDescription, UINT nDelay, BYTE * pInsOuts, UINT nIOsCount)
	: m_bNeedParameter(bNeedParameter), m_nProcessingDelay(nDelay), m_sDescription(sDescription)
{
	m_pDecoded = (BYTE *) malloc(nSize);
	if (m_pDecoded != NULL) {
		memcpy(m_pDecoded, pDecoded, nSize);
		m_nSize = nSize;
	}
	else {
		m_pDecoded = NULL;
		m_nSize = 0;
	}

	if (pInsOuts) {
		assert(nIOsCount > 0);
		m_nInsOutsCount = nIOsCount;
		m_pInsOuts = new BYTE[m_nInsOutsCount];
		memcpy(m_pInsOuts, pInsOuts, m_nInsOutsCount);
	}
	else {
		m_nInsOutsCount = 0;
		m_pInsOuts = NULL;
	}

	// Choose the instruction type by the header
	DWORD nHeader = 0;
	for (int i = 0; i < sizeof(DWORD); i++) {
		nHeader <<= 8;
		nHeader += pDecoded[i];
	}

	if (nHeader & MOV_MASK) {
		if ((nHeader & IN_CONNECTION_MASK) == IN_CONNECTION_MASK) {
			m_eType = INST_IN;
		}
		else if ((nHeader & OUT_CONNECTION_MASK) == OUT_CONNECTION_MASK) {
			m_eType = INST_OUT;
		}
		else if ((nHeader & LOAD_MASK) == LOAD_MASK) {
			m_eType = INST_LOAD;
		}
		else {
			m_eType = INST_MOV;
		}
	}
	else if (nHeader & JXX_MASK)
		m_eType = INST_JMP;
	else
		m_eType = INST_OTHER;
}

// Copy constructor
CInstruction::CInstruction(const CInstruction & source)
{
	m_nSize = source.m_nSize;
	m_pDecoded = source.m_pDecoded;
	m_pDecoded = new BYTE[m_nSize];
	memcpy(m_pDecoded, source.m_pDecoded, m_nSize);
	m_bNeedParameter = source.m_bNeedParameter;
	m_nProcessingDelay = source.m_nProcessingDelay;
	m_eType = source.m_eType;
	m_sDescription = source.m_sDescription;
	m_nInsOutsCount = source.m_nInsOutsCount;
	m_pInsOuts = new BYTE[m_nInsOutsCount];
	memcpy(m_pInsOuts, source.m_pInsOuts, m_nInsOutsCount);
}

CInstruction::~CInstruction()
{
	if (m_pDecoded)
		delete m_pDecoded;

	if (m_pInsOuts)
		delete m_pInsOuts;
}

CInstrBlock::CInstrBlock(UINT nInstrNo, REGISTER_TYPE param, CInstrBlock * pInsertAfter, BYTE * pIOs, DWORD nModulesMask)
{
	m_vecInstrNo.push_back(nInstrNo);
	m_vecParam.push_back(param);
	m_vecIOs.push_back(pIOs);
	m_nModulesMask = nModulesMask;

	InsertAfter(pInsertAfter);
}

CInstrBlock::CInstrBlock(CInstrBlock * pSourceBlock, CInstrBlock * pInsertAfter, CArchitecture * pArch)
{
	for (int nInstrIndex = 0; nInstrIndex < pSourceBlock->GetInstrCount(); nInstrIndex++) {
		m_vecInstrNo.push_back(pSourceBlock->GetInstrNo(nInstrIndex));
		m_vecParam.push_back(pSourceBlock->GetParam(nInstrIndex));
		m_nModulesMask = pSourceBlock->GetModulesMask();
		BYTE * pSourceIOs = pSourceBlock->GetInstrIOs(nInstrIndex);
		CInstruction * pInstr = pArch->GetInstruction(m_vecInstrNo.back());
		UINT nIOsCount = pInstr->GetInsOutsCount();
		BYTE * pIOs = new BYTE[nIOsCount];
		
		memcpy(pIOs, pSourceIOs, nIOsCount);
		m_vecIOs.push_back(pIOs);
	}

	InsertAfter(pInsertAfter);
}

CInstrBlock::~CInstrBlock()
{
	for (std::vector<BYTE*>::iterator itIOs = m_vecIOs.begin(); itIOs != m_vecIOs.end(); itIOs++)
		if (*itIOs)
			delete (*itIOs);
}

void CInstrBlock::InsertAfter(CInstrBlock * pInsertAfter)
{
	if (!pInsertAfter) {
		m_pPrev = NULL;
		m_pNext = NULL;
	}
	else {
		m_pNext = pInsertAfter->ReplaceNext(this);
		if (m_pNext)
			m_pPrev = m_pNext->ReplacePrev(this);
		else
			m_pPrev = pInsertAfter;
	}
}

UINT CInstrBlock::GetInstrNo(UINT nInstrIndex)
{
	if (nInstrIndex < m_vecInstrNo.size())
		return m_vecInstrNo[nInstrIndex];
	else {
		assert(false);
		return 0;
	}
}

BYTE * CInstrBlock::GetInstrIOs(UINT nInstrIndex)
{
	if (nInstrIndex < m_vecIOs.size()) {
		return m_vecIOs[nInstrIndex];
	}
	else {
		assert(false);
		return NULL;
	}
}

void CInstrBlock::DeleteInstrIOs(UINT nInstrIndex)
{
	if (nInstrIndex < m_vecIOs.size()) {
		if (m_vecIOs[nInstrIndex] != NULL) {
			delete m_vecIOs[nInstrIndex];
			m_vecIOs[nInstrIndex] = NULL;
		}
	}
}

bool CInstrBlock::ReplaceInstrIOs(UINT nInstrIndex, BYTE * pInstrIOs) 
{
	if (nInstrIndex < m_vecIOs.size()) {
		if (m_vecIOs[nInstrIndex] != NULL)
			delete m_vecIOs[nInstrIndex];

		m_vecIOs[nInstrIndex] = pInstrIOs;
		return true;
	}
	else {
		assert(false);
		return false;
	}
}

bool CInstrBlock::SetInstrNo(UINT nInstrIndex, UINT nInstrNo)
{
	if (nInstrIndex < m_vecInstrNo.size()) {
		m_vecInstrNo[nInstrIndex] = nInstrNo;
		return true;
	}
	else {
		assert(false);
		return false;
	}
}

REGISTER_TYPE CInstrBlock::GetParam(UINT nInstrIndex)
{
	if (nInstrIndex < m_vecParam.size())
		return m_vecParam[nInstrIndex];
	else {
		assert(false);
		return 0;
	}
}

bool CInstrBlock::SetParam(UINT nInstrIndex, REGISTER_TYPE param)
{
	if (nInstrIndex < m_vecParam.size()) {
		m_vecParam[nInstrIndex] = param;
		return true;
	}
	else {
		assert(false);
		return false;
	}
}

bool CInstrBlock::SetIO(UINT nInstrIndex, UINT nIOIndex, BYTE nIOValue)
{
	if (nInstrIndex < m_vecParam.size()) {
		m_vecIOs[nInstrIndex][nIOIndex] = nIOValue;
		return true;
	}
	else {
		assert(false);
		return false;
	}
}

strArchitectureSettings::strArchitectureSettings(UINT nModulesCount)
	: nModCount(nModulesCount), pModulesOrder(NULL), pModulesInverseOrder(NULL)
{
	pModulesOrder = new UINT[nModCount];
	pModulesInverseOrder = new UINT[nModCount];
}

strArchitectureSettings::strArchitectureSettings(const strArchitectureSettings & source)
{
	nModCount = source.nModCount;
	pModulesOrder = new UINT[nModCount];
	pModulesInverseOrder = new UINT[nModCount];

	memcpy(pModulesOrder, source.pModulesOrder, nModCount*sizeof(UINT));
	memcpy(pModulesInverseOrder, source.pModulesInverseOrder, nModCount*sizeof(UINT));
}

strArchitectureSettings::~strArchitectureSettings()
{
	delete[] pModulesOrder;
	delete[] pModulesInverseOrder;
}

CArchitecture::CArchitecture(UINT nRegCount, REGISTER_TYPE nDefRegMask, UINT nModCount, CModule ** pMods, UINT nInstrCount, CInstruction ** pInstrSet, strArchitectureParams * pStrParams, bool bDynamic)
{
	m_pArchParams = pStrParams;
	m_nRegistersCount = nRegCount;
	m_pRegisters = new REGISTER_TYPE[nRegCount];
	m_pRegisterMasks = new REGISTER_TYPE[nRegCount];
	m_bDynamicArchitecture = bDynamic;
	m_nDefRegMask = nDefRegMask;
	
	// Filling default register masks and values
	for (UINT i = 0; i < nRegCount; i++) {
		m_pRegisterMasks[i] = nDefRegMask;
		m_pRegisters[i] = REG_DEF_VAL;
	}

	m_nModulesCount = nModCount;
	m_nInstrCount = nInstrCount;

	m_nMaxPossibleInputs = m_nRegistersCount;

	if ((nModCount != 0) && (pMods != NULL)) {		
		m_pModules = new CModule *[nModCount];
		m_pModulesUsed = new bool[nModCount];
		for (UINT i = 0; i < nModCount; i++) {
			m_pModules[i] = pMods[i];
			m_pModulesUsed[i] = true;
			m_nMaxPossibleInputs += m_pModules[i]->GetOutputsCount();
		}
	}

	if (nInstrCount > 0) {
		m_pInstrSet = new CInstruction *[nInstrCount];
		for (UINT i = 0; i < nInstrCount; i++) {
			m_pInstrSet[i] = new CInstruction(*pInstrSet[i]);
		}
	}
}

CArchitecture::CArchitecture(strArchitectureParams * pStrParams)
{
	m_pArchParams = pStrParams;
	m_bDynamicArchitecture = true;
}

// Copy constructor
CArchitecture::CArchitecture(const CArchitecture & source)
{
	m_nRegistersCount = source.m_nRegistersCount;
	m_pRegisterMasks = new REGISTER_TYPE[m_nRegistersCount];
	m_pRegisters = new REGISTER_TYPE[m_nRegistersCount];
	m_nDefRegMask = source.m_nDefRegMask;

	for (UINT i = 0; i < m_nRegistersCount; i++) {
		m_pRegisterMasks[i] = source.m_pRegisterMasks[i];
		m_pRegisters[i] = source.m_pRegisters[i];
	}

	m_nModulesCount = source.m_nModulesCount;
	m_nInstrCount = source.m_nInstrCount;
	m_pArchParams = source.m_pArchParams;
	m_bDynamicArchitecture = source.m_bDynamicArchitecture;

	m_nMaxPossibleInputs = source.m_nMaxPossibleInputs;

	m_pModules = new CModule*[m_nModulesCount];
	m_pModulesUsed = new bool[m_nModulesCount];
	for (UINT i = 0; i < m_nModulesCount; i++) {
		m_pModules[i] = source.m_pModules[i];
		m_pModulesUsed[i] = source.m_pModulesUsed[i];
	}
	
	m_pInstrSet = source.m_pInstrSet;
	m_pInstrSet = new CInstruction*[m_nInstrCount];
	for (UINT i = 0; i < m_nInstrCount; i++)
		m_pInstrSet[i] = new CInstruction(*source.m_pInstrSet[i]);
}

CArchitecture::~CArchitecture()
{
	delete[] m_pRegisters;
	delete[] m_pRegisterMasks;

	for (UINT i = 0; i < m_nInstrCount; i++)
		delete m_pInstrSet[i];

	delete m_pInstrSet;
	delete[] m_pModules;
	delete[] m_pModulesUsed;
}

CModule * CArchitecture::GetModule(UINT nModIndex)
{
	if (nModIndex < m_nModulesCount)
		return m_pModules[nModIndex];
	else {
		assert(false);
		return NULL;
	}
}

CModule * CArchitecture::GetModuleByTopologyOrder(UINT nModOrder)
{
	if (nModOrder < m_nModulesCount) {
		UINT nModIndex = m_pArchSettings->pModulesOrder[nModOrder];
		return m_pModules[nModIndex];
	}
	else {
		assert(false);
		return NULL;
	}
}

UINT CArchitecture::GetModulesCount()
{
	return m_nModulesCount;
}

REGISTER_TYPE CArchitecture::GetRegisterValue(UINT nRegIndex)
{
	if (nRegIndex < m_nRegistersCount)
#ifndef FP_REGISTER_TYPE
		return m_pRegisters[nRegIndex] & m_pRegisterMasks[nRegIndex];
#else
		return m_pRegisters[nRegIndex];
#endif
	else {
		assert(false);
		return 0;
	}
}

REGISTER_TYPE CArchitecture::GetRegisterMask(UINT nRegIndex)
{
	if (nRegIndex < m_nRegistersCount)
		return m_pRegisterMasks[nRegIndex];
	else {
		assert(false);
		return 0;
	}
}

int CArchitecture::SetRegisterValue(UINT nRegIndex, REGISTER_TYPE nNewValue)
{
	if (nRegIndex < m_nRegistersCount) {
#ifndef FP_REGISTER_TYPE
		m_pRegisters[nRegIndex] = nNewValue & m_pRegisterMasks[nRegIndex];
#else
		m_pRegisters[nRegIndex] = nNewValue;
#endif
		return ERR_SUCCESS;
	}
	else {
		assert(false);
		return ERR_INTERNAL_ERROR;
	}
}

void CArchitecture::SetRegisterMask(UINT nRegIndex, REGISTER_TYPE nNewMask)
{
	assert(nRegIndex >= 0 && nRegIndex < m_nRegistersCount);
	m_pRegisterMasks[nRegIndex] = nNewMask;
}

UINT CArchitecture::GetRegistersCount()
{
	return m_nRegistersCount;
}

void CArchitecture::ResetRegisters()
{
	for (UINT i=0; i < m_nRegistersCount; i++)
		m_pRegisters[i] = REG_DEF_VAL;
}

void CArchitecture::ResetModules()
{
	for (UINT i = 0; i < m_nModulesCount; i++)
		m_pModules[i]->ResetFlags();
}

int CArchitecture::ExecuteProgram(CInstrBlock * pFirstInstr, CEnvironment * pEnvironment, UINT nMaxLogTime, UINT & nExecTime, bool bPrintDebug)
{
	ResetRegisters();
	ResetModules();
	CInstructionDecoder decoder(this);
	return decoder.Execute(pFirstInstr, pEnvironment, nMaxLogTime, nExecTime, bPrintDebug);
}

CInstruction * CArchitecture::GetInstruction(BYTE nInstrCode)
{
	if (nInstrCode > m_nInstrCount)
		nInstrCode = nInstrCode % m_nInstrCount;
	return m_pInstrSet[nInstrCode];
}

UINT CArchitecture::GetInstructionCount()
{
	return m_nInstrCount;
}

BYTE * CArchitecture::GetDecoded(CInstrBlock * pBlock, UINT nInstrIndex)
{
	UINT nInstrNo = pBlock->GetInstrNo(nInstrIndex);
	assert(nInstrNo < m_nInstrCount);
	CInstruction * pInstr = m_pInstrSet[nInstrNo];
	UINT nInstrSize = pInstr->GetSize();
	bool bParametric = pInstr->IsParametric();
	UINT nInsOutsCount = pInstr->GetInsOutsCount();

	UINT nDecodedSize = nInstrSize;
	if (pInstr->IsParametric())
		nDecodedSize += sizeof(REGISTER_TYPE);
	if (nInsOutsCount > 0)
		nDecodedSize += nInsOutsCount;

	BYTE * pDecoded = new BYTE[nDecodedSize];
	memcpy(pDecoded, pInstr->GetDecoded(), sizeof(DWORD));
	// Place the parameters if needed
	if (bParametric) {
		REGISTER_TYPE param = pBlock->GetParam(nInstrIndex);
		memcpy(&pDecoded[sizeof(DWORD)], &param, sizeof(REGISTER_TYPE));
	}

	BYTE * pDecodedInsOuts = bParametric ? &pDecoded[sizeof(DWORD) + sizeof(REGISTER_TYPE)] : &pDecoded[sizeof(DWORD)];
	if (nInsOutsCount > 0) {
		BYTE * pInstrInsOuts = pInstr->GetInsOuts();
		BYTE * pBlockInsOuts = pBlock->GetInstrIOs(nInstrIndex);
		for (UINT nActIO = 0; nActIO < nInsOutsCount; nActIO++) {
			if (pInstrInsOuts[nActIO] == 0xFF || (pInstrInsOuts[nActIO] & 0x80) == 0) {
				pDecodedInsOuts[nActIO] = pBlockInsOuts[nActIO];
			}
			else {
				if ((pInstrInsOuts[nActIO] & 0x40) == 0)
					pDecodedInsOuts[nActIO] = pInstrInsOuts[nActIO];
				else
					pDecodedInsOuts[nActIO] = pBlockInsOuts[nActIO];
			}
		}
	}

	return pDecoded;
}

bool CArchitecture::IsModuleUsed(UINT nInd)
{
	if (nInd < 0 || nInd >= m_nModulesCount) {
		assert(false);
		return false;
	}
	else {
		return m_pModulesUsed[nInd];
	}
}

void CArchitecture::SetModuleUsed(UINT nInd, bool bUsed)
{
	if (nInd < 0 || nInd >= m_nModulesCount) {
		assert(false);
	}
	else {
		m_pModulesUsed[nInd] = bUsed;
	}
}
