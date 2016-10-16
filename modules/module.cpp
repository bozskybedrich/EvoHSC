#include "module.h"
#include "errorstatus.h"
//#include <crtdbg.h>

/*#ifdef _DEBUG
	#define new MYDEBUG_NEW
#endif*/

CModule::CModule() :
m_nInputCount(0), m_nOutputCount(0), m_pInputMasks(NULL), m_pOutputMasks(NULL), m_nFlags(0)
{
}

CModule::CModule(const CModule & source)
{
	m_nInputCount = source.m_nInputCount;
	m_nOutputCount = source.m_nOutputCount;
	
	if (source.m_pInputMasks > 0) {
		m_pInputMasks = new DWORD[m_nInputCount];
		for (UINT i = 0; i < m_nInputCount; i++)
			m_pInputMasks[i] = source.m_pInputMasks[i];
	}
	else
		m_pInputMasks = NULL;

	if (source.m_pOutputMasks > 0) {
		m_pOutputMasks = new DWORD[m_nOutputCount];
		for (UINT i = 0; i < m_nOutputCount; i++)
			m_pOutputMasks[i] = source.m_pOutputMasks[i];
	}
	else
		m_pOutputMasks = NULL;
}

CModule::~CModule()
{
	if (m_pInputMasks)
		delete[] m_pInputMasks;

	if (m_pOutputMasks)
		delete[] m_pOutputMasks;
}

int CModule::Execute(REGISTER_TYPE * pInputs, REGISTER_TYPE * pOutputs, UINT * pExecDelay)
{
	if (pExecDelay)
		*pExecDelay = 0;

	return ERR_SUCCESS;
}

UINT CModule::GetInputsCount()
{
	return m_nInputCount;
}

UINT CModule::GetOutputsCount()
{
	return m_nOutputCount;
}

UINT CModule::GetArea()
{
	return 0;
}

UINT CModule::GetPrice()
{
	return 0;
}

/*void CModule::SetZero(bool bIsZero)
{
	if (bIsZero)
		m_nFlags = m_nFlags | ZERO_FLAG_MASK;
	else
		m_nFlags = m_nFlags & ~ZERO_FLAG_MASK;
}

void CModule::SetSign(bool bIsSign)
{
	if (bIsSign)
		m_nFlags = m_nFlags | SIGN_FLAG_MASK;
	else
		m_nFlags = m_nFlags & ~SIGN_FLAG_MASK;
}
*/