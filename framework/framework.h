#pragma once
#include "individual.h"
#include "framework_typedefs.h"

typedef enum {
	ROULETTE,
	TOURNAMENT
} SELECTION_TYPE;

struct strMutationParams {
	strMutationParams() {
		//fHWMutationProb = DEF_HW_MUT_PROB;
		fProbs[infoMutationReg] = DEF_HW_REG_CHANGE;
		fProbs[infoMutationModule] = DEF_HW_MOD_CHANGE, 
		fProbs[infoMutationInstrSet] = DEF_HW_INSTR_SET_CHANGE,
		fProbs[infoMutationInstrType] = DEF_SW_INSTR_TYPE_CHANGE;
		fProbs[infoMutationInstrParam] = DEF_SW_INSTR_PARAM_CHANGE;
		fProbs[infoMutationInstrIOs] = DEF_SW_INSTR_IOS_CHANGE;
		fProbs[infoMutationInstrPos] = DEF_SW_INSTR_POS_CHANGE;
		fProbs[infoMutationModulesMask] = DEF_SW_INSTR_MODS_CHANGE;
	}

	double fProbs[infoOperatorsCount];
};

struct strCrossoverParams {
	strCrossoverParams() : nParentsCount(2), nCrossoverPoints(2)
	{
		fProbs[infoCrossoverHW] = DEF_CROSS_HW;
		fProbs[infoCrossoverSW] = DEF_CROSS_SW;
	}

	UINT nParentsCount;
	UINT nCrossoverPoints;
	double fProbs[infoOperatorsCount];
};

struct strSimulationParams {
	strSimulationParams(UINT nMaxTime = DEF_SIMUL_MAX_TIME) : nMaxLogicTime(nMaxTime), bPrintInfo(false), bPrintDebug(false) {}
	UINT nMaxLogicTime;
	bool bPrintInfo;
	bool bPrintDebug;
};

struct strSelectionParams {
	strSelectionParams() : eSelectionType(TOURNAMENT), nTournamentSize(2) {}
	SELECTION_TYPE eSelectionType;
	UINT nTournamentSize;
};

struct strEvolutionParams {
	strEvolutionParams(UINT nPopSize, UINT nMaxGen, double fCross, double fMut, UINT nEliteCount)
		: nPopulationSize(nPopSize), 
		nMaxGenerations(nMaxGen), 
		fCrossoverProbability(fCross), 
		fMutationProbability(fMut), 
		nEliteSize(nEliteCount) {}
	UINT nPopulationSize;
	UINT nMaxGenerations;
	double fCrossoverProbability;
	double fMutationProbability;
	strSelectionParams strSelectParams;
	strCrossoverParams strCrossParams;
	strMutationParams strMutParams;
	strSimulationParams strSimulParams;
	UINT nEliteSize;
};

struct strIndividualParams {
	strIndividualParams(UINT nProgSize, UINT nJmpOffset = MAX_JUMP_OFFSET, UINT nRandParam = MAX_RAND_NUM) 
		: nMaxProgramSize(nProgSize), nMaxJmpOffset(nJmpOffset), nMaxRandParam(nRandParam) {}
	UINT nMaxProgramSize;
	UINT nMaxJmpOffset;
	UINT nMaxRandParam;
};

class CFramework {
public:
	CFramework(CArchitecture * pArchitecture, 
		CEnvironment * pEnvironment,
		strEvolutionParams & strEvoParams,
		strIndividualParams & strIndivParams,
		bool bPrintInfo = true);
	virtual ~CFramework();

	// Framework manipulation functions
	virtual CIndividual * CreateNewIndividual();
	virtual void GenerateInitialPopulation();
	virtual void GenerateNextGeneration() {}
	virtual void EvaluateFitness();
	virtual void PopulateNextGeneration();
	virtual void MakeCrossover(CIndividual ** pParents, CIndividual ** pOffsprings);
	virtual void CopyBestIndividuals(CIndividual ** pSelectionSet, UINT nPopSize, CIndividual ** pDestSet, UINT nIndivCount);
	static bool CompareIndividuals(CIndividual * pIndiv1, CIndividual * pIndiv2) { return pIndiv1->LessThan(*pIndiv2); }
	virtual void MakeMutation();
	virtual void StartEvolution();
	virtual void PrintGenerationStatistics(UINT nGeneration);
	virtual bool EvaluateStopCondition(UINT nGeneration);
	virtual void SelectParents(CIndividual ** pParents, UINT nParCount = -1);
	virtual void CrossIndividualsHW(CIndividual ** pIndivs, UINT nCrossPoint);
	virtual void CrossIndividualsSW(CIndividual ** pIndivs, UINT nCrossPoint);
	virtual void SwitchInstrBlocks(CInstrBlock * pBlock1, CInstrBlock * pBlock2);
	virtual void UpdateOpsProbabilities();
	virtual void ResetAfterEval() {}
	virtual bool IsNextInputSet() { return false; }

	// Get methods
	strEvolutionParams & GetEvolutionParams() { return m_strEvoParams; }
	strIndividualParams & GetIndividualParams() { return m_strIndivParams; }
	CIndividual ** GetActGeneration() { return m_pActGeneration; }

	// Set methods
	void SetEvolutionParams(strEvolutionParams strEvoParams) { m_strEvoParams = strEvoParams; }
	void SetIndividualParams(strIndividualParams strIndivParams) { m_strIndivParams = strIndivParams; }

protected:
	strEvolutionParams m_strEvoParams;
	strIndividualParams m_strIndivParams;
	strArchitectureParams * m_strArchParams;
	CIndividual ** m_pActGeneration;
	CArchitecture * m_pArchitecture;
	CEnvironment * m_pEnvironment;
	CIndividual ** m_pBestIndivs;
	bool m_bPrintInfo;
public:
	CRunInfo m_info;
};
