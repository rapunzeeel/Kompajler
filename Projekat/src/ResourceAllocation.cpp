/* Autor: Nevena Prokic Datum: 30.05.2021. */
#include "ResourceAllocation.h"

//provera da li registar nije u listi
bool notIn(Regs r, vector<Regs>listOfChooseRegs) 
{
	for (int j = 0; j < listOfChooseRegs.size(); j++)
	{
		if (r == listOfChooseRegs[j])
		{
			return true;
		}
	}
	return false;
}


bool doResourceAllocation(SimplificationStack* simplificationStack, InterferenceGraph* ig, int num)
{
	while (!simplificationStack->empty()) 
	{  //sve dok imamo elemente na steku
		Variable* var = simplificationStack->top(); //preuzimanje varijable sa vrha
		simplificationStack->pop();//skidanje varijable sa steka
		vector<Regs> listOfChooseRegs = makeListOfChooseRegs(ig, var, num);

		for (int i = t0; i != t3 + 1; i++) 
		{
			Regs r = static_cast<Regs>(i);
			if (listOfChooseRegs.size() == 0) //ako je lista prazna dodeli prvi registar
			{
				var->setAssigment(r);
				break;
			}
			if (!notIn(r, listOfChooseRegs))//ako tekuci registar nije u listi dodeljenih registara promeljivim u smetnji dodeli ga tekucoj promenljivoj
			{
				var->setAssigment(r);
				break;
			}
		}
		if (var->getAssignment() == __UNDEFINE__) 
		{
			return false;
		}
	}
	return true;
}

//lista registara koji su iskorisceni za promenljive sa kojima je data promenljiva u smetnji
vector<Regs> makeListOfChooseRegs(InterferenceGraph* ig, Variable* var, int num)
{
	vector<Regs> listOfChooseRegs;
	Variables::iterator it1 = (ig->variables)->begin();

	for (; it1 != (ig->variables)->end(); it1++)
	{
		if (ig->matrix[var->getPosition() - num][(*it1)->getPosition() - num] == __INTERFERENCE__)
		{
			if ((*it1)->getAssignment() != __UNDEFINE__)
			{
				listOfChooseRegs.push_back((*it1)->getAssignment());
			}
		}
	}
	return listOfChooseRegs;
}
