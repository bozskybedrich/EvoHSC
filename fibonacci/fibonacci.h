#pragma once
#include "framework.h"

class CFibonacciTest {
public:
	CFibonacciTest() {}
	~CFibonacciTest() {}
};

class CFibonacciIndiv : public CIndividual
{
public:
	//CFibonacciIndiv(strArchitectureParams * pParams, CEnvironment * pEnv);
	CFibonacciIndiv(CArchitecture * pArch, CEnvironment * pEnv, strIndividualParams * pIndivParams)
		: CIndividual(pArch, pEnv, pIndivParams) {}
	CIndividual * Clone() { return new CFibonacciIndiv(*this); }

	double EvaluateOutputs(CEnvironment * pEnv);
	
	virtual bool LessThan(const CIndividual & other) const {
		CFibonacciIndiv & fibIndiv = (CFibonacciIndiv &) other;
		if (m_strFitness.dResultsFitness == fibIndiv.m_strFitness.dResultsFitness && m_strFitness.dResultsFitness > 9) {
			if (m_strFitness.dAreaFitness == fibIndiv.m_strFitness.dAreaFitness) {
				return m_strFitness.dPriceFitness < fibIndiv.m_strFitness.dPriceFitness;
			} return m_strFitness.dAreaFitness < fibIndiv.m_strFitness.dAreaFitness;
		} return m_strFitness.dResultsFitness < fibIndiv.m_strFitness.dResultsFitness;
	}
	
	void PrintProgram();
};

class CFibonacciFramework : public CFramework
{
public:
	CFibonacciFramework(CArchitecture * pArchitecture, 
		CEnvironment * pEnvironment,
		strEvolutionParams & strEvoParams,
		strIndividualParams & strIndivParams)
		: CFramework(pArchitecture, pEnvironment, strEvoParams, strIndivParams) { m_bPrintInfo = false; }
	CIndividual * CreateNewIndividual() { return new CFibonacciIndiv(m_pArchitecture, m_pEnvironment, &m_strIndivParams); }
	bool EvaluateStopCondition(UINT nGeneration);
};