#include "ConstantModule.h"

CConstantModule::CConstantModule(REGISTER_TYPE constant)
	: CModule(), m_constant(constant)
{
	m_nInputCount = 0;
	m_nOutputCount = 1;
}

int CConstantModule::Execute(REGISTER_TYPE * pInputs, REGISTER_TYPE * pOutputs, UINT * pExecDelay)
{
	pOutputs[0] = m_constant;
	if (pExecDelay)
		*pExecDelay = 1;

	return ERR_SUCCESS;
}

void CConstantModule::SetConst(REGISTER_TYPE constant)
{
	m_constant = constant;
}
