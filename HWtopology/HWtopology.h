#pragma once
#include "framework.h"

class CHWTopologyTest {
public:
	CHWTopologyTest() {}
	~CHWTopologyTest() {}
};

class CHWTopologyIndiv : public CIndividual
{
public:
	//CSymRegIndiv(strArchitectureParams * pParams, CEnvironment * pEnv);
	CHWTopologyIndiv(CArchitecture * pArch, CEnvironment * pEnv, strIndividualParams * pIndivParams)
		: CIndividual(pArch, pEnv, pIndivParams) {}
	CIndividual * Clone() { return new CHWTopologyIndiv(*this); }

	double EvaluateOutputs(CEnvironment * pEnv);
	
	virtual bool LessThan(const CIndividual & other) const {
		CHWTopologyIndiv & fibIndiv = (CHWTopologyIndiv &) other;
		if (GetFitness(eResultsFitness) == fibIndiv.GetFitness(eResultsFitness) && GetFitness(eResultsFitness) > 0) {
			if (GetFitness(eAreaFitness) == fibIndiv.GetFitness(eAreaFitness)) {
				return GetFitness(ePriceFitness) < fibIndiv.GetFitness(ePriceFitness);
			} return GetFitness(eAreaFitness) < fibIndiv.GetFitness(eAreaFitness);
		} return GetFitness(eResultsFitness) < fibIndiv.GetFitness(eResultsFitness);
	}
	
	void PrintProgram();
	virtual void GenerateRandomProgram();
	virtual void GenerateRandomHWTopology();
};

class CHWTopologyFramework : public CFramework
{
public:
	CHWTopologyFramework(CArchitecture * pArchitecture, 
		CEnvironment * pEnvironment,
		strEvolutionParams & strEvoParams,
		strIndividualParams & strIndivParams)
		: CFramework(pArchitecture, pEnvironment, strEvoParams, strIndivParams) { m_bPrintInfo = true; }
	CIndividual * CreateNewIndividual() { return new CHWTopologyIndiv(m_pArchitecture, m_pEnvironment, &m_strIndivParams); }
	bool EvaluateStopCondition(UINT nGeneration);
	virtual void ResetAfterEval();
};