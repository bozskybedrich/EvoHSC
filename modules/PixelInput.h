#pragma once
#include "MaxInputModule.h"
#include <sstream>

class CPixelInputModule : public CMaxInputModule
{
public:
	CPixelInputModule(REGISTER_TYPE ** pInputs, int nInsCount, int nIndex, int nTestSetsCount, int * pActTestSet);
	virtual int Execute(REGISTER_TYPE * pInputs, REGISTER_TYPE * pOutputs, UINT * pExecDelay = NULL);
	virtual std::string GetLabel() { std::stringstream ss; ss << "PIX_IN" << m_nIndex; return ss.str(); }
protected:
	int m_nIndex;
};
