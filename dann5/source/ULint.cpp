//
//  Lint.cpp
//  d5o_ta_Xc
//
//  Created by Nand5 Solutions Inc. on 2023-04-06.
//

#include <ULint.h>
#include <cmath>
#include <climits>
#include <stdexcept>

#include <iostream>

using namespace std;
using namespace dann5;

ULint::ULint(unsigned long long value)
{
    initValue(value);
}

dann5::ULint::ULint(string argument, Byte base)
{
    size_t noDigits = argument.size();
    ULint b(base);
    initBytes(noDigits * size_t(log2(base) + 1));
    for (size_t at = 0; at < noDigits; at++)
    {
        Byte digit = argument[at];
        if(digit == '\n') break;
        if(digit >= 'a')
            digit = digit - 'a' + 10;
        else if(digit >= 'A')
            digit = digit - 'A' + 10;
        else
            digit -= '0';
        ULint d(digit);
        (*this) = (*this) * b + d;
    }
    trim();
}

void ULint::initBytes(size_t noBits)
{
    size_t noBytes = size_t(noBits / 8) - mValue.size();
    const Byte zero = 0;
    if (noBits % 8 != 0) noBytes++;
    for (size_t at = 0; at < noBytes; at++)
        mValue.push_back(zero);
}


void ULint::initValue(unsigned long long value)
{
    if (value == 0)
        mValue.push_back(0);
    else
    {
        size_t noBits = size_t(log2(value)) + 1;
        initBytes(noBits);
        const unsigned long long mask = 1;
        unsigned long long v = value;
        for (size_t at = 0; at < noBits; at++)
        {
            set(at, mask & v);
            v >>= 1;
        }
    }
}

void ULint::set(size_t at, bool bit)
{
    size_t atByte = at / 8;
    size_t atBit = at % 8;
    char value = 1 << atBit;
    bool resize = false;
    if(bit)
    {
        if (atByte >= mValue.size())
        {
            initBytes(at + 1);
        }
        mValue[atByte] |= value;
    }
    else if (atByte < mValue.size())
    {
        mValue[atByte] &= ~value;
    }
}

void ULint::trim()
{
    size_t size = mValue.size();
    while (size > 1 && mValue[size - 1] == 0)
    {
        mValue.pop_back();
        size = mValue.size();
    }
}

string ULint::toString(Byte base) const
{
    if (noBytes() == 1 && at(0) == 0) return "0";

    string sValue = "";
    Byte byte;
    ULint value(*this), bs(base);
    do{
        ULint dgt = value % bs;
        Byte digit = dgt[0];
        if(digit > 9)
            digit +=  'a' - 10;
        else
            digit += '0';
        sValue.insert(sValue.begin(), digit);
        value /= bs;
//        cout << sValue<< " : \n"; //  << value.toString();
        byte = value[0];
        } while(value.noBytes() > 1 || byte != 0);
    return sValue;
}

ULint& ULint::operator=(const ULint& right)
{
    mValue.clear();
    mValue = right.mValue;
    return(*this);
}

ULint& ULint::operator>>= (size_t noBits)
{
    size_t size = mValue.size();
    const Byte _128 = 128;
    for (size_t atBit = 0; atBit < noBits; atBit++)
    {
        Byte carry = 0;
        for (size_t at = size - 1; (at + 1) > 0; at--)
        {
            Byte& byt = mValue[at];
            Byte c = Byte(byt * _128);
            byt >>= 1;
            byt |= carry;
            carry = c;
        }
    }
    trim();
    return(*this);
}

ULint& ULint::operator<<= (size_t noBits)
{
    const Byte _128 = 128;
    for (size_t atBit = 0; atBit < noBits; atBit++)
    {
        Byte carry = 0;
        for (size_t at = 0; at < mValue.size(); at++)
        {
            Byte& byt = mValue[at];
            Byte c = Byte(byt / _128);
            byt <<= 1;
            byt |= carry;
            carry = c;
        }
        if (carry > 0)
            mValue.push_back(carry);
    }
    return(*this);
}


ULint& ULint::operator+=(const ULint & right)
{
    size_t size = mValue.size(), rSize = right.noBytes();
    if (rSize > size)
    {
        size = rSize;
        mValue.resize(size, 0);
    }
    Byte carry = 0;
    for (size_t at = 0; at < size; at++)
    {
        unsigned short value = 0;
        if(rSize > at)
            value = mValue[at] + right[at] + carry;
        else
            value = mValue[at] + carry;
        mValue[at] = Byte(value);
        value >>= 8;
        carry = Byte(value);
    }
    if (carry > 0)
        mValue.push_back(carry);
    return(*this);
}

ULint& ULint::operator-=(const ULint & right)
{
    size_t size = mValue.size(), rSize = right.noBytes();
    if (rSize > size || (rSize == size && mValue[size - 1] < right[size - 1]))
        throw logic_error("Error@ULint:Minuend has to be larger than subtrahend.");
    Byte carry = 0;
    ULint r(right);
    Range tR(*this, 0, noBits()), rR(r, 0, right.noBits());
    tR -= rR;
    trim();
    return(*this);
}

ULint& ULint::operator*=(const ULint& right)
{
    ULint value(*this), r(right);
    Range multiplier(r, 0, right.noBits());
    Iterator at = multiplier.end(), start = multiplier.start();
    --at;
    while (at != start)
    {
        value <<= 1;
        if (*(--at))
            value += (*this);
    }
    mValue = value.mValue;
    return(*this);
}

ULint& ULint::operator/=(const ULint & right)
{
    ULint r(right), quotient;
    size_t size = noBits(), rSize = right.noBits();
    if (rSize <= size)
    {
        Range dividend(*this, size - rSize, size), divisor(r, 0, rSize);
        Iterator last(*this, this->noBits());
        size_t dvdndExtnd = 0;
        while (dividend.start().atByte() != ULLONG_MAX && dividend.start().atBit() >= 0)
        {
            last--;
            Byte b = (*dividend)[0];
            quotient <<= 1;
            if (dividend >= divisor)
            {
                quotient[0] |= 1;
                dividend -= divisor;
                Iterator next(last); next--;
                if (dvdndExtnd > 0 && !*(next))
                {
                    Iterator& end = dividend.end();
                    while (dvdndExtnd > 0) { end--; dvdndExtnd--; last--; }
                    b = (*dividend)[0];
                }
            }
            else if (*last)
            {
                dividend.left();
                last++;
                dvdndExtnd++;
            }
            dividend--;
        }
    }
    mValue.clear();
    mValue = quotient.mValue;
    return(*this);
}

ULint& ULint::operator%=(const ULint& right)
{
    if ((*this) > right)
    {
        ULint quotient = (*this) / right;
        (*this) -= quotient * right;
    } 
    else if ((*this) == right)
    {
        mValue.clear();
        mValue.push_back(0);
    }
    return(*this);
}

ULint ULint::pow(size_t exponent) const
{
    ULint powered(1);
    for (size_t exp = 1; exp < exponent; exp++)
    {
        powered *= (*this);
    }
    return powered;
}

ULint ULint::operator>> (size_t noBits) const
{
    ULint result(*this);
    result >>= noBits;
    return result;
}

ULint ULint::operator<< (size_t noBits) const
{
    ULint result(*this);
    result <<= noBits;
    return result;
}


ULint ULint::operator+(const ULint& right) const
{
    ULint result(*this);
    result += right;
    return result;
}

ULint ULint::operator-(const ULint& right) const
{
    ULint result(*this);
    result -= right;
    return result;
}


ULint ULint::operator*(const ULint& right) const
{
    ULint result(*this);
    result *= right;
    return result;
}


ULint ULint::operator/(const ULint& right) const
{
    ULint result(*this);
    result /= right;
    return result;
}


ULint ULint::operator%(const ULint& right) const
{
    ULint result(*this);
    result %= right;
    return result;
}

bool ULint::operator==(const ULint& right) const
{
    ULint t(*this), r(right);
    Range tR(t, 0, noBits()), rR(r, 0, r.noBits());
    return tR == rR;
}

bool ULint::operator>(const ULint& right) const
{
    ULint t(*this), r(right);
    Range tR(t, 0, noBits()), rR(r, 0, r.noBits());
    return tR > rR;
}

ostream& dann5::operator << (std::ostream& out, const ULint& r)
{
    if((out.flags() & out.oct) == out.oct)
        out << r.toString(8);
    else if((out.flags() & out.hex) == out.hex)
        out << r.toString(16);
    else
       out << r.toString();
    return out;
}

/*** ULint::Iterator***/
ULint::Iterator& ULint::Iterator::operator++()
{
    if (++mAtBit == 8)
    {
        mAtByte++;
        mAtBit = 0;
    }
    return *this;
}

ULint::Iterator& ULint::Iterator::operator--()
{
    if (mAtBit-- == 0)
    {
        mAtByte--;
        mAtBit = 7;
    }
    return *this;
}

bool ULint::Iterator::operator==(const ULint::Iterator& right) const
{
    return &mULint == &right.mULint && mAtByte == right.mAtByte && mAtBit == right.mAtBit;
    //return *as_const(*this) == *right;
}

bool ULint::Iterator::operator*() const
{
    Byte& byte = mULint[mAtByte];
    Byte mask = Byte(std::pow(2, mAtBit));
    return ((byte & mask) == mask);
}

size_t ULint::Iterator::operator-(const Iterator& right) const
{
    size_t noBytes = mAtByte - right.mAtByte;
    size_t noBits = noBytes * 8 + mAtBit - right.mAtBit;
    return noBits;
}

ULint::Iterator& ULint::Iterator::operator^=(bool value)
{
    Byte& byte = mULint[mAtByte];
    Byte mask = Byte(std::pow(2, mAtBit));
    if (value)
        if ((byte & mask) == mask)
            byte &= ~mask;
        else
            byte |= mask;
        
    return(*this);
}

/*** ULint::Range ***/
ULint::Range& ULint::Range::operator++()
{
    ++mStart;
    ++mEnd;
    return (*this);
}

ULint::Range& ULint::Range::operator--()
{
    --mStart;
    --mEnd;
    return (*this);
}

ULint::Range& ULint::Range::left(size_t noBits)
{
    for(size_t at = 0; at < noBits; at++)
        ++mEnd;
    return (*this);
}

ULint::Range& ULint::Range::right(size_t noBits)
{
    for (size_t at = 0; at < noBits; at++)
        --mStart;
    return (*this);
}

ULint ULint::Range::operator*() const
{
    ULint range;
    Iterator at = mEnd;
    do
    {
        at--;
        range <<= 1;
        if (*as_const(at))
            range[0] |= 1;
    } while (at != mStart);
    return range;
}

ULint::Range& ULint::Range::operator+=(const ULint::Range& right)
{
    if (noBits() < right.noBits())
        throw logic_error("Error@ULint::Range::operator+= left operand has to be larger than right!");

    const Iterator& end = right.end();
    bool carry = false;
    for (Iterator at = mStart, rAt = right.start();  at != mEnd; at++)
    {
        bool c = (*as_const(at));
        if (rAt != end)
        {
            c &= (*as_const(rAt));
            at ^= (*as_const(rAt)) ^ carry;
            rAt++;
        }
        else
        {
            c &= carry;
            at ^= carry;
        }
        carry = c;
    }
    return (*this);
}

ULint::Range& ULint::Range::operator-=(const ULint::Range& right)
{
    if (noBits() < right.noBits())
        throw logic_error("Error@ULint::Range::operator-= left operand has to be larger than right!");

    const Iterator& end = right.end();
    bool carry = false;
    for (Iterator at = mStart, rAt = right.start(); at != mEnd; at++)
    {
        bool bThis = (*as_const(at));
        bool c = false;
        if (rAt != end)
        {
            bool bRight = (*as_const(rAt));
            c = ((carry || bRight) && !bThis) || ((carry && bRight) && bThis);
//            c = ((carry ^ bRight) && !bThis) || ((carry && bRight) && bThis);
//            at ^= bRight || carry;
            at ^= bRight ^ carry;
            rAt++;
        }
        else
        {
            c = (carry && !bThis);
            at ^= carry;
        }
        carry = c;
    }
    return (*this);
}

bool ULint::Range::operator==(const Range& right) const
{
    size_t size = noBits(), rSize = right.noBits();
    if (size != rSize)
        return false;
    for (Iterator at = mStart, rAt = right.start(); at != mEnd; at++, rAt++)
        if (*at != *rAt)
            return false;
    return true;
}

bool ULint::Range::operator>(const Range& right) const
{
    size_t size = noBits(), rSize = right.noBits();
    Iterator rEnd = right.end(), rStart = right.start();
    Iterator at = mEnd, rAt = rEnd;
    while (size < rSize) { if (*(--at)) return false; rSize--; }
    while (size > rSize) { if (*(--at)) return true; size--; }
    // when size == rSize compare bits
    do
    {
        bool bAt = *(--at), brAt = *(--rAt);
        if (bAt < brAt)      return false;
        else if (bAt > brAt) return true;
    } while (at != mStart);
    return false;
}
