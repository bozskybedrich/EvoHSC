#include "architecture.h"
#include "environment.h"

int main(int argc, char *argv[])
{
	CAdder addMod1;
	CAdder addMod2;
	
	char xml[] = "\
		<environment>\
  <inputs>\
    <input index=\"0\">\
      <change time=\"0\" value=\"2\" />\
    </input>\
    <input index=\"3\">\
      <change time=\"0\" value=\"2\" />\
    </input>\
  </inputs>\
  <outputs>\
    <output index=\"0\">\
      <change time=\"330\" value=\"34\">\
    </output>\
  </outputs>\
</environment>";

	CEnvironment env(xml);
	CDynamicTimeline * pOutputs = new CDynamicTimeline[env.GetOutputsCount()];

	BYTE instLOAD1[] = {0x00, 0x00, 0x00, 0x01};
	BYTE instLOAD2[] = {0x00, 0x00, 0x00, 0x02};
	BYTE instADD[] = {0x01, 0x00, 0x00, 0x00, 0x01, 0x02, 0x80, 0x02}; // ADD r1,r2 -> r3; LOAD r1, const
	BYTE instJMP[] = {0x00, 0x00, 0x00, 0x10};
	BYTE instJZ[] = {0x01, 0x00, 0x00, 0x20}; // JUMP if module 1 has zero flag
	BYTE instJNS[] = {0x01, 0x00, 0x00, 0x50}; // JUMP if module 1 doesn't have zero flag
	BYTE instDEC[] = {0x01, 0x00, 0x00, 0x00/*, 0x00, 0x00, 0x83, 0x00*/}; // DEC r1
	BYTE instLOAD3[] = {0x00, 0x00, 0x00, 0x03};
	BYTE instMOVr1[] = {0x00, 0x00, 0x00, 0x81};
	BYTE instrINr1[] = {0x00, 0x00, 0x00, 0x81};
	BYTE instrINr2[] = {0x00, 0x00, 0x00, 0x82};
	BYTE instrOUTr3[] = {0x00, 0x00, 0x00, 0x83};
#define INSTR_COUNT 12
	CInstruction ** pArrInstrSet = new CInstruction*[INSTR_COUNT];
	pArrInstrSet[0] = new CInstruction(4, instLOAD1, true);
	pArrInstrSet[1] = new CInstruction(4, instLOAD2, true);
	pArrInstrSet[2] = new CInstruction(8, instADD, false);
	pArrInstrSet[3] = new CInstruction(4, instJMP, true);
	pArrInstrSet[4] = new CInstruction(4, instDEC, true);
	pArrInstrSet[5] = new CInstruction(4, instJZ, true);
	pArrInstrSet[6] = new CInstruction(4, instJNS, true);
	pArrInstrSet[7] = new CInstruction(4, instLOAD3, true);
	pArrInstrSet[8] = new CInstruction(4, instMOVr1, true);
	pArrInstrSet[9] = new CInstruction(4, instrINr1, true);
	pArrInstrSet[10] = new CInstruction(4, instrINr2, true);
	pArrInstrSet[11] = new CInstruction(4, instrOUTr3, true);
	
	CModule * modules[] = { &addMod1, &addMod2 };
	CArchitecture tmpArch(4, (REGISTER_TYPE) 0xFFFFFFFF, 2, modules, INSTR_COUNT, pArrInstrSet);
	BYTE program[] = {	/*0x00, 0x04, 0x00, 0x00, 0x00,*/ // load r1, 4
						0x09, 0x00, 0x00, 0x00, 0x80, // IN r1
						0x04, 0x00, 0x00, 0x83, 0x00,
						/*0x01, 0x02, 0x00, 0x00, 0x00,*/ // load r2, 2
						0x0A, 0x03, 0x00, 0x00, 0x80, // IN r2
						0x07, 0x00, 0x00, 0x00, 0x00, // load r3, 0
						0x02, /*0x07, 0x00, 0x00, 0x00,*/ // instADD
						/*0x03, 0x05, 0x00, 0x00, 0x00, // JMP to DEC
						0x00, 0x04, 0x00, 0x00, 0x00,*/
						0x04, 0x00, 0x00, 0x83, 0x00, // DEC r1
						0x06, 0xF5, 0xFF, 0xFF, 0xFF,
						0x0B, 0x00, 0x00, 0x00, 0x40, // OUT r3
						0x03, 0xD7, 0xFF, 0xFF, 0xFF
						//0x08, 0x02, 0x00, 0x00, 0x00
	};

	//tmpArch.ExecuteProgram((void*) program, sizeof(program), &env);
	return 0;
}