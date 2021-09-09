/* Autor: Nevena Prokic Datum: 04.05.2021. */
#include <iostream>
#include <exception>

#include "SyntaxAnalysis.h"
#include "LivenessAnalysis.h"
#include "InterferenceGraph.h"
#include "Simplification.h"
#include "ResourceAllocation.h"
#include "SelectionOfInstructions.h"
#include "MakeFlowControl.h"
#include "WorkWithFile.h"

using namespace std;



int main()
{
	try
	{
		std::string fileName = ".\\..\\examples\\simple.mavn";//simple.mavn  multiply
		string outputFileName = ".\\..\\izlaz.s";
		bool retVal = false;

		LexicalAnalysis lex;

		if (!lex.readInputFile(fileName))
			throw runtime_error("\nException! Failed to open input file!\n");

		lex.initialize();

		retVal = lex.Do();

		if (retVal)
		{
			cout << "Lexical analysis finished successfully!" << endl;
			lex.printTokens();
		}
		else
		{
			lex.printLexError();
			throw runtime_error("\nException! Lexical analysis failed!\n");
		}
		//Sintaksna analiza
		SyntaxAnalysis syntax(lex);

		if (syntax.Do())
		{
			cout << "Syntax analysis finished successfully!" << endl;
		}
		else
		{
			throw runtime_error("\nException! Syntax analysis failed!\n");
		}

		Variables* variables = makeVariables(lex);//formiranje varijabli
		GlobalDatas* data = makeGlobalDatas(lex);//formiranje labela i funkcija
		Instructions* instructions = makeInstruction(lex, data, variables);//formiranje instrukcija

		//formiranje toka upravljanja
		makeFlowControl(instructions);
		//analiza zivotnog veka promenljivih
		livenessAnalysis(*instructions);

		int num = getNumberOfMemVariables(variables);
		for (Instruction* in : *instructions) 
		{
			in->printInstruction();
		}

		//formiranje grafa smetnji
		InterferenceGraph& ig = buildInterferenceGraph(*instructions, num);
		printInterferenceMatrix();

		//formiranje steka
		std::stack<Variable*>* simplificationStack = doSimplification(ig, *variables, num);
		if (simplificationStack == NULL)
		{
			throw runtime_error("\nSpill detected!\n");
		}
		else
		{
			//dodela resursa
			if (doResourceAllocation(simplificationStack, &ig, num) == true)
			{
				cout << "Resource allocation finished successfully!" << endl;
			}
			else {
				throw runtime_error("\nResource allocation failed!\n");
			}
		}

		//upis u izlazni fajl
		WriteFile* w = new WriteFile(outputFileName, instructions, data, variables);
		w->write();

	}
	catch (runtime_error e) 
	{
		cout << e.what() << endl;
		return 1;
	}

	return 0;
}



	