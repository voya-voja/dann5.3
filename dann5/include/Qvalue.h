//
//  Qvalue.h
//  d5o_ta_Xc
//
//  Created by Nebojsa Vojinovic on 2023-11-10.
//

#ifndef Qvalue_h
#define Qvalue_h

#include <vector>

using namespace std;

namespace dann5 {
    // Quantum value is in S-uperposition state for any value except 0 and 1
    typedef unsigned char Qvalue;

    // Quantum default superposition value,
    // though any Q bit value except 0 and 1 will be considered superposition
    static const Qvalue cSuperposition = 'S'; // value 83

    // an array of Quantum values
    typedef vector<Qvalue> Qvalues;
};

#endif /* Qvalue_h */
