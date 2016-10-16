#include "bit_shift.h"

#ifdef _DEBUG
	#define new MYDEBUG_NEW
#endif

enum SHIFT_DIRECTION {
	SHIFT_RIGHT,
	SHIFT_LEFT
};

CBitShifter::CBitShifter() :
	CModule()
{
	m_nInputCount = 3;
	m_nOutputCount = 1;
}

int CBitShifter::Execute(REGISTER_TYPE * pInputs, REGISTER_TYPE * pOutputs, UINT * pExecDelay)
{
	switch ((DWORD) pInputs[2] & 0x1) {
	case SHIFT_LEFT:
		pOutputs[0] = pInputs[0] << pInputs[1];
		break;
	case SHIFT_RIGHT:
		pOutputs[0] = pInputs[0] >> pInputs[1];
		break;
	}

	SetZero(pOutputs[0] == 0);
	SetSign(pOutputs[0] < 0);

	if (pExecDelay)
		*pExecDelay = 20;

	return ERR_SUCCESS;
}
