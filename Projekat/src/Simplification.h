/* Autor: Nevena Prokic Datum: 30.05.2021. */
#pragma once
#include "IR.h"

bool notOnStack(string v, vector<string> s);
std::stack<Variable*>* doSimplification(InterferenceGraph& ig, Variables& variables, int num);
void reduceDegreeForVariables(Variables::iterator& var, Variables& variables);
void setDegreeForVariables(Variables& variables, InterferenceGraph& ig, int number);
