#pragma once

#include "typedefs.h"
#include "errorstatus.h"

#define ZERO_FLAG_MASK	0x01
#define SIGN_FLAG_MASK	0x02

class CModule {
public:
	CModule();
	CModule(const CModule & source);
	~CModule();
	virtual int Execute(REGISTER_TYPE * pInputs, REGISTER_TYPE * pOutputs, UINT * pExecDelay = NULL);
	UINT GetInputsCount();
	UINT GetOutputsCount();
	virtual UINT GetArea();
	virtual UINT GetPrice();
	inline void SetZero(bool bIsZero)
		{
			if (bIsZero)
				m_nFlags = m_nFlags | ZERO_FLAG_MASK;
			else
				m_nFlags = m_nFlags & ~ZERO_FLAG_MASK;
		}

	inline bool IsZeroSet() { return (m_nFlags & ZERO_FLAG_MASK) != 0; }
	inline void SetSign(bool bIsSign)
		{
			if (bIsSign)
				m_nFlags = m_nFlags | SIGN_FLAG_MASK;
			else
				m_nFlags = m_nFlags & ~SIGN_FLAG_MASK;
		}

	inline bool IsSignSet() { return (m_nFlags & SIGN_FLAG_MASK) != 0; }

	inline void ResetFlags() { m_nFlags = 0; }
protected:
	UINT m_nInputCount;
	DWORD * m_pInputMasks;
	UINT m_nOutputCount;
	DWORD * m_pOutputMasks;
	BYTE m_nFlags;
};