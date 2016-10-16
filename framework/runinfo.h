#include "typedefs.h"

typedef enum CEEvoOperators {
	infoMutationReg,
	infoMutationModule,
	infoMutationInstrSet,
	infoMutationInstrType,
	infoMutationInstrParam,
	infoMutationInstrIOs,
	infoMutationInstrPos,
	infoMutationModulesMask,
	infoCrossoverHW,
	infoCrossoverSW,
	infoOperatorsCount
};

class CIndivInfo
{
public:
	CIndivInfo() { Reset(); }
	void Reset() {
		for (UINT i = 0; i < infoOperatorsCount; i++)
			m_arrOpUsed[i] = false;
	}

	bool m_arrOpUsed[infoOperatorsCount];
};

class CRunInfo
{
public:
	CRunInfo() { Reset(); }
	void Reset() {
		for (UINT i = 0; i < infoOperatorsCount; i++)
			m_arrOpUsage[i] = 0;
	}

	UINT m_arrOpUsage[infoOperatorsCount];
};
