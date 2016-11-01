#include "AndOr.h"

#ifdef _DEBUG
	#define new MYDEBUG_NEW
#endif

enum MOD_FUNCTION {
	MOD_AND,
	MOD_OR
};

CAndOr::CAndOr(UINT nInputsCount) :
	CModule()
{
	m_nInputCount = nInputsCount + 1;
	m_nOutputCount = 1;
}

int CAndOr::Execute(REGISTER_TYPE * pInputs, REGISTER_TYPE * pOutputs, UINT * pExecDelay)
{
	pOutputs[0] = pInputs[0];

	for (int i = 1; i < m_nInputCount - 1; i++)
	{
		switch ((DWORD) pInputs[m_nInputCount - 1] & 0x1) {
		case MOD_AND:
			pOutputs[0] = pOutputs[0] & pInputs[i];
			break;
		case MOD_OR:
			pOutputs[0] = pOutputs[0] | pInputs[i];
			break;
		}
	}

	SetZero(pOutputs[0] == 0);
	SetSign(pOutputs[0] < 0);

	if (pExecDelay)
		*pExecDelay = 20;

	return ERR_SUCCESS;
}
