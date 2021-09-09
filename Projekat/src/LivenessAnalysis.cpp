/* Autor: Nevena Prokic Datum: 30.05.2021. */
#include "LivenessAnalysis.h"

void printInstructions(Instructions instrs)
{
	Instructions::iterator iter;

	for (iter = instrs.begin(); iter != instrs.end(); iter++)
	{
		Instruction* instr = *iter;

		printf("-----------------------------------------------\n");
		instr->printInstruction();
		printf("-----------------------------------------------\n");
	}
}
//provera da li varijabla vec postoji u listi 
bool variableExists(Variable* v, Variables variables) {
	Variables::iterator it;

	for (it = variables.begin(); it != variables.end(); it++)
	{
		if (*it == v)
			return false;
	}
	return true;
}

void livenessAnalysis(Instructions& instructions)
{
	for (Instruction* instr : instructions) 
	{
		instr->getIn().clear();
		instr->getOut().clear();
	}

	bool change = true;
	while (change) 
	{
		for (int i = 0; i < 2; i++) 
		{
			for (Instructions::reverse_iterator it = instructions.rbegin(); it != instructions.rend(); it++) 
			{
				Instruction* instr = *it;
				Variables inPrim = instr->getIn();
				Variables outPrim = instr->getOut();

				for (Instruction* succ : instr->getSucc()) //out je unija skupova svih naslednika
				{
					for (Variable* var : succ->getIn()) 
					{
						if (var->getType() == 1) 
						{
							if (variableExists(var, instr->getOut()))
								instr->setOut(var);
						}
					}
				}
				for (Variable* var : instr->getUse()) //popunjavanje in skupa sa svim varijablama iz use skupa
				{
					if (var->getType() == 1)
					{
						if (variableExists(var, instr->getIn()))
							instr->setIn(var);
					}
				}
				for (Variable* var : instr->getOut())  //popunjavanje in skupa sa svim varijablama koje su u out a nisu u def
				{
					if (variableExists(var, instr->getDef()))
					{
						if (var->getType() == 1)
						{
							if (variableExists(var, instr->getIn()))
								instr->setIn(var);
						}
					}
				}
				instr->getIn().sort();
				instr->getIn().unique();
				instr->getOut().sort();
				instr->getOut().unique();

				if ((inPrim == instr->getIn()) && (outPrim == instr->getOut())) 
				{
					change = false;
				}
			}
		}
	}
}
