#include "multiplier.h"

#ifdef _DEBUG
	#define new MYDEBUG_NEW
#endif

CMultiplier::CMultiplier(UINT nFractBits) :
	CModule()
{
	m_nInputCount = 2;
	m_nOutputCount = 1;
	m_nFractBits = nFractBits;
}

int CMultiplier::Execute(REGISTER_TYPE * pInputs, REGISTER_TYPE * pOutputs, UINT * pExecDelay)
{
	pOutputs[0] = pInputs[0] * pInputs[1];
	if (m_nFractBits > 0)
		pOutputs[0] >>= m_nFractBits;

	SetZero(pOutputs[0] == 0);
	SetSign(pOutputs[0] < 0);

	if (pExecDelay)
		*pExecDelay = 20;

	return ERR_SUCCESS;
}