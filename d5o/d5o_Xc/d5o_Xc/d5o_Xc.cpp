//
//  d5o_Xc.cpp
//  d5o_Xc
//
//  Created by Nebojsa Vojinovic on 2022-09-05.
//

#include <iostream>
#include "d5o_Xc.hpp"
#include "d5o_XcPriv.hpp"

void d5o_Xc::HelloWorld(const char * s)
{
    d5o_XcPriv *theObj = new d5o_XcPriv;
    theObj->HelloWorldPriv(s);
    delete theObj;
};

void d5o_XcPriv::HelloWorldPriv(const char * s) 
{
    std::cout << s << std::endl;
};

