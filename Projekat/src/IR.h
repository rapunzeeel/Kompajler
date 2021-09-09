/* Autor: Nevena Prokic Datum: 30.05.2021. */
#ifndef __IR__
#define __IR__
#include<iostream>
#include "Types.h"

using namespace std;

class GlobalData {
public:
	enum GlobalDataType {
		LABEL,
		FUNCTION,
		NO_TYPE
	};
	GlobalData(): m_type(NO_TYPE), m_name(""), m_position(-1) {}
	GlobalData(GlobalDataType type, string name) : m_type(type), m_name(name) {}
	int getPosition() { return m_position; }
	string getName() { return m_name; }
	GlobalDataType getType() { return m_type; }

	void setPosition(int p) { m_position = p; }

private:
	int m_position;
	string m_name;
	GlobalDataType m_type;
};
typedef std::list<GlobalData*> GlobalDatas;



/**
 * This class represents one variable from program code.
 */

class Variable
{
public:
	enum VariableType
	{
		MEM_VAR,
		REG_VAR,
		NO_TYPE
	};

	Variable() : m_type(NO_TYPE), m_name(""), m_position(-1), m_assignment(no_assign) {}
	Variable(VariableType type, string name, int pos) : m_type(type), m_name(name), m_position(pos), m_assignment(no_assign), m_value(0) {}
	
	VariableType getType() { return m_type; }
	string getName() { return m_name; }
	int getPosition() { return m_position; }
	Regs getAssignment() { return m_assignment; }
	int getDegree() { return m_degree; }
	vector<string> getInterference() { return m_interference; }
	int getValue() { return m_value; }

	void setType(VariableType type) { m_type = type; }
	void setAssigment(Regs as) { m_assignment = as; }
	void setDegree(int d) { m_degree = d; }
	void setInterference(string n) { m_interference.push_back(n); }
	void setValue(int v) { m_value = v; }
	
	void reduceDegree() { m_degree--; }
	void printVariable();
	
private:
	VariableType m_type;
	std::string m_name;
	int m_position;
	Regs m_assignment;
	int m_degree;
	vector<string> m_interference;
	int m_value;
};


/**
 * This type represents list of variables from program code.
 */
typedef std::list<Variable*> Variables;



class Instruction
{
public:
	Instruction () : m_position(0), m_type(I_NO_TYPE) {}
	Instruction (int pos, InstructionType type, Variables& dst, Variables& src) :
		m_position(pos), m_type(type), m_dst(dst), m_src(src), m_use(src), m_def(dst), m_label(NULL), m_offset(-1) {}
	
	int getPosition() { return m_position; }
	InstructionType getType() { return m_type; }
	int getValue() { return m_value; }
	int getOffset() { return m_offset; }
	Variables getUse() { return m_use; }
	Variables getDef() { return m_def; }
	Variables getIn() { return m_in; }
	Variables getOut() { return m_out; }
	list<Instruction*> getSucc() { return m_succ; }
	list<Instruction*> getPred() { return m_pred; }
	GlobalData* getLabel() { return m_label; }

	void setValue(int v) { m_value = v; }
	void setOffset(int o) { m_offset = o; }
	void setSrc(Variable* v) { m_src.push_back(v); }
	void setUse(Variable* v) { m_use.push_back(v); }
	void setIn(Variable* var) { m_in.push_back(var); }
	void setOut(Variable* var) { m_out.push_back(var); }
	void setSucc(Instruction* instr) { m_succ.push_back(instr); }
	void setPred(Instruction* instr) { m_pred.push_back(instr); }
	void setLabel(GlobalData* g) { m_label = g; }
	
	void printVariablesNames(Variables variables);
	void printInstruction();
		
private:
	int m_position;
	InstructionType m_type;
	int m_value;
	int m_offset;
	
	Variables m_dst;
	Variables m_src;

	Variables m_use;
	Variables m_def;
	Variables m_in;
	Variables m_out;
	std::list<Instruction*> m_succ;
	std::list<Instruction*> m_pred;

	GlobalData* m_label;
};


/**
 * This type represents list of instructions from program code.
 */
typedef std::list<Instruction*> Instructions;

typedef std::vector<std::vector<int>> InterferenceMatrix;

typedef struct InterferenceGraphStruct
{
	Variables* variables;		// all variables from instructions
	InterferenceMatrix matrix;	// interference matrix

	/**
	 * Default construction.
	 */
	InterferenceGraphStruct()
	{
	}
} InterferenceGraph;

typedef std::stack<Variable*> SimplificationStack;

int getNumberOfMemVariables(Variables* variables);

#endif
