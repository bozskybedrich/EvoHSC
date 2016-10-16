#include "instrdecoder.h"
#include "errorstatus.h"
#include "architecture.h"
#include <assert.h>
#include <string.h>
#include <iostream>
#include <ctime>

#ifdef _DEBUG
	#define new MYDEBUG_NEW
#endif

CInstructionDecoder::CInstructionDecoder(CArchitecture * pArchitecture)
{
	m_pArchitecture = pArchitecture;
}

CInstructionDecoder::~CInstructionDecoder()
{
}

int CInstructionDecoder::DecodeAndExecute(void * pInstrAddr, DWORD nModulesMask, int * nOffset, UINT * pActLogicTime, CEnvironment * pEnvironment)
{
	void * pOrigInstr = pInstrAddr;
	assert(m_pArchitecture);
	*nOffset = 0;
	UINT nExecutionDelay = 0;

	if (!pInstrAddr) {
		assert(false);
		return ERR_INSTRUCTION_ERROR;
	}

	DWORD nHeader = 0;
	for (int nHeaderByte = 0; nHeaderByte < sizeof(DWORD); nHeaderByte++) {
		nHeader <<= 8;
		nHeader += *((BYTE *) pInstrAddr);
		pInstrAddr = ((BYTE *) pInstrAddr) + 1;
	}

	// Process special instructions
	if (nHeader == 0) {	// NOP instruction
		*nOffset = 0;
		return ERR_SUCCESS;
	}
	else if (nHeader == 0xFFFFFFFF) {	// RST instruction
		for (UINT nRegister = 0; nRegister < m_pArchitecture->GetRegistersCount(); nRegister++) {
			m_pArchitecture->SetRegisterValue(nRegister, REG_DEF_VAL);
		}
		for (UINT nModule = 0; nModule < m_pArchitecture->GetModulesCount(); nModule++) {
			m_pArchitecture->GetModule(nModule)->SetSign(false);
			m_pArchitecture->GetModule(nModule)->SetZero(false);
		}
		return ERR_SUCCESS;
	}

	// Create temporary variables for outputs (values will be copied 
	// to registers at the end of instruction execution)
	UINT nRegCount = m_pArchitecture->GetRegistersCount();
	REGISTER_TYPE * pTmpRegisters = new REGISTER_TYPE[nRegCount];
	bool * pTmpRegisterUsed = new bool[nRegCount];
	memset(pTmpRegisterUsed, 0, nRegCount * sizeof(bool));

	//pInstrAddr = (BYTE *) pInstrAddr + sizeof(DWORD);

	// true if constant should be loaded to register (it cannot be JMP instruction)
	bool bConst = ((nHeader & MOV_MASK) != 0);
	REGISTER_TYPE nConst;
	if (bConst) {
		nConst = *((REGISTER_TYPE *) pInstrAddr);
		pInstrAddr = (BYTE *) pInstrAddr + sizeof(REGISTER_TYPE);
		DWORD regIndex = (nHeader & CONST_LOAD_MASK) >> 24;
		//regIndex--;
		bool bRegUsed = true;
		// If it is MOV, IN or OUT operation
		if (nHeader & MOV_MASK) {
			if ((nHeader & IN_CONNECTION_MASK) == IN_CONNECTION_MASK) {
				nExecutionDelay = IO_DELAY;
				UINT nIndex = nConst;
				assert(nIndex >= 0 && nIndex < pEnvironment->GetInputsCount());
				// Load the input from the environment according to current time
				pTmpRegisters[regIndex] = pEnvironment->GetInputValue(nIndex, *pActLogicTime);
			}
			else if ((nHeader & OUT_CONNECTION_MASK) == OUT_CONNECTION_MASK) {
				nExecutionDelay = IO_DELAY;
				UINT nIndex = nConst;
				assert(nIndex >= 0 && nIndex < pEnvironment->GetOutputsCount());
				bRegUsed = false;
				// If there is buffer for outputs, store the output
				pEnvironment->InsertRealOutput(nIndex, *pActLogicTime + nExecutionDelay, m_pArchitecture->GetRegisterValue(regIndex));
			}
			// If it is LOAD const operation
			else if ((nHeader & LOAD_MASK) == LOAD_MASK) {
				nExecutionDelay = REG_MANIPULATION_DELAY;
				pTmpRegisters[regIndex] = nConst;
			}
			// Else it is MOV instruction
			else {
				nExecutionDelay = REG_MANIPULATION_DELAY;
				assert(nConst >=0 && nConst < nRegCount);
				pTmpRegisters[regIndex] = m_pArchitecture->GetRegisterValue(nConst);
			}
		}

		if (bRegUsed)
			pTmpRegisterUsed[regIndex] = true;
	}
	
	// If it is one of jump instructions
	if ((nHeader & JXX_MASK) != 0 && (nHeader & MOV_MASK) == 0) {
		if (nExecutionDelay < JUMP_DELAY)
			nExecutionDelay = JUMP_DELAY;

		REGISTER_TYPE jmpOffset = *((REGISTER_TYPE *) pInstrAddr);
		*nOffset = (DWORD) jmpOffset;

		if (nHeader & CONDITIONAL_JUMP_MASK) {
			UINT nIndex = GetModuleIndex(nHeader);
			assert(nIndex < m_pArchitecture->GetModulesCount());
			
			// Shift by 28 bits to get rid of unconditional jump bit
			// Therefore 1 is JMP, 2 is JZ... (see typedefs.h)
			switch ((nHeader & JXX_MASK) >> 28) {
			case JZ:
				if (!m_pArchitecture->GetModule(nIndex)->IsZeroSet())
					*nOffset = 0;
				break;
			case JS:
				if (!m_pArchitecture->GetModule(nIndex)->IsSignSet())
					*nOffset = 0;
				break;
			case JNZ:
				if (m_pArchitecture->GetModule(nIndex)->IsZeroSet())
					*nOffset = 0;
				break;
			case JNS:
				if (m_pArchitecture->GetModule(nIndex)->IsSignSet())
					*nOffset = 0;
				break;
			default:
				assert(false);
				break;
			}
		}
		
		*pActLogicTime += nExecutionDelay;
		delete[] pTmpRegisters;
		delete[] pTmpRegisterUsed;

		return ERR_SUCCESS; // Remove, if the jump should be combined with other instruction
	}

	DWORD nBitMask = 1;
	DWORD nMods = nHeader & ~HEADER_MASK;

	UINT nInstrExecDelay = 0;
	CInstrExecInfo instrExecInfo(nMods, nModulesMask, m_pArchitecture, (BYTE *) pInstrAddr);
	instrExecInfo.Execute(pTmpRegisters, pTmpRegisterUsed, nInstrExecDelay);

	if (nInstrExecDelay > nExecutionDelay)
		nExecutionDelay = nInstrExecDelay;
	
	/*for (UINT nMod = 0; nMod < instrInfo.GetModsCount(); nMod++) {
		// Get current module exec info
		CModExecInfo * pExecInfo = instrInfo.GetModExecInfo(nMod);
		
		// Get input connections from the instruction
		UINT nInputsCount = pCurrentModule->GetInputsCount();
		UINT nOutputsCount = pCurrentModule->GetOutputsCount();
			
		if ((nInputsCount == 0) && (nOutputsCount == 0))
			// If module is not connected to the architecture
			return ERR_INVALID_MODULE;
		else {
			// Fill the inputs from register
			REGISTER_TYPE * pInputs = new REGISTER_TYPE[nInputsCount];
			for (UINT nCurrentInput = 0; nCurrentInput < nInputsCount; nCurrentInput++) {
				BYTE inputIndex = *((BYTE *) pInstrAddr);

				if ((inputIndex & 0x80) != 0)
					pInputs[nCurrentInput] = (inputIndex & 0x7F);
				else {
					pInputs[nCurrentInput] = m_pArchitecture->GetRegisterValue(inputIndex);
#ifdef _DEBUG
					assert((UINT) (inputIndex & 0x0F) < nRegCount);
#endif
				}
				pInstrAddr = (BYTE *) pInstrAddr + 1;
			}
			// Create output array
			REGISTER_TYPE * pOutputs = new REGISTER_TYPE[nOutputsCount];

			// Execute instruction
			UINT nModExecDelay = 0;
			if (pCurrentModule->Execute(pInputs, pOutputs, &nModExecDelay) != ERR_SUCCESS)
				assert(false);
			if (nModExecDelay > nExecutionDelay)
				nExecutionDelay = nModExecDelay;

			// Fill output values to temporary registers
			for (UINT nCurrentOutput = 0; nCurrentOutput < nOutputsCount; nCurrentOutput++) {
				BYTE outputIndex = *((BYTE *) pInstrAddr);
				assert(pTmpRegisterUsed[outputIndex] == false);
				pTmpRegisters[outputIndex] = pOutputs[nCurrentOutput];
				pTmpRegisterUsed[outputIndex] = true;
				pInstrAddr = (BYTE *) pInstrAddr + 1;
			}

			delete pInputs;
			delete pOutputs;
		}
	}*/

	for (UINT nRegister = 0; nRegister < nRegCount; nRegister++) {
		if (pTmpRegisterUsed[nRegister]) {
#ifndef FP_REGISTER_TYPE
			m_pArchitecture->SetRegisterValue(nRegister, pTmpRegisters[nRegister] & m_pArchitecture->GetRegisterMask(nRegister));
#else
			m_pArchitecture->SetRegisterValue(nRegister, pTmpRegisters[nRegister]);
#endif
		}
	}

	delete[] pTmpRegisters;
	delete[] pTmpRegisterUsed;

	*pActLogicTime += nExecutionDelay;
	return ERR_SUCCESS;
}

int CInstructionDecoder::ExecuteProgram(void * pFirstInstr, UINT nProgramSize, CEnvironment * pEnvironment, UINT nMaxLogTime)
{
	UINT nActLogicTime = 0;

	void * pProgramEnd = (BYTE *) pFirstInstr + nProgramSize;
	void * pActInstr = pFirstInstr;
	while (pActInstr < pProgramEnd && nActLogicTime < nMaxLogTime) {
		// Get the instruction code, decode it and place the expanded form in new array for execution
		BYTE nInstrCode = * ((BYTE *) pActInstr);
		CInstruction * pInstr = m_pArchitecture->GetInstruction(nInstrCode);
		nActLogicTime += pInstr->GetProcessingDelay();

		UINT nInstrSize = pInstr->GetSize();
		bool bParametric = pInstr->IsParametric();
		BYTE * pDecoded = new BYTE[pInstr->IsParametric() ? nInstrSize + sizeof(REGISTER_TYPE) : nInstrSize];
		memcpy(pDecoded, pInstr->GetDecoded(), sizeof(DWORD));
		pActInstr = (BYTE *) pActInstr + sizeof(BYTE);
		// Place the parameters if needed
		if (bParametric) {
			memcpy(&pDecoded[sizeof(DWORD)], pActInstr, sizeof(REGISTER_TYPE));
			pActInstr = (BYTE *) pActInstr + sizeof(REGISTER_TYPE);
		}
		if (nInstrSize > sizeof(DWORD)) {
			memcpy(&pDecoded[sizeof(DWORD) + (bParametric ? sizeof(REGISTER_TYPE) : 0)], &pInstr->GetDecoded()[sizeof(DWORD)], nInstrSize - sizeof(DWORD));
		}

		int errCode, nOffset;
		if ((errCode = DecodeAndExecute(pDecoded, 0x00FFFFFF, &nOffset, &nActLogicTime, pEnvironment)) != ERR_SUCCESS)
			return errCode;

		// Process the result of jump instruction
		assert(nOffset == 0 || ((((BYTE *) pActInstr + nOffset) > pFirstInstr) && ((BYTE *) pActInstr + nOffset < (BYTE *) pFirstInstr + nProgramSize)));
		pActInstr = (BYTE *) pActInstr + nOffset;

		delete[] pDecoded;
	}

	return ERR_SUCCESS;
}

int CInstructionDecoder::Execute(CInstrBlock * pFirstInstr, CEnvironment * pEnvironment, UINT nMaxLogTime, UINT & nExecTime)
{
	UINT nActLogicTime = 0;
	
	CInstrBlock * pActInstr = pFirstInstr;
	while (pActInstr && (nActLogicTime < nMaxLogTime)) {
		bool bJump = false;
		for (UINT nInstrIndex = 0; nInstrIndex < pActInstr->GetInstrCount(); nInstrIndex++) {
			// Get the instruction expanded form in new array for execution
			BYTE * pDecoded = m_pArchitecture->GetDecoded(pActInstr, nInstrIndex);
			DWORD nModulesMask = pActInstr->GetModulesMask();
			int errCode = 0, nOffset = 0;
			if ((errCode = DecodeAndExecute(pDecoded, nModulesMask, &nOffset, &nActLogicTime, pEnvironment)) != ERR_SUCCESS)
				return errCode;

			// Process the result of jump instruction
			if (nOffset != 0) {
				bJump = true;
				int nInnerOffset = nInstrIndex + nOffset;
				if (nInnerOffset < 0)
					nOffset = nInnerOffset;
				else if (nInnerOffset >= pActInstr->GetInstrCount())
					nOffset = nInnerOffset - pActInstr->GetInstrCount();
				else {
					nInstrIndex = nInnerOffset;
					continue;
				}

				CInstrBlock * pNextInstr = pActInstr;
				while (pNextInstr && nOffset != 0) {
					if (nOffset > 0) {
						pNextInstr = pNextInstr->GetNext();
						nOffset--;
					}
					else {
						pNextInstr = pNextInstr->GetPrev();
						nOffset++;
					}

					if (pNextInstr)
						pActInstr = pNextInstr;
				}
			}

			delete pDecoded;
		}
		
		if (!bJump)
			pActInstr = pActInstr->GetNext();
	}

	nExecTime = nActLogicTime;
	return ERR_SUCCESS;
}

UINT CInstructionDecoder::GetModuleIndex(DWORD nHeader)
{
	DWORD nBitMask = 1;
	DWORD nMods = nHeader & ~HEADER_MASK;

	for (UINT bitIndex = 0; bitIndex < sizeof(DWORD)*8; bitIndex++) {
		if (nMods & nBitMask) {
			return bitIndex;
		}
		nBitMask <<= 1;
	}

	assert(false);
	return 0;
}

CInstrExecInfo::CInstrExecInfo(DWORD nMods, DWORD nModulesMask, CArchitecture * pArchitecture, BYTE * pIOsStart)
{
	// Initialize the variables needed
	UINT nModCount = pArchitecture->GetModulesCount();
	strArchitectureSettings * pSettings = pArchitecture->GetArchSettings();
	BYTE * pCurrentIOs = pIOsStart;

	// Initialize member variables
	m_pArchitecture = pArchitecture;
	m_nModsCount = 0;
	m_nPossibleInputsCount = m_pArchitecture->GetRegistersCount();
	m_pModsExecOrder = new CModExecInfo*[nModCount];
	memset(m_pModsExecOrder, 0, nModCount*sizeof(CModExecInfo*));

	CModExecInfo ** pModInfos = new CModExecInfo*[nModCount];
	memset(pModInfos, 0, nModCount*sizeof(CModExecInfo*));

	// Go through the modules in order specified by their position in the architecture
	for (UINT nMod = 0; nMod < nModCount; nMod++) {
		DWORD nBitMask = 1 << nMod;

		// Check whether the module is involved in instruction
		if ((nMods & nBitMask) != 0) {
			// Get current module
			CModule * pCurrentModule = pArchitecture->GetModule(nMod);
			// Get input connections from the instruction
			UINT nInputsCount = pCurrentModule->GetInputsCount();
			UINT nOutputsCount = pCurrentModule->GetOutputsCount();

			// If the module is used and is connected to the architecture, store it in the execution order buffer
			if (pArchitecture->IsModuleUsed(nMod) && ((nBitMask & nModulesMask) != 0) && ((nInputsCount != 0) || (nOutputsCount != 0))) {
				CModExecInfo * pInfo = new CModExecInfo(pCurrentModule, pCurrentIOs);
				pModInfos[nMod] = pInfo;
				m_nModsCount++;
				m_nPossibleInputsCount += nOutputsCount;
			}

			// Move the current IOs pointer to IO bytes for next module
			pCurrentIOs += (nInputsCount + nOutputsCount);
		}
	}

	UINT nCurrPos = 0;
	for (UINT nMod = 0; nMod < nModCount; nMod++) {
		UINT nInd = pSettings->pModulesOrder[nMod];
		if (pModInfos[nInd]) {
			assert(m_pModsExecOrder[nCurrPos] == NULL);
			assert(m_pModsExecOrder[0] == NULL || m_pModsExecOrder[0] != m_pModsExecOrder[1]);
			m_pModsExecOrder[nCurrPos] = pModInfos[nInd];
			nCurrPos++;
		}
	}

	assert(nCurrPos == m_nModsCount);

	m_pPossibleInputs = new strInputWithDelay[m_nPossibleInputsCount];

	// Free the garbage
	delete[] pModInfos;
}

CInstrExecInfo::~CInstrExecInfo()
{
	// Free module execution infos created in constructor
	for (UINT nMod = 0; nMod < m_nModsCount; nMod++)
		delete m_pModsExecOrder[nMod];

	// Free the mod exec infos array
	delete m_pModsExecOrder;

	// Delete possible inputs array
	delete[] m_pPossibleInputs;
}

CModExecInfo * CInstrExecInfo::GetModExecInfo(UINT nIndex)
{
	if (nIndex < m_nModsCount)
		return m_pModsExecOrder[nIndex];
	else {
		assert(false);
		return NULL;
	}
}

int CInstrExecInfo::Execute(REGISTER_TYPE * pTmpRegisters, bool * pTmpRegistersUsed, UINT & nInstrExecDelay)
{
	// Fill current register values to the array of possible module inputs
	UINT nCurrPossibleMax = m_pArchitecture->GetRegistersCount();
	UINT nMaxModuleDelay = 0;
	for (UINT nReg = 0; nReg < nCurrPossibleMax; nReg++) {
		m_pPossibleInputs[nReg].value = m_pArchitecture->GetRegisterValue(nReg);
		m_pPossibleInputs[nReg].nDelay = 0;
	}

	for (UINT nMod = 0; nMod < m_nModsCount; nMod++) {
		CModExecInfo * pModExecInfo = m_pModsExecOrder[nMod];
		CModule * pMod = pModExecInfo->GetModule();
		UINT nMaxInputDelay = 0;

		// Create and fill inputs
		UINT nInsCount = pMod->GetInputsCount();
		REGISTER_TYPE * pInputs = new REGISTER_TYPE[nInsCount];
		for (UINT nInput = 0; nInput < nInsCount; nInput++) {
			UINT nIndex = pModExecInfo->GetInputIndex(nInput);
			
			if ((nIndex & 0x80) != 0 && nIndex != 0xFF)
					pInputs[nInput] = (nIndex & 0x7F);
			else {
				nIndex = nIndex % nCurrPossibleMax;
				pInputs[nInput] = m_pPossibleInputs[nIndex].value;
				if (m_pPossibleInputs[nIndex].nDelay > nMaxInputDelay)
					nMaxInputDelay = m_pPossibleInputs[nIndex].nDelay;
			}
		}

		// Create storage for outputs and execute module
		UINT nOutsCount = pMod->GetOutputsCount();
		REGISTER_TYPE * pOutputs = new REGISTER_TYPE[nOutsCount];
		UINT nModDelay = 0;
		int nRet = pMod->Execute(pInputs, pOutputs, &nModDelay);
		if (nRet != ERR_SUCCESS) {
			delete[] pInputs;
			delete[] pOutputs;
			return nRet;
		}

		nModDelay += nMaxInputDelay;
		if (nModDelay > nMaxModuleDelay)
			nMaxModuleDelay = nModDelay;

		// Fill output values to temporary registers
		for (UINT nCurrentOutput = 0; nCurrentOutput < nOutsCount; nCurrentOutput++) {
			BYTE outputIndex = pModExecInfo->GetOutputIndex(nCurrentOutput);
			// 0xFF value is used if the module output should not be transferred to register
			if (outputIndex != 0xFF) {
				outputIndex %= m_pArchitecture->GetRegistersCount();
				//assert(pTmpRegistersUsed[outputIndex] == false);
				pTmpRegisters[outputIndex] = pOutputs[nCurrentOutput];
				pTmpRegistersUsed[outputIndex] = true;
			}
			
			// Fill output to the array of possible inputs
			m_pPossibleInputs[nCurrPossibleMax].value = pOutputs[nCurrentOutput];
			m_pPossibleInputs[nCurrPossibleMax].nDelay = nModDelay;
			nCurrPossibleMax++;
		}

		delete[] pInputs;
		delete[] pOutputs;
	}

	nInstrExecDelay = nMaxModuleDelay;
	return ERR_SUCCESS;
}

CModExecInfo::CModExecInfo(CModule * pMod, BYTE * pIOs)
	: m_pModule(pMod), m_pInputsIndices(pIOs)
{
	if (m_pModule) {
		UINT nInsCount = m_pModule->GetInputsCount();
		m_pOutputsRegIndices = pIOs + nInsCount;
	}
	else
		assert(false);
}

CModExecInfo::~CModExecInfo()
{

}

BYTE CModExecInfo::GetInputIndex(UINT nInput)
{
	if (nInput < m_pModule->GetInputsCount()) {
		return *(m_pInputsIndices + nInput);
	}
	else {
		assert(false);
		return 0;
	}
}

BYTE CModExecInfo::GetOutputIndex(UINT nOutput)
{
	if (nOutput < m_pModule->GetOutputsCount()) {
		return *(m_pOutputsRegIndices + nOutput);
	}
	else {
		assert(false);
		return 0;
	}
}
