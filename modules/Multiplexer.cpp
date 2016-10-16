#include "Multiplexer.h"

CMultiplexerModule::CMultiplexerModule()
	: CModule()
{
	m_nInputCount = 3;
	m_nOutputCount = 1;
}

int CMultiplexerModule::Execute(REGISTER_TYPE * pInputs, REGISTER_TYPE * pOutputs, UINT * pExecDelay)
{
	pOutputs[0] = ((pInputs[2] & 1) == 0) ? pInputs[0] : pInputs[1];

	if (pExecDelay)
		*pExecDelay = 2;

	return ERR_SUCCESS;
}
