#include "ComparatorModule.h"

CComparatorModule::CComparatorModule()
	: CModule()
{
	m_nInputCount = 2;
	m_nOutputCount = 2;
}

int CComparatorModule::Execute(REGISTER_TYPE * pInputs, REGISTER_TYPE * pOutputs, UINT * pExecDelay)
{
	pOutputs[0] = pInputs[0] < pInputs[1] ? pInputs[0] : pInputs[1];
	pOutputs[1] = pInputs[0] < pInputs[1] ? pInputs[1] : pInputs[0];

	if (pExecDelay)
		*pExecDelay = 2;

	return ERR_SUCCESS;
}
