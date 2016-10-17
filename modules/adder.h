#include "module.h"

class CAdder : public CModule
{
public:
	CAdder();
	int Execute(REGISTER_TYPE * pInputs, REGISTER_TYPE * pOutputs, UINT * pExecDelay = NULL);
	UINT GetArea() { return 100; }
	UINT GetPrice() { return 100; }
	virtual std::string GetLabel() { return "ADD"; }
};