/* Autor: Nevena Prokic Datum: 30.05.2021. */
#pragma once
#include "IR.h"

void printInterferenceMatrix();

bool check(Variable* var, Variables listOfVar);
void makeListOfVariable(Instructions& instructions);
void initialisationMatrix();
void makeMatrix(Instructions& instructions, int num);
void writeInterferenceOut(Instruction* ins, Variable* var, int num);
void writeInterferenceIn(Instruction* ins, Variable* var, int num);
InterferenceGraph& buildInterferenceGraph(Instructions& instructions, int num);