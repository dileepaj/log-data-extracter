//
//  main.m
//  LogAnalyzer
//
//  Created by Dileepa Jayathilake on 11/7/12.
//  Copyright (c) 2012 99x Eurocenter. All rights reserved.
//

#import <Foundation/Foundation.h>
#include <map>
#include <string>
#include <iostream>
#include "CPlusPlusEntry.h"

using namespace std;

int main(int argc, const char * argv[])
{

    @autoreleasepool {
        
        // insert code here...
        NSLog(@"Hello, World!");
//        map<string, string> myMap;
//        myMap["abc"] = "xyz";
//        cout << myMap["abc"];
        CPlusPlusEntry cppEntry;
        cppEntry.RunDefault();
    }
    return 0;
}

