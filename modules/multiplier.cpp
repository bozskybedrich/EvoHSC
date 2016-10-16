#include "multiplier.h"

#ifdef _DEBUG
	#define new MYDEBUG_NEW
#endif

CMultiplier::CMultiplier() :
	CModule()
{
	m_nInputCount = 2;
	m_nOutputCount = 1;
}

int CMultiplier::Execute(REGISTER_TYPE * pInputs, REGISTER_TYPE * pOutputs, UINT * pExecDelay)
{
	pOutputs[0] = pInputs[0] * pInputs[1];

	SetZero(pOutputs[0] == 0);
	SetSign(pOutputs[0] < 0);

	if (pExecDelay)
		*pExecDelay = 20;

	return ERR_SUCCESS;
}