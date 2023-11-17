#pragma once

#include <pybind11/pybind11.h>

#include <map>
#include <utility>

#include <Qdef.h>
#include <Qcompiler.h>
#include <Qevaluation.h>

using namespace std;

namespace dann5 {

    // An apstraction of a Quantum operation is a Q definition
    class Qop
    {
    public:
        static string Id(const string& outType);

        // Qop's shared pointer
        typedef shared_ptr<Qop> Sp;

        // An Q operation has number of inputs and an output
        Qop(size_t noArguments);

        // Copy constructor
        Qop(const Qop&);

        // Destruct Q operation with shared pointers to its inputs
        ~Qop();

        // override to return true, if the operation needs to be treated just as a definition
        virtual bool asDefinition() { return false; };

        // The Qop's number of inputs
        size_t noInputs() const { return mNoInputs; }

        // Return Q operation inputs
        const Qdefs& inputs() const { return mInputs; };

        // Set Q operation inputs
        // throw invalid_argument exception when # of inputs in the list is not
        // the same as value defined by noInputs() const
        virtual void inputs(const Qdefs&);

        // Remove all the Qdefs from the list of inputs
        void releaseArguments();

        // add Q operation input to the list of the inputs
        // throw invalid_argument exception preventing a new argument to be added to
        // the list once the # of inputs is equal to the # defined by
        // noInputs() method
        virtual void append(Qdef::Sp input);

        string outId() const { return "_" + identifier() + Qop::Id(identifier()); };

        virtual Qdef::Sp output(size_t forBit = cAllBits) const;

        // override output setter to sync its behaviour to specific Q operation
        // when forBit is not specified, sets a new output and if it is Qnary
        // resizes output to be same as operations noqbs
        // when forBit is specified, updates that bit cell in the output,
        // if output is Qnary and pOut is Qcell
        virtual void output(const Qdef::Sp& pOut, size_t forBit = cAllBits);

        // Return Q operation outputs, by default has one ouput
        virtual Qdefs outputs() const { return Qdefs({ Qop::output() }); };

        // Returns the largest number of Q bits of all its inputs
        // override if derived class is not Qdef object
        virtual string identifier() const noexcept {
            return dynamic_cast<const Qdef*>(this)->id();
        };

        // Returns the largest number of Q bits of all its inputs
        virtual std::size_t noqbs() const noexcept;

        // convert operation expration into a string
        virtual string toString(bool decomposed = false, size_t forBit = cAllBits) const;

        // Compiles this quantum operation to generate quantum solver code
        virtual void compile(Qcompiler& compiler) const;

        // Override to set solution values from the list of evaluations for this operation
        virtual void add(const Qevaluations& evaluations);

        // Override to return a string representation of a solution value for a sample id
        virtual string solution(size_t sampleId) const;

        // Reset the quantum operation into its initial state without solutions by clear
        // all solution samples
        virtual void reset();

    protected:
        // The Qop's number of inputs
        void noInputs(size_t nIns) { mNoInputs = nIns; }

        // Return the list of inputs
        Qdefs& inputs() { return mInputs; };

        void setOutput(const Qdef::Sp& pOut) { mpOutput = pOut; };

    private:
        static map<string, unsigned long long> gOutputIndices;

        Qdefs		mInputs;
        size_t		mNoInputs;
        Qdef::Sp	mpOutput;
    };
};
