#include "XorModule.h"

CXorModule::CXorModule()
	: CModule()
{
	m_nInputCount = 2;
	m_nOutputCount = 1;
}

int CXorModule::Execute(REGISTER_TYPE * pInputs, REGISTER_TYPE * pOutputs, UINT * pExecDelay)
{
	pOutputs[0] = (pInputs[0]) ^ (pInputs[1]);

	if (pExecDelay)
		*pExecDelay = 2;

	return ERR_SUCCESS;
}
