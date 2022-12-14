//
//  UTestQbit.h
//  d5o_ta_Xc
//
//  Created by Nebojsa Vojinovic on 2022-12-13.
//

#ifndef UTestQbit_h
#define UTestQbit_h

#include <iostream>

using namespace std;

namespace dann5
{
    class UTestQbit
    {
    public:
        UTestQbit() {};
        
        void runAll(ostream& out);
        
        void initialization(ostream& out);
        void assignment(ostream& out);
        void bitwise(ostream& out);
        
    protected:
    private:
    };
};

#endif /* UTestQbit_h */
