#pragma once
#include "framework.h"

class CGoldschmidtTest {
public:
	CGoldschmidtTest() {}
	~CGoldschmidtTest() {}
};

class CGoldschmidtIndiv : public CIndividual
{
public:
	//CSymRegIndiv(strArchitectureParams * pParams, CEnvironment * pEnv);
	CGoldschmidtIndiv(CArchitecture * pArch, CEnvironment * pEnv, CFramework * pFramework, strIndividualParams * pIndivParams)
		: CIndividual(pArch, pEnv, pFramework, pIndivParams) {}
	CIndividual * Clone() { return new CGoldschmidtIndiv(*this); }

	double EvaluateOutputs(CEnvironment * pEnv);
	
	virtual bool LessThan(const CIndividual & other) const {
		CGoldschmidtIndiv & otherIndiv = (CGoldschmidtIndiv &) other;
		if (GetFitness(eResultsFitness) == otherIndiv.GetFitness(eResultsFitness) && GetFitness(eResultsFitness) > 0) {
			if (GetFitness(eAreaFitness) == otherIndiv.GetFitness(eAreaFitness)) {
				return GetFitness(ePriceFitness) < otherIndiv.GetFitness(ePriceFitness);
			} return GetFitness(eAreaFitness) < otherIndiv.GetFitness(eAreaFitness);
		} return GetFitness(eResultsFitness) < otherIndiv.GetFitness(eResultsFitness);
	}
	
	void PrintProgram();
	virtual void GenerateRandomProgram();
	virtual void GenerateRandomHWTopology();
};

class CGoldschmidtFramework : public CFramework
{
public:
	CGoldschmidtFramework(CArchitecture * pArchitecture, 
		CEnvironment * pEnvironment,
		strEvolutionParams & strEvoParams,
		strIndividualParams & strIndivParams)
		: CFramework(pArchitecture, pEnvironment, strEvoParams, strIndivParams) { m_bPrintInfo = true; }
	CIndividual * CreateNewIndividual() { return new CGoldschmidtIndiv(m_pArchitecture, m_pEnvironment, this, &m_strIndivParams); }
	bool EvaluateStopCondition(UINT nGeneration);
};