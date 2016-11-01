#include "SigmoidInput.h"

CSigmoidInputModule::CSigmoidInputModule(REGISTER_TYPE ** pInputs, int nInsCount, int nIndex, int nTestSetsCount, int * pActTestSet)
	: CMaxInputModule(pInputs, nInsCount, &nIndex, nTestSetsCount, pActTestSet), m_nIndex(nIndex)
{
	m_nInputCount = 0;
	m_nOutputCount = 2;
}

int CSigmoidInputModule::Execute(REGISTER_TYPE * pInputs, REGISTER_TYPE * pOutputs, UINT * pExecDelay)
{
	bool bLastSet = *m_pActTestSet >= m_nTestSetsCount;
	if (bLastSet) {
		pOutputs[0] = 0;
		if (m_bSetZero)
			SetZero(true);
		if (m_bSetSign)
			SetSign(true);
	}
	else {
		pOutputs[0] = m_pInputs[*m_pActTestSet][m_nIndex];
		pOutputs[1] = ~pOutputs[0];

		if (m_bSetZero)
			SetZero(false);
		if (m_bSetSign)
			SetSign(false);
	}

	if (pExecDelay)
		*pExecDelay = 1;

	return ERR_SUCCESS;
}
