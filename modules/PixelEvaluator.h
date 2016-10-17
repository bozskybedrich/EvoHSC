#include "module.h"

class CPixelEvaluatorModule : public CModule
{
public:
	CPixelEvaluatorModule();
	int Execute(REGISTER_TYPE * pInputs, REGISTER_TYPE * pOutputs, UINT * pExecDelay);
	UINT GetArea() { return 50; }
	virtual std::string GetLabel() { return "PIX_EVAL"; }
};
