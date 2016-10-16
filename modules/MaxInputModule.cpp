#include "MaxInputModule.h"

CMaxInputModule::CMaxInputModule(REGISTER_TYPE ** pInputs, int nInsCount, int * pActIn, int nTestSetsCount, int * pActTestSet)
	: CModule(), m_pInputs(pInputs), m_nInsCount(nInsCount), m_nInitialIn(*pActIn), m_pActIn(pActIn), m_nTestSetsCount(nTestSetsCount), m_pActTestSet(pActTestSet)
{
	m_nInputCount = 0;
	m_nOutputCount = 1;
	m_bSetZero = true;
	m_bSetSign = false;
	m_bCyclic = true;
}

int CMaxInputModule::Execute(REGISTER_TYPE * pInputs, REGISTER_TYPE * pOutputs, UINT * pExecDelay)
{
	bool bLastIn = *m_pActIn >= m_nInsCount;
	bool bLastSet = *m_pActTestSet >= m_nTestSetsCount;
	if (bLastSet || (bLastIn && !m_bCyclic)) {
		pOutputs[0] = 0;
		if (m_bSetZero)
			SetZero(true);
		if (m_bSetSign)
			SetSign(true);
	}
	else {
		if (bLastIn)
			*m_pActIn = m_nInitialIn;

		pOutputs[0] = m_pInputs[*m_pActTestSet][*m_pActIn];
		*m_pActIn = *m_pActIn + 1;
		if (m_bSetZero)
			SetZero(false);
		if (m_bSetSign)
			SetSign(false);
	}

	if (pExecDelay)
		*pExecDelay = 1;

	return ERR_SUCCESS;
}
