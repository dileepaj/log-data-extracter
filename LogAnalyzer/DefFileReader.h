#ifndef _DEFFILEREADER_H
#define _DEFFILEREADER_H

#include "CommonIncludes.h"

class MetaData;

class DefFileReader
{
public:
	MetaData* Read(MSTRING sFile);

private:
	void ProcessLine(MSTRING& sLine, MetaData* pMD);
	void AddKeyAndValue(MetaData* pMD, MSTRING sKey, MSTRING sVal);
};

#endif