#include "ExecutionTemplate.h"
#include "Command.h"
#include "MemMan.h"
#include "ExecutionContext.h"
#include "Utils.h"
#include "EntityList.h"

ExecutionTemplate::ExecutionTemplate()
:s_StartVarName(EMPTY_STRING), p_Entity(0), ul_SpecialCommand(0), s_CodeLine(EMPTY_STRING)
{
	ul_Type = ENTITY_TYPE_EXECUTION_TEMPLATE;
}

ExecutionTemplate::~ExecutionTemplate()
{

}

void ExecutionTemplate::Destroy()
{
	if(0 != p_Entity)
	{
		p_Entity->Destroy();
	}
	DestroyCollection(lst_Commands);

	MemoryManager::Inst.DeleteObject(this);
}

ExecutionTemplate* ExecutionTemplate::GetCopy()
{
	ExecutionTemplate* pCopy = 0;
	MemoryManager::Inst.CreateObject(&pCopy);
	if((0 != p_Entity) && (ENTITY_TYPE_NODE != p_Entity->ul_Type))
	{
		pCopy->SetEntity(p_Entity->GetCopy());
	}
	else
	{
		pCopy->SetEntity(p_Entity);
	}	
	pCopy->SetStartVarName(s_StartVarName);
	LST_COMMANDPTR::iterator ite1 = lst_Commands.begin();
	LST_COMMANDPTR::iterator iteEnd1 = lst_Commands.end();
	for( ; ite1 != iteEnd1; ++ite1)
	{
		pCopy->AddCommand((*ite1)->GetCopy());
	}
	return pCopy;
}

void ExecutionTemplate::SetStartVarName(MSTRING sName)
{
	s_StartVarName = sName;
}

void ExecutionTemplate::AddCommand(Command* pCommand)
{
	lst_Commands.push_back(pCommand);
}

void ExecutionTemplate::SetEntity(PENTITY pEntity)
{
	p_Entity = pEntity;
}

void ExecutionTemplate::SetSpecialCommand(MULONG ulCmd)
{
	ul_SpecialCommand = ulCmd;
}

void ExecutionTemplate::SetCodeLine(MSTRING sLine)
{
	s_CodeLine = sLine;
}

MSTRING ExecutionTemplate::GetStartVarName()
{
	return s_StartVarName;
}

PENTITY ExecutionTemplate::GetEntity()
{
	return p_Entity;
}

MULONG ExecutionTemplate::GetSpecialCommand()
{
	return ul_SpecialCommand;
}

MSTRING ExecutionTemplate::GetCodeLine()
{
	return s_CodeLine;
}

PENTITY ExecutionTemplate::Execute(ExecutionContext *pContext)
{
    //std::cout<<s_CodeLine<<std::endl;
    if(!s_StartVarName.empty())
	{
		// Extract the start variable from the context
		PENTITY pVar = 0;
		MAP_STR_ENTITYPTR::const_iterator iteFind = pContext->map_Var.find(s_StartVarName);
		if(pContext->map_Var.end() == iteFind)
		{
			return 0;
		}
		else
		{
			pVar = (*iteFind).second;
		}

		if(lst_Commands.size() == 0)
		{
			PENTITY pRes = pVar;
			if(pRes->ul_Type != ENTITY_TYPE_NODE)
			{
				pRes = pRes->GetCopy();
			}
			return pRes;
		}

		LST_COMMANDPTR::const_iterator ite1 = lst_Commands.begin();
		LST_COMMANDPTR::const_iterator iteEnd1 = lst_Commands.end();
		PENTITY pCurr = (*ite1)->Execute(pVar, pContext);
		++ite1;
		while(ite1 != iteEnd1)
		{
			PENTITY pNew = (*ite1)->Execute(pCurr, pContext);
			pCurr->Destroy();
			pCurr = pNew;
			++ite1;
		}
		return pCurr;
	}
	else if(0 != p_Entity)
	{
		if(ENTITY_TYPE_LIST == p_Entity->ul_Type)
		{
			EntityList* pEL = (EntityList*)p_Entity;
			if(0 != pEL)
			{
				pEL->ExecuteElements(pContext);
			}
		}
		if(lst_Commands.size() == 0)
		{
			PENTITY pRes = p_Entity;
			if(p_Entity->ul_Type != ENTITY_TYPE_NODE)
			{
				pRes = p_Entity->GetCopy();
			}
			return pRes;
		}
		LST_COMMANDPTR::const_iterator ite1 = lst_Commands.begin();
		LST_COMMANDPTR::const_iterator iteEnd1 = lst_Commands.end();
		PENTITY pCurr = (*ite1)->Execute(p_Entity, pContext);
		++ite1;
		while(ite1 != iteEnd1)
		{
			PENTITY pNew = (*ite1)->Execute(pCurr, pContext);
			pCurr->Destroy();
			pCurr = pNew;
			++ite1;
		}
		return pCurr;
	}

	return 0;
}

bool ExecutionTemplate::IsEmpty()
{
	return ((s_StartVarName.empty()) && (0 == p_Entity));
}