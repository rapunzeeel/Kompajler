/* Autor: Nevena Prokic Datum: 30.05.2021. */
#include "MakeFlowControl.h"

void makeFlowControl(Instructions* instructions)
{
	Instructions::iterator iter;
	for (iter = instructions->begin(); iter != instructions->end(); iter++)
	{
		if ((*iter)->getType() == I_B) { //ukoliko je instrukcija tipa B izvrsice se upis samo njenog sledbenika i prethodnika, instrukcija koja je u fajlu nakon B nece imati B za svog prethodnika
			fillSetsIfBranchInstructions(iter, instructions);
		}
		else {
			if ((*iter)->getType() == I_BLTZ)//ukoliko je BLTZ upisace se u instrukciju BLTZ jos jedan sledbenik a u njegovog sledbenika BLTZ ce biti upisan kao prethodnik
				fillSetsIfBranchInstructions(iter, instructions);

			Instructions::iterator it;
			for (it = instructions->begin(); it != instructions->end(); it++)//serijski upis funkcija u sledbenike i prethodnike
			{
				if (((*iter)->getPosition() + 1) == (*it)->getPosition())
				{
					(*iter)->setSucc((*it));
					(*it)->setPred((*iter));
				}
			}
			
		}
	}
}

void fillSetsIfBranchInstructions(Instructions::iterator& iter, Instructions* instructions)
{
	Instructions::iterator it;
	for (it = instructions->begin(); it != instructions->end(); it++)
	{
		if ((*iter)->getLabel()->getPosition() == (*it)->getPosition())
		{
			(*iter)->setSucc((*it));
			(*it)->setPred((*iter));
		}
	}
	
}

