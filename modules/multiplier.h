#include "module.h"

class CMultiplier : public CModule
{
public:
	CMultiplier(UINT nFractBits = 0);
	int Execute(REGISTER_TYPE * pInputs, REGISTER_TYPE * pOutputs, UINT * pExecDelay = NULL);
	UINT GetArea() { return 100; }
	UINT GetPrice() { return 100; }
	virtual std::string GetLabel() { return "MULT"; }
protected:
	UINT m_nFractBits;
};