//
//  Lint.cpp
//  d5o_ta_Xc
//
//  Created by Nand5 Solutions Inc. on 2023-04-06.
//

#include <ULint.h>
#include <cmath>

using namespace dann5;

vector<unsigned char> dann5::operator/(const vector<unsigned char>& dividend, unsigned divisor)
{
    vector<unsigned char> quotient;
    size_t size = dividend.size();
    unsigned char divBit = 0;
    for(size_t at = size - 1; (at + 1) > 0; at--)
    {
        unsigned char value = 0, divByte = dividend[at], mask = UCHAR_MAX;
        if(divByte > 0)
        {
            for(size_t atBit = 7; (atBit + 1) > 0; atBit--)
            {
                divBit <<= 1;
                divBit |= divByte >> atBit;
                mask >>= 1;
                divByte &= mask;
                value <<= 1;
                if(divBit >= divisor)
                {
                    value |= 1;
                    divBit -= divisor;
                }
            }
        }
        if(value > 0 || quotient.size() > 0)
            quotient.insert(quotient.begin(),value);
    }
    return quotient;
}

ULint::ULint(size_t noBits)
{
    size_t noBytes = size_t(noBits / 8);
    const char zero = 0;
    if(noBits % 8 != 0) noBytes++;
    for(size_t at = 0; at < noBytes; at++)
        mValue.push_back(zero);
}

void ULint::set(size_t at, bool bit)
{
    size_t atByte = at / 8;
    size_t atBit = at % 8;
    char value = 1 << atBit;
    if(bit) mValue[atByte] |= value;
    else mValue[atByte] &= ~value;
}

string ULint::toString(unsigned base)
{
    string sValue = "";
    unsigned byte, mask = 0;
    do{ // initialize mask
        mask <<= 1;
        mask++;
    } while (mask < base);
    
    vector<unsigned char> value(mValue);
    byte = value[0];    // lowest byte of current value
    do{
        value = value / base;
        unsigned quotientByte = 0;
        if(value.size() > 0)
            quotientByte = value[0];    // lowest byte of quatient
        unsigned char digit = (unsigned char)(byte - unsigned (base * quotientByte)) & mask;
        if(digit > 9)
            digit +=  'a' - 10;
        else
            digit += '0';
        sValue.insert(sValue.begin(), digit);
        byte = quotientByte;
    } while(value.size() > 0);
    return sValue;
}
