#pragma once
#include "framework.h"

class CSqrtTest {
public:
	CSqrtTest() {}
	~CSqrtTest() {}
};

class CSqrtIndiv : public CIndividual
{
public:
	//CSqrtIndiv(strArchitectureParams * pParams, CEnvironment * pEnv);
	CSqrtIndiv(CArchitecture * pArch, CEnvironment * pEnv, strIndividualParams * pIndivParams)
		: CIndividual(pArch, pEnv, pIndivParams) {}
	CIndividual * Clone() { return new CSqrtIndiv(*this); }

	double EvaluateOutputs(CEnvironment * pEnv);
	
	virtual bool LessThan(const CIndividual & other) const {
		CSqrtIndiv & fibIndiv = (CSqrtIndiv &) other;
		if (m_strFitness.dResultsFitness == fibIndiv.m_strFitness.dResultsFitness && m_strFitness.dResultsFitness > 9) {
			//if (m_strFitness.dSpeedFitness == fibIndiv.m_strFitness.dSpeedFitness) {
				if (m_strFitness.dAreaFitness == fibIndiv.m_strFitness.dAreaFitness) {
					return m_strFitness.dPriceFitness < fibIndiv.m_strFitness.dPriceFitness;
				} return m_strFitness.dAreaFitness < fibIndiv.m_strFitness.dAreaFitness;
			//} return m_strFitness.dSpeedFitness < fibIndiv.m_strFitness.dSpeedFitness;
		} return m_strFitness.dResultsFitness < fibIndiv.m_strFitness.dResultsFitness;
	}
	
	void PrintProgram();
};

class CSqrtFramework : public CFramework
{
public:
	CSqrtFramework(CArchitecture * pArchitecture, 
		CEnvironment * pEnvironment,
		strEvolutionParams & strEvoParams,
		strIndividualParams & strIndivParams,
		bool bPrintInfo = false)
		: CFramework(pArchitecture, pEnvironment, strEvoParams, strIndivParams, bPrintInfo) {}
	CIndividual * CreateNewIndividual() { return new CSqrtIndiv(m_pArchitecture, m_pEnvironment, &m_strIndivParams); }
	bool EvaluateStopCondition(UINT nGeneration);
};