#include "ImageFilter.h"
#include <time.h>
#include <iostream>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include "../modules/modules.h"

#define	IMAGE_COUNT	1
#define	IMAGE_WIDTH	64
#define	IMAGE_HEIGHT	64
#define SAMPLES_COUNT	(IMAGE_COUNT * (IMAGE_WIDTH - 2) * (IMAGE_HEIGHT - 2))
#define INPUTS_COUNT	9
#define MODULES_COUNT	15	// 9 inputs, 1 shifter, 1 adder, 1 multiplier, 1 comparator, 1 multiplexer, 1 evaluator

REGISTER_TYPE ** pInputs;
REGISTER_TYPE ** pOutputs;

int nActIn[INPUTS_COUNT];
int nActTestSet = 0;
int nTestSetsCount = 0;
int nDamagedPixelsCount = 0;
bool bDetailedOut = false;
BYTE * pTmpImage;

void test(double fCross, double fMut);

#pragma region bitmap
bool ReadBmpHeader(FILE * fIn, BYTE ** pHeader, DWORD & dwDataOffset, DWORD & dwWidth, DWORD & dwHeight)
{
	if (fseek(fIn, 0x0A, SEEK_SET) != 0)
		return false;

	dwDataOffset = -1;
	if (fread(&dwDataOffset, sizeof(DWORD), 1, fIn) != 1)
		return false;

	if (dwDataOffset == -1)
		return false;

	DWORD pDimensions[2];
	if (fseek(fIn, 0x12, SEEK_SET) != 0)
		return false;

	if (fread(pDimensions, sizeof(DWORD), 2, fIn) != 2)
		return false;

	dwWidth = pDimensions[0];
	dwHeight = pDimensions[1];

	*pHeader = new BYTE[dwWidth*dwHeight];
	if (!*pHeader)
		return false;

	if (fseek(fIn, 0, SEEK_SET) != 0)
	{
		delete[] *pHeader;
		return false;
	}

	if (fread(*pHeader, sizeof(BYTE), dwDataOffset, fIn) != dwDataOffset)
	{
		delete[] *pHeader;
		return false;
	}

	return true;
}

bool ReadImageData(FILE * fIn, const DWORD dwOffset, const DWORD dwWidth, const DWORD dwHeight, BYTE ** pImageData)
{
	*pImageData = new BYTE[dwWidth*dwHeight];
	if (!*pImageData)
		return false;

	if (fseek(fIn, dwOffset, SEEK_SET) != 0)
	{
		delete[] *pImageData;
		return false;
	}

	if (fread(*pImageData, sizeof(BYTE), dwWidth*dwHeight, fIn) != dwWidth*dwHeight)
	{
		delete[] *pImageData;
		return false;
	}

	return true;
}

bool LoadBmp(std::string sPath, BYTE ** pHeader, BYTE ** pImage, DWORD & dwOffset, DWORD & dwWidth, DWORD & dwHeight)
{
	FILE * fIn = fopen(sPath.c_str(), "rb");
	if (!fIn)
		return false;

	dwOffset = 0;
	dwWidth = 0;
	dwHeight = 0;
	*pHeader = NULL;

	if (!ReadBmpHeader(fIn, pHeader, dwOffset, dwWidth, dwHeight))
	{
		fclose(fIn);
		return false;
	}

	if (!ReadImageData(fIn, dwOffset, dwWidth, dwHeight, pImage))
	{
		delete[] pHeader;
		fclose(fIn);
		return false;
	}

	fclose(fIn);
	return true;
}

bool AddPepperAndSalt(std::string & sIn, std::string & sOut, double fSaltProb, double fPepperProb)
{
	BYTE * pHeader = NULL;
	BYTE * pImage = NULL;
	DWORD dwWidth = 0, dwHeight = 0, dwHeaderSize = 0;

	srand((UINT) time(NULL));

	if (!LoadBmp(sIn, &pHeader, &pImage, dwHeaderSize, dwWidth, dwHeight))
		return false;

	for (size_t i = dwWidth; i < (dwHeight - 1)*dwWidth; ++i)	// omit first and last image row
	{
		if ((i % dwWidth != 0) && ((i + 1) % dwWidth != 0))	// omit first and last image column
		{
			double fRnd = rand() / (double) RAND_MAX;
			if (fRnd < fSaltProb)
				pImage[i] = 0xFF;
			else if (fRnd < (fSaltProb + fPepperProb))
				pImage[i] = 0x00;
		}
	}

	FILE * fOut = fopen(sOut.c_str(), "wb");
	bool bOutSuccess = false;
	if (fOut)
	{
		fwrite(pHeader, sizeof(BYTE), dwHeaderSize, fOut);
		fwrite(pImage, sizeof(BYTE), dwWidth*dwHeight, fOut);
		fclose(fOut);
		bOutSuccess = true;
	}

	delete[] pHeader;
	delete[] pImage;

	return bOutSuccess;
}

bool SaveRawImage(std::string sPath, DWORD dwWidth, DWORD dwHeight, BYTE * pImage)
{
	FILE * fOut = fopen(sPath.c_str(), "wb");
	if (!fOut)
		return false;
	
	fwrite(pImage, sizeof(BYTE), dwWidth*dwHeight, fOut);

	fclose(fOut);
	return true;
}

#pragma endregion bitmap

int main(int argc, char* argv[])
{
	//AddPepperAndSalt(std::string("C:\\vutbr\\lena64.bmp"), std::string("C:\\vutbr\\lena_salt_64.bmp"), 0.025, 0.025);

	if (argc == 3) {
		double fCross = atof(argv[1]);
		double fMut = atof(argv[2]);
		test(fCross, fMut);
	}
	else
		test(0.1, 0.7);

	//system("PAUSE");
#ifdef _DEBUG
	_CrtDumpMemoryLeaks();
#endif

	return 0;
}

BYTE GetPixel(BYTE * pPixels, DWORD dwWidth, int nCol, int nRow)
{
	return pPixels[nRow*dwWidth + nCol];
}

void GenerateIOs(REGISTER_TYPE ** &pInputs, REGISTER_TYPE ** &pOutputs, int & nSamplesCount)
{
	int nSamplesGap = 1;

	BYTE *pTestImageHeader = NULL, *pTestImage = NULL;
	DWORD dwTestOffset = 0, dwTestWidth = 0, dwTestHeight = 0;
	BYTE *pOrigImageHeader = NULL, *pOrigImage = NULL;
	DWORD dwOrigOffset = 0, dwOrigWidth = 0, dwOrigHeight = 0;

	LoadBmp(std::string("C:\\vutbr\\lena_salt_64.bmp"), &pTestImageHeader, &pTestImage, dwTestOffset, dwTestWidth, dwTestHeight);
	LoadBmp(std::string("C:\\vutbr\\lena64.bmp"), &pOrigImageHeader, &pOrigImage, dwOrigOffset, dwOrigWidth, dwOrigHeight);

	//SaveRawImage("C:\\vutbr\\lena_raw.raw", dwOrigWidth, dwOrigHeight, pOrigImage);

	nSamplesCount = 0;
	for (size_t nRow = 1; nRow < dwTestHeight - 1; ++nRow)	// omit first and last image row
		for (size_t nCol = 1; nCol < dwTestWidth - 1; ++nCol)
			if ((nRow * dwTestWidth + nCol) % nSamplesGap == 0)
				++nSamplesCount;

	pInputs = new REGISTER_TYPE*[nSamplesCount];
	pOutputs = new REGISTER_TYPE*[nSamplesCount];
	pTmpImage = new BYTE[nSamplesCount];

	int nSampleNo = 0;
	double dFitness = 0;
	nDamagedPixelsCount = 0;

	for (size_t nRow = 1; nRow < dwTestHeight - 1; ++nRow)	// omit first and last image row
	{
		for (size_t nCol = 1; nCol < dwTestWidth - 1; ++nCol)
		{
			if ((nRow * dwTestWidth + nCol) % nSamplesGap == 0)
			{
				pInputs[nSampleNo] = new REGISTER_TYPE[9];
				pInputs[nSampleNo][0] = GetPixel(pTestImage, dwTestWidth, nCol, nRow);
				pInputs[nSampleNo][1] = GetPixel(pTestImage, dwTestWidth, nCol - 1, nRow - 1);
				pInputs[nSampleNo][2] = GetPixel(pTestImage, dwTestWidth, nCol, nRow - 1);
				pInputs[nSampleNo][3] = GetPixel(pTestImage, dwTestWidth, nCol + 1, nRow - 1);
				pInputs[nSampleNo][4] = GetPixel(pTestImage, dwTestWidth, nCol - 1, nRow);
				pInputs[nSampleNo][5] = GetPixel(pTestImage, dwTestWidth, nCol + 1, nRow);
				pInputs[nSampleNo][6] = GetPixel(pTestImage, dwTestWidth, nCol - 1, nRow + 1);
				pInputs[nSampleNo][7] = GetPixel(pTestImage, dwTestWidth, nCol, nRow + 1);
				pInputs[nSampleNo][8] = GetPixel(pTestImage, dwTestWidth, nCol + 1, nRow + 1);

				pOutputs[nSampleNo] = new REGISTER_TYPE[1];
				pOutputs[nSampleNo][0] = GetPixel(pOrigImage, dwOrigWidth, nCol, nRow);

				if (pOutputs[nSampleNo][0] != pInputs[nSampleNo][0])
					nDamagedPixelsCount++;

				dFitness += ((1.0 - abs(pInputs[nSampleNo][0] - pOutputs[nSampleNo][0]) / 255.0));
				++nSampleNo;
			}
		}
	}

	dFitness *= (100.0 / nSamplesCount);

	delete[] pTestImage;
	delete[] pOrigImage;
}

void test(double fCross, double fMut)
{
	srand((UINT) time(NULL));
	CModule * pModules[MODULES_COUNT];

	GenerateIOs(pInputs, pOutputs, nTestSetsCount);

	for (int i = 0; i < INPUTS_COUNT; i++) {
		nActIn[i] = i;
		pModules[i] = new CPixelInputModule(pInputs, INPUTS_COUNT, i, nTestSetsCount, &nActTestSet);
	}

	pModules[INPUTS_COUNT] = new CBitShifter();
	pModules[INPUTS_COUNT + 1] = new CAdder();
	pModules[INPUTS_COUNT + 2] = new CMultiplier();
	pModules[INPUTS_COUNT + 3] = new CComparatorModule();
	//pModules[INPUTS_COUNT + 4] = new CMedianModule();
	pModules[INPUTS_COUNT + 4] = new CMultiplexerModule();
	pModules[INPUTS_COUNT + 5] = new CPixelEvaluatorModule();

	CEnvironment * pEnv = new CEnvironment(NULL, NULL, 1, 1, NULL);
	
	BYTE instMod1[] = {0x00, 0x00, 0x00, 0x01};
	BYTE instMod2[] = {0x00, 0x00, 0x00, 0x02};
	BYTE instMod3[] = {0x00, 0x00, 0x00, 0x04};
	BYTE instMod4[] = {0x00, 0x00, 0x00, 0x08};
	BYTE instMod5[] = {0x00, 0x00, 0x00, 0x10};
	BYTE instMod6[] = {0x00, 0x00, 0x00, 0x20};
	BYTE instMod7[] = {0x00, 0x00, 0x00, 0x40};
	BYTE instMod8[] = {0x00, 0x00, 0x00, 0x80};
	BYTE instMod9[] = {0x00, 0x00, 0x01, 0x00};
	BYTE instMod10[] = {0x00, 0x00, 0x02, 0x00};
	BYTE instMod11[] = {0x00, 0x00, 0x04, 0x00};
	BYTE instMod12[] = {0x00, 0x00, 0x08, 0x00};
	BYTE instMod13[] = {0x00, 0x00, 0x10, 0x00};
	BYTE instModAll[] = {0x00, 0x00, 0xFD, 0xFF};

	/*BYTE instMod128[] = {0x00, 0x00, 0x00, 0x83};
	BYTE instMod1234[] = {0x00, 0x00, 0x00, 0x0F};*/

	//BYTE instModAll[] = {0x00, 0x00, 0xFF, 0xFF};

	BYTE iosModInput[] = {0xFF, 0xFF};
	BYTE iosShifter[] = {0xFF, 0xFF, 0xC2, 0xFF};
	BYTE iosAdder[] = {0xFF, 0xFF, 0xC4, 0xFF};
	BYTE iosMultiplier[] = {0xFF, 0xFF, 0xFF};
	BYTE iosComparator[] = {0xFF, 0xFF, 0xFF};
	BYTE iosModAll[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // Input modules
		0xFF, 0xFF, 0xC2, 0xFF, // Shifter
		0xFF, 0xFF, 0xC4, 0xFF, // Adder
		0xFF, 0xFF, 0xFF, // Multiplier
		0xFF, 0xFF, 0xFF, // Comparator
		//0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // Median
		0xFF, 0xFF, 0xFF, 0xFF, // Multiplexer
		0xFF, 0xFF // Pixel evaluator
	};
	
	// MOV instructions
	BYTE instMOVr1[] = {0x80, 0x00, 0x00, 0x00};
	BYTE instMOVr2[] = {0x81, 0x00, 0x00, 0x00};
	BYTE instMOVr3[] = {0x82, 0x00, 0x00, 0x00};
	BYTE instMOVr4[] = {0x83, 0x00, 0x00, 0x00};
	BYTE instMOVr5[] = {0x84, 0x00, 0x00, 0x00};
	BYTE instMOVr6[] = {0x85, 0x00, 0x00, 0x00};

	BYTE instJMP[] = {0x10, 0x00, 0x00, 0x00};
	BYTE instJZ5[] = {0x20, 0x00, 0x00, 0x05}; // JUMP if module 5 (shifter) has zero flag
	BYTE instJS5[] = {0x30, 0x00, 0x00, 0x05}; // JUMP if module 5 has sign flag
	BYTE instJNZ5[] = {0x40, 0x00, 0x00, 0x05}; // JUMP if module 5 doesn't have zero flag
	BYTE instJNS5[] = {0x50, 0x00, 0x00, 0x05}; // JUMP if module 5 doesn't have sign flag
	BYTE instJZ6[] = {0x20, 0x00, 0x00, 0x06}; // JUMP if module 6 (shifter) has zero flag
	BYTE instJS6[] = {0x30, 0x00, 0x00, 0x06}; // JUMP if module 6 has sign flag
	BYTE instJNZ6[] = {0x40, 0x00, 0x00, 0x06}; // JUMP if module 6 doesn't have zero flag
	BYTE instJNS6[] = {0x50, 0x00, 0x00, 0x06}; // JUMP if module 6 doesn't have sign flag
	BYTE instJZ7[] = {0x20, 0x00, 0x00, 0x07}; // JUMP if module 7 (shifter) has zero flag
	BYTE instJS7[] = {0x30, 0x00, 0x00, 0x07}; // JUMP if module 7 has sign flag
	BYTE instJNZ7[] = {0x40, 0x00, 0x00, 0x07}; // JUMP if module 7 doesn't have zero flag
	BYTE instJNS7[] = {0x50, 0x00, 0x00, 0x07}; // JUMP if module 7 doesn't have sign flag
	BYTE instJZ8[] = {0x20, 0x00, 0x00, 0x08}; // JUMP if module 8 (shifter) has zero flag
	BYTE instJS8[] = {0x30, 0x00, 0x00, 0x08}; // JUMP if module 8 has sign flag
	BYTE instJNZ8[] = {0x40, 0x00, 0x00, 0x08}; // JUMP if module 8 doesn't have zero flag
	BYTE instJNS8[] = {0x50, 0x00, 0x00, 0x08}; // JUMP if module 8 doesn't have sign flag
	BYTE instJZ9[] = {0x20, 0x00, 0x00, 0x09}; // JUMP if module 9 (shifter) has zero flag
	BYTE instJS9[] = {0x30, 0x00, 0x00, 0x09}; // JUMP if module 9 has sign flag
	BYTE instJNZ9[] = {0x40, 0x00, 0x00, 0x09}; // JUMP if module 9 doesn't have zero flag
	BYTE instJNS9[] = {0x50, 0x00, 0x00, 0x09}; // JUMP if module 9 doesn't have sign flag
	BYTE instJZ10[] = {0x20, 0x00, 0x00, 0x0A}; // JUMP if module 10 (shifter) has zero flag
	BYTE instJS10[] = {0x30, 0x00, 0x00, 0x0A}; // JUMP if module 10 has sign flag
	BYTE instJNZ10[] = {0x40, 0x00, 0x00, 0x0A}; // JUMP if module 10 doesn't have zero flag
	BYTE instJNS10[] = {0x50, 0x00, 0x00, 0x0A}; // JUMP if module 10 doesn't have sign flag
	BYTE instJZ11[] = {0x20, 0x00, 0x00, 0x0B}; // JUMP if module 11 (shifter) has zero flag
	BYTE instJS11[] = {0x30, 0x00, 0x00, 0x0B}; // JUMP if module 11 has sign flag
	BYTE instJNZ11[] = {0x40, 0x00, 0x00, 0x0B}; // JUMP if module 11 doesn't have zero flag
	BYTE instJNS11[] = {0x50, 0x00, 0x00, 0x0B}; // JUMP if module 11 doesn't have sign flag
	BYTE instJZ12[] = {0x20, 0x00, 0x00, 0x0C}; // JUMP if module 12 (shifter) has zero flag
	BYTE instJS12[] = {0x30, 0x00, 0x00, 0x0C}; // JUMP if module 12 has sign flag
	BYTE instJNZ12[] = {0x40, 0x00, 0x00, 0x0C}; // JUMP if module 12 doesn't have zero flag
	BYTE instJNS12[] = {0x50, 0x00, 0x00, 0x0C}; // JUMP if module 12 doesn't have sign flag

	BYTE instrOUTr1[] = {0xC0, 0x00, 0x00, 0x00};
	BYTE instrOUTr2[] = {0xC1, 0x00, 0x00, 0x00};
	BYTE instrOUTr3[] = {0xC2, 0x00, 0x00, 0x00};
	BYTE instrOUTr4[] = {0xC3, 0x00, 0x00, 0x00};
	BYTE instrOUTr5[] = {0xC4, 0x00, 0x00, 0x00};
	BYTE instrOUTr6[] = {0xC5, 0x00, 0x00, 0x00};
	BYTE instrOUTr7[] = {0xC6, 0x00, 0x00, 0x00};
	BYTE instrOUTr8[] = {0xC7, 0x00, 0x00, 0x00};
	BYTE instrOUTr9[] = {0xC8, 0x00, 0x00, 0x00};

	BYTE instrNOP[] = { 0x00, 0x00, 0x00, 0x00 };
	BYTE instrRST[] = { 0xFF, 0xFF, 0xFF, 0xFF };
#define INSTR_COUNT (28)

	CInstruction* arrInstrSet[INSTR_COUNT] = { 
									new CInstruction(4, instMOVr1, true, std::string("MOVr1")),
									new CInstruction(4, instMOVr2, true, std::string("MOVr2")),
									new CInstruction(4, instMOVr3, true, std::string("MOVr3")),
									new CInstruction(4, instMOVr4, true, std::string("MOVr4")),
									new CInstruction(4, instMOVr5, true, std::string("MOVr5")),
									new CInstruction(4, instMOVr6, true, std::string("MOVr6")),

									/*new CInstruction(4, instJMP, true, std::string("JMP")),
									new CInstruction(4, instJZ5, true, std::string("JZ5")),
									new CInstruction(4, instJZ6, true, std::string("JZ6")),
									new CInstruction(4, instJZ7, true, std::string("JZ7")),
									new CInstruction(4, instJZ8, true, std::string("JZ8")),
									new CInstruction(4, instJZ9, true, std::string("JZ9")),
									new CInstruction(4, instJZ10, true, std::string("JZ10")),
									new CInstruction(4, instJZ11, true, std::string("JZ11")),
									new CInstruction(4, instJZ12, true, std::string("JZ12")),
									new CInstruction(4, instJNZ5, true, std::string("JNZ5")),
									new CInstruction(4, instJNZ6, true, std::string("JNZ6")),
									new CInstruction(4, instJNZ7, true, std::string("JNZ7")),
									new CInstruction(4, instJNZ8, true, std::string("JNZ8")),
									new CInstruction(4, instJNZ9, true, std::string("JNZ9")),
									new CInstruction(4, instJNZ10, true, std::string("JNZ10")),
									new CInstruction(4, instJNZ11, true, std::string("JNZ11")),
									new CInstruction(4, instJNZ12, true, std::string("JNZ12")),
									new CInstruction(4, instJS5, true, std::string("JS5")),
									new CInstruction(4, instJS6, true, std::string("JS6")),
									new CInstruction(4, instJS7, true, std::string("JS7")),
									new CInstruction(4, instJS8, true, std::string("JS8")),
									new CInstruction(4, instJS9, true, std::string("JS9")),
									new CInstruction(4, instJS10, true, std::string("JS10")),
									new CInstruction(4, instJS11, true, std::string("JS11")),
									new CInstruction(4, instJS12, true, std::string("JS12")),
									new CInstruction(4, instJNS5, true, std::string("JNS5")),
									new CInstruction(4, instJNS6, true, std::string("JNS6")),
									new CInstruction(4, instJNS7, true, std::string("JNS7")),
									new CInstruction(4, instJNS8, true, std::string("JNS8")),
									new CInstruction(4, instJNS9, true, std::string("JNS9")),
									new CInstruction(4, instJNS10, true, std::string("JNS10")),
									new CInstruction(4, instJNS11, true, std::string("JNS11")),
									new CInstruction(4, instJNS12, true, std::string("JNS12")),*/
						
									new CInstruction(4, instrOUTr1, true, std::string("OUTr1")),
									new CInstruction(4, instrOUTr2, true, std::string("OUTr2")),
									new CInstruction(4, instrOUTr3, true, std::string("OUTr3")),
									new CInstruction(4, instrOUTr4, true, std::string("OUTr4")),
									new CInstruction(4, instrOUTr5, true, std::string("OUTr5")),
									new CInstruction(4, instrOUTr6, true, std::string("OUTr6")),
									new CInstruction(4, instrOUTr7, true, std::string("OUTr7")),
									new CInstruction(4, instrOUTr8, true, std::string("OUTr8")),
									new CInstruction(4, instrOUTr9, true, std::string("OUTr9")),

									new CInstruction(4, instrNOP, false, std::string("NOP")),
									new CInstruction(4, instrRST, false, std::string("RST")),
									
									/*new CInstruction(4, instMod1, false, std::string("MOD1"), 1, iosModInput, 1),
									new CInstruction(4, instMod2, false, std::string("MOD2"), 1, iosModInput, 1),
									new CInstruction(4, instMod3, false, std::string("MOD3"), 1, iosModInput, 1),
									new CInstruction(4, instMod4, false, std::string("MOD4"), 1, iosModInput, 1),
									new CInstruction(4, instMod5, false, std::string("MOD5"), 1, iosModInput, 1),
									new CInstruction(4, instMod6, false, std::string("MOD6"), 1, iosModInput, 1),
									new CInstruction(4, instMod7, false, std::string("MOD7"), 1, iosModInput, 1),
									new CInstruction(4, instMod8, false, std::string("MOD8"), 1, iosModInput, 1),
									new CInstruction(4, instMod9, false, std::string("MOD9"), 1, iosModInput, 1),
									new CInstruction(4, instMod10, false, std::string("SHIFT"), 1, iosShifter, 4),
									new CInstruction(4, instMod11, false, std::string("ADD"), 1, iosAdder, 4),
									new CInstruction(4, instMod12, false, std::string("MULT"), 1, iosMultiplier, 3),
									new CInstruction(4, instMod13, false, std::string("COMP"), 1, iosComparator, 3),*/
									new CInstruction(4, instModAll, false, std::string("ALL"), 1, iosModAll, 38),
									new CInstruction(4, instModAll, false, std::string("ALL"), 1, iosModAll, 38),
									new CInstruction(4, instModAll, false, std::string("ALL"), 1, iosModAll, 38),
									new CInstruction(4, instModAll, false, std::string("ALL"), 1, iosModAll, 38),
									new CInstruction(4, instModAll, false, std::string("ALL"), 1, iosModAll, 38),
									new CInstruction(4, instModAll, false, std::string("ALL"), 1, iosModAll, 38),
									new CInstruction(4, instModAll, false, std::string("ALL"), 1, iosModAll, 38),
									new CInstruction(4, instModAll, false, std::string("ALL"), 1, iosModAll, 38),
									new CInstruction(4, instModAll, false, std::string("ALL"), 1, iosModAll, 38),
									new CInstruction(4, instModAll, false, std::string("ALL"), 1, iosModAll, 38),
									new CInstruction(4, instModAll, false, std::string("ALL"), 1, iosModAll, 38)
	};
	
	strArchitectureParams archParams;
	archParams.nMaxModulesCount = MODULES_COUNT;
	archParams.nMaxRegistersCount = 10;
	archParams.pAllowedInstructions = arrInstrSet;
	archParams.pAllowedModules = pModules;
	
	bool bDynamic = true;
	for (UINT i = 0; i < 50; i++) {
		CArchitecture * pArch = new CArchitecture(10, (REGISTER_TYPE) 0xFFFFFFFF, MODULES_COUNT, pModules, INSTR_COUNT, arrInstrSet, &archParams, bDynamic);
		strArchitectureSettings settings(MODULES_COUNT);
		pArch->SetArchSettings(&settings);

		strEvolutionParams evoParams(5, 50000, 0.1, 0.7,/*fCross, 0.1*prob,*/
			1);
		evoParams.strMutParams.fProbs[infoMutationReg] = 0;
		evoParams.strMutParams.fProbs[infoMutationModule] = 0;

		evoParams.strSimulParams.nMaxLogicTime = 200;
				
		strIndividualParams indivParams(10,5);
		CImageFilterFramework * pFramework = new CImageFilterFramework(pArch, pEnv, evoParams, indivParams);
		pFramework->StartEvolution();
		delete pFramework;
		delete pArch;
	}
	
	delete pEnv;

	for (int i = 0; i < MODULES_COUNT; i++) {
		delete pModules[i];
	}
	
	for (UINT i = 0; i < INSTR_COUNT; i++)
		delete arrInstrSet[i];

	delete[] pInputs;
	delete[] pOutputs;
	delete[] pTmpImage;

	//tmpArch.ExecuteProgram((void*) program, sizeof(program), &env);
	return;
}

double CImageFilterIndiv::EvaluateOutputs(CEnvironment * pEnv)
{
	double dFitness = 0;
	CDynamicTimeline * pTimeline = pEnv->GetOutput(0);
	if (pTimeline) {
		strDynamicTimeVal * pVal = pTimeline->GetFirst();
		if (pVal) {
			pTmpImage[nActTestSet] = BYTE(pVal->val);

			if (abs(pVal->val - pOutputs[nActTestSet][0]) < 256)
				dFitness = (100.0 / nTestSetsCount /*nDamagedPixelsCount*/) * (1.0 - abs(pVal->val - pOutputs[nActTestSet][0]) / 255.0);
			else
				dFitness = 0;

			//dFitness = pVal->val / (double) (nActTestSet + 1);
			//dFitness = (100.0 / nTestSetsCount) * (1 / pow((double) (1 + abs(pVal->val - pOutputs[nActTestSet][0])), 2));
			//dFitness = (pVal->val == pOutputs[nActTestSet][0]) ? 1 : 0;
			if (bDetailedOut)
				std::cerr << pVal->val << std::endl;
		}
	}

	return dFitness;
}

int CImageFilterIndiv::PregenerateProgram()
{
	return 0;
	BYTE * pIOs = new BYTE;
	*pIOs = 0x00;
	AppendInstrBlock(INSTR_COUNT - 12, 0, pIOs);
	pIOs = new BYTE;
	*pIOs = 0x01;
	AppendInstrBlock(INSTR_COUNT - 11, 0, pIOs);
	pIOs = new BYTE;
	*pIOs = 0x02;
	AppendInstrBlock(INSTR_COUNT - 10, 0, pIOs);
	pIOs = new BYTE;
	*pIOs = 0x03;
	AppendInstrBlock(INSTR_COUNT - 9, 0, pIOs);
	pIOs = new BYTE;
	*pIOs = 0x04;
	AppendInstrBlock(INSTR_COUNT - 8, 0, pIOs);
	pIOs = new BYTE;
	*pIOs = 0x05;
	AppendInstrBlock(INSTR_COUNT - 7, 0, pIOs);
	pIOs = new BYTE;
	*pIOs = 0x06;
	AppendInstrBlock(INSTR_COUNT - 6, 0, pIOs);
	pIOs = new BYTE;
	*pIOs = 0x07;
	AppendInstrBlock(INSTR_COUNT - 5, 0, pIOs);
	pIOs = new BYTE;
	*pIOs = 0x08;
	AppendInstrBlock(INSTR_COUNT - 4, 0, pIOs);

	return 9;
}

void CImageFilterIndiv::PrintProgram()
{
	for (UINT m = 0; m < m_pArch->GetModulesCount(); m++) {
		printf("mod%d: %d\n", m, m_pArchSettings->pModulesOrder[m]);
	}

	for (UINT i = 0; i < m_pArch->GetRegistersCount(); i++)
		printf("reg%d: %X\n", i, m_pArch->GetRegisterMask(i));

	for (CInstrBlock * pActInstr = m_pFirstInstr; pActInstr != NULL; pActInstr = pActInstr->GetNext()) {
		for (UINT nActInstr = 0; nActInstr < pActInstr->GetInstrCount(); nActInstr++) {
			UINT nInstrNo = pActInstr->GetInstrNo(nActInstr);
			CInstruction * pInstr = m_pArch->GetInstruction(nInstrNo);
			if (pInstr->IsParametric())
#ifndef FP_REGISTER_TYPE
				printf("%s %d", pInstr->GetDescription().c_str(), pActInstr->GetParam(nActInstr));
#else
				printf("%s %f", pInstr->GetDescription().c_str(), pActInstr->GetParam(nActInstr));
#endif
			else
				printf("%s", pInstr->GetDescription().c_str());
			
			if (pInstr->GetInsOuts() != NULL) {
				BYTE * pIOs = pActInstr->GetInstrIOs(nActInstr);
				BYTE * pInstrIOs = pInstr->GetInsOuts();
				for (UINT nIO = 0; nIO < pInstr->GetInsOutsCount(); nIO++) {
					if (pInstrIOs[nIO] != 0xFF && ((pInstrIOs[nIO] & 0x80) != 0) && ((pInstrIOs[nIO] & 0xC0) != 0xC0))
						printf(" %X", pInstrIOs[nIO]);
					else
						printf(" %X", pIOs[nIO]);
				}
				printf("\n");
			}
			else
				printf("\n");
		}
	}
}

double CImageFilterIndiv::EvaluateSpeed(UINT nExecTime, UINT nMaxTime)
{
	double dFitness = 0;
	CDynamicTimeline * pTimeline = m_pEnv->GetOutput(0);
	if (pTimeline) {
		strDynamicTimeVal * pVal = pTimeline->GetFirst();
		if (pVal)
			dFitness = 1 - ((double) pVal->time / nMaxTime);
	}

	return dFitness;
}

bool CImageFilterFramework::EvaluateStopCondition(UINT nGeneration)
{
	static bool bPartialPrinted;
	if (nGeneration == 1)
		bPartialPrinted = false;

	for (UINT nBest = 0; nBest < m_strEvoParams.nEliteSize; nBest++) {
		//if (nGeneration == 1)
		//	m_pBestIndivs[nBest]->EvaluateFitness(m_strEvoParams.strSimulParams);
		if (m_pBestIndivs[nBest]->GetFitness(eResultsFitness) >= 98) {
			if (true) {
				if (m_bPrintInfo) {
					m_pBestIndivs[nBest]->PrintProgram();
					m_strEvoParams.strSimulParams.bPrintInfo = false;
					m_pBestIndivs[nBest]->EvaluateFitness(m_strEvoParams.strSimulParams);
					m_strEvoParams.strSimulParams.bPrintInfo = false;
					SaveRawImage("C:\\vutbr\\result_raw.raw", 62, 62, pTmpImage);
					system("PAUSE");
				}
			
				return true;
			}
		}
	}
	
	return false;
}

void CImageFilterFramework::ResetAfterEval()
{
	for (int i = 0; i < INPUTS_COUNT; ++i)
		nActIn[i] = i;
}

bool CImageFilterFramework::IsNextInputSet()
{
	if (nActTestSet < (nTestSetsCount - 1)) {
		nActTestSet++;
		return true;
	}
	else {
		nActTestSet = 0;
		return false;
	}
}
