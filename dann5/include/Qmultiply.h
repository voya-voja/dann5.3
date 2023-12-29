//
//  Qmultiply.h
//  d5
//
//  Created by Nebojsa Vojinovic on 2022-11-12.
//

#ifndef Qmultiply_h
#define Qmultiply_h
#pragma once

#include <pybind11/pybind11.h>

#include <utility>

#include <QnaryOp.h>

using namespace std;

namespace dann5 {
    // A quantum multiplication is a specific implementation of a Q operation
    class Qmultiply : public QnaryOp
    {
    public:
        // Qmultiply's shared pointer
        typedef shared_ptr<Qmultiply> Sp;

        // Quantum multiply operation identifiers
        static const string cMark() { return "*"; };
        static const string cName() { return "mul"; };

        // An Q multiply has identity and should have at least two argument
        Qmultiply() : QnaryOp(cMark(), 2) {};

        // Copy constructor
        Qmultiply(const Qmultiply& right) : QnaryOp(right) {};

        // Destruct the Q multiply instance
        ~Qmultiply() {};

        // Return a Qdef's shared pointer pointing to a copy of this object
        virtual Qdef::Sp clone() const { return Qdef::Sp(new Qmultiply(*this)); };

    protected:
        // Refreshes the multiplication logic when inputs are added
        virtual void refreshOnInputs();

        // Return Q bit definition matrix that is result of matrix multiplication
        // of two Q bit vectors, this and right Q expression objects
        QcellMatrix x(const Qdefs& args) const;

        // Return x Q expression by adding elements of Q bit definition matrix
        // digonal for a given forBit.
        void sumDiagonal(const QcellMatrix& matrix);

    private:
    };
};

#endif /* Qmultiply_h */