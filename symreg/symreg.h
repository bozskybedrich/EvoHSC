#pragma once
#include "framework.h"

class CSymRegTest {
public:
	CSymRegTest() {}
	~CSymRegTest() {}
};

class CSymRegIndiv : public CIndividual
{
public:
	//CSymRegIndiv(strArchitectureParams * pParams, CEnvironment * pEnv);
	CSymRegIndiv(CArchitecture * pArch, CEnvironment * pEnv, strIndividualParams * pIndivParams)
		: CIndividual(pArch, pEnv, pIndivParams) {}
	CIndividual * Clone() { return new CSymRegIndiv(*this); }

	double EvaluateOutputs(CEnvironment * pEnv);
	
	virtual bool LessThan(const CIndividual & other) const {
		CSymRegIndiv & fibIndiv = (CSymRegIndiv &) other;
		if (m_strFitness.dResultsFitness == fibIndiv.m_strFitness.dResultsFitness && m_strFitness.dResultsFitness > 9) {
			if (m_strFitness.dAreaFitness == fibIndiv.m_strFitness.dAreaFitness) {
				return m_strFitness.dPriceFitness < fibIndiv.m_strFitness.dPriceFitness;
			} return m_strFitness.dAreaFitness < fibIndiv.m_strFitness.dAreaFitness;
		} return m_strFitness.dResultsFitness < fibIndiv.m_strFitness.dResultsFitness;
	}
	
	void PrintProgram();
};

class CSymRegFramework : public CFramework
{
public:
	CSymRegFramework(CArchitecture * pArchitecture, 
		CEnvironment * pEnvironment,
		strEvolutionParams & strEvoParams,
		strIndividualParams & strIndivParams)
		: CFramework(pArchitecture, pEnvironment, strEvoParams, strIndivParams) { m_bPrintInfo = false; }
	CIndividual * CreateNewIndividual() { return new CSymRegIndiv(m_pArchitecture, m_pEnvironment, &m_strIndivParams); }
	bool EvaluateStopCondition(UINT nGeneration);
};