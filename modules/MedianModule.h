#include "module.h"

class CMedianModule : public CModule
{
public:
	CMedianModule();
	int Execute(REGISTER_TYPE * pInputs, REGISTER_TYPE * pOutputs, UINT * pExecDelay);
	UINT GetArea() { return 50; }
	virtual std::string GetLabel() { return "MEDIAN"; }
};
