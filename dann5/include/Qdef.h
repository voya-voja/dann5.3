#pragma once

#include <pybind11/pybind11.h>

#include <utility>
#include <Eigen/Dense>

#include <Qevaluation.h>

using namespace std;
using namespace Eigen;

namespace dann5 {
    // A constant indicating all Q bits should be processed
    static const size_t cAllBits = ULLONG_MAX;

    // A supertype for all definitions in dann5 Quantum library
    class Qdef
    {
    public:
        // Q definition shared pointer
        typedef shared_ptr<Qdef> Sp;

        // Instantiate a Q definition with identity
        Qdef(const string& id) : mID(id) {
        };

        // Copy constructor
        Qdef(const Qdef& right) : mID(right.mID) {
        };

        // destruct the Q definition
        ~Qdef() {
        };

        // Return the Q definition identity string
        // override to provide identity at the point of time
        virtual string id() const { return mID; };

        // Set a new Q definition identity
        virtual void id(const string& id) { mID = id; };

        // Return a Qdef's shared pointer pointing to a copy of this object
        virtual Qdef::Sp clone() const = 0;

        // Returns the number of Q bits that the Q definition holds
        virtual size_t noqbs() const noexcept = 0;

        // return string presentation of this quantum definition
        virtual string toString(bool decomposed = false,
                                size_t forBit = cAllBits) const
        {
            return mID;
        };

        // Override to set solution values from the evaluation set for this
        // deffinition
        virtual void add(const Qevaluations& evaluations) = 0;

        // Override to return a string representation of a solution value for
        // an evaluation at 'atEvltn'
        virtual string solution(size_t atEvltn) const = 0;

        // Override to reset the quantum definition into its initial state
        // without solutions by clearing all evaluations
        virtual void reset() = 0;

    protected:
    private:
        string mID;	// The identity of a Q definition
    };

    // An std::vector containing a list of shared pointers pointing to
    // quantum definitions
    typedef vector <Qdef::Sp> Qdefs;

    // Quantum Eigen vector of Q cell references
    typedef Matrix<Qdef::Sp, Dynamic, 1> QdefVector;

    // Quantum Eigen matrix of Q cell references
    typedef Matrix<Qdef::Sp, Dynamic, Dynamic> QdefMatrix;
};
