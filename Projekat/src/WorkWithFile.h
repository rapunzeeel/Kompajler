/* Autor: Nevena Prokic Datum: 30.05.2021. */
#pragma once
#include "IR.h"
#include <fstream>
#include <iostream>
#include <string>

class WriteFile {
public:
	WriteFile(string outFileName, Instructions* i, GlobalDatas* g, Variables* v) :
		 m_fileName(outFileName), m_instructions(i), m_data(g), m_variables(v) {}

	void write();
	/*string convertInstrToString(InstructionType type);
	string convertRegToString(Regs r);
	GlobalData* isLabelExist(int position);*/

private:
	Instructions* m_instructions;
	GlobalDatas* m_data;
	Variables* m_variables;

	string m_fileName;
	ofstream m_outputFile;

	void writeGlobal();
	void writeData();
	void writeText();
};