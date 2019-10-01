#ifndef _METADATA_H
#define _METADATA_H

#include "CommonIncludes.h"

class MetaData
{
public:
	MSTRING s_RuleFileName;
	MSTRING	s_FuncSeperator;
	MSTRING s_EqualSign;
	MSTRING s_LineContinuation;
	MSTRING s_CommentStart;
	MSTRING s_ArgumentStart;
	MSTRING s_ArgumentEnd;
	MSTRING s_ListStart;
	MSTRING s_ListEnd;
	MSTRING s_ListElementSeperator;
	MSTRING s_FuncStart;
	MSTRING s_FuncEnd;
	MSTRING s_FuncArg;
	MSTRING s_FuncRet;
	MSTRING s_VarNamePrefix;
	MSTRING s_IntPrefix;
	MSTRING s_BoolTrue;
	MSTRING s_BoolFalse;
	MSTRING s_If;
	MSTRING s_IfNot;
	MSTRING s_EndIf;
	MSTRING s_While;
	MSTRING s_Do;
	MSTRING s_Break;
	MSTRING s_Continue;
	MAP_MULONG_STR	map_FuncNames;
	MAP_STR_MULONG	map_FuncNamesReverse;
    
    // Added for LDEL
    MSTRING s_ScriptFile;
    MSTRING s_LogFile;
    MSTRING s_ELAssignment;
    MSTRING s_ELVarPrefix;
    MSTRING s_ELLineTemplatePrefix;
    MSTRING s_ELBlockTemplatePrefix;
    MSTRING s_ELNumber;
    MSTRING s_ELString;
    MSTRING s_ELText;
    MSTRING s_ELSpacesString;
    MSTRING s_ELFloat;
    MSTRING s_ELVarSequenceStart;
    MSTRING s_ELVarSequenceEnd;
    MSTRING s_ELVarSequenceSeperator;
    MSTRING s_ELStringLiteralStart;
    MSTRING s_ELStringLiteralEnd;
    MSTRING s_ELStringLiteralEscape;
    MSTRING s_ELSetStart;
    MSTRING s_ELSetEnd;
    MSTRING s_ELSetSeperator;
    MSTRING s_ELSequenceVarStartIndicator;
    MSTRING s_ELSequenceVarSuffix;
};

#endif