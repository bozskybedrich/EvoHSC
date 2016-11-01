#include "environment.h"
#include "errorstatus.h"
#include "tinyxml.h"

#ifdef _DEBUG
	#define new MYDEBUG_NEW
#endif

/*
 * XML file format
<environment>
	<inputs>
		<input index="0">
			<change time="123" value="875" />
			<change time="237" value="85" />
			<change time="544" value="725" />
		</input>
		<input index="1">
			<change time="154" value="235" />
		</input>
	</inputs>
	<outputs>
		<output index="0">
			<change time="330" value="34">
		</output>
	</outputs>
</environment>
*/

CEnvironment::~CEnvironment()
{
	if (m_pInputs) {
		for (UINT i = 0; i < m_nInputsCount; i++)
			delete m_pInputs[i];
		delete m_pInputs;
	}

	if (m_pExpectedOutputs) {
		for (UINT i = 0; i < m_nOutputsCount; i++) {
			delete m_pExpectedOutputs[i];
		}
		delete m_pExpectedOutputs;
	}

	DeleteRealOutputs();
}

CEnvironment::CEnvironment(const char * pXml)
	: m_pExpectedOutputs(NULL), m_pInputs(NULL), m_pRealOutputs(NULL), m_pCallbackIn(NULL), m_pCallbackOut(NULL), m_pResetAfterEval(NULL)
{
	Parse(pXml);
}

CEnvironment::CEnvironment(envCallbackIn pCallbackIn, envCallbackOut pCallbackOut, UINT nInputsCount, UINT nOutputsCount, envResetAfterEval pResetAfterEval)
{
	m_pCallbackIn = pCallbackIn;
	m_pCallbackOut = pCallbackOut;
	m_nInputsCount = nInputsCount;
	m_nOutputsCount = nOutputsCount;
	m_pInputs = NULL;
	m_pExpectedOutputs = NULL;
	m_pResetAfterEval = pResetAfterEval;

	m_pRealOutputs = new CDynamicTimeline*[m_nOutputsCount];
	for (UINT i = 0; i < m_nOutputsCount; i++)
		m_pRealOutputs[i] = new CDynamicTimeline();
}

void CEnvironment::DeleteRealOutputs()
{
	if (m_pRealOutputs) {
		for (UINT i = 0; i < m_nOutputsCount; i++) {
			delete m_pRealOutputs[i];
		}
		delete m_pRealOutputs;
		m_pRealOutputs = NULL;
	}
}

void CEnvironment::ResetRealOutputs()
{
	DeleteRealOutputs();
	m_pRealOutputs = new CDynamicTimeline*[m_nOutputsCount];
	for (UINT i = 0; i < m_nOutputsCount; i++)
		m_pRealOutputs[i] = new CDynamicTimeline();
}

void CEnvironment::ResetAfterEval()
{
	if (m_pResetAfterEval)
		m_pResetAfterEval();
}

int CEnvironment::Parse(const char * pXml)
{
	TiXmlDocument doc;
	doc.Parse(pXml);

	TiXmlElement * pEnvironment = doc.FirstChildElement("environment");
	if (!pEnvironment)
		return ERR_INPUT_ERROR;


	// Parse inputs
	TiXmlElement * pInputs = pEnvironment->FirstChildElement("inputs");
	if (!pInputs)
		return ERR_INPUT_ERROR;

	UINT nInputs = 0;
	for (TiXmlElement * pInput  = pInputs->FirstChildElement("input"); pInput; pInput = pInput->NextSiblingElement("input")) {
		UINT nIndex = atoi(pInput->Attribute("index"));
		if ((nIndex + 1) > nInputs)
			nInputs = nIndex + 1;
	}

	m_nInputsCount = nInputs;
	m_pInputs = new CDynamicTimeline*[nInputs];
	for (UINT i = 0; i < nInputs; i++)
		m_pInputs[i] = new CDynamicTimeline();

	for (TiXmlElement * pInput  = pInputs->FirstChildElement("input"); pInput; pInput = pInput->NextSiblingElement("input")) {
		UINT nActInput = atoi(pInput->Attribute("index"));
	
		for (TiXmlElement * pChange = pInput->FirstChildElement("change"); pChange; pChange = pChange->NextSiblingElement("change")) {
			m_pInputs[nActInput]->InsertItem(new strDynamicTimeVal((UINT) atol(pChange->Attribute("time")), (REGISTER_TYPE) atof(pChange->Attribute("value"))));
		}
	}

	// Parse outputs
	TiXmlElement * pOutputs = pEnvironment->FirstChildElement("outputs");
	if (!pOutputs)
		return ERR_INPUT_ERROR;

	UINT nOutputs = 0;
	for (TiXmlElement * pOutput  = pOutputs->FirstChildElement("output"); pOutput; pOutput = pOutput->NextSiblingElement("output")) {
		UINT nIndex = atoi(pOutput->Attribute("index"));
		if ((nIndex + 1) > nOutputs)
			nOutputs = nIndex + 1;
	}

	m_nOutputsCount = nOutputs;
	m_pExpectedOutputs = new CDynamicTimeline*[nOutputs];
	m_pRealOutputs = new CDynamicTimeline*[nOutputs];
	for (UINT i = 0; i < nOutputs; i++) {
		m_pExpectedOutputs[i] = new CDynamicTimeline();
		m_pRealOutputs[i] = new CDynamicTimeline();
	}

	UINT nActOutput = 0;
	for (TiXmlElement * pOutput  = pOutputs->FirstChildElement("output"); pOutput; pOutput = pOutput->NextSiblingElement("output")) {
		for (TiXmlElement * pChange = pOutput->FirstChildElement("change"); pChange; pChange = pChange->NextSiblingElement("change")) {
			m_pExpectedOutputs[nActOutput]->InsertItem(new strDynamicTimeVal(atol(pChange->Attribute("time")), atof(pChange->Attribute("value"))));
		}
	}

	return ERR_SUCCESS;
}

REGISTER_TYPE CEnvironment::GetInputValue(UINT nIndex, UINT time)
{
	if (m_pCallbackIn)
		return m_pCallbackIn(nIndex, time);

	if (nIndex >= m_nInputsCount) {
		assert(false);
		return REGISTER_UNDEF;
	}

	return (*m_pInputs[nIndex]).GetVal(time);
}

void CEnvironment::InsertRealOutput(UINT nIndex, UINT time, REGISTER_TYPE val)
{
	if (m_pCallbackOut)
		m_pCallbackOut(nIndex, time, val);

	if (nIndex >= m_nOutputsCount) {
		assert(false);
		return;
	}

	m_pRealOutputs[nIndex]->InsertItem(new strDynamicTimeVal(time, val));

	//(*m_pInputs[0]).InsertItem(new strDynamicTimeVal(time, val));
	//(*m_pInputs[3]).InsertItem(new strDynamicTimeVal(time, val));
}

void CEnvironment::PrintOutputs()
{
	for (int nOut = 0; nOut < m_nOutputsCount; nOut++) {
		std::cerr << "Otuput " << nOut << std::endl;
		for (strDynamicTimeVal * pVal = m_pRealOutputs[nOut]->GetFirst(); pVal; pVal = pVal->pSucc)
			std::cerr << "Time: " << pVal->time << " Value: " << pVal->val << " (0x" << std::hex << pVal->val << std::dec << ")" << std::endl;
		std::cerr << std::endl;
	}
}

CDynamicTimeline::~CDynamicTimeline()
{
	strDynamicTimeVal * pActVal = m_pFirst;

	while (pActVal) {
		strDynamicTimeVal * pNextVal = pActVal->pSucc;
		delete pActVal;
		pActVal = pNextVal;
	}
}

void CDynamicTimeline::InsertItem(strDynamicTimeVal * pVal)
{
	nCount++;
	if (nCount > 9)
		nCount = nCount;

	if (!m_pFirst && !m_pLast) { // If there are no items
		m_pFirst = m_pLast = pVal;
		return;
	}
	else { // If there is at least one item
		strDynamicTimeVal * pPred = m_pLast;
		// Find the last item with time less than new item
		while (pPred && !(pPred->time < pVal->time))
			pPred = pPred->pPred;

		// If it was found within the list, insert the new item
		if (pPred) {
			strDynamicTimeVal * pOldSucc = pPred->pSucc;
			pPred->pSucc = pVal;
			pVal->pSucc = pOldSucc;
			pVal->pPred = pPred;
			if (pOldSucc)
				pOldSucc->pPred = pVal;
			else
				m_pLast = pVal; // In case the item should be added at the end of list
		}
		// If it should be the first item
		else {
			pVal->pSucc = m_pFirst;
			m_pFirst->pPred = pVal;
			m_pFirst = pVal;
		}
	}
}

REGISTER_TYPE CDynamicTimeline::GetVal(UINT time)
{
	if (!m_pFirst) {
		return REG_DEF_VAL;
		//return REGISTER_UNDEF;
	}

	for (strDynamicTimeVal * pAct = m_pFirst; pAct; pAct = pAct->pSucc)
		if (pAct->time <= time && (!pAct->pSucc || pAct->pSucc->time > time))
			return pAct->val;

	return REGISTER_UNDEF;
}
