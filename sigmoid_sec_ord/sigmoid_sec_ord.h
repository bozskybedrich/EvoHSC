#pragma once
#include "framework.h"

class CSigmoidSecOrdIndiv : public CIndividual
{
public:
	//CSymRegIndiv(strArchitectureParams * pParams, CEnvironment * pEnv);
	CSigmoidSecOrdIndiv(CArchitecture * pArch, CEnvironment * pEnv, CFramework * pFramework, strIndividualParams * pIndivParams)
		: CIndividual(pArch, pEnv, pFramework, pIndivParams) {}
	CSigmoidSecOrdIndiv(std::string & sXml, CArchitecture * pArch, CEnvironment * pEnv, CFramework * pFramework, strArchitectureParams * pArchParams = NULL)
		: CIndividual(sXml, pArch, pEnv, pFramework, pArchParams) {}
	CSigmoidSecOrdIndiv * Clone() { return new CSigmoidSecOrdIndiv(*this); }

	double EvaluateOutputs(CEnvironment * pEnv);
	//double EvaluateSpeed(UINT nExecTime, UINT nMaxTime);
	
	virtual bool LessThan(const CIndividual & other) const {
		CSigmoidSecOrdIndiv & otherIndiv = (CSigmoidSecOrdIndiv &) other;
		if (GetFitness(eResultsFitness) == otherIndiv.GetFitness(eResultsFitness) && GetFitness(eResultsFitness) > 98) {
			if (GetFitness(eSpeedFitness) == otherIndiv.GetFitness(eSpeedFitness)) {
				return GetFitness(eAreaFitness) < otherIndiv.GetFitness(eAreaFitness);
			} return GetFitness(eSpeedFitness) < otherIndiv.GetFitness(eSpeedFitness);
		} return GetFitness(eResultsFitness) < otherIndiv.GetFitness(eResultsFitness);
	}
	
	int PregenerateProgram();
};

class CSigmoidSecOrdFramework : public CFramework
{
public:
	CSigmoidSecOrdFramework(CArchitecture * pArchitecture, 
		CEnvironment * pEnvironment,
		strEvolutionParams & strEvoParams,
		strIndividualParams & strIndivParams)
		: CFramework(pArchitecture, pEnvironment, strEvoParams, strIndivParams) { m_bPrintInfo = true; }
	CIndividual * CreateNewIndividual() { return new CSigmoidSecOrdIndiv(m_pArchitecture, m_pEnvironment, this, &m_strIndivParams); }
	bool EvaluateStopCondition(UINT nGeneration);
	void ResetAfterEval();
	bool IsNextInputSet();
};
