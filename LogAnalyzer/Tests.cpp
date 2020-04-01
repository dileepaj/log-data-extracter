#include "Tests.h"
#include "DefFileReader.h"
#include "ScriptReader.h"
#include "MetaData.h"
#include "ExecutionTemplateList.h"
#include "ExecutionContext.h"
#include "Node.h"
#include "MemMan.h"
#include "ELInterpretter.h"
#include <iostream>

using namespace std;

void Tests::RunTest1()
{
	DefFileReader dfr;
	MetaData* pMD = dfr.Read("../tests/test1/Defs.txt");
	ScriptReader sr;
	ScriptReaderOutput op;
	bool bSucc = sr.ProcessScript(pMD->s_RuleFileName, pMD, op);
	if(!bSucc)
	{
		std::wcout<<"\nFailed to read script\n";
	}
	ExecutionContext ec;
	ec.p_mapFunctions = &op.map_Functions;
	ec.p_MD = pMD;
	Node* pX = MemoryManager::Inst.CreateNode(1);
	Node* pY = MemoryManager::Inst.CreateNode(2);
	ec.map_Var["X"] = pX;
	ec.map_Var["Y"] = pY;
	op.p_ETL->Execute(&ec);
}

void Tests::RunTest2()
{
	DefFileReader dfr;
	MetaData* pMD = dfr.Read("../tests/test3/Defs.txt");
	ScriptReader sr;
	ScriptReaderOutput op;
	bool bSucc = sr.ProcessScript(pMD->s_RuleFileName, pMD, op);
	if(!bSucc)
	{
		std::wcout<<"\nFailed to read script\n";
	}
	ExecutionContext ec;
	ec.p_mapFunctions = &op.map_Functions;
	ec.p_MD = pMD;
	Node* pX = MemoryManager::Inst.CreateNode(1);
	Node* pY = MemoryManager::Inst.CreateNode(2);
	Node* pZ = MemoryManager::Inst.CreateNode(3);
	ec.map_Var["X"] = pX;
	ec.map_Var["Y"] = pY;
	ec.map_Var["Z"] = pZ;
	op.p_ETL->Execute(&ec);
	pX->DestroyWithSubTree();
	pY->DestroyWithSubTree();
	pZ->DestroyWithSubTree();
	op.p_ETL->Destroy();
}

void Tests::RunTest3()
{
	DefFileReader dfr;
	MetaData* pMD = dfr.Read("../tests/test3/Defs.txt");
	ScriptReader sr;
	ScriptReaderOutput op;
	bool bSucc = sr.ProcessScript(pMD->s_RuleFileName, pMD, op);
	if(!bSucc)
	{
		std::wcout<<"\nFailed to read script\n";
	}
	ExecutionContext ec;
	ec.p_mapFunctions = &op.map_Functions;
	ec.p_MD = pMD;
	Node* pX = MemoryManager::Inst.CreateNode(1);
	Node* pY = MemoryManager::Inst.CreateNode(2);
	Node* pZ = MemoryManager::Inst.CreateNode(3);
	ec.map_Var["X"] = pX;
	ec.map_Var["Y"] = pY;
	ec.map_Var["Z"] = pZ;
	op.p_ETL->Execute(&ec);
	pX->DestroyWithSubTree();
	pY->DestroyWithSubTree();
	pZ->DestroyWithSubTree();
//	op.p_ETL->Destroy();
}

void Tests::RunLDELtest1() {
    ELInterpretter intp;
    intp.EvaluateCase("../../Files/Defs.txt");
}

void Tests::RunLDELtest2() {
    ELInterpretter intp;
    intp.EvaluateCase("../tests/LDEL_test2/Defs.txt");
}

