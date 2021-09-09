/* Autor: Nevena Prokic Datum: 30.05.2021. */
#pragma once
#include "IR.h"


void printInstructions(Instructions instrs);
bool variableExists(Variable* v, Variables variables);
void livenessAnalysis(Instructions& instructions);
