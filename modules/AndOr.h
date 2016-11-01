#include "module.h"

class CAndOr : public CModule
{
public:
	CAndOr(UINT nInputsCount);
	int Execute(REGISTER_TYPE * pInputs, REGISTER_TYPE * pOutputs, UINT * pExecDelay = NULL);
	UINT GetArea() { return 10; }
	UINT GetPrice() { return 10; }
	virtual std::string GetLabel() { return "ANDOR"; }
};