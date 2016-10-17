#pragma once
#include <stdlib.h>
#include "typedefs.h"
#include "../modules/modules.h"
#include "environment.h"
#include <string>
#include <vector>

typedef enum {
	INST_JMP,
	INST_MOV,
	INST_IN,
	INST_OUT,
	INST_LOAD,
	INST_OTHER
} INSTR_TYPE;

class CInstruction {
public:
	CInstruction(UINT size, BYTE * pDecoded, bool bNeedParameter = false, const std::string & sDescription = std::string(), UINT nDelay = 1, BYTE * pInsOuts = NULL, UINT nIOsCount = 0);
	CInstruction(const CInstruction & source);
	~CInstruction();
	// Get methods
	UINT GetSize() { return m_nSize; }
	BYTE * GetDecoded() { return m_pDecoded; }
	bool IsParametric() { return m_bNeedParameter; }
	UINT GetProcessingDelay() { return m_nProcessingDelay; }
	INSTR_TYPE GetType() { return m_eType; }
	std::string & GetDescription() { return m_sDescription; }
	UINT GetInsOutsCount() { return m_nInsOutsCount; }
	BYTE * GetInsOuts() { return m_pInsOuts; }

protected:
	UINT m_nSize;
	BYTE * m_pDecoded;
	bool m_bNeedParameter;
	UINT m_nProcessingDelay;
	INSTR_TYPE m_eType;
	std::string m_sDescription;
	BYTE * m_pInsOuts;
	UINT m_nInsOutsCount;
};

typedef enum {
	SINGLE_INSTR,
	SERIAL_INSTR
} INSTR_BLOCK_TYPE;

class CArchitecture;

class CInstrBlock
{
public:
	CInstrBlock(UINT nInstrNo, REGISTER_TYPE param, CInstrBlock * pInsertAfter, BYTE * pIOs, DWORD nModulesMask); // Create instruction using the given parameters and position
	CInstrBlock(CInstrBlock * pInsertAfter); // Create random instruction
	CInstrBlock(CInstrBlock * pSourceBlock, CInstrBlock * pInsertAfter, CArchitecture * pArch);
	~CInstrBlock();
	
	// Get functions
	inline CInstrBlock * GetPrev() { return m_pPrev; }
	inline CInstrBlock * GetNext() { return m_pNext; }
	UINT GetInstrNo(UINT nInstrIndex);
	DWORD GetModulesMask() { return m_nModulesMask; }
	//BYTE * GetDecoded(UINT nInstrNo, UINT nInstrIndex);
	REGISTER_TYPE GetParam(UINT nInstrIndex);
	UINT GetInstrCount() { return m_vecInstrNo.size(); }
	BYTE * GetInstrIOs(UINT nInstrIndex);
	void DeleteInstrIOs(UINT nInstrIndex);
	bool ReplaceInstrIOs(UINT nInstrIndex, BYTE * pInstrIOs);

	// Set functions
	inline void SetPrev(CInstrBlock * pPrev) { m_pPrev = pPrev; }
	inline void SetNext(CInstrBlock * pNext) { m_pNext = pNext; }
	bool SetInstrNo(UINT nInstrIndex, UINT nInstrNo);
	bool SetParam(UINT nInstrIndex, REGISTER_TYPE param);
	bool SetIO(UINT nInstrIndex, UINT nIOIndex, BYTE nIOValue);
	bool SetModulesMask(DWORD nModulesMask) { m_nModulesMask = nModulesMask; }

	// Placement functions
	void InsertAfter(CInstrBlock * pInsertAfter);
	inline CInstrBlock * ReplacePrev(CInstrBlock * pPrev)
	{
		CInstrBlock * pRet = m_pPrev;
		m_pPrev = pPrev;
		return pRet;
	}

	inline CInstrBlock * ReplaceNext(CInstrBlock * pNext)
	{
		CInstrBlock * pRet = m_pNext;
		m_pNext = pNext;
		return pRet;
	}

protected:
	std::vector<UINT> m_vecInstrNo;
	std::vector<REGISTER_TYPE> m_vecParam;
	std::vector<BYTE *> m_vecIOs;
	CInstrBlock * m_pPrev;
	CInstrBlock * m_pNext;
	DWORD m_nModulesMask;
};

struct strArchitectureParams {
	// Default constructor - cannot be used as valid parameter for individual generation
	strArchitectureParams() :
		nMaxRegistersCount(0),
		pRegistersMaxWidths(NULL),
		nMaxModulesCount(0),
		pAllowedModules(NULL),
		nMaxInstructionCount(0),
		pAllowedInstructions(NULL) {}

	UINT nMaxRegistersCount;
	REGISTER_TYPE * pRegistersMaxWidths;
	UINT nMaxModulesCount;
	CModule ** pAllowedModules;
	UINT nMaxInstructionCount;
	CInstruction ** pAllowedInstructions;
};

struct strArchitectureSettings {
	strArchitectureSettings(const strArchitectureSettings & source);
	strArchitectureSettings(UINT nModulesCount);
	~strArchitectureSettings();

	UINT nModCount;
	UINT * pModulesOrder;
	UINT * pModulesInverseOrder;
};

class CArchitecture {
public:
	CArchitecture(UINT nRegCount, REGISTER_TYPE nDefRegMask = 0xFFFFFFFF, UINT nModCount = 0, CModule ** pMods = NULL, UINT nInstrCount = 0, CInstruction ** pInstrSet = NULL, strArchitectureParams * pStrParams = NULL, bool bDynamic = true);
	CArchitecture(strArchitectureParams * pStrParams);
	CArchitecture(const CArchitecture & source);
	~CArchitecture();
	void SetRegistersCount(UINT nRegCount, REGISTER_TYPE nDefRegMask = 0xFFFFFFFF);
	UINT GetRegistersCount();
	int AddRegister(REGISTER_TYPE nRegMask = 0xFFFFFFFF);
	int DelRegister(UINT nRegIndex = 0);
	REGISTER_TYPE GetRegisterValue(UINT nRegIndex);
	REGISTER_TYPE GetRegisterMask(UINT nRegIndex);
	int SetRegisterValue(UINT nRegIndex, REGISTER_TYPE nNewValue);
	void SetRegisterMask(UINT nRegIndex, REGISTER_TYPE nNewMask);
	CModule * GetModule(UINT nModIndex);
	CModule * GetModuleByTopologyOrder(UINT nModIndex);
	UINT GetModulesCount();
	int ExecuteProgram(CInstrBlock * pFirstInstr, CEnvironment * pEnvironment, UINT nMaxLogTime, UINT & nExecTime, bool bPrintDebug = false);
	CInstruction * GetInstruction(BYTE nInstrCode);
	UINT GetInstructionCount();
	BYTE * GetDecoded(CInstrBlock * pBlock, UINT nInstrIndex);
	strArchitectureParams * GetArchParams() { return m_pArchParams; }
	strArchitectureSettings * GetArchSettings() { return m_pArchSettings; }
	void SetArchSettings(strArchitectureSettings * pArchSettings) { m_pArchSettings = pArchSettings; }
	bool IsDynamic() { return m_bDynamicArchitecture; }
	void ResetRegisters();
	void ResetModules();
	bool IsModuleUsed(UINT nInd);
	void SetModuleUsed(UINT nInd, bool bUsed);
	REGISTER_TYPE GetDefRegMask() { return m_nDefRegMask; }
	UINT GetMaxPossibleInputs() { return m_nMaxPossibleInputs; }

protected:
	UINT m_nRegistersCount;
	REGISTER_TYPE * m_pRegisterMasks;
	REGISTER_TYPE * m_pRegisters;
	REGISTER_TYPE m_nDefRegMask;
	UINT m_nModulesCount;
	CModule ** m_pModules;
	bool * m_pModulesUsed;
	UINT m_nInstrCount;
	CInstruction ** m_pInstrSet;
	strArchitectureParams * m_pArchParams;
	strArchitectureSettings * m_pArchSettings;
	UINT m_nMaxPossibleInputs;

	// Should be true in case when each individual has its own copy of the architecture
	bool m_bDynamicArchitecture;
};