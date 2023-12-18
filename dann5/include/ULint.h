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
#include <cstddef>  // std::byte
#include <ostream>
#include <cmath>

using namespace std;

namespace dann5
{
    // A byte is unsigned char
    typedef unsigned char Byte;

    // Unsigned Long Integer supports manipulation of whole numbers larger
    // than operating system maximum, e.g. 64 bits.
    class ULint
    {
    public:
        // default constructor
        ULint() { mValue.push_back(0); };
        
        // copy constructor
        ULint(const ULint& right): mValue(right.mValue) {};
        
        // Constructs unsigned long integer object initialized with the
        // unsigned long long value
        ULint(unsigned long long value);
        
        // Constructs unsigned long integer object by
        // converting a string argument for given base
        ULint(string argument, Byte base = 10);

        // destructor
        ~ULint() {};
        
        // Sets bit at position 'at' to 1 if 'bit' is true, otherwise 0
        void set(size_t at, bool bit = true);
        
        // Converts unsigned long integer to string presentation for given base
        string toString(Byte base = 10) const;

        // Returns number of containing bytes.
        size_t noBytes() const { return mValue.size(); };

        // Returns number of containing bits.
        size_t noBits() const {
            size_t last = noBytes() - 1;
            Byte value = at(last);
            if (last == 0 && value == 0) return 1;
            return last * 8 + size_t(log2<Byte>(value)) + 1;
        };
        
        // Returns a reference of a byte (unsigned char) at position 'at'
        Byte& at(size_t at) {
            return mValue.at(at);
        };

        // Returns a constant reference of a byte (unsigned char) at position 'at'
        const Byte& at(size_t at) const{
            return mValue.at(at);
        };
        
        // Returns a reference of a byte (unsigned char) at position 'i'
        Byte& operator[](size_t i){
            return at(i);
        };

        // Returns a constant reference of a byte (unsigned char) at position 'i'
        const Byte& operator[](size_t i) const{
            return at(i);
        };

        // increment operators
        ULint& operator++() { (*this) += 1; return(*this); };
        ULint& operator++(int) { return ++(*this); };

        // decrement operators
        ULint& operator--() { (*this) -= 1; return(*this); };
        ULint& operator--(int) { return --(*this); };

        /*** Assignment operattors ***/
        // Assigns the value on the right to the variable on the left 
        ULint& operator=(const ULint& right);

        // Shifts the value to right by the number of bits specified by the right operand and then assigns the result to the variable on the left.
        ULint& operator>>= (size_t noBits);

        // Shifts the value to left by the number of bits specified by the right operand and then assigns the result to the variable on the left.
        ULint& operator<<= (size_t noBits);

        // First adds the current value of the variable on left to the value on the right and then assigns the result to the variable on the left 
        ULint& operator+=(const ULint& right);

        // First subtracts the value on the right from the current value of the variable on left and then assign the result to the variable on the left
        ULint& operator-=(const ULint& right);

        // First multiplies the current value of the variable on left to the value on the right and then assign the result to the variable on the left
        ULint& operator*=(const ULint& right);

        // First divides the current value of the variable on left by the value on the right and then assign the result to the variable on the left
        ULint& operator/=(const ULint& right);

        // First calculates mode of the current value of the variable on left by the value on the right and then assign the result to the variable on the left
        ULint& operator%=(const ULint & right);

        // Returns a ULint object as a power of 'this' object's value raised to the provided exponent
        ULint pow(size_t exponent) const;

        /*** Bitwise operattors ***/
        // Shifts the value to right by the number of bits specified by the right operand
        ULint operator>> (size_t noBits) const;

        // Shifts the value to left by the number of bits specified by the right operand
        ULint operator<< (size_t noBits) const;

        /*** Arithmetic operattors ***/
        // Returns addition of two operands
        ULint operator+(const ULint& right) const;

        // Returns subtraction of second operand from the first
        ULint operator-(const ULint& right) const;

        // Returns multiplication of two operands
        ULint operator*(const ULint& right) const;

        // Returns divission of first operand by the second operand 
        ULint operator/(const ULint& right) const;

        // Returns calculated mode, i.e. returns the remainder an integer division of first operand by the second operand
        ULint operator%(const ULint& right) const;

        /*** Relational operattors ***/
        // Checks if both operands are equal
        bool operator==(const ULint& right) const;

        // Checks if both operands are not equal
        bool operator!=(const ULint& right) const
        {
            return(!((*this) == right));
        };

        // Checks if first operand is greater than the second operand
        bool operator>(const ULint& right) const;
        
        // Checks if first operand is greater than or equal to the second operand
        bool operator>=(const ULint& right) const
        {
            return(((*this) > right) || ((*this) == right));
        };

        // Checks if first operand is lesser than the second operand
        bool operator<(const ULint& right) const
        {
            return(right > (*this));
        };

        // Checks if first operand is lesser than or equal to the second operand
        bool operator<=(const ULint& right) const
        {
            return(right >= (*this));
        };

        // Inserts into the output stream ULint value as a string, formated
        // according to the preset stream base flag (oct, dec or hex).
        friend std::ostream& operator << (std::ostream&, const ULint&);

    protected:
        // A ULint bit iterator
        class Iterator
        {
            friend class ULint;
        public:
        protected:
            // Construction only by assigning ULint object and iterator's starting position
            explicit Iterator(ULint& ulint, size_t atBit = 0)
                :mULint(ulint), mAtByte(size_t(atBit / 8)), mAtBit(Byte(atBit % 8)) {};


            // increment operators: move the iterator to the next bit
            Iterator& operator++();
            Iterator& operator++(int) { return ++(*this); };

            // decrement operators: move the iterator to the previous bit
            Iterator& operator--();
            Iterator& operator--(int) { return --(*this); };

            // Checks if both iterators are equal
            bool operator==(const Iterator& other) const;

            // Checks if both iterators are not equal
            bool operator!=(const Iterator& other) const
            {
                return !(*this == other);
            };

            // Returns a reference to the current byte
            Byte& at() { return mULint[mAtByte]; };

            // Returns the value of the current bit
            bool operator*() const;

            // Returns the distance between this and right iterator 
            size_t operator-(const Iterator& right) const;

            // Returns the position of the current byte
            size_t  atByte() const { return mAtByte; };

            // Returns the position of the current bit within current byte
            Byte atBit() const { return mAtBit; };

            // Bitwise exclusive/inclusive OR assignment of current bit and the right value
            ULint::Iterator& operator^=(bool value);

        private:
            ULint& mULint;      // Reference to the ULint object
            size_t  mAtByte;    // A position of the current byte
            Byte    mAtBit;     // A position of the current bit within the current byte
        };
        friend class Iterator;

        class Range
        {
            friend class ULint;
        public:
            // Construction only by assigning ULint object with start and end bit position
            explicit Range(ULint& ulint, size_t startBit, size_t endBit)
                :mULint(ulint), mStart(ulint, startBit), mEnd(ulint, endBit) {}

            // increment operators: move the range start and end to the next bits
            Range& operator++();
            Range& operator++(int) { return ++(*this); };

            // decrement operators: move the range start and end to the previous bits
            Range& operator--();
            Range& operator--(int) { return --(*this); };

            // Extends range to the left for the specified number of bits
            // by default it will increment end for 1 position
            Range& left(size_t noBits = 1);

            // Extends range to the right for the specified number of bits
            // by default it will decrement start for 1 position
            Range& right(size_t noBits = 1);

            // Retuns ULint object corresponding to the bits within the rage
            ULint operator*() const;

            // First adds the current value of the range on left to the value on the right range and then assigns the result to the range on the left 
            Range& operator+=(const Range& right);

            // First subtracts the current value of the range on left to the value on the right range and then assigns the result to the range on the left 
            Range& operator-=(const Range& right);

            // Checks if both ranges are equal
            bool operator==(const Range& right) const;

            // Checks if both ranges are not equal
            bool operator!=(const Range& right) const
            {
                return !(*this == right);
            };

            // Checks if first range is greater than the second range
            bool operator>(const Range& right) const;

            // Checks if first range is greater than or equal to the second range
            bool operator>=(const Range& right) const
            {
                return(((*this) > right) || ((*this) == right));
            };

            // Checks if first range is lesser than the second range
            bool operator<(const Range& right) const
            {
                return(right > (*this));
            };

            // Checks if first range is lesser than or equal to the second range
            bool operator<=(const Range& right) const
            {
                return(right >= (*this));
            };

            // Returns reference to the start iterator
            Iterator& start() { return mStart; };
            const Iterator& start() const { return mStart; };

            // Returns reference to the end iterator
            Iterator& end() { return mEnd; };
            const Iterator& end() const { return mEnd; };

            // Returns the number of bits within the range
            size_t noBits() const { return mEnd - mStart; };

        protected:
        private:
            ULint& mULint;      // A reference to the ULint object
            Iterator mStart;    // The range start position
            Iterator mEnd;      // The range end position
        };
        friend class Range;

    private:
        // initalized ULint object with required number of zero value bytes 
        void initBytes(size_t noBytes);

        // initalized ULint object with value 
        void initValue(unsigned long long value);

        // Trim's all leading 0 bytes
        void trim();

        vector<Byte> mValue;   // array of ULint bytes
    };

    // Inserts into the output stream ULint value as a string, formated
    // according to the preset stream base flag (oct, dec or hex).
    std::ostream& operator << (std::ostream& output, const ULint& value);
};

#endif /* Lint_h */
