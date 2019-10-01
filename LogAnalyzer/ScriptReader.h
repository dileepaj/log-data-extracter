#ifndef _SCRIPOTREADER_H
#define _SCRIPOTREADER_H

#include "CommonIncludes.h"

class ExecutionTemplate;
class ExecutionTemplateList;
class MetaData;
class Entity;
class Command;

class ScriptReaderOutput
{
public:
	ExecutionTemplateList* p_ETL;
	MAP_STR_EXECTMPLIST	map_Functions;
};

class ScriptReader
{
	enum CommandElementType
	{
		CET_String,
		CET_VarName,
		CET_Int,
		CET_BoolTrue,
		CET_BoolFalse,
		CET_FuncStart,
		CET_ArgStart,
		CET_ArgEnd,
		CET_ListStart,
		CET_ListEnd,
		CET_ListElemSep,
		CET_EqualSign,
		CET_FunctionStart,
		CET_FunctionEnd,
		CET_FuncArg,
		CET_FuncRet,
		CET_If,
		CET_IfNot,
		CET_EndIf,
		CET_While,
		CET_Do,
		CET_Break,
		CET_Continue
	};

	enum SpecialLineType
	{
		SLT_Normal,
		SLT_FuncStart,
		SLT_FuncEnd
	};

	class CommandElement
	{
	public:
		CommandElementType e_Type;
		MSTRING s_Str;
	};

	typedef std::vector<CommandElement>	VEC_CE;

	class ProcessLineRetVal
	{
	public:
		ExecutionTemplate* p_ET;
		SpecialLineType slt;
		MSTRING s_Str;

		ProcessLineRetVal()
			: p_ET(0), slt(SLT_Normal), s_Str(EMPTY_STRING)
		{

		}
	};

	MetaData* p_MetaData;
	
public:
	bool ProcessScript(MSTRING sFile, MetaData* pMD, ScriptReaderOutput& op);

private:
	void ReadFileToLines(MSTRING sFile, MSTRING sLineContinuation, MSTRING sCommentStart, LST_STR& lstLines, LST_INT& lstLineNumbers);
	ProcessLineRetVal ProcessLine(MSTRING sLine, MetaData* pMD);
	void GetCommandElements(MSTRING sCommand, VEC_CE& vecCE, MetaData* pMD);
	ExecutionTemplate* GetEntity(VEC_CE& vecCE, VEC_CE::size_type stStart, VEC_CE::size_type stEnd);
	EntityList* GetList(VEC_CE& vecCE, VEC_CE::size_type stStart, VEC_CE::size_type stEnd);
	Command* GetFunction(VEC_CE& vecCE, VEC_CE::size_type stStart, VEC_CE::size_type stEnd);
	void GetNextFirstLevelCommandElementPos(VEC_CE& vecCE, VEC_CE::size_type stStart, VEC_CE::size_type stEnd, CommandElementType cet, std::map<CommandElementType, CommandElementType>& mapContextChangeElements, VEC_CE::size_type& stElemPos);
};

#endif