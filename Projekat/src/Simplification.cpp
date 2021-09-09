/* Autor: Nevena Prokic Datum: 30.05.2021. */
#include "Simplification.h"

//promenljiva nije na steku
bool notOnStack(string v, vector<string> s) 
{
	for (int i = 0; i < s.size(); i++) 
	{
		if (strcmp(v.c_str(), s[i].c_str()) == 0) 
		{
			return false;
		}
	}
	return true;
}

std::stack<Variable*>* doSimplification(InterferenceGraph& ig, Variables& variables, int num) 
{
	std::stack<Variable*>* s = new SimplificationStack;

	setDegreeForVariables(variables, ig, num);//postavljanje stepena varijabli

	vector<string> variablesOnStack;
	int max = __REG_NUMBER__ - 1;//maksimalni stepen

	while (max != -1) 
	{
		Variables::iterator var;
		for (var = variables.begin(); var != variables.end(); var++)
		{

			if ((*var)->getDegree() == max)
			{
				if (notOnStack((*var)->getName(), variablesOnStack))//ukoliko je stepen tekuce varijable jednak max i ako varijabla nije vec na steku
				{
					s->push(*var); //postavljanje varijable na stek
					reduceDegreeForVariables(var, variables);
				}
				variablesOnStack.push_back((*var)->getName());//dodavanje u listu varijabli koje su na steku
			}
		}

		if ((variablesOnStack.size() + num) == variables.size())
			break;

		bool check = false;
		Variables::iterator v;
		for (v = variables.begin(); v != variables.end(); v++) 
		{ //provera da li postoji jos neka varijabla sa max stepenom nakon sklanjanja nekih veza iz grafa
			if (((*v)->getDegree() == max)&&(max!=0))
				check = true;
		}
		if(check == false)
			max--;
	}
	if (variablesOnStack.size()+num < variables.size())//ako nisu sve varijable na steku
		return NULL;

	return s;
}
//smanjenje stepena varijable koja je postavljena na stek i svih varijabli sa kojima je u smetnji
void reduceDegreeForVariables(Variables::iterator& var, Variables& variables)
{
	if ((*var)->getDegree() != 0)
	{
		Variables::iterator v;
		for (v = variables.begin(); v != variables.end(); v++)
		{
			for (int i = 0; i < (*var)->getInterference().size(); i++)
			{
				if (((*v)->getName() == (*var)->getInterference()[i]))
				{
					(*v)->reduceDegree();
					(*var)->reduceDegree();
				}
			}
		}
	}
}

//za svaku varijablu koja je REG treba postaviti stepen(broj varijabli sa kojima je u smetnji)
void setDegreeForVariables(Variables& variables, InterferenceGraph& ig, int number)
{
	Variables::iterator var;
	int num = 0;
	for (var = variables.begin(); var != variables.end(); var++) 
	{
		if ((*var)->getType() == Variable::VariableType::REG_VAR) 
		{
			int degree = 0;
			Variables::iterator v = variables.begin();
			for (int i = 0; i < number; i++) 
			{
				v++;
			}
			for (auto it1 = ig.matrix[num-number].begin(); it1 != ig.matrix[num-number].end(); it1++)
			{
				if ((*it1) == 1) 
				{
					degree++; 
					(*var)->setInterference((*v)->getName());//zapisivanje sa kojom varijablom je u smetnji
				}
				v++;
			}
			(*var)->setDegree(degree);
		}
		num++;
	}
}

