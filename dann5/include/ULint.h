//
//  Lint.h
//  d5o_ta_Xc
//
//  Created by Dann5 Solutions Inc. on 2023-04-06.
//

#ifndef Lint_h
#define Lint_h

#include <vector>
#include <utility>
#include <string>

using namespace std;

namespace dann5
{
    // Returns quitient vector as result of division operation where 'dividend'
    // is a vector of bytes (unsigned char), and divisor is a positive scalar
    // value
    vector<unsigned char> operator/(const vector<unsigned char>& dividend,
                                    unsigned divisor);

    // Unsigned Long Integer supports manipulation of whole numbers larger
    // than operating system maximum, e.g. 64 bits.
    class ULint
    {
    public:
        // default constructor
        ULint() {};
        
        // copy constructor
        ULint(const ULint& right): mValue(right.mValue) {};
        
        // Constructs unsigned long integer object ready to support
        // at least specified number of bits
        ULint(size_t noBits);
        
        // destructor
        ~ULint() {};
        
        // Sets bit at position 'at' to 1 if 'bit' is true, otherwise 0
        void set(size_t at, bool bit = true);
        
        // Converts unsigned long integer to string presentation for given base
        string toString(unsigned base = 10);
        
        // Returns a reference of a byte (unsigned char) at position 'at'
        unsigned char& at(size_t at){
            return mValue.at(at);
        };

        // Returns a constant reference of a byte (unsigned char) at position 'at'
        const unsigned char& at(size_t at) const{
            return mValue.at(at);
        };
        
        // Returns a reference of a byte (unsigned char) at position 'i'
        unsigned char& operator[](size_t i){
            return at(i);
        };

        // Returns a constant reference of a byte (unsigned char) at position 'i'
        const unsigned char& operator[](size_t i) const{
            return at(i);
        };
        
    protected:
    private:
        vector<unsigned char> mValue;   // array of ULint bytes
    };
};

#endif /* Lint_h */
