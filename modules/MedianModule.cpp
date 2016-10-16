#include "MedianModule.h"
#include <vector>
#include <algorithm>

CMedianModule::CMedianModule()
	: CModule()
{
	m_nInputCount = 9;
	m_nOutputCount = 1;
}

int CMedianModule::Execute(REGISTER_TYPE * pInputs, REGISTER_TYPE * pOutputs, UINT * pExecDelay)
{
	std::vector<REGISTER_TYPE> vecInputs;
	for (int i = 0; i < m_nInputCount; i++)
		vecInputs.push_back(pInputs[i]);

	std::sort(vecInputs.begin(), vecInputs.end());
	pOutputs[0] = vecInputs.at(4);

	if (pExecDelay)
		*pExecDelay = 2;

	return ERR_SUCCESS;
}
