#pragma once
#include "MaxInputModule.h"

class CPixelInputModule : public CMaxInputModule
{
public:
	CPixelInputModule(REGISTER_TYPE ** pInputs, int nInsCount, int nIndex, int nTestSetsCount, int * pActTestSet);
	virtual int Execute(REGISTER_TYPE * pInputs, REGISTER_TYPE * pOutputs, UINT * pExecDelay = NULL);
protected:
	int m_nIndex;
};
