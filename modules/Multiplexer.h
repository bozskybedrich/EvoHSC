#include "module.h"

class CMultiplexerModule : public CModule
{
public:
	CMultiplexerModule();
	int Execute(REGISTER_TYPE * pInputs, REGISTER_TYPE * pOutputs, UINT * pExecDelay);
	UINT GetArea() { return 50; }
	virtual std::string GetLabel() { return "MUX"; }
};
