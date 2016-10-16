#include "PixelEvaluator.h"

CPixelEvaluatorModule::CPixelEvaluatorModule()
	: CModule()
{
	m_nInputCount = 1;
	m_nOutputCount = 1;
}

int CPixelEvaluatorModule::Execute(REGISTER_TYPE * pInputs, REGISTER_TYPE * pOutputs, UINT * pExecDelay)
{
	pOutputs[0] = (pInputs[0] == 0 || pInputs[0] == 0xFF) ? 1 : 0;

	if (pExecDelay)
		*pExecDelay = 2;

	return ERR_SUCCESS;
}
