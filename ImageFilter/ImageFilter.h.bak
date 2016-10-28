#pragma once
#include "framework.h"

class CImageFilterTest {
public:
	CImageFilterTest() {}
	~CImageFilterTest() {}
};

class CImageFilterIndiv : public CIndividual
{
public:
	//CSymRegIndiv(strArchitectureParams * pParams, CEnvironment * pEnv);
	CImageFilterIndiv(CArchitecture * pArch, CEnvironment * pEnv, CFramework * pFramework, strIndividualParams * pIndivParams)
		: CIndividual(pArch, pEnv, pFramework, pIndivParams) {}
	CImageFilterIndiv * Clone() { return new CImageFilterIndiv(*this); }

	double EvaluateOutputs(CEnvironment * pEnv);
	double EvaluateSpeed(UINT nExecTime, UINT nMaxTime);
	
	virtual bool LessThan(const CIndividual & other) const {
		CImageFilterIndiv & otherIndiv = (CImageFilterIndiv &) other;
		if (GetFitness(eResultsFitness) == otherIndiv.GetFitness(eResultsFitness) && GetFitness(eResultsFitness) > 98) {
			if (GetFitness(eSpeedFitness) == otherIndiv.GetFitness(eSpeedFitness)) {
				return GetFitness(eAreaFitness) < otherIndiv.GetFitness(eAreaFitness);
			} return GetFitness(eSpeedFitness) < otherIndiv.GetFitness(eSpeedFitness);
		} return GetFitness(eResultsFitness) < otherIndiv.GetFitness(eResultsFitness);
	}
	
	void PrintProgram();
	//virtual void GenerateRandomProgram();
	virtual int PregenerateProgram();
};

class CImageFilterFramework : public CFramework
{
public:
	CImageFilterFramework(CArchitecture * pArchitecture, 
		CEnvironment * pEnvironment,
		strEvolutionParams & strEvoParams,
		strIndividualParams & strIndivParams)
		: CFramework(pArchitecture, pEnvironment, strEvoParams, strIndivParams) { m_bPrintInfo = true; }
	CIndividual * CreateNewIndividual() { return new CImageFilterIndiv(m_pArchitecture, m_pEnvironment, this, &m_strIndivParams); }
	bool EvaluateStopCondition(UINT nGeneration);
	void ResetAfterEval();
	bool IsNextInputSet();
};
