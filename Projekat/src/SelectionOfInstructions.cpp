/* Autor: Nevena Prokic Datum: 30.05.2021. */
#include "SelectionOfInstructions.h"

bool checkVariable(string v, Variables* variables) 
{
	Variables::iterator it;
	for (it = variables->begin(); it != variables->end(); it++)
	{
		if ((*it)->getName() == v)
		{
			throw runtime_error("\nException! Variable with that name already exists!\n");
		}
	}
	return true;
}
bool checkGlobal(string v, GlobalDatas* global) 
{
	GlobalDatas::iterator it;
	for (it = global->begin(); it != global->end(); it++)
	{
		if ((*it)->getName() == v)
		{
			throw runtime_error("\nException! Label or function with that name already exists!\n");
		}
	}
	return true;
}

Variables* makeVariables(LexicalAnalysis& lex)
{
	Variables* variables = new Variables;
	int number = 0;
	TokenList::iterator it;
	for (it = lex.getTokenList().begin(); it != lex.getTokenList().end(); it++) //prolazak kroz tokene i formiranje varijabli
	{
		if (((*it).getType() == T_REG) || ((*it).getType() == T_MEM))  
		{
			it++;
			if ((*it).getType() == T_R_ID) //varijabla je tipa REG
			{
				if (checkVariable((*it).getValue(), variables))
				{
					Variable* var = new Variable(Variable::REG_VAR, (*it).getValue(), number);
					variables->push_back(var);
					number++;
				}
				
			}
			else if ((*it).getType() == T_M_ID) //varijabla je tipa MEM
			{
				if (checkVariable((*it).getValue(), variables))
				{
					Variable* var = new Variable(Variable::MEM_VAR, (*it).getValue(), number);
					variables->push_back(var);
					number++;
					it++;
					var->setValue(stoi((*it).getValue()));
				}
			}
		}
	}
	return variables;
}
GlobalDatas* makeGlobalDatas(LexicalAnalysis& lex) 
{
	GlobalDatas* global = new GlobalDatas;
	TokenList::iterator it;
	for (it = lex.getTokenList().begin(); it != lex.getTokenList().end(); it++)//prolazak kroz tokene i formiranje labela i funkcija
	{
		if ((*it).getType() == T_COL) //formiranje labela
		{
			it--;
			if (checkGlobal((*it).getValue(), global)) 
			{
				GlobalData* data = new GlobalData(GlobalData::LABEL, (*it).getValue());
				global->push_back(data);
				
			}
			it++;
		}
		else if ((*it).getType() == T_FUNC)//formiranje funkcija
		{
			it++;
			if (checkGlobal((*it).getValue(), global))
			{
				GlobalData* data = new GlobalData(GlobalData::FUNCTION, (*it).getValue());
				global->push_back(data);
			}
		}
	}
	return global;
}
Instructions* makeInstruction(LexicalAnalysis& lex, GlobalDatas* global, Variables* variables)
{
	Instructions* instructions = new Instructions;
	int number = 0;

	TokenList::iterator it;
	for (it = lex.getTokenList().begin(); it != lex.getTokenList().end(); it++) //prolazak kroz tokene i formiranje instrukcija
	{
		if ((*it).getType() == T_COL)// dodavanje pozicije labeli
		{
			int a = number;
			GlobalDatas::iterator g;
			it--;
			for (g = global->begin(); g != global->end(); g++)
			{
				if ((*g)->getName() == (*it).getValue())
					(*g)->setPosition(a++);

			}
			it++;
		}
		else if ((*it).getType() == T_FUNC)//dodavanje pozicije funkciji
		{
			int a = number;
			it++;
			GlobalDatas::iterator g;
			for (g = global->begin(); g != global->end(); g++)
			{
				if ((*g)->getName() == (*it).getValue())
					(*g)->setPosition(a++);

			}
		}//formiranje instrukcija ADD,ADDI,SUB,AND,SLT,ORI
		else if (((*it).getType() == T_ADD) || ((*it).getType() == T_ADDI) || ((*it).getType() == T_SUB) || ((*it).getType() == T_AND) || ((*it).getType() == T_SLT) || ((*it).getType() == T_ORI)) 
		{
			TokenType s = (*it).getType();
			Variables dst, src, use;
			Instruction* inst;
			Variables::iterator it1 = variables->begin();
			//dst = def, src = use
			it++;
			for (; it1 != variables->end(); it1++)
			{
				if ((*it1)->getName() == (*it).getValue())
				{
					dst.push_back(*it1); //Popunjavanje dst skupa instrucije
				}
			}

			it++; it++;
			it1 = variables->begin();
			for (; it1 != variables->end(); it1++)
			{
				if ((*it1)->getName() == (*it).getValue())
				{
					src.push_back(*it1);//Popunjavanje src skupa instrukcije
				}
			}
			it++; it++;
			if ((*it).getType() == T_R_ID)
			{
				it1 = variables->begin();
				for (; it1 != variables->end(); it1++)
				{
					if ((*it1)->getName() == (*it).getValue())
					{
						src.push_back(*it1);//Popunjavanje src skupa instrukcije
					}
				}

				if (s == T_ADD)
					inst = new Instruction(number, I_ADD, dst, src);
				else if (s == T_SUB)
					inst = new Instruction(number, I_SUB, dst, src);
				else if (s == T_AND)
					inst = new Instruction(number, I_AND, dst, src);
				else if (s == T_SLT)
					inst = new Instruction(number, I_SLT, dst, src);
			}
			else
			{
				if (s == T_ADDI)
					inst = new Instruction(number, I_ADDI, dst, src);
				else if (s == T_ORI)
					inst = new Instruction(number, I_ORI, dst, src);
				inst->setValue(stoi((*it).getValue()));//Upisivanje konstante
			}

			instructions->push_back(inst);
			number++;

		}//formiranje instrukcije B
		else if ((*it).getType() == T_B) 
		{
			Variables dst, src;
			Instruction* inst = new Instruction(number, I_B, dst, src);
			it++;
			GlobalDatas::iterator g = global->begin();
			for (; g != global->end(); g++)
			{
				if ((*g)->getName() == (*it).getValue())
					inst->setLabel((*g)); //UPISIVANJE LABELE
			}
			instructions->push_back(inst);
			number++;

		}//formiranje instrukcije BLTZ
		else if ((*it).getType() == T_BLTZ) 
		{
			Variables dst, src;
			Instruction* inst;
			it++;
			Variables::iterator it1 = variables->begin();
			for (; it1 != variables->end(); it1++)
			{
				if ((*it1)->getName() == (*it).getValue())
				{
					src.push_back(*it1);//Popunjavanje src skupa instrukcije
				}
			}
			it++; it++;
			GlobalData* d;
			GlobalDatas::iterator g = global->begin();
			for (; g != global->end(); g++) {
				if ((*g)->getName() == (*it).getValue())
					d = (*g);
			}
			inst = new Instruction(number, I_BLTZ, dst, src);
			inst->setLabel(d);//upisivanje labele
			instructions->push_back(inst);
			number++;
		}//formiranje instrukcije LA
		else if ((*it).getType() == T_LA) 
		{
			Variables dst, src;
			Instruction* inst;
			Variables::iterator it1 = variables->begin();
			it++;
			for (; it1 != variables->end(); it1++) 
			{
				if ((*it1)->getName() == (*it).getValue()) 
				{
					dst.push_back(*it1);//Popunjavanje dst skupa instrukcije
				}
			}

			it++; it++;
			it1 = variables->begin();
			for (; it1 != variables->end(); it1++) 
			{
				if ((*it1)->getName() == (*it).getValue()) 
				{
					src.push_back(*it1);//Popunjavanje src skupa instrukcije
				}
			}
			inst = new Instruction(number, I_LA, dst, src);
			instructions->push_back(inst);
			number++;
		}//formiranje instrukcije LI I LUI
		else if (((*it).getType() == T_LI)|| ((*it).getType() == T_LUI))
		{
			TokenType s = (*it).getType();
			Variables dst, src;
			Instruction* inst;
			Variables::iterator it1 = variables->begin();
			it++;
			for (; it1 != variables->end(); it1++) 
			{
				if ((*it1)->getName() == (*it).getValue()) 
				{
					dst.push_back(*it1);//Popunjavanje dst skupa instrukcije
				}
			}
			if (s == T_LI)
				inst = new Instruction(number, I_LI, dst, src);
			else if (s == T_LUI)
				inst = new Instruction(number, I_LUI, dst, src);
			it++;it++;
			inst->setValue(stoi((*it).getValue()));//upisivanje konstante
			instructions->push_back(inst);
			number++;
		}//formiranje instrukcije LW
		else if ((*it).getType() == T_LW) 
		{
			Variables dst, src;
			Instruction* inst;
			Variables::iterator it1 = variables->begin();
			it++;
			for (; it1 != variables->end(); it1++) 
			{
				if ((*it1)->getName() == (*it).getValue()) 
				{
					dst.push_back(*it1);//Popunjavanje dst skupa instrukcije
				}
			}

			it++; it++;
			int offset = stoi((*it).getValue());
			it++; it++;
			it1 = variables->begin();
			for (; it1 != variables->end(); it1++) 
			{
				if ((*it1)->getName() == (*it).getValue()) 
				{
					src.push_back(*it1);//Popunjavanje src skupa instrukcije
				}
			}
			inst = new Instruction(number, I_LW, dst, src);
			inst->setOffset(offset);
			instructions->push_back(inst);
			number++;
		}//formiranje instrukcije NOP
		else if ((*it).getType() == T_NOP) 
		{
			Variables dst, src;
			Instruction* inst = new Instruction(number, I_NOP, dst, src);
			instructions->push_back(inst);
			number++;
			continue;
		}//formiranje instrukcije SW
		else if ((*it).getType() == T_SW) 
		{
			Variables dst, src;
			Instruction* inst;
			Variables::iterator it1 = variables->begin();
			it++;
			for (; it1 != variables->end(); it1++) 
			{
				if ((*it1)->getName() == (*it).getValue()) 
				{
					src.push_back(*it1); //Popunjavanje src skupa instrukcije
				}
			}

			it++; it++;
			int offset = stoi((*it).getValue());
			it++; it++;
			it1 = variables->begin();
			for (; it1 != variables->end(); it1++) 
			{
				if ((*it1)->getName() == (*it).getValue()) 
				{
					src.push_back(*it1);//Popunjavanje src skupa instrukcije
				}
			}
			inst = new Instruction(number, I_SW, dst, src);
			inst->setOffset(offset);
			instructions->push_back(inst);
			number++;
		}
	}
	return instructions;
}