#pragma once
#include "framework.h"

class CParityTest {
public:
	CParityTest() {}
	~CParityTest() {}
};

class CParityIndiv : public CIndividual
{
public:
	//CSymRegIndiv(strArchitectureParams * pParams, CEnvironment * pEnv);
	CParityIndiv(CArchitecture * pArch, CEnvironment * pEnv, CFramework * pFramework, strIndividualParams * pIndivParams)
		: CIndividual(pArch, pEnv, pFramework, pIndivParams) {}
	CIndividual * Clone() { return new CParityIndiv(*this); }

	double EvaluateFitness(strSimulationParams & simParams) {
		double dFit = CIndividual::EvaluateFitness(simParams);
		if (m_mapFitness[eResultsFitness] == 50 || m_mapFitness[eResultsFitness] == 25)
			m_mapFitness[eResultsFitness] = m_mapFitness[eResultsFitness] / 10;

		return dFit;
	}

	double EvaluateOutputs(CEnvironment * pEnv);
	double EvaluateSpeed(UINT nExecTime, UINT nMaxTime);
	
	virtual bool LessThan(const CIndividual & other) const {
		CParityIndiv & otherIndiv = (CParityIndiv &) other;
		if (GetFitness(eResultsFitness) == otherIndiv.GetFitness(eResultsFitness) && GetFitness(eResultsFitness) > 0) {
			if (GetFitness(eSpeedFitness) == otherIndiv.GetFitness(eSpeedFitness)) {
				return GetFitness(eAreaFitness) < otherIndiv.GetFitness(eAreaFitness);
			} return GetFitness(eSpeedFitness) < otherIndiv.GetFitness(eSpeedFitness);
		} return GetFitness(eResultsFitness) < otherIndiv.GetFitness(eResultsFitness);
	}
	
	void PrintProgram();
	virtual void GenerateRandomProgram();
	virtual void GenerateRandomHWTopology();
};

class CParityFramework : public CFramework
{
public:
	CParityFramework(CArchitecture * pArchitecture, 
		CEnvironment * pEnvironment,
		strEvolutionParams & strEvoParams,
		strIndividualParams & strIndivParams)
		: CFramework(pArchitecture, pEnvironment, strEvoParams, strIndivParams) { m_bPrintInfo = true; }
	CIndividual * CreateNewIndividual() { return new CParityIndiv(m_pArchitecture, m_pEnvironment, this, &m_strIndivParams); }
	bool EvaluateStopCondition(UINT nGeneration);
	void ResetAfterEval();
	bool IsNextInputSet();
};
