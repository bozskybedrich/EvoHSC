#pragma once
#include "typedefs.h"
#include <map>

struct strTimeVal
{
	strTimeVal() : time(0), val(0) {}
	strTimeVal(UINT nTime, REGISTER_TYPE nVal) : time(nTime), val(nVal) {}
	UINT time;
	REGISTER_TYPE val;
};

struct strInputTimeline
{
	strInputTimeline() : nChanges(0), pChanges(NULL) {}
	UINT nChanges;
	strTimeVal *pChanges;
};

struct strDynamicTimeVal : public strTimeVal
{
	strDynamicTimeVal() : strTimeVal(), pPred(NULL), pSucc(NULL) {}
	strDynamicTimeVal(UINT nTime, REGISTER_TYPE nVal) : strTimeVal(nTime, nVal), pPred(NULL), pSucc(NULL) {}
	strDynamicTimeVal * pPred;
	strDynamicTimeVal * pSucc;
};

class CDynamicTimeline {
public:
	CDynamicTimeline() : m_pFirst(NULL), m_pLast(NULL), nCount(0) {}
	~CDynamicTimeline();
	// Manipulation methods
	void InsertItem(strDynamicTimeVal * pVal);
	strDynamicTimeVal * GetFirst() { return m_pFirst; }
	REGISTER_TYPE GetVal(UINT time);
protected:
	strDynamicTimeVal * m_pFirst;
	strDynamicTimeVal * m_pLast;
	UINT nCount;
};

typedef REGISTER_TYPE (*envCallbackIn)(UINT nIndex, UINT nTime);
typedef void (*envCallbackOut)(UINT nIndex, UINT nTime, REGISTER_TYPE val);
typedef void (*envResetAfterEval)();

class CEnvironment
{
public:
	CEnvironment(const char * pXml);
	CEnvironment(envCallbackIn pCallbackIn, envCallbackOut pCallbackOut, UINT nInputsCount, UINT nOutputsCount, envResetAfterEval pResetAfterEval = NULL);
	~CEnvironment();
	int Parse(const char * pXml);

	// Get methods
	REGISTER_TYPE GetInputValue(UINT nIndex, UINT time);
	void InsertRealOutput(UINT nIndex, UINT time, REGISTER_TYPE val);
	UINT GetInputsCount() { return m_nInputsCount; }
	UINT GetOutputsCount() { return m_nOutputsCount; }
	CDynamicTimeline * GetOutput(UINT nIndex) { return nIndex < m_nOutputsCount ? m_pRealOutputs[nIndex] : NULL; }
	CDynamicTimeline * GetExpectedOutput(UINT nIndex) { return nIndex < m_nOutputsCount ? m_pExpectedOutputs[nIndex] : NULL; }
	void DeleteRealOutputs();
	void ResetRealOutputs();
	void ResetAfterEval();
	void PrintOutputs();
protected:
	UINT m_nInputsCount;
	UINT m_nOutputsCount;
	CDynamicTimeline ** m_pInputs;
	CDynamicTimeline ** m_pExpectedOutputs;
	CDynamicTimeline ** m_pRealOutputs;
	envCallbackIn m_pCallbackIn;
	envCallbackOut m_pCallbackOut;
	envResetAfterEval m_pResetAfterEval;
};
