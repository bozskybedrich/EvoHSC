#include "typedefs.h"
#include "environment.h"

class CArchitecture;
class CInstrBlock;
class CModule;

class CModExecInfo
{
public:
	CModExecInfo(CModule * pMod, BYTE * pIOs);
	~CModExecInfo();
	int Execute(REGISTER_TYPE * pInputs);
	CModule * GetModule() { return m_pModule; }
	BYTE GetInputIndex(UINT nInput);
	BYTE GetOutputIndex(UINT nOutput);
protected:
	CModule * m_pModule;
	BYTE * m_pInputsIndices;
	BYTE * m_pOutputsRegIndices;
};

struct strInputWithDelay
{
	REGISTER_TYPE value;
	UINT nDelay;
};

class CInstrExecInfo
{
public:
	CInstrExecInfo(DWORD nMods, DWORD nModulesMask, CArchitecture * pArchitecture, BYTE * pIOsStart);
	~CInstrExecInfo();
	UINT GetModsCount() { return m_nModsCount; }
	CModExecInfo * GetModExecInfo(UINT nIndex);
	int Execute(REGISTER_TYPE * pTmpRegisters, bool * pTmpRegistersUsed, UINT & nInstrExecDelay, bool bPrintDebug = false);
protected:
	UINT m_nModsCount;
	CModExecInfo ** m_pModsExecOrder;
	CArchitecture * m_pArchitecture;
	strInputWithDelay * m_pPossibleInputs;
	UINT m_nPossibleInputsCount;
};

class CInstructionDecoder {
public:
	CInstructionDecoder(CArchitecture * pArchitecture);
	~CInstructionDecoder();

	// Decodes current instruction, sets the connections 
	// and returns the address of the next instruction
	int DecodeAndExecute(void * pInstrAddr, DWORD nModulesMask, int * nOffset, UINT * pActLogicTime, CEnvironment * pEnvironment, bool bPrintDebug = false);
	int ExecuteProgram(void * pFirstInstr, UINT nProgramSize, CEnvironment * pEnvironment, UINT nMaxLogTime);
	int Execute(CInstrBlock * pFirstInstr, CEnvironment * pEnvironment, UINT nMaxLogTime, UINT & nExecTime, bool bPrintDebug = false);
	UINT GetModuleIndex(DWORD nHeader);

protected:
	CArchitecture * m_pArchitecture;
};