#pragma once
#include "framework.h"

class CSigmoidIndiv : public CIndividual
{
public:
	//CSymRegIndiv(strArchitectureParams * pParams, CEnvironment * pEnv);
	CSigmoidIndiv(CArchitecture * pArch, CEnvironment * pEnv, CFramework * pFramework, strIndividualParams * pIndivParams)
		: CIndividual(pArch, pEnv, pFramework, pIndivParams), m_errBits(0) {}
	CSigmoidIndiv(std::string & sXml, CArchitecture * pArch, CEnvironment * pEnv, CFramework * pFramework, strArchitectureParams * pArchParams = NULL)
		: CIndividual(sXml, pArch, pEnv, pFramework, pArchParams), m_errBits(0) {}
	CSigmoidIndiv * Clone() { return new CSigmoidIndiv(*this); }

	double EvaluateOutputs(CEnvironment * pEnv);
	//double EvaluateSpeed(UINT nExecTime, UINT nMaxTime);
	
	virtual bool LessThan(const CIndividual & other) const {
		CSigmoidIndiv & otherIndiv = (CSigmoidIndiv &) other;
		if (GetFitness(eResultsFitness) == otherIndiv.GetFitness(eResultsFitness) && GetFitness(eResultsFitness) > 98) {
			if (GetFitness(eSpeedFitness) == otherIndiv.GetFitness(eSpeedFitness)) {
				return GetFitness(eAreaFitness) < otherIndiv.GetFitness(eAreaFitness);
			} return GetFitness(eSpeedFitness) < otherIndiv.GetFitness(eSpeedFitness);
		} return GetFitness(eResultsFitness) < otherIndiv.GetFitness(eResultsFitness);
	}
	
	REGISTER_TYPE GetErrBits() { return m_errBits; }
	int PregenerateProgram();

protected:
	REGISTER_TYPE m_errBits;
};

class CSigmoidFramework : public CFramework
{
public:
	CSigmoidFramework(CArchitecture * pArchitecture, 
		CEnvironment * pEnvironment,
		strEvolutionParams & strEvoParams,
		strIndividualParams & strIndivParams)
		: CFramework(pArchitecture, pEnvironment, strEvoParams, strIndivParams), m_errBits(0) { m_bPrintInfo = true; }
	CIndividual * CreateNewIndividual() { return new CSigmoidIndiv(m_pArchitecture, m_pEnvironment, this, &m_strIndivParams); }
	bool EvaluateStopCondition(UINT nGeneration);
	void ResetAfterEval();
	bool IsNextInputSet();
	REGISTER_TYPE GetErrBits() { return m_errBits; }
protected:
	REGISTER_TYPE m_errBits;
};
