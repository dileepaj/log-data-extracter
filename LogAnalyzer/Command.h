#include "CommonIncludes.h"

class ExecutionTemplate;
class ExecutionContext;

class Command
{
protected:
	MULONG ul_CommandType;
	ExecutionTemplate* p_Arg;
	PENTITY p_EntityArg;
	MSTRING s_AdditionalFuncName;
	
public:
	Command();
	~Command();

	void Destroy();

	Command* GetCopy();
	void SetType(MULONG ulType);
	void SetArg(ExecutionTemplate* pArg);
	void SetEntityArg(PENTITY pArg);
	void SetAdditionalFuncName(MSTRING sFun);
	PENTITY Execute(PENTITY pEntity, ExecutionContext* pContext);

private:
	PENTITY ExecuteNodeCommand(MULONG ulCommand, PENTITY pEntity, PENTITY pArg);
	PENTITY ExecuteStringCommand(MULONG ulCommand, PENTITY pEntity, PENTITY pArg);
	PENTITY ExecuteIntCommand(MULONG ulCommand, PENTITY pEntity, PENTITY pArg);
	PENTITY ExecuteBoolCommand(MULONG ulCommand, PENTITY pEntity, PENTITY pArg);
	PENTITY ExecuteListCommand(MULONG ulCommand, PENTITY pEntity, PENTITY pArg);
	PENTITY ExecuteEntityCommand(MULONG ulCommand, PENTITY pEntity, PENTITY pArg);
	void	AddSubtreeToNodeList(PENTITYLIST pList, PNODE pRoot);
};