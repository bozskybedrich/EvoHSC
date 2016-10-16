#include "architecture.h"
#include "framework.h"
#include <assert.h>
#include <list>
#include <iostream>
#include <algorithm>
#include <ctime>

#ifdef _DEBUG
	#define new MYDEBUG_NEW
	//#define DETAILED_FITNESS_OUT
#endif

double dPreviousOuts = 0.0;
double dPreviousSpeed = 0.0;

CFramework::CFramework(CArchitecture * pArchitecture, CEnvironment * pEnvironment, strEvolutionParams & strEvoParams, strIndividualParams & strIndivParams, bool bPrintInfo)
	: m_pArchitecture(pArchitecture), 
	m_pEnvironment(pEnvironment), 
	m_strEvoParams(strEvoParams), 
	m_strIndivParams(strIndivParams), 
	m_pActGeneration(NULL),
	m_bPrintInfo(bPrintInfo)
{
	if (m_strEvoParams.nEliteSize > 0)
		m_pBestIndivs = new CIndividual*[m_strEvoParams.nEliteSize];
}

CFramework::~CFramework()
{
	if (m_pActGeneration) {
		for (UINT i = 0; i < m_strEvoParams.nPopulationSize; i++)
			delete m_pActGeneration[i];
		delete m_pActGeneration;
	}

	if (m_strEvoParams.nEliteSize > 0)
		delete[] m_pBestIndivs;
}

CIndividual * CFramework::CreateNewIndividual()
{
	return new CIndividual(m_pArchitecture, m_pEnvironment, this, &m_strIndivParams);
}

void CFramework::GenerateInitialPopulation()
{
	m_pActGeneration = new CIndividual*[m_strEvoParams.nPopulationSize];
	for (UINT i = 0; i < m_strEvoParams.nPopulationSize; i++) {
		m_pActGeneration[i] = CreateNewIndividual();
		if (m_pActGeneration[i]) {
			m_pActGeneration[i]->GenerateRandomProgram();
			m_pActGeneration[i]->GenerateRandomHWTopology();
		}
		else
			assert(false);
	}
	EvaluateFitness();
}

void CFramework::EvaluateFitness()
{
	for (UINT i = 0; i < m_strEvoParams.nPopulationSize; i++) {
		m_pActGeneration[i]->EvaluateFitness(m_strEvoParams.strSimulParams);
	}
}

void CFramework::MakeMutation()
{
	for (UINT i = 0; i < m_strEvoParams.nPopulationSize; i++) {
		if ((rand() / (float) RAND_MAX) <= m_strEvoParams.fMutationProbability)
			m_pActGeneration[i]->MakeMutation(m_strEvoParams.strMutParams);
	}
}

void CFramework::PopulateNextGeneration()
{
	UINT nPopSize = m_strEvoParams.nPopulationSize;
	CIndividual ** pOffsprings = new CIndividual*[nPopSize];

	CopyBestIndividuals(m_pActGeneration, nPopSize, pOffsprings, m_strEvoParams.nEliteSize);
	
	for (UINT nBest = 0; nBest < m_strEvoParams.nEliteSize; nBest++) {
		pOffsprings[nBest]->EvaluateFitness(m_strEvoParams.strSimulParams);
	}
	
	UINT nOffspringCount;
	UINT nParentsCount = m_strEvoParams.strCrossParams.nParentsCount;
	for (nOffspringCount = m_strEvoParams.nEliteSize; nOffspringCount + 1 < nPopSize; nOffspringCount += 2) {
		CIndividual ** pParents = new CIndividual*[nParentsCount];
		SelectParents(pParents);

		// Make crossover of the parents with certain probability
		if (rand() / (double) RAND_MAX < m_strEvoParams.fCrossoverProbability) {
			MakeCrossover(pParents, &pOffsprings[nOffspringCount]);

			// Evaluate effect of crossover
			for (UINT nChild = 0; nChild < nParentsCount; nChild++) {
				pOffsprings[nOffspringCount + nChild]->EvaluateFitness(m_strEvoParams.strSimulParams);
				bool bAllWorse = true;
				UINT nBetterOffIndex = 0;
				for (UINT nPar = 0; nPar < nParentsCount; nPar++) {
					if (!pParents[nPar]->LessThan(*pOffsprings[nOffspringCount + nChild])) {
						bAllWorse = false;
						UINT nBetterOffIndex = nOffspringCount + nChild;
						break;
					}
				}
				if (!bAllWorse) {
					for (UINT nOp = infoCrossoverHW; nOp < infoOperatorsCount; nOp++) {
						if (pOffsprings[nBetterOffIndex]->m_info.m_arrOpUsed[nOp])
							m_info.m_arrOpUsage[nOp]++;
					}
				}
			}
		}
		else {
			for (UINT nPar = 0; nPar < nParentsCount; nPar++) {
				pOffsprings[nOffspringCount + nPar] = pParents[nPar]->Clone();
			}
		}

		// Make mutation of the offspring with certain probability
		for (UINT nActOffspring = nOffspringCount; nActOffspring < nOffspringCount + nParentsCount; nActOffspring++) {
			double dMut = rand() / (double) RAND_MAX;
			if (dMut < m_strEvoParams.fMutationProbability) {
				CIndividual * pOld = pOffsprings[nActOffspring]->Clone();
				pOffsprings[nActOffspring]->MakeMutation(m_strEvoParams.strMutParams);

				// Evaluate effect of mutation operator
				pOffsprings[nActOffspring]->EvaluateFitness(m_strEvoParams.strSimulParams);
				bool bBetter = false;
				if (m_strEvoParams.nEliteSize > 0) {
					bBetter = true;
					for (UINT nBest = 0; nBest < m_strEvoParams.nEliteSize; nBest++)
						if (!m_pBestIndivs[nBest]->LessThan(*pOffsprings[nActOffspring]))
							bBetter = false;
				}
				else if (pOld->LessThan(*pOffsprings[nActOffspring])) {
					bBetter = true;
				}
				if (bBetter) {
					for (UINT nOp = infoMutationReg; nOp < infoCrossoverHW; nOp++)
						if (pOffsprings[nActOffspring]->m_info.m_arrOpUsed[nOp])
							m_info.m_arrOpUsage[nOp]++;
				}
				delete pOld;
			}
			else {
				pOffsprings[nActOffspring]->EvaluateFitness(m_strEvoParams.strSimulParams);
			}
		}
		
		delete[] pParents;
	}

	// Fill the missing offspring if the population size is an odd number
	if (nOffspringCount < nPopSize) {
		CIndividual * pFill;
		SelectParents(&pFill, 1);
		pOffsprings[nPopSize - 1] = pFill->Clone();
		pOffsprings[nPopSize - 1]->EvaluateFitness(m_strEvoParams.strSimulParams);
	}

	for (UINT i = 0; i < nPopSize; i++)
		delete m_pActGeneration[i];
	delete m_pActGeneration;
	m_pActGeneration = pOffsprings;
}

void CFramework::CopyBestIndividuals(CIndividual ** pSelectionSet, UINT nPopSize, CIndividual ** pDestSet, UINT nIndivCount)
{
	std::vector<CIndividual *> vecIndiv;

	for (UINT i = 0; i < nPopSize; i++)
		vecIndiv.push_back(pSelectionSet[i]);

	std::sort(vecIndiv.begin(), vecIndiv.end(), &CFramework::CompareIndividuals);

#ifdef DETAILED_FITNESS_OUT
	for (auto itIndiv = vecIndiv.rbegin(); itIndiv != vecIndiv.rend(); itIndiv++) {
		CInstrBlock * pFirstInstr = (*itIndiv)->GetFirstInstr();
		for (CInstrBlock * pInstr = pFirstInstr; pInstr != NULL; pInstr = pInstr->GetNext()) {
			for (int nInstr = 0; nInstr < pInstr->GetInstrCount(); nInstr++) {
				UINT nInstrNo = pInstr->GetInstrNo(nInstr);
				std::cerr << std::hex << nInstrNo << "|";
				UINT nIOCount = m_pArchitecture->GetInstruction(nInstrNo)->GetInsOutsCount();
				BYTE * pActIO = pInstr->GetInstrIOs(nInstr);
				for (int nIO = 0; nIO < nIOCount; nIO++, pActIO++) {
					BYTE io = (BYTE) *pActIO;
					std::cerr << std::hex << (int) io;
				}
				std::cerr << "| ";
			}
		}
		std::cerr << " " << (*itIndiv)->GetFitness(eResultsFitness) << std::endl;
	}
	std::cerr << std::endl;
#endif

	UINT nActPos = 0;
	for (std::vector<CIndividual *>::reverse_iterator rItIndiv = vecIndiv.rbegin(); rItIndiv != vecIndiv.rend() && nActPos < nIndivCount; rItIndiv++, nActPos++) {
		(*rItIndiv)->EvaluateFitness(m_strEvoParams.strSimulParams);
		/*if ((*rItIndiv)->GetFitness(eSpeedFitness) < dPreviousSpeed) {
			double dCurrentOuts = (*rItIndiv)->GetFitness(eResultsFitness);
			double dCurrentSpeed = (*rItIndiv)->GetFitness(eSpeedFitness);
			dPreviousOuts = dCurrentOuts;
			dPreviousSpeed = dCurrentSpeed;
		}
		else {
			dPreviousOuts = (*rItIndiv)->GetFitness(eResultsFitness);
			dPreviousSpeed = (*rItIndiv)->GetFitness(eSpeedFitness);
		}*/
		pDestSet[nActPos] = (*rItIndiv)->Clone();
		m_pBestIndivs[nActPos] = pDestSet[nActPos];
	}
}

void CFramework::MakeCrossover(CIndividual ** pParents, CIndividual ** pOffsprings)
{ 

	for (UINT nActPar = 0; nActPar < m_strEvoParams.strCrossParams.nParentsCount; nActPar++) {
			pOffsprings[nActPar] = pParents[nActPar]->Clone();
			pOffsprings[nActPar]->m_info.m_arrOpUsed[infoCrossoverSW] = true;
	}

	// Choose whether to perform HW or SW crossover
	bool bHW = (rand() / (double) RAND_MAX < m_strEvoParams.strCrossParams.fProbs[infoCrossoverHW]);
	for (UINT nActPoint = 0; nActPoint < m_strEvoParams.strCrossParams.nCrossoverPoints; nActPoint++) {
		UINT nPos = rand() % (bHW ? m_pArchitecture->GetArchSettings()->nModCount /*m_strArchParams->nMaxModulesCount*/ : m_strIndivParams.nMaxProgramSize);
		if (bHW)
			CrossIndividualsHW(pOffsprings, nPos);
		else
			CrossIndividualsSW(pOffsprings, nPos);
	}
}

void CFramework::CrossIndividualsHW(CIndividual ** pIndivs, UINT nCrossPoint)
{
	// Create temporary array for swaping the inverse module order arrays
	UINT nParCount = m_strEvoParams.strCrossParams.nParentsCount;
	UINT * pTmpModOrder = new UINT[m_pArchitecture->GetArchSettings()->nModCount /*m_strArchParams->nMaxModulesCount*/];

	for (UINT nActPar = 0; nActPar < nParCount - 1; nActPar++) {
		// Get pointers to inverse module orders
		UINT * pFirstOrder = pIndivs[nActPar]->GetArchSettings()->pModulesInverseOrder;
		UINT * pSecondOrder = pIndivs[nActPar + 1]->GetArchSettings()->pModulesInverseOrder;
		// Make the crossover
		//memcpy(pTmpModOrder, pFirstOrder, nCrossPoint);
		//memcpy(pFirstOrder, pSecondOrder, nCrossPoint);
		//memcpy(pSecondOrder, pTmpModOrder, nCrossPoint);
		// Update the module order array based on inverse module order array
		pIndivs[nActPar]->UpdateHWTopology();
	}

	delete[] pTmpModOrder;

	// Update also the last parent, which hasn't got updated yet
	pIndivs[nParCount - 1]->UpdateHWTopology();
}

void CFramework::CrossIndividualsSW(CIndividual ** pIndivs, UINT nCrossPoint)
{
	UINT nParCount = m_strEvoParams.strCrossParams.nParentsCount;
	for (UINT nActPar = 0; nActPar < nParCount - 1; nActPar++) {
		SwitchInstrBlocks(pIndivs[nActPar]->GetInstrBlockByPos(nCrossPoint), pIndivs[nActPar + 1]->GetInstrBlockByPos(nCrossPoint));
		CInstrBlock * pLast = pIndivs[nActPar]->GetLastInstr();
		pIndivs[nActPar]->SetLastInstr(pIndivs[nActPar + 1]->GetLastInstr());
		pIndivs[nActPar + 1]->SetLastInstr(pLast);
	}
}

void CFramework::SwitchInstrBlocks(CInstrBlock * pBlock1, CInstrBlock * pBlock2)
{
	CInstrBlock * pTmp;
	pTmp = pBlock1->GetPrev();
	pBlock1->SetPrev(pBlock2->GetPrev());
	pBlock2->SetPrev(pTmp);

	pTmp = pBlock1->GetPrev();
	if (pTmp)
		pTmp->SetNext(pBlock1);

	pTmp = pBlock2->GetPrev();
	if (pTmp)
		pTmp->SetNext(pBlock2);
}

void CFramework::StartEvolution()
{
	bool bFound = false;

	srand((UINT) time(NULL));
	GenerateInitialPopulation();
	CopyBestIndividuals(m_pActGeneration, m_strEvoParams.nPopulationSize, m_pBestIndivs, m_strEvoParams.nEliteSize);

	// Initial reset
	ResetAfterEval();
	for (UINT nBest = 0; nBest < m_strEvoParams.nEliteSize; nBest++) {
		m_pBestIndivs[nBest]->EvaluateFitness(m_strEvoParams.strSimulParams);
	}

	UINT i;
	for (i = 1; i < m_strEvoParams.nMaxGenerations; i++) {
		// Reset operators efficiency information
		//m_info.Reset();
		
		PrintGenerationStatistics(i);
		if (EvaluateStopCondition(i)) {
			bFound = true;
			break;
		}
		if (i == 1) {
			for (UINT nBest = 0; nBest < m_strEvoParams.nEliteSize; nBest++)
				delete m_pBestIndivs[nBest];
		}
		PopulateNextGeneration();

		if (i % 50 == 0) {
			//std::cout << i;
			UpdateOpsProbabilities();
			m_info.Reset();
		}
	}

	for (UINT nBest = 0; nBest < m_strEvoParams.nEliteSize; nBest++) {
		std::cout << m_pBestIndivs[nBest]->GetFitness(eResultsFitness) << " ";
		std::cout << m_pBestIndivs[nBest]->GetFitness(eAreaFitness) << " ";
		std::cout << m_pBestIndivs[nBest]->GetFitness(eSpeedFitness) << " ";
		std::cout << std::endl;
	}

	std::cout << m_strEvoParams.fCrossoverProbability << ";" << m_strEvoParams.fMutationProbability << ";" << (bFound ? (int) i : -1) << std::endl;
}

void CFramework::UpdateOpsProbabilities()
{
	strMutationParams & mutParams = m_strEvoParams.strMutParams;
	double fProbSum = 0;
	UINT nPortionsSum = 0;

	for (UINT nOp = infoMutationReg; nOp < infoOperatorsCount; nOp++) {
		nPortionsSum += m_info.m_arrOpUsage[nOp];
		//std::cout << "\t" << m_info.m_arrOpUsage[nOp];
	}
	//std::cout << std::endl;

	if (nPortionsSum == 0)
		return;

	for (UINT nOp = infoMutationReg; nOp < infoOperatorsCount; nOp++) {
		if (mutParams.fProbs[nOp] >= 0.05) {
			mutParams.fProbs[nOp] -= 0.05;
			fProbSum += 0.05;
		}
	}

	if (nPortionsSum > 0) {
		for (UINT nOp = infoMutationReg; nOp < infoOperatorsCount; nOp++) {
			mutParams.fProbs[nOp] += fProbSum * ((double) m_info.m_arrOpUsage[nOp] / nPortionsSum);
		}
	}
}

void CFramework::PrintGenerationStatistics(UINT nGeneration)
{
	if (m_bPrintInfo && nGeneration % 100 == 0) {
		std::cout << nGeneration << "th generation, max fitness: ";
#ifdef DETAILED_FITNESS_OUT
		std::cerr << nGeneration << "th generation, max fitness: ";
#endif
		if (m_strEvoParams.nEliteSize > 0) {
			std::cout << m_pBestIndivs[0]->GetFitness(eResultsFitness) << " ";
			std::cout << m_pBestIndivs[0]->GetFitness(eAreaFitness) << " ";
			std::cout << m_pBestIndivs[0]->GetFitness(eSpeedFitness) << " ";
			std::cout << std::endl;
#ifdef DETAILED_FITNESS_OUT
			std::cerr << m_pBestIndivs[0]->GetFitness(eResultsFitness) << " ";
			std::cerr << m_pBestIndivs[0]->GetFitness(eAreaFitness) << " ";
			std::cerr << m_pBestIndivs[0]->GetFitness(eSpeedFitness) << " ";
			std::cerr << std::endl;
#endif
		}
	}

	if (m_bPrintInfo && nGeneration % 1000 == 0) {
		m_pBestIndivs[0]->PrintProgram();
//#ifdef DETAILED_FITNESS_OUT
		m_strEvoParams.strSimulParams.bPrintInfo = false; //(nGeneration == 99000);
//#endif
		m_pBestIndivs[0]->EvaluateFitness(m_strEvoParams.strSimulParams);
//#ifdef DETAILED_FITNESS_OUT
		m_strEvoParams.strSimulParams.bPrintInfo = false;
//#endif
	}
}

bool CFramework::EvaluateStopCondition(UINT nGeneration)
{
	for (UINT nBest = 0; nBest < m_strEvoParams.nEliteSize; nBest++) {
		if (m_pBestIndivs[nBest]->GetFitness(eResultsFitness) > 9) {
			if (m_bPrintInfo) {
				m_pBestIndivs[nBest]->PrintProgram();
				system("PAUSE");
			}
			return true;
		}
	}
	
	return false;
}

void CFramework::SelectParents(CIndividual ** pParents, UINT nParCount)
{
	SELECTION_TYPE eType = m_strEvoParams.strSelectParams.eSelectionType;
	if (nParCount == -1)
		nParCount = m_strEvoParams.strCrossParams.nParentsCount;

	switch (eType) {
	case TOURNAMENT:
		UINT nTournSize = m_strEvoParams.strSelectParams.nTournamentSize;
		// Make a tournament for every parent
		for (UINT par = 0; par < nParCount; par++) {
			// Generate tournament members
			std::vector<CIndividual *> vecCandidates;
			for (UINT cand = 0; cand < nTournSize; cand++)
				vecCandidates.push_back(m_pActGeneration[rand() % m_strEvoParams.nPopulationSize]);
			
			// Sort the individuals by fitness and select the best one as a parent
			std::sort(vecCandidates.begin(), vecCandidates.end(), CFramework::CompareIndividuals);
			pParents[par] = *vecCandidates.rbegin();
		}
		break;
	}
}
