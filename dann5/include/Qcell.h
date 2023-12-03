#pragma once

#include <pybind11/pybind11.h>

#include <utility>
#include <Eigen/Dense>

#include <Qdef.h>
#include <Qvalue.h>

using namespace std;
using namespace Eigen;

namespace dann5 {
    // Quantum cell is an abstration of variables, operations and constructs,
    // which can have values 0, 1 and 'S'-uperposition, value 83
    class Qcell : public Qdef
    {
    public:
        // Q cell shared pointer
        typedef shared_ptr<Qcell> Sp;

        // Default constructor initializes Q bit with an empty id and supperposition
        // value
        Qcell(const string& id) : Qdef(id) {};

        // copy constructor
        Qcell(const Qcell& right) : Qdef(right) {};

        // Returns the number of Q bits that the Q binary holds
        virtual std::size_t noqbs() const noexcept { return 1; };

        // return string presentation of this Qcell
        virtual string toString(bool decomposed = false,
            size_t forBit = cAllBits) const;

        // type conversion operator
        operator const Qvalue() const { return value(); };
        operator const Qvalue() { return value(); };

        // set and get the value of this Q cell
        virtual void value(Qvalue v) = 0;
        virtual Qvalue value() const = 0;
        virtual Qvalue value() = 0;

        // Override to set solution values from the sample set for this deffinition
        virtual void add(const Qevaluations& evaluations);

        // Override to return a string representation of a solution value for
        // 'atEvltn'
        virtual string solution(size_t atEvltn) const;

        // Reset the quantum cell into its initial state without solutions by clear
        // all solution samples
        virtual void reset();

        // returns a reference of solutions, known for this cell
        Qvalues& solutions() { return mSolutions; };

        // Returns a solution value for evaluation 'atEvltn'
        Qvalue solutionValue(size_t atEvltn) const;

    protected:
    private:
        Qvalues	mSolutions;
    };

    // An std::vector containing a list of shared pointers pointing to
    // Q-cell definitions
    class Qcells : public vector<Qcell::Sp>
    {
    public:
        Qcells() {};
        Qcells(size_t size) :vector<Qcell::Sp>(size) {};
        Qcells(const Qdefs& list){
            for(auto pDef: list)
                push_back(dynamic_pointer_cast<Qcell>(pDef));
        };
    };

    // Quantum Eigen vector of Q cell references
    typedef Matrix<Qcell::Sp, Dynamic, 1> QcellVector;

    // Quantum Eigen matrix of Q cell references
    typedef Matrix<Qcell::Sp, Dynamic, Dynamic> QcellMatrix;
};
