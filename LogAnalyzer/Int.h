#include "Value.h"

class Int : public Value<MULONG, ENTITY_TYPE_INT>
{
public:
	bool b_IsNegative;

	Int()
		: b_IsNegative(false), Value<MULONG, ENTITY_TYPE_INT>()
	{

	}

	virtual ~Int()
	{

	}
};