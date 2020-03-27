//  Created by Afaz
//  Created by Afaz Deen on 10 October 2019
//  Copyright (c) 2013 99x Technology. All rights reserved.
#ifndef __LogAnalyzer__ELJSON__
#define __LogAnalyzer__ELJSON__

#include <iostream>
#include "ELVariable.h"

class ELJSON : public ELVariable {
public:
    ELJSON();
    virtual ~ELJSON();
    bool EvaluateString (MSTRING& str, MSTRING::size_type startPos, MSTRING::size_type& newPos);
};


#endif //LOGANALYZER_ELJSON_H
