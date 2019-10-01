#ifndef _NODE_H
#define _NODE_H

#include "CommonIncludes.h"
#include "Entity.h"

class Node : public Entity
{
protected:
	MULONG ul_ID;
	MUSHORT us_Type;
	PMCHAR z_Value;
	PMCHAR z_LValue;
	PMCHAR z_RValue;
	PMCHAR z_CustomStr;
	MBYTE by_Nature;
	MULONG ul_Weight;
	MULONG ul_MinChildWeight;
	MULONG ul_MaxChildWeight;
	PNODE p_Next;
	PNODE p_Prev;
	PNODE p_Parent;
	PNODE p_FirstChild;
	PNODE p_LastChild;
    PVOID p_CustomObj;

	MULONG ul_ChildCount;

public:
	Node(MULONG ulID);
	virtual ~Node();

	virtual void Destroy();
	void DestroyNodeAlone();
	void DestroyWithSubTree();

	// Get functions
	PNODE GetLeftSibling();
	PNODE GetRightSibling();
	PNODE GetParent();
	PNODE GetFirstChild();
	PNODE GetLastChild();
	MULONG GetChildCount();
	PMCHAR GetValue();
	PMCHAR GetLVal();
	PMCHAR GetRVal();
	PMCHAR GetCustomString();
	MULONG GetID();
	MUSHORT GetType();
	MULONG GetWeight();
	MBYTE GetNature();	
	MSTRING GetAggregatedValue();
	PNODE GetCopy();
	PNODE GetCopyWithSubTree();
	MULONG GetMinimumChildWeight();
	MULONG GetMaximumChildWeight();
    PVOID GetCustomObj();

	// Set functions
	void SetValue(PMCHAR pVal);
	void SetLValue(PMCHAR pLVal);
	void SetRValue(PMCHAR pRVal);
	void SetCustomString(PMCHAR pCustStr);
	void SetID(MULONG ulID);
	void SetWeight(MULONG ulWeight);
	void SetNature(MBYTE byNature);
	void SetType(MUSHORT usType);
	void SetRightSibling(PNODE pNext);
	void SetLeftSibling(PNODE pPrev);
	void SetParent(PNODE pParent);
	void SetMinimumChildWeight(MULONG ulMinChildWeight);
	void SetMaximumChildWeight(MULONG ulMinChildWeight);
    void SetCustomObj(PVOID obj);
	
	// Other functions
	void Expand(LST_STR& lstTokens);
	PNODE AddNode();
    void AppendNode(PNODE node);
	PNODE AddNode(PNODE pNode, bool bMakeCopy);
	PNODE AddNodeWithWeight(MULONG ulWeight);
	void ReadValueFromFile(CPMCHAR zFilename);

private:
	void AddAggregatedValue(MSTRING& sStr);
};

#endif