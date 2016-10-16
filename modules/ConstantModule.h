#pragma once
#include "module.h"

class CConstantModule : public CModule
{
public:
	CConstantModule(REGISTER_TYPE constant);
	virtual int Execute(REGISTER_TYPE * pInputs, REGISTER_TYPE * pOutputs, UINT * pExecDelay = NULL);
	UINT GetArea() { return 20; }
	void SetConst(REGISTER_TYPE constant);
protected:
	REGISTER_TYPE m_constant;
};
