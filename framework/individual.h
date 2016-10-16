#pragma once
#include "simulator.h"
#include "runinfo.h"
#include <vector>
#include <set>
#include <map>

#define GAUSS(x) \
        ((((rand() / (float) RAND_MAX) + (rand() / (float) RAND_MAX) \
        + (rand() / (float) RAND_MAX) + (rand() / (float) RAND_MAX) \
        + (rand() / (float) RAND_MAX) + (rand() / (float) RAND_MAX) \
        + (rand() / (float) RAND_MAX) + (rand() / (float) RAND_MAX) \
        + (rand() / (float) RAND_MAX) + (rand() / (float) RAND_MAX) \
        + (rand() / (float) RAND_MAX) + (rand() / (float) RAND_MAX)) - 6) / 6 * x) 

struct strIndividualParams;
struct strMutationParams;
struct strSimulationParams;
struct strArchitectureParams;

class CFramework;

enum enumFitnessType
{
	eResultsFitness,
	eSpeedFitness,
	ePriceFitness,
	eAreaFitness
};

struct strNsgaII {
	strNsgaII() : nRank(0), dDistance(0) {}
	UINT nRank;
	double dDistance;
	bool operator<(const strNsgaII & other) const {
		if (nRank == other.nRank)
			return dDistance > other.dDistance;
		return nRank < other.nRank;
	}
};

class CIndividual {
public:
	CIndividual(const CIndividual & source);
	CIndividual(CArchitecture * pArch, CEnvironment * pEnv, CFramework * pFramework, strIndividualParams * pIndivParams);
	virtual ~CIndividual();
	virtual bool LessThan(const CIndividual & other) const {
		return m_strNsgaII < other.m_strNsgaII;
	}

	virtual void GenerateRandomProgram();
	virtual int PregenerateProgram();
	virtual void GenerateRandomHWTopology();
	void UpdateHWTopology();
	virtual BYTE * GenerateRandomIOs(UINT nInstrNum) { return GenerateRandomIOs(m_pArch->GetInstruction(nInstrNum)); }
	virtual BYTE * GenerateRandomIOs(CInstruction * pInstr);
	virtual DWORD GenerateRandomModulesMask();
	virtual CInstrBlock * AppendInstrBlock(CInstrBlock * pSourceBlock);
	virtual CInstrBlock * AppendInstrBlock(UINT nInstrNo, REGISTER_TYPE param, BYTE * pIOs, DWORD nModulesMask = 0x00FFFFFF);
	virtual CInstrBlock * SpliceOutInstrBlock(CInstrBlock * pBlock);
	virtual CInstrBlock * InsertAfter(CInstrBlock * pBlockToInsert, CInstrBlock * pInsertAfter);
	virtual CInstrBlock * GetInstrBlockByPos(UINT nPos);
	virtual CInstrBlock * GetFirstInstr() { return m_pFirstInstr; }
	virtual CInstrBlock * GetLastInstr() { return m_pLastInstr; }
	virtual void SetFirstInstr(CInstrBlock * pFirstBlock) { m_pFirstInstr = pFirstBlock; }
	virtual void SetLastInstr(CInstrBlock * pLastBlock) { m_pLastInstr = pLastBlock; }
	virtual CIndividual * Clone();
	virtual double EvaluateFitness(strSimulationParams & simParams);
	virtual double EvaluateOutputs(CEnvironment * pEnv);
	virtual void EvaluateHW();
	virtual double EvaluateSpeed(UINT nExecTime, UINT nMaxTime);
	virtual void MakeMutation(strMutationParams & mutParams);
	double GetFitness(const enumFitnessType eType) const;
	inline REGISTER_TYPE MutateRandomParam(REGISTER_TYPE origParam, INSTR_TYPE eType);
	inline REGISTER_TYPE OffsetParam(REGISTER_TYPE origParam, REGISTER_TYPE maxDelta, REGISTER_TYPE minVal, REGISTER_TYPE maxVal);
	virtual void PrintProgram() {};
	virtual strArchitectureSettings * GetArchSettings() { return m_pArchSettings; }
	virtual void SetArchitectureSettings(strArchitectureSettings * pSettings) { m_pArchSettings = pSettings; }
	std::map<enumFitnessType, double> & GetFitnessMap() { return m_mapFitness; }
protected:
	CArchitecture * m_pArch;

	CEnvironment * m_pEnv;
	CFramework * m_pFramework;
	UINT m_nInstrCount;
	CInstrBlock * m_pFirstInstr;
	CInstrBlock * m_pLastInstr;
	strIndividualParams * m_pIndivParams;
	strArchitectureParams * m_pArchParams;
	strArchitectureSettings * m_pArchSettings;
	std::map<enumFitnessType, double> m_mapFitness;
	strNsgaII m_strNsgaII;
public:
	CIndivInfo m_info;
};
