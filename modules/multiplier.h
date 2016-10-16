#include "module.h"

class CMultiplier : public CModule
{
public:
	CMultiplier();
	int Execute(REGISTER_TYPE * pInputs, REGISTER_TYPE * pOutputs, UINT * pExecDelay = NULL);
	UINT GetArea() { return 100; }
	UINT GetPrice() { return 100; }
};