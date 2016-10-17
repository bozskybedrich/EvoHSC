#include "module.h"

class CComparatorModule : public CModule
{
public:
	CComparatorModule();
	int Execute(REGISTER_TYPE * pInputs, REGISTER_TYPE * pOutputs, UINT * pExecDelay);
	UINT GetArea() { return 50; }
	virtual std::string GetLabel() { return "COMP"; }
};
