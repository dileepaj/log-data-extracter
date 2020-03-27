//
//  ELJSON.cpp
//  LogAnalyzer
//
//  Created by Afaz
//  Created by Afaz Deen on 10 October 2019
//  Copyright (c) 2013 99x Technology. All rights reserved.

// Created class to identify json objects in a log file

#include "ELJSON.h"
#include <iostream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;
using namespace std;

ELJSON::ELJSON()
        : ELVariable() {
}

ELJSON::~ELJSON() {

}

// Created the function by Michelle - Able to identify single line JSON objects in a log line - using a count for open and close curly braces determined the object's start and end point then using a JSON parser verified the object is indeed a JSON object.
bool ELJSON:: EvaluateString (MSTRING& str, MSTRING::size_type startPos, MSTRING::size_type& newPos) {
    MSTRING::size_type pos = startPos;
    bool succ = false;


    int openParenthesisCount=0;
    int closeParenthesisCount=0;

    MSTRING stringEvaluatedSoFar = EMPTY_STRING;
    while (true) {
        if (pos == MSTRING::npos) {
            break;
        }
        MCHAR ch = str.at(pos);
        stringEvaluatedSoFar+=ch;
        if( ch =='{'){
            //break;
            openParenthesisCount++;
        }
        if(ch =='}'){
            closeParenthesisCount++;

        }
        if(openParenthesisCount<=closeParenthesisCount)
        {
            newPos=++pos;
            break;
        }
        ++pos;
        if(pos>=str.size()){
            break;
        }
    }

    try
    {
        json j1=json::parse(stringEvaluatedSoFar);
        evaluatedStr = stringEvaluatedSoFar;
        succ=true;
    }
    catch (exception& e)
    {
        cout << "Standard exception: " << e.what() << endl;
        exit (EXIT_FAILURE);
    }

    return succ;
}
