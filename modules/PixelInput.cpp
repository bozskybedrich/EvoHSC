#include "PixelInput.h"

CPixelInputModule::CPixelInputModule(REGISTER_TYPE ** pInputs, int nInsCount, int nIndex, int nTestSetsCount, int * pActTestSet)
	: CMaxInputModule(pInputs, nInsCount, &m_nIndex, nTestSetsCount, pActTestSet), m_nIndex(nIndex)
{
	m_nInputCount = 0;
	m_nOutputCount = 2;
}

int CPixelInputModule::Execute(REGISTER_TYPE * pInputs, REGISTER_TYPE * pOutputs, UINT * pExecDelay)
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
		pOutputs[1] = (pOutputs[0] == 0 || pOutputs[0] == 0xFF) ? 1 : 0;

		if (m_bSetZero)
			SetZero(false);
		if (m_bSetSign)
			SetSign(false);
	}

	if (pExecDelay)
		*pExecDelay = 1;

	return ERR_SUCCESS;
}
