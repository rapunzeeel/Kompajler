/* Autor: Nevena Prokic Datum: 30.05.2021. */
#include "InterferenceGraph.h"

static InterferenceGraph ig;

void printInterferenceMatrix()
{
	if (ig.matrix.size() == 0)
	{
		cout << "There is nothing to print!" << endl;
		return;
	}

	cout << "==========================================" << endl;
	cout << "Interference matrix:" << endl;
	cout << "==========================================" << endl;

	for (auto varIt = ig.variables->begin(); varIt != ig.variables->end(); varIt++)
	{
		if ((*varIt)->getType() == Variable::VariableType::REG_VAR)
			cout << "\t" << (*varIt)->getName();
	}
	cout << endl;

	auto varIt = ig.variables->begin();
	for (auto it1 = ig.matrix.begin(); it1 != ig.matrix.end(); it1++)
	{
		if ((*varIt)->getType() == Variable::VariableType::REG_VAR) 
			cout << (*varIt++)->getName();
		for (auto it2 = (*it1).begin(); it2 != (*it1).end(); it2++)
		{
			cout << "\t" << *it2;
		}
		cout << endl;
	}
	cout << "==========================================" << endl;
}

bool check(Variable* var, Variables listOfVar) 
{
	for (Variable* v : listOfVar) 
	{
		if (var->getName() == v->getName()) 
		{
			return true;
		}
	}
	return false;
}

//pravljenje lista varijabli koje dolaze u obzir za graf smetnji
void makeListOfVariable(Instructions& instructions) {
	ig.variables = new list<Variable*>;

	for (Instruction* ins : instructions) 
	{
		for (Variable* varDef : ins->getDef()) 
		{
			ig.variables->push_back(varDef);
		}
		for (Variable* varIn : ins->getIn()) 
		{
			ig.variables->push_back(varIn);
		}
		for (Variable* varOut : ins->getOut()) 
		{
			ig.variables->push_back(varOut);

		}
	}
	ig.variables->sort();
	ig.variables->unique();
}

//inicijalizacija matrice grafa smetnji
void initialisationMatrix() 
{
	vector<int>vec;
	for (int i = 0; i < ig.variables->size(); i++) 
	{
		vec.push_back(__EMPTY__);
	}
	for (int i = 0; i < ig.variables->size(); i++) 
	{
		ig.matrix.push_back(vec);
	}
}

//popunjavanje matrice smetnji
void makeMatrix(Instructions& instructions, int num) 
{
	initialisationMatrix();
	
	for (Variable* var : *ig.variables) 
	{
		if (var->getType() == Variable::VariableType::REG_VAR) 
		{ 
			for (Instruction* ins : instructions) 
			{
				if (check(var, ins->getIn())) 
				{ //ako je varijabla u in skupu obelezi njene smetnje
					writeInterferenceIn(ins, var, num);
				}
				if (check(var, ins->getOut())) 
				{ //ako je varijabla u out skupu obelezi njene smetnje
					writeInterferenceOut(ins, var, num);
				}
			}
		}
	}
}

void writeInterferenceOut(Instruction* ins, Variable* var, int num)
{
	for (Variable* varOut : ins->getOut()) 
	{
		if (varOut->getType() == Variable::VariableType::REG_VAR) 
		{
			if (var->getName() != varOut->getName()) 
			{
				ig.matrix[var->getPosition() - num][varOut->getPosition() - num] = __INTERFERENCE__;
			}
		}
	}
}

void writeInterferenceIn(Instruction* ins, Variable* var, int num)
{
	for (Variable* varIn : ins->getIn()) 
	{
		if (varIn->getType() == Variable::VariableType::REG_VAR) 
		{
			if (var->getName() != varIn->getName()) 
			{
				ig.matrix[var->getPosition() - num][varIn->getPosition() - num] = __INTERFERENCE__;
			}
		}
	}
}

//pravljenje grafa smetnji
InterferenceGraph& buildInterferenceGraph(Instructions& instructions, int num)
{
	makeListOfVariable(instructions);
	makeMatrix(instructions, num);

	return ig;
}