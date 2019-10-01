#include "DefFileReader.h"
#include "MetaData.h"
#include "MemMan.h"
#include "Utils.h"

MetaData* DefFileReader::Read(MSTRING sFile)
{
	MetaData* pMD = 0;
	MIFSTREAM file(sFile.c_str());
	MSTRING sLine;
	if(file.is_open())
	{
		MemoryManager::Inst.CreateObject(&pMD);
		while(!file.eof())
		{
			getline(file, sLine);
			ProcessLine(sLine, pMD);
		}
		file.close();
	}
	return pMD;
}

void DefFileReader::ProcessLine(MSTRING& sLine, MetaData* pMD)
{
	Utils::TrimLeft(sLine, _MSTR( \t));
	Utils::TrimRight(sLine, _MSTR( \t));
	LST_STR lstTokens;
	lstTokens.push_back(SPACE);
	lstTokens.push_back(_MSTR(\t));
	LST_STR lstSep;
	LST_STR lstVal;
	Utils::TokenizeString(sLine, lstTokens, lstSep, lstVal);
	if(lstVal.size() < 3)
	{
		return;
	}
	// Example for Line format 
	// DEF PARENT Parent	#This is a comment
	// Any string after the third string is regarded as a comment
	// Any line that does not start with DEF is a comment line
	LST_STR::const_iterator ite = lstVal.begin();
	MSTRING sStr = *ite;
	Utils::MakeUpper(sStr);
	if(sStr != _MSTR(DEF))
	{
		return;
	}
	++ite;
	MSTRING sKey = *ite;
	++ite;
	MSTRING sVal = *ite;
	AddKeyAndValue(pMD, sKey, sVal);
}

// We use the following macro to ease typing of else if blocks for function names inside the function AddKeyAndValue
#define ADD_FUNC_NAME(X) else if(_MSTR(X) == sKey){pMD->map_FuncNames[COMMAND_TYPE_##X] = sVal;pMD->map_FuncNamesReverse[sVal] = COMMAND_TYPE_##X;}

void DefFileReader::AddKeyAndValue(MetaData* pMD, MSTRING sKey, MSTRING sVal)
{
	if(_MSTR(RULE_FILE_NAME) == sKey)
	{
		pMD->s_RuleFileName = sVal;
	}
	else if(_MSTR(LINE_CONTINUATION_STRING) == sKey)
	{
		pMD->s_LineContinuation = sVal;
	}
	else if(_MSTR(FUNCTION_SEPERATOR) == sKey)
	{
		pMD->s_FuncSeperator = sVal;
	}
	else if(_MSTR(EQUAL_SIGN) == sKey)
	{
		pMD->s_EqualSign = sVal;
	}
	else if(_MSTR(COMMENT_START) == sKey)
	{
		pMD->s_CommentStart = sVal;
	}
	else if(_MSTR(ARGUMENT_START) == sKey)
	{
		pMD->s_ArgumentStart = sVal;
	}
	else if(_MSTR(ARGUMENT_END) == sKey)
	{
		pMD->s_ArgumentEnd = sVal;
	}
	else if(_MSTR(LIST_START) == sKey)
	{
		pMD->s_ListStart = sVal;
	}
	else if(_MSTR(LIST_END) == sKey)
	{
		pMD->s_ListEnd = sVal;
	}
	else if(_MSTR(LIST_ELEMENT_SEPERATOR) == sKey)
	{
		pMD->s_ListElementSeperator = sVal;
	}
	else if(_MSTR(FUNCTION_START) == sKey)
	{
		pMD->s_FuncStart = sVal;
	}
	else if(_MSTR(FUNCTION_END) == sKey)
	{
		pMD->s_FuncEnd = sVal;
	}
	else if(_MSTR(FUNCTION_ARGUMENT) == sKey)
	{
		pMD->s_FuncArg = sVal;
	}
	else if(_MSTR(FUNCTION_RETURN_VALUE) == sKey)
	{
		pMD->s_FuncRet = sVal;
	}
	else if(_MSTR(VARIABLE_NAME_PREFIX) == sKey)
	{
		pMD->s_VarNamePrefix = sVal;
	}
	else if(_MSTR(INTEGER_PREFIX) == sKey)
	{
		pMD->s_IntPrefix = sVal;
	}
	else if(_MSTR(BOOL_TRUE) == sKey)
	{
		pMD->s_BoolTrue = sVal;
	}
	else if(_MSTR(BOOL_FALSE) == sKey)
	{
		pMD->s_BoolFalse = sVal;
	}
	else if(_MSTR(IF) == sKey)
	{
		pMD->s_If = sVal;
	}
	else if(_MSTR(IFNOT) == sKey)
	{
		pMD->s_IfNot = sVal;
	}
	else if(_MSTR(ENDIF) == sKey)
	{
		pMD->s_EndIf = sVal;
	}
	else if(_MSTR(WHILE) == sKey)
	{
		pMD->s_While = sVal;
	}
	else if(_MSTR(DO) == sKey)
	{
		pMD->s_Do = sVal;
	}
	else if(_MSTR(BREAK) == sKey)
	{
		pMD->s_Break = sVal;
	}
	else if(_MSTR(CONTINUE) == sKey)
	{
		pMD->s_Continue = sVal;
	}
    
    // LDEL
    else if(_MSTR(LDEL_SCRIPT_FILE) == sKey) {
        pMD->s_ScriptFile = sVal;
    }
    else if(_MSTR(LDEL_LOG_FILE) == sKey) {
        pMD->s_LogFile = sVal;
    }
    else if(_MSTR(LDEL_ASSIGNMENT) == sKey)
	{
		pMD->s_ELAssignment = sVal;
	}
    else if(_MSTR(LDEL_VARIABLE_PREFIX) == sKey)
	{
		pMD->s_ELVarPrefix = sVal;
	}
    else if(_MSTR(LDEL_LINE_TEMPLATE_PREFIX) == sKey)
	{
		pMD->s_ELLineTemplatePrefix = sVal;
	}
    else if(_MSTR(LDEL_BLOCK_TEMPLATE_PREFIX) == sKey)
	{
		pMD->s_ELBlockTemplatePrefix = sVal;
	}
    else if(_MSTR(LDEL_NUMBER) == sKey)
	{
		pMD->s_ELNumber = sVal;
	}
    else if(_MSTR(LDEL_STRING) == sKey)
	{
		pMD->s_ELString = sVal;
	}
    else if(_MSTR(LDEL_TEXT) == sKey)
	{
		pMD->s_ELText = sVal;
	}
    else if(_MSTR(LDEL_SPACE_STRING) == sKey)
	{
		pMD->s_ELSpacesString = sVal;
	}
    else if(_MSTR(LDEL_FLOAT) == sKey)
	{
		pMD->s_ELFloat = sVal;
	}
    else if(_MSTR(LDEL_VAR_SEQUENCE_START) == sKey)
	{
		pMD->s_ELVarSequenceStart = sVal;
	}
    else if(_MSTR(LDEL_VAR_SEQUENCE_END) == sKey)
	{
		pMD->s_ELVarSequenceEnd = sVal;
	}
    else if(_MSTR(LDEL_VAR_SEQUENCE_SEPARATOR) == sKey)
	{
		pMD->s_ELVarSequenceSeperator = sVal;
	}
    else if(_MSTR(LDEL_STRING_LITERAL_START) == sKey)
	{
		pMD->s_ELStringLiteralStart = sVal;
	}
    else if(_MSTR(LDEL_STRING_LITERAL_END) == sKey)
	{
		pMD->s_ELStringLiteralEnd = sVal;
	}
    else if(_MSTR(LDEL_STRING_LITERAL_ESCAPE) == sKey)
	{
		pMD->s_ELStringLiteralEscape = sVal;
	}
    else if(_MSTR(LDEL_SET_START) == sKey)
	{
		pMD->s_ELSetStart = sVal;
	}
    else if(_MSTR(LDEL_SET_END) == sKey)
	{
		pMD->s_ELSetEnd = sVal;
	}
    else if(_MSTR(LDEL_SET_SEPARATOR) == sKey)
	{
		pMD->s_ELSetSeperator = sVal;
	}
    else if(_MSTR(LDEL_SEQUENCE_VAR_START_INDICATOR) == sKey)
	{
		pMD->s_ELSequenceVarStartIndicator = sVal;
	}
    else if(_MSTR(LDEL_SEQUENCE_VAR_SUFFIX) == sKey)
	{
		pMD->s_ELSequenceVarSuffix = sVal;
	}
    // LDEL
    
	ADD_FUNC_NAME(LEFT_SIBLING)
	ADD_FUNC_NAME(RIGHT_SIBLING)
	ADD_FUNC_NAME(PARENT)
	ADD_FUNC_NAME(FIRST_CHILD)
	ADD_FUNC_NAME(CHILDREN)
	ADD_FUNC_NAME(CHILD_COUNT)
	ADD_FUNC_NAME(GET_VALUE)
	ADD_FUNC_NAME(GET_LVALUE)
	ADD_FUNC_NAME(GET_RVALUE)
	ADD_FUNC_NAME(GET_CUSTOM_STRING)
	ADD_FUNC_NAME(GET_ID)
	ADD_FUNC_NAME(GET_TYPE)
	ADD_FUNC_NAME(GET_NATURE)
	ADD_FUNC_NAME(GET_WEIGHT)
	ADD_FUNC_NAME(GET_MIN_CHILD_WEIGHT)
	ADD_FUNC_NAME(GET_MAX_CHILD_WEIGHT)
	ADD_FUNC_NAME(SET_VALUE)
	ADD_FUNC_NAME(SET_LVALUE)
	ADD_FUNC_NAME(SET_RVALUE)
	ADD_FUNC_NAME(SET_TYPE)
	ADD_FUNC_NAME(SET_NATURE)
	ADD_FUNC_NAME(SET_CUSTOM_STRING)
	ADD_FUNC_NAME(SET_MIN_CHILD_WEIGHT)
	ADD_FUNC_NAME(SET_MAX_CHILD_WEIGHT)
	ADD_FUNC_NAME(SET_WEIGHT)
	ADD_FUNC_NAME(EXPAND)
	ADD_FUNC_NAME(ADD_NODE)
	ADD_FUNC_NAME(ADD_NODE_WITH_WEIGHT)
	ADD_FUNC_NAME(READ_FROM_FILE)
	ADD_FUNC_NAME(GET_AGGREGATED_VALUE)
	ADD_FUNC_NAME(GET_SUBTREE)
	ADD_FUNC_NAME(IS_TYPE)
	ADD_FUNC_NAME(IS_VALUE)
	ADD_FUNC_NAME(GET_CHILD_OF_TYPE)
	ADD_FUNC_NAME(IS_STRING_EQUAL_TO)
	ADD_FUNC_NAME(IS_STRING_MEMBER_OF)
	ADD_FUNC_NAME(IS_HAVING_SUBSTRING)
	ADD_FUNC_NAME(IS_HAVING_LEFT_SUBSTRING)
	ADD_FUNC_NAME(IS_HAVING_RIGHT_SUBSTRING)
	ADD_FUNC_NAME(ADD_PREFIX)
	ADD_FUNC_NAME(ADD_POSTFIX)
	ADD_FUNC_NAME(TRIM_LEFT)
	ADD_FUNC_NAME(TRIM_RIGHT)
	ADD_FUNC_NAME(WRITE_TO_FILE)
	ADD_FUNC_NAME(GET_LENGTH)
	ADD_FUNC_NAME(IS_INT_EQUAL_TO)
	ADD_FUNC_NAME(IS_INT_MEMBER_OF)
	ADD_FUNC_NAME(IS_LESS_THAN)
	ADD_FUNC_NAME(IS_LESS_THAN_OR_EQUAL_TO)
	ADD_FUNC_NAME(IS_GREATER_THAN)
	ADD_FUNC_NAME(IS_GREATER_THAN_OR_EQUAL_TO)
	ADD_FUNC_NAME(ADD)
	ADD_FUNC_NAME(SUBTRACT)
	ADD_FUNC_NAME(TOSTRING)
	ADD_FUNC_NAME(GET_ITEM_COUNT)
	ADD_FUNC_NAME(SEEK)
	ADD_FUNC_NAME(SEEK_TO_BEGIN)
	ADD_FUNC_NAME(SEEK_TO_END)
	ADD_FUNC_NAME(GET_CURR_ELEM)
	ADD_FUNC_NAME(IS_NULL)
	ADD_FUNC_NAME(IS_NOT_NULL)	
	ADD_FUNC_NAME(BOOL_AND)
	ADD_FUNC_NAME(BOOL_OR)
}
