#include "Command.h"
#include "Entity.h"
#include "ExecutionContext.h"
#include "ExecutionTemplate.h"
#include "Node.h"
#include "Value.h"
#include "Int.h"
#include "EntityList.h"
#include "MemMan.h"
#include "Null.h"
#include "Utils.h"
#include "AdditionalFunctions.h"
#include "Bool.h"
#include "MetaData.h"

Command::Command()
:ul_CommandType(COMMAND_TYPE_INVALID), p_Arg(0), p_EntityArg(0), s_AdditionalFuncName(EMPTY_STRING)
{

}

Command::~Command()
{

}

void Command::Destroy()
{
	if(0 != p_Arg)
	{
		p_Arg->Destroy();
	}
	if(0 != p_EntityArg)
	{
		p_EntityArg->Destroy();
	}
}

Command* Command::GetCopy()
{
	Command* pCopy = 0;
	MemoryManager::Inst.CreateObject(&pCopy);
	pCopy->SetType(ul_CommandType);
	if(0 != p_Arg)
	{
		pCopy->SetArg(p_Arg->GetCopy());	
	}
	if(0 != p_EntityArg)
	{
		if(ENTITY_TYPE_NODE != p_EntityArg->ul_Type)
		{
			pCopy->SetEntityArg(p_EntityArg->GetCopy());
		}
		else
		{
			pCopy->SetEntityArg(p_EntityArg);
		}
	}
	pCopy->SetAdditionalFuncName(s_AdditionalFuncName);
	return pCopy;
}

void Command::SetType(MULONG ulType)
{
	ul_CommandType = ulType;
}

void Command::SetArg(ExecutionTemplate* pArg)
{
	p_Arg = pArg;
}

void Command::SetEntityArg(PENTITY pArg)
{
	p_EntityArg = pArg;
}

void Command::SetAdditionalFuncName(MSTRING sFun)
{
	s_AdditionalFuncName = sFun;
}


PENTITY Command::Execute(PENTITY pEntity, ExecutionContext* pContext)
{
	if(COMMAND_TYPE_ADDITIONAL_FUNCTION == ul_CommandType)
	{
		// Additional function can be defined either in the control code or inside the script
		// First check whether it is a function defined in the script
		MAP_STR_EXECTMPLIST::const_iterator iteFind2 = pContext->p_mapFunctions->find(s_AdditionalFuncName);
		if(pContext->p_mapFunctions->end() != iteFind2)
		{
			ExecutionContext ec;
			ec.p_mapFunctions = pContext->p_mapFunctions;
			ec.p_MD = pContext->p_MD;
			ec.map_Var[pContext->p_MD->s_FuncArg] = pEntity;
			((*iteFind2).second)->Execute(&ec);
			MAP_STR_ENTITYPTR::iterator iteFind3 = ec.map_Var.find(pContext->p_MD->s_FuncRet);
			if(ec.map_Var.end() == iteFind3)
			{
				return 0;
			}
			return (*iteFind3).second;
		}
		else
		{
			// Now try functions defined in control code
			AdditionalFunc fun = 0;
			MAP_STR_ADDITIONAL_FUNC::const_iterator iteFind = map_AdditionalFunctions.find(s_AdditionalFuncName);
			if(map_AdditionalFunctions.end() == iteFind)
			{
				return 0;
			}
			fun = (*iteFind).second;
			if(0 != p_Arg)
			{
				p_EntityArg = p_Arg->Execute(pContext);
			}
			return fun(p_EntityArg);
		}		
	}
	else if(COMMAND_TYPE_STORE_AS_VARIABLE == ul_CommandType)
	{
		// This will change the execution context
		// Get a copy of the entity and add it as a new variable to the context
		// Variable name will be in the p_EntityArg
		PString pVarName = (PString)p_EntityArg;
		if(0 == pVarName)
		{
			return 0;
		}
		PENTITY pVar = pEntity;
		if(ENTITY_TYPE_NODE != pEntity->ul_Type)
		{
			pVar = pEntity->GetCopy();
		}
		MAP_STR_ENTITYPTR::iterator iteFind = pContext->map_Var.find(pVarName->GetValue());
		if(pContext->map_Var.end() != iteFind)
		{
			((*iteFind).second)->Destroy();
		}
		pContext->map_Var[pVarName->GetValue()] = pVar;
		// Create a Null entity and return
		PNull pRet = 0;
		MemoryManager::Inst.CreateObject(&pRet);
		return pRet;
	}
	else
	{
		if(0 != p_Arg)
		{
			p_EntityArg = p_Arg->Execute(pContext);
		}
		return ExecuteEntityCommand(ul_CommandType, pEntity, p_EntityArg);
	}
	return 0;
}

PENTITY Command::ExecuteEntityCommand(MULONG ulCommand, PENTITY pEntity, PENTITY pArg)
{
	// General functions in Entity level
	if(COMMAND_TYPE_IS_NULL == ulCommand)
	{
		PBool pBool = 0;
		MemoryManager::Inst.CreateObject(&pBool);
		pBool->SetValue(pEntity->IsNull());
		return pBool;
	}

	if(COMMAND_TYPE_IS_NOT_NULL == ulCommand)
	{
		PBool pBool = 0;
		MemoryManager::Inst.CreateObject(&pBool);
		pBool->SetValue(!pEntity->IsNull());
		return pBool;
	}

	switch (pEntity->ul_Type)
	{
	case ENTITY_TYPE_INT:
		{
			return ExecuteIntCommand(ulCommand, pEntity, pArg);
		}
	case ENTITY_TYPE_STRING:
		{
			return ExecuteStringCommand(ulCommand, pEntity, pArg);
		}
	case ENTITY_TYPE_NODE:
		{
			return ExecuteNodeCommand(ulCommand, pEntity, pArg);
		}
	case ENTITY_TYPE_BOOL:
		{
			return ExecuteBoolCommand(ulCommand, pEntity, pArg);
		}
	case ENTITY_TYPE_LIST:
		{
			return ExecuteListCommand(ulCommand, pEntity, pArg);
		}
	}
	return 0;
}

PENTITY Command::ExecuteIntCommand(MULONG ulCommand, PENTITY pEntity, PENTITY pArg)
{
	PInt pInt = (PInt)pEntity;
	if(0 == pInt)
	{
		return 0;
	}

	PBool pBoolRes = 0;
	PNull pNullRes = 0;
	PString pStrRes = 0;

	switch(ulCommand)
	{
	case COMMAND_TYPE_IS_INT_EQUAL_TO:
		{
			if(ENTITY_TYPE_INT == pArg->ul_Type)
			{
				MemoryManager::Inst.CreateObject(&pBoolRes);
				PInt pIntArg = (PInt)pArg;
				pBoolRes->SetValue(pInt->GetValue() == pIntArg->GetValue());
				break;
			}
			break;
		}
	case COMMAND_TYPE_IS_INT_MEMBER_OF:
		{
			if(ENTITY_TYPE_LIST == pArg->ul_Type)
			{
				MemoryManager::Inst.CreateObject(&pBoolRes);
				pBoolRes->SetValue(false);
				MULONG ulVal = pInt->GetValue();
				PENTITYLIST pIntListArg = (PENTITYLIST)pArg;
				EntityList::const_iterator ite1 = pIntListArg->begin();
				EntityList::const_iterator iteEnd1 = pIntListArg->end();
				for( ; ite1 != iteEnd1; ++ite1)
				{
					if(((PInt)(*ite1))->GetValue() == pInt->GetValue())
					{
						pBoolRes->SetValue(true);
						break;
					}
				}
			}
			break;
		}
	case COMMAND_TYPE_IS_LESS_THAN:
		{
			if(ENTITY_TYPE_INT == pArg->ul_Type)
			{
				MemoryManager::Inst.CreateObject(&pBoolRes);
				PInt pIntArg = (PInt)pArg;
				pBoolRes->SetValue(pInt->GetValue() < pIntArg->GetValue());
			}
			break;
		}
	case COMMAND_TYPE_IS_LESS_THAN_OR_EQUAL_TO:
		{
			if(ENTITY_TYPE_INT == pArg->ul_Type)
			{
				MemoryManager::Inst.CreateObject(&pBoolRes);
				PInt pIntArg = (PInt)pArg;
				pBoolRes->SetValue(pInt->GetValue() <= pIntArg->GetValue());
			}
			break;
		}
	case COMMAND_TYPE_IS_GREATER_THAN:
		{
			if(ENTITY_TYPE_INT == pArg->ul_Type)
			{
				MemoryManager::Inst.CreateObject(&pBoolRes);
				PInt pIntArg = (PInt)pArg;
				pBoolRes->SetValue(pInt->GetValue() > pIntArg->GetValue());
			}
			break;
		}
	case COMMAND_TYPE_IS_GREATER_THAN_OR_EQUAL_TO:
		{
			if(ENTITY_TYPE_INT == pArg->ul_Type)
			{
				MemoryManager::Inst.CreateObject(&pBoolRes);
				PInt pIntArg = (PInt)pArg;
				pBoolRes->SetValue(pInt->GetValue() >= pIntArg->GetValue());
			}
			break;
		}
	case COMMAND_TYPE_ADD:
		{
			if(ENTITY_TYPE_INT == pArg->ul_Type)
			{
				MemoryManager::Inst.CreateObject(&pNullRes);
				PInt pIntArg = (PInt)pArg;
				MULONG ulVal = pInt->GetValue();
				ulVal += (pIntArg->GetValue());
				pInt->SetValue(ulVal);
			}
			break;
		}
	case COMMAND_TYPE_SUBTRACT:
		{
			if(ENTITY_TYPE_INT == pArg->ul_Type)
			{
				MemoryManager::Inst.CreateObject(&pNullRes);
				PInt pIntArg = (PInt)pArg;
				MULONG ulVal = pInt->GetValue();
				ulVal -= (pIntArg->GetValue());
				pInt->SetValue(ulVal);
			}
			break;
		}
	case COMMAND_TYPE_TOSTRING:
		{
			MSTRINGSTREAM ss;
			ss<<pInt->GetValue();
			MemoryManager::Inst.CreateObject(&pStrRes);
			pStrRes->SetValue(ss.str());
			break;
		}
	}

	if(0 != pBoolRes)
	{
		return pBoolRes;
	}
	if(0 != pNullRes)
	{
		return pNullRes;
	}
	if(0 != pStrRes)
	{
		return pStrRes;
	}
	return 0;
}

PENTITY Command::ExecuteBoolCommand(MULONG ulCommand, PENTITY pEntity, PENTITY pArg)
{
	PBool pBool = (PBool)pEntity;
	if(0 == pBool)
	{
		return 0;
	}

	PBool pBoolRes = 0;

	switch(ulCommand)
	{
	case COMMAND_TYPE_BOOL_AND:
		{
			PBool pBoolArg = (PBool)pArg;
			if(0 != pBoolArg)
			{
				MemoryManager::Inst.CreateObject(&pBoolRes);
				pBoolRes->SetValue(pBool->And(pBoolArg)->GetValue());
			}
			break;
		}
	case COMMAND_TYPE_BOOL_OR:
		{
			PBool pBoolArg = (PBool)pArg;
			if(0 != pBoolArg)
			{
				MemoryManager::Inst.CreateObject(&pBoolRes);
				pBoolRes->SetValue(pBool->Or(pBoolArg)->GetValue());
			}
			break;
		}
	}

	if(0 != pBoolRes)
	{
		return pBoolRes;
	}

	return 0;
}

PENTITY Command::ExecuteStringCommand(MULONG ulCommand, PENTITY pEntity, PENTITY pArg)
{
	PString pString = (PString)pEntity;
	if(0 == pString)
	{
		return 0;
	}

	PInt pIntRes = 0;
	PNull pNullRes = 0;
	PBool pBoolRes = 0;

	switch(ulCommand)
	{
	case COMMAND_TYPE_IS_STRING_EQUAL_TO:
		{
			if(ENTITY_TYPE_STRING == pArg->ul_Type)
			{
				MemoryManager::Inst.CreateObject(&pBoolRes);
				PString pStrArg = (PString)pArg;
				pBoolRes->SetValue(pString->GetValue() == pStrArg->GetValue());
			}
			break;
		}
	case COMMAND_TYPE_IS_STRING_MEMBER_OF:
		{
			if(ENTITY_TYPE_LIST == pArg->ul_Type)
			{
				MemoryManager::Inst.CreateObject(&pBoolRes);
				PENTITYLIST pStrListArg = (PENTITYLIST)pArg;
				MSTRING sVal = pString->GetValue();
				pBoolRes->SetValue(false);
				EntityList::const_iterator ite1 = pStrListArg->begin();
				EntityList::const_iterator iteEnd1 = pStrListArg->end();
				for( ; ite1 != iteEnd1; ++ite1)
				{
					if(((PString)(*ite1))->GetValue() == sVal)
					{
						pBoolRes->SetValue(true);
						break;
					}
				}
			}
			break;
		}
	case COMMAND_TYPE_IS_HAVING_SUBSTRING:
		{
			if(ENTITY_TYPE_STRING == pArg->ul_Type)
			{
				MemoryManager::Inst.CreateObject(&pBoolRes);
				PString pStrArg = (PString)pArg;
				pBoolRes->SetValue(pString->GetValue().find(pStrArg->GetValue()) != MSTRING::npos);
			}
			break;
		}
	case COMMAND_TYPE_IS_HAVING_LEFT_SUBSTRING:
		{
			if(ENTITY_TYPE_STRING == pArg->ul_Type)
			{
				MemoryManager::Inst.CreateObject(&pBoolRes);
				PString pStrArg = (PString)pArg;
				MSTRING sArg = pStrArg->GetValue();
				pBoolRes->SetValue(pString->GetValue().substr(0, sArg.length()) == sArg);
			}
			break;
		}
	case COMMAND_TYPE_IS_HAVING_RIGHT_SUBSTRING:
		{
			if(ENTITY_TYPE_STRING == pArg->ul_Type)
			{
				MemoryManager::Inst.CreateObject(&pBoolRes);
				PString pStrArg = (PString)pArg;
				MSTRING sArg = pStrArg->GetValue();
				pBoolRes->SetValue(pString->GetValue().substr(pString->GetValue().length() - sArg.length(), sArg.length()) == sArg);
			}
			break;
		}
	case COMMAND_TYPE_ADD_PREFIX:
		{
			MemoryManager::Inst.CreateObject(&pNullRes);
			if(ENTITY_TYPE_STRING == pArg->ul_Type)
			{
				PString pStrArg = (PString)pArg;
				MSTRING sVal = pString->GetValue();
				sVal = pStrArg->GetValue() + sVal;
				pString->SetValue(sVal);
			}
			break;
		}
	case COMMAND_TYPE_ADD_POSTFIX:
		{
			MemoryManager::Inst.CreateObject(&pNullRes);
			if(ENTITY_TYPE_STRING == pArg->ul_Type)
			{
				PString pStrArg = (PString)pArg;
				MSTRING sVal = pString->GetValue();
				sVal += pStrArg->GetValue();
				pString->SetValue(sVal);
			}
			break;
		}
	case COMMAND_TYPE_TRIM_LEFT:
		{
			MemoryManager::Inst.CreateObject(&pNullRes);
			MSTRING sVal = pString->GetValue();
			Utils::TrimLeft(sVal, _MSTR( \t\n));
			pString->SetValue(sVal);
			break;
		}
	case COMMAND_TYPE_TRIM_RIGHT:
		{
			MemoryManager::Inst.CreateObject(&pNullRes);
			MSTRING sVal = pString->GetValue();
			Utils::TrimRight(sVal, _MSTR( \t\n));
			pString->SetValue(sVal);
			break;
		}
	case COMMAND_TYPE_WRITE_TO_FILE:
		{
			MemoryManager::Inst.CreateObject(&pNullRes);
			PString pStrArg = (PString)pArg;
			if(0 != pStrArg)
			{
				MOFSTREAM file;
				file.open(pStrArg->GetValue().c_str(), std::ios::out | std::ios::trunc);
				file<<(pString->GetValue().c_str());
				file.close();				
			}
			break;
		}
	case COMMAND_TYPE_GET_LENGTH:
		{
			MemoryManager::Inst.CreateObject(&pIntRes);
			pIntRes->SetValue(pString->GetValue().length());
			break;
		}		
	}

	if(0 != pIntRes)
	{
		return pIntRes;
	}
	if(0 != pNullRes)
	{
		return pNullRes;
	}
	if(0 != pBoolRes)
	{
		return pBoolRes;
	}
	return 0;
}

PENTITY Command::ExecuteNodeCommand(MULONG ulCommand, PENTITY pEntity, PENTITY pArg)
{
	PNODE pNode = (PNODE)pEntity;
	if(0 == pNode)
	{
		return 0;
	}

	PNODE pNodeRes = 0;
	PInt pIntRes = 0;
	PString pStrRes = 0;
	PENTITYLIST pNodeListRes = 0;
	PNull pNullRes = 0;
	PBool pBoolRes = 0;

	switch(ulCommand)
	{
	case COMMAND_TYPE_LEFT_SIBLING:
		{
			pNodeRes = pNode->GetLeftSibling();
			break;
		}
	case COMMAND_TYPE_RIGHT_SIBLING:
		{
			pNodeRes = pNode->GetRightSibling();
			break;
		}
	case COMMAND_TYPE_PARENT:
		{
			pNodeRes = pNode->GetParent();
			break;
		}
	case COMMAND_TYPE_FIRST_CHILD:
		{
			pNodeRes = pNode->GetFirstChild();
			break;
		}
	case COMMAND_TYPE_CHILDREN:
		{
			MemoryManager::Inst.CreateObject(&pNodeListRes);
			PNODE pChild = pNode->GetFirstChild();
			while(0 != pChild)
			{
				pNodeListRes->push_back((PENTITY)pChild);
				pChild = pChild->GetRightSibling();
			}
			break;
		}
	case COMMAND_TYPE_CHILD_COUNT:
		{
			MemoryManager::Inst.CreateObject(&pIntRes);
			pIntRes->SetValue(pNode->GetChildCount());
			break;
		}
	case COMMAND_TYPE_GET_VALUE:
		{
			MemoryManager::Inst.CreateObject(&pStrRes);
			pStrRes->SetValue(MSTRING(pNode->GetValue()));
			break;
		}
	case COMMAND_TYPE_GET_LVALUE:
		{
			MemoryManager::Inst.CreateObject(&pStrRes);
			pStrRes->SetValue(MSTRING(pNode->GetLVal()));
			break;
		}
	case COMMAND_TYPE_GET_RVALUE:
		{
			MemoryManager::Inst.CreateObject(&pStrRes);
			pStrRes->SetValue(MSTRING(pNode->GetRVal()));
			break;
		}
	case COMMAND_TYPE_GET_CUSTOM_STRING:
		{
			MemoryManager::Inst.CreateObject(&pStrRes);
			pStrRes->SetValue(MSTRING(pNode->GetCustomString()));
			break;
		}
	case COMMAND_TYPE_GET_ID:
		{
			MemoryManager::Inst.CreateObject(&pIntRes);
			pIntRes->SetValue(pNode->GetID());
			break;
		}
	case COMMAND_TYPE_GET_TYPE:
		{
			MemoryManager::Inst.CreateObject(&pIntRes);
			pIntRes->SetValue(pNode->GetType());
			break;
		}
	case COMMAND_TYPE_GET_NATURE:
		{
			MemoryManager::Inst.CreateObject(&pIntRes);
			pIntRes->SetValue(pNode->GetNature());
			break;
		}
	case COMMAND_TYPE_GET_WEIGHT:
		{
			MemoryManager::Inst.CreateObject(&pIntRes);
			pIntRes->SetValue(pNode->GetWeight());
			break;
		}
	case COMMAND_TYPE_GET_MIN_CHILD_WEIGHT:
		{
			MemoryManager::Inst.CreateObject(&pIntRes);
			pIntRes->SetValue(pNode->GetMinimumChildWeight());
			break;
		}
	case COMMAND_TYPE_GET_MAX_CHILD_WEIGHT:
		{
			MemoryManager::Inst.CreateObject(&pIntRes);
			pIntRes->SetValue(pNode->GetMaximumChildWeight());
			break;
		}
	case COMMAND_TYPE_SET_VALUE:
		{
			MemoryManager::Inst.CreateObject(&pNullRes);
			if(ENTITY_TYPE_STRING == pArg->ul_Type)
			{
				String* pStrArg = (String*)pArg;
				if(0 != pStrArg)
				{
					pNode->SetValue((PMCHAR)pStrArg->GetValue().c_str());
				}
			}
			break;
		}
	case COMMAND_TYPE_SET_LVALUE:
		{
			MemoryManager::Inst.CreateObject(&pNullRes);
			if(ENTITY_TYPE_STRING == pArg->ul_Type)
			{
				String* pStrArg = (String*)pArg;
				if(0 != pStrArg)
				{
					pNode->SetLValue((PMCHAR)pStrArg->GetValue().c_str());
				}
			}
			break;
		}
	case COMMAND_TYPE_SET_RVALUE:
		{
			MemoryManager::Inst.CreateObject(&pNullRes);
			if(ENTITY_TYPE_STRING == pArg->ul_Type)
			{
				String* pStrArg = (String*)pArg;
				if(0 != pStrArg)
				{
					pNode->SetRValue((PMCHAR)pStrArg->GetValue().c_str());
				}
			}
			break;
		}
	case COMMAND_TYPE_SET_TYPE:
		{
			MemoryManager::Inst.CreateObject(&pNullRes);
			if(ENTITY_TYPE_INT == pArg->ul_Type)
			{
				Int* pIntArg = (Int*)pArg;
				if(0 != pIntArg)
				{
					pNode->SetType((MUSHORT)pIntArg->GetValue());
				}
			}
			break;
		}
	case COMMAND_TYPE_SET_NATURE:
		{
			MemoryManager::Inst.CreateObject(&pNullRes);
			if(ENTITY_TYPE_INT == pArg->ul_Type)
			{
				Int* pIntArg = (Int*)pArg;
				if(0 != pIntArg)
				{
					pNode->SetNature((MBYTE)pIntArg->GetValue());
				}
			}
			break;
		}
	case COMMAND_TYPE_SET_CUSTOM_STRING:
		{
			MemoryManager::Inst.CreateObject(&pNullRes);
			if(ENTITY_TYPE_STRING == pArg->ul_Type)
			{
				String* pStrArg = (String*)pArg;
				if(0 != pStrArg)
				{
					pNode->SetCustomString((PMCHAR)pStrArg->GetValue().c_str());
				}
			}
			break;
		}
	case COMMAND_TYPE_SET_MIN_CHILD_WEIGHT:
		{
			MemoryManager::Inst.CreateObject(&pNullRes);
			if(ENTITY_TYPE_INT == pArg->ul_Type)
			{
				Int* pIntArg = (Int*)pArg;
				if(0 != pIntArg)
				{
					pNode->SetMinimumChildWeight(pIntArg->GetValue());
				}
			}
			break;
		}
	case COMMAND_TYPE_SET_MAX_CHILD_WEIGHT:
		{
			MemoryManager::Inst.CreateObject(&pNullRes);
			if(ENTITY_TYPE_INT == pArg->ul_Type)
			{
				Int* pIntArg = (Int*)pArg;
				if(0 != pIntArg)
				{
					pNode->SetMaximumChildWeight(pIntArg->GetValue());
				}
			}
			break;
		}
	case COMMAND_TYPE_SET_WEIGHT:
		{
			MemoryManager::Inst.CreateObject(&pNullRes);
			if(ENTITY_TYPE_INT == pArg->ul_Type)
			{
				Int* pIntArg = (Int*)pArg;
				if(0 != pIntArg)
				{
					pNode->SetWeight(pIntArg->GetValue());
				}
			}
			break;
		}
	case COMMAND_TYPE_EXPAND:
		{
			MemoryManager::Inst.CreateObject(&pNullRes);
			if(ENTITY_TYPE_LIST == pArg->ul_Type)
			{
				PENTITYLIST pStrListArg = (PENTITYLIST)pArg;
				if(0 != pStrListArg)
				{
					LST_STR lstTokens;
					EntityList::const_iterator ite1 = pStrListArg->begin();
					EntityList::const_iterator iteEnd1 = pStrListArg->end();
					for( ; ite1 != iteEnd1; ++ite1)
					{
						lstTokens.push_back(((PString)(*ite1))->GetValue());
					}
					pNode->Expand(lstTokens);
				}
			}
			break;
		}
	case COMMAND_TYPE_ADD_NODE:
		{
			MemoryManager::Inst.CreateObject(&pNullRes);
			if(0 == pArg)
			{
				pNodeRes = pNode->AddNode();
			}
			else if(ENTITY_TYPE_NODE == pArg->ul_Type)
			{
				pNodeRes = pNode->AddNode((PNODE)pArg, true);
			}
			break;
		}
	case COMMAND_TYPE_ADD_NODE_WITH_WEIGHT:
		{
			MemoryManager::Inst.CreateObject(&pNullRes);
			if(ENTITY_TYPE_INT == pArg->ul_Type)
			{
				Int* pIntArg = (Int*)pArg;
				if(0 != pIntArg)
				{
					pNodeRes = pNode->AddNodeWithWeight(pIntArg->GetValue());
				}
			}
			break;
		}
	case COMMAND_TYPE_READ_FROM_FILE:
		{
			MemoryManager::Inst.CreateObject(&pNullRes);
			if(ENTITY_TYPE_STRING == pArg->ul_Type)
			{
				PString pStrArg = (PString)pArg;
				if(0 != pStrArg)
				{
					pNode->ReadValueFromFile(pStrArg->GetValue().c_str());
				}
			}
			break;
		}
	case COMMAND_TYPE_GET_AGGREGATED_VALUE:
		{
			MemoryManager::Inst.CreateObject(&pStrRes);
			pStrRes->SetValue(pNode->GetAggregatedValue());
			break;
		}
	case COMMAND_TYPE_GET_SUBTREE:
		{
			MemoryManager::Inst.CreateObject(&pNodeListRes);
			AddSubtreeToNodeList(pNodeListRes, pNode);
			break;
		}
	case COMMAND_TYPE_IS_TYPE:
		{
			MemoryManager::Inst.CreateObject(&pBoolRes);
			if(ENTITY_TYPE_INT == pArg->ul_Type)
			{
				PInt pIntArg = (PInt)pArg;
				if(0 != pIntArg)
				{
					pBoolRes->SetValue(pNode->GetType() == pIntArg->GetValue());
				}
			}
			break;
		}
	case COMMAND_TYPE_IS_VALUE:
		{
			MemoryManager::Inst.CreateObject(&pBoolRes);
			if(ENTITY_TYPE_STRING == pArg->ul_Type)
			{
				PString pStrArg = (PString)pArg;
				if(0 != pStrArg)
				{
					pBoolRes->SetValue(MSTRING(pNode->GetValue()) == pStrArg->GetValue());
				}
			}
			break;
		}
	case COMMAND_TYPE_GET_CHILD_OF_TYPE:
		{
			if(ENTITY_TYPE_INT == pArg->ul_Type)
			{
				PInt pIntArg = (PInt)pArg;
				PNODE pChild = pNode->GetFirstChild();
				while(0 != pChild)
				{
					if(pChild->GetType() == pIntArg->GetValue())
					{
						pNodeRes = pChild;
						break;
					}
					pChild = pChild->GetRightSibling();
				}
			}
			break;
		}
	}
	
	if(0 != pNodeRes)
	{
		return pNodeRes;
	}
	if(0 != pNodeListRes)
	{
		return pNodeListRes;
	}
	if(0 != pStrRes)
	{
		return pStrRes;
	}
	if(0 != pIntRes)
	{
		return pIntRes;
	}
	if(0 != pNullRes)
	{
		return pNullRes;
	}
	if(0 != pBoolRes)
	{
		return pBoolRes;
	}
	return 0;
}

PENTITY Command::ExecuteListCommand(MULONG ulCommand, PENTITY pEntity, PENTITY pArg)
{
	PENTITYLIST pEntityList = (PENTITYLIST)pEntity;
	if(0 == pEntityList)
	{
		return 0;
	}

	PInt pIntRes = 0;
	PENTITYLIST pListRes = 0;
	PNull pNullRes = 0;
	PENTITY pEntityRes = 0;

	if(COMMAND_TYPE_GET_ITEM_COUNT == ulCommand)
	{
		MemoryManager::Inst.CreateObject(&pIntRes);
		pIntRes->SetValue(pEntityList->size());
	}
	else if(COMMAND_TYPE_SEEK == ulCommand)
	{
		PInt pInt = (PInt)p_Arg->GetEntity();
		if(0 != p_EntityArg)
		{
			pEntityList->Seek(pInt->GetValue(), pInt->b_IsNegative);
		}
		MemoryManager::Inst.CreateObject(&pNullRes);
	}
	else if(COMMAND_TYPE_SEEK_TO_BEGIN == ulCommand)
	{
		pEntityList->SeekToBegin();
		MemoryManager::Inst.CreateObject(&pNullRes);
	}
	else if(COMMAND_TYPE_SEEK_TO_END == ulCommand)
	{
		pEntityList->SeekToEnd();
		MemoryManager::Inst.CreateObject(&pNullRes);
	}
	else if(COMMAND_TYPE_GET_CURR_ELEM == ulCommand)
	{
		PENTITY pEntity = pEntityList->GetCurrElem();
		if(0 != pEntity)
		{
			// If the entity is not a node then it will be deleted immediately after use.
			// Therefore we need to get a copy.
			if(ENTITY_TYPE_NODE != pEntity->ul_Type)
			{
				pEntityRes = pEntity->GetCopy();
			}
			else
			{
				pEntityRes = pEntity;
			}
		}
		else
		{
			MemoryManager::Inst.CreateObject(&pNullRes);
		}
	}
	else
	{
		MemoryManager::Inst.CreateObject(&pListRes);
		EntityList::const_iterator ite1 = pEntityList->begin();
		EntityList::const_iterator iteEnd1 = pEntityList->end();
		for( ; ite1 != iteEnd1; ++ite1)
		{
			PENTITY pRes = ExecuteEntityCommand(ulCommand, *ite1, pArg);
			switch(pRes->ul_Type)
			{
			case ENTITY_TYPE_NULL:
				{
					MemoryManager::Inst.DeleteObject(pRes);
				}
			case ENTITY_TYPE_INT:
			case ENTITY_TYPE_NODE:
			case ENTITY_TYPE_STRING:
			case ENTITY_TYPE_LIST:
				{
					pListRes->push_back(pRes);
					break;
				}
			case ENTITY_TYPE_BOOL:
				{
					if(((PBool)pRes)->GetValue())
					{
						if(ENTITY_TYPE_NODE != (*ite1)->ul_Type)
						{
							pListRes->push_back((*ite1)->GetCopy());
						}	
						else
						{
							pListRes->push_back(*ite1);
						}
					}
					MemoryManager::Inst.DeleteObject(pRes);
					break;
				}
			}
		}
	}

	if(0 != pIntRes)
	{
		return pIntRes;
	}
	if(0 != pListRes)
	{
		return pListRes;
	}
	if(0 != pNullRes)
	{
		return pNullRes;
	}
	if(0 != pEntityRes)
	{
		return pEntityRes;
	}
	return 0;
}

void Command::AddSubtreeToNodeList(PENTITYLIST pList, PNODE pRoot)
{
	pList->push_back(pRoot);
	PNODE pChild = pRoot->GetFirstChild();
	while(0 != pChild)
	{
		AddSubtreeToNodeList(pList, pChild);
		pChild = pChild->GetRightSibling();
	}
}
