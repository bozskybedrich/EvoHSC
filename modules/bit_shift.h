#include "module.h"

class CBitShifter : public CModule
{
public:
	CBitShifter();
	int Execute(REGISTER_TYPE * pInputs, REGISTER_TYPE * pOutputs, UINT * pExecDelay = NULL);
	UINT GetArea() { return 10; }
	UINT GetPrice() { return 10; }
	virtual std::string GetLabel() { return "SHIFT"; }
};