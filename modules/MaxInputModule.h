#pragma once
#include "module.h"

class CMaxInputModule : public CModule
{
public:
	CMaxInputModule(REGISTER_TYPE ** pInputs, int nInsCount, int * pActIn, int nTestSetsCount, int * pActTestSet);
	virtual int Execute(REGISTER_TYPE * pInputs, REGISTER_TYPE * pOutputs, UINT * pExecDelay = NULL);
	UINT GetArea() { return 20; }
	virtual std::string GetLabel() { return "MAX_IN"; }
	void SetDefZero(bool bZero) { m_bSetZero = bZero; }
	void SetDefSign(bool bSign) { m_bSetSign = bSign; }
protected:
	REGISTER_TYPE ** m_pInputs;
	int m_nInsCount;
	int m_nInitialIn;
	int * m_pActIn;
	int m_nTestSetsCount;
	int * m_pActTestSet;
	bool m_bSetZero;
	bool m_bSetSign;
	bool m_bCyclic;
};
