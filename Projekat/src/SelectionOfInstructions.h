#pragma once
/* Autor: Nevena Prokic Datum: 30.05.2021. */
#include "IR.h"
#include "LexicalAnalysis.h"


Variables* makeVariables(LexicalAnalysis& lex);
GlobalDatas* makeGlobalDatas(LexicalAnalysis& lex);
Instructions* makeInstruction(LexicalAnalysis& lex, GlobalDatas* global, Variables* variables);