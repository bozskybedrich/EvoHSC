#include "module.h"

class CXorModule : public CModule
{
public:
	CXorModule();
	int Execute(REGISTER_TYPE * pInputs, REGISTER_TYPE * pOutputs, UINT * pExecDelay);
	UINT GetArea() { return 50; }
};
