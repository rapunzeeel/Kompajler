/* Autor: Nevena Prokic Datum: 30.05.2021. */
#include "IR.h"

void Variable::printVariable()
{
	printf("(Name: %s, Pos: %d, Assignment: %d)  ", m_name.c_str(), m_position, m_assignment);
	cout << m_type << endl;
}

void Instruction::printInstruction()
{
	cout << "-----------------------------------------------------------------" << endl;
	printf("%d\n\n", m_position);

	printf("Type: %d\n\n", m_type);

	if (m_pred.size() != 0) 
	{
		printf("\nPRED:\t");
		cout << (*m_pred.begin())->getType() << endl;
		printf("\n");
	}
	if (m_succ.size() != 0) 
	{
		printf("\nSUCC:\t");
		cout << (*m_succ.begin())->getType() << endl;
		printf("\n");
	}

	printf("IN:\t");
	printVariablesNames(m_in);
	printf("\n");

	printf("OUT:\t");
	printVariablesNames(m_out);
	printf("\n");
	cout << "-----------------------------------------------------------------" << endl;

}
void Instruction::printVariablesNames(Variables variables)
{
	Variables::iterator iter;

	for (iter = variables.begin(); iter != variables.end(); iter++)
	{
		printf("%s", (*iter)->getName().c_str());
		printf(" ");
	}
	printf("\n");
}

//broj varijabla koje su memorijske varijable
int getNumberOfMemVariables(Variables* variables)
{
	int num = 0;
	Variables::iterator it;
	for (it = (*variables).begin(); it != (*variables).end(); it++) 
	{
		if ((*it)->getType() == Variable::MEM_VAR)
			num++;
	}
	return num;
}