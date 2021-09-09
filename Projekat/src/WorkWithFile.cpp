/* Autor: Nevena Prokic Datum: 01.05.2021. */
#include "WorkWithFile.h"

//upis dela .text u fajl, upisuju se sve instrukcije i labele
void WriteFile::writeText() 
{
    Instructions::iterator i;
    m_outputFile << ".text" << endl; 
    for (i = m_instructions->begin(); i != m_instructions->end(); i++)
    {
        GlobalDatas::iterator g;
        for (g = m_data->begin(); g != m_data->end(); g++)
        {
            if (((*g)->getType() == GlobalData::LABEL)&&((*g)->getPosition() == (*i)->getPosition()))
                m_outputFile << (*g)->getName() << ":   " << endl;
        }
        string lineInFile = "";
        if ((*i)->getType() == InstructionType::I_ADD)
        {
            lineInFile = "\t add \t $t" + to_string((*(*i)->getDef().begin())->getAssignment()-1) + ", ";
            for (Variable* var : (*i)->getUse()) 
            {
                lineInFile += "$t" + to_string(var->getAssignment() - 1) + ", ";
            }
        }
        else if ((*i)->getType() == InstructionType::I_LA)
        {
            lineInFile = "\t la \t $t" + to_string((*(*i)->getDef().begin())->getAssignment() - 1) + ", ";
            lineInFile += (*(*i)->getUse().begin())->getName() + ", ";
        }
        else if ((*i)->getType() == InstructionType::I_LW)//DODAJ OFFSET
        {
            lineInFile = "\t lw \t $t" + to_string((*(*i)->getDef().begin())->getAssignment() - 1) + ", ";
            lineInFile += to_string((*i)->getOffset()) + "($t" + to_string((*(*i)->getUse().begin())->getAssignment() - 1) + ")" + ", ";
        }
        else if ((*i)->getType() == InstructionType::I_LI)
        {
            lineInFile = "\t li \t $t" + to_string((*(*i)->getDef().begin())->getAssignment() - 1) + ", ";
            lineInFile += to_string((*i)->getValue()) + ", ";
        }
        else if ((*i)->getType() == InstructionType::I_SUB)
        {
            lineInFile = "\t sub \t $t" + to_string((*(*i)->getDef().begin())->getAssignment() - 1) + ", ";
            for (Variable* var : (*i)->getUse())
            {
                lineInFile += "$t" + to_string(var->getAssignment() - 1) + ", ";
            }
        }
        else if ((*i)->getType() == InstructionType::I_ADDI)
        {
            lineInFile = "\t addi \t $t" + to_string((*(*i)->getDef().begin())->getAssignment() - 1) + ", ";
            lineInFile += "$t" + to_string((*(*i)->getUse().begin())->getAssignment() - 1) + ", ";
            lineInFile += to_string((*i)->getValue()) + ", ";
        }
        else if ((*i)->getType() == InstructionType::I_BLTZ)
        {
            lineInFile = "\t bltz \t $t" + to_string((*(*i)->getUse().begin())->getAssignment() - 1) + ", ";
            lineInFile += (*i)->getLabel()->getName() + ", ";
        }
        else if ((*i)->getType() == InstructionType::I_SW)
        {
            lineInFile = "\t sw \t $t" + to_string((*(*i)->getUse().begin())->getAssignment() - 1) + ", ";
            Variable* v;
            for (Variable* var : (*i)->getUse())
            {
                v = var;
            }
            lineInFile += to_string((*i)->getOffset()) + "($t" + to_string((v)->getAssignment() - 1) + ")" + ", ";
        }
        else if ((*i)->getType() == InstructionType::I_NOP)
        {
            lineInFile = "\t nop  ";
        }
        else if ((*i)->getType() == InstructionType::I_AND)
        {
            lineInFile = "\t and \t $t" + to_string((*(*i)->getDef().begin())->getAssignment() - 1) + ", ";
            for (Variable* var : (*i)->getUse())
            {
                lineInFile += "$t" + to_string(var->getAssignment() - 1) + ", ";
            }
        }
        else if ((*i)->getType() == InstructionType::I_SLT)
        {
            lineInFile = "\t slt \t $t" + to_string((*(*i)->getDef().begin())->getAssignment() - 1) + ", ";
            for (Variable* var : (*i)->getUse())
            {
                lineInFile += "$t" + to_string(var->getAssignment() - 1) + ", ";
            }
        }
        else if ((*i)->getType() == InstructionType::I_LUI)
        {
            lineInFile = "\t lui \t $t" + to_string((*(*i)->getDef().begin())->getAssignment() - 1) + ", ";
            lineInFile += to_string((*i)->getValue()) + ", ";
        }
        else if ((*i)->getType() == InstructionType::I_ORI)
        {
            lineInFile = "\t ori \t $t" + to_string((*(*i)->getDef().begin())->getAssignment() - 1) + ", ";
            lineInFile += "$t" + to_string((*(*i)->getUse().begin())->getAssignment() - 1) + ", ";
            lineInFile += to_string((*i)->getValue()) + ", ";
        }
        
        m_outputFile << lineInFile.substr(0, lineInFile.size()-2);
        m_outputFile << "\n";
    }
    m_outputFile << "\n";
}

//upis u fajl dela .data, sve varijable tipa MEM
void WriteFile::writeData() 
{
    Variables::iterator v;
    m_outputFile << ".data" << endl;
    for (v = m_variables->begin(); v != m_variables->end(); v++)
    {
        if((*v)->getType() == Variable::MEM_VAR)
            m_outputFile << (*v)->getName() << ": \t .word " << (*v)->getValue() << endl;
    }
    m_outputFile << "\n";
}

//upis dela .globl
void WriteFile::writeGlobal() 
{
    GlobalDatas::iterator g;
    for (g = m_data->begin(); g != m_data->end(); g++)
    {
        if((*g)->getType() == GlobalData::FUNCTION)
            m_outputFile << ".globl " << (*g)->getName() << endl;
    }
    m_outputFile << "\n";
}

void WriteFile::write() 
{

    m_outputFile.open(m_fileName);
    if (m_outputFile.is_open()) 
    {
        writeGlobal();
        writeData();
        writeText();
    }
   
    m_outputFile.close();
}

