/* Autor: Nevena Prokic Datum: 30.05.2021. */
#pragma once
#include "IR.h"

bool doResourceAllocation(SimplificationStack* simplificationStack, InterferenceGraph* ig, int num);
vector<Regs> makeListOfChooseRegs(InterferenceGraph* ig, Variable* var, int num);
