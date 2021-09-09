/* Autor: Nevena Prokic Datum: 30.05.2021. */
#pragma once
#include "IR.h"

void makeFlowControl(Instructions* instructions);
void fillSetsIfBranchInstructions(Instructions::iterator& iter, Instructions* instructions);
