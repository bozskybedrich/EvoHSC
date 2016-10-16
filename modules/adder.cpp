#include "adder.h"
#include <assert.h>

#ifdef _DEBUG
	#define new MYDEBUG_NEW
#endif

enum ADDER_OP_TYPE {
	ADD,
	SUB,
	INC,
	DEC
};

CAdder::CAdder() :
	CModule()
{
	m_nInputCount = 3;
	m_nOutputCount = 1;
}

int CAdder::Execute(REGISTER_TYPE * pInputs, REGISTER_TYPE * pOutputs, UINT * pExecDelay)
{
	switch ((DWORD) (pInputs[2] & 0x3)) {
	case ADD:
		pOutputs[0] = pInputs[0] + pInputs[1];
		break;
	case SUB:
		pOutputs[0] = pInputs[0] - pInputs[1];
		break;
	case INC:
		pOutputs[0] = pInputs[0] + 1;
		break;
	case DEC:
		pOutputs[0] = pInputs[0] - 1;
		break;
	default:
		assert(false);
		break;
	}

	SetZero(pOutputs[0] == 0);
	SetSign(pOutputs[0] < 0);

	if (pExecDelay)
		*pExecDelay = 20;

	return ERR_SUCCESS;
}