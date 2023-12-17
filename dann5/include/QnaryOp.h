#pragma once

#include <pybind11/pybind11.h>

#include <Qnary.h>
#include <Qop.h>

using namespace Eigen;

namespace dann5 {
    typedef Matrix<Qop::Sp, Dynamic, 1> QopVector;
    typedef Matrix<Qop::Sp, Dynamic, Dynamic> QopMatrix;

    // An abstraction of a Quantum operation that is executed agains operands
    // with an array of Quantum cells
    class QnaryOp : public Qnary, public Qop
    {
    public:
        // QnaryOp's shared pointer
        typedef shared_ptr<QnaryOp> Sp;

        // An Q derived operation has identity and should have at least one argument
        QnaryOp(const string& id, size_t noArguments = 2);

        // Copy constructor
        QnaryOp(const QnaryOp& right) : Qnary(right), Qop(right){};

        // Destruct Q derived operation with shared pointers to its inputs
        ~QnaryOp() {};

        // Returns the higher number of Q bits that the inputs of the
        // Q operation hold
        virtual size_t noqbs() const noexcept { return Qnary::noqbs(); };

        // Resizes the Q array of oprations to contain a specified size of elements. If the size is
        // smaller than the current Q array size, the content is reduced to its
        // first size elements, removing those beyond (and destroying them). If the size
        // is greater than the current container size, the content is expanded by
        // inserting at the end as many Q null operations as needed.
        virtual void resize(size_t size, Qvalue value = 0);
        
        // Set quantum operation output and the list of inputs and calls
        // refresh() method
        // throws invalid_argument exception when # of inputs in the list is not
        // the same as value defined by noInputs() const
        virtual void operands(const Qdef::Sp& pOut, const Qdefs& ins);
        
        // Set Q operation inputs and calls refresh() method
        // throws invalid_argument exception when # of inputs in the list is not
        // the same as value defined by noInputs() const
        virtual void inputs(const Qdefs&);

        // add Q operation argument to the list of the inputs
        // throw invalid_argument exception preventing a new argument to be added to
        // the list once the # of inputs is equal to the # defined by
        // noInputs() method
        virtual void append(Qdef::Sp argument);

        virtual Qdef::Sp output(size_t forBit = cAllBits) const;
        virtual void output(const Qdef::Sp& out, size_t forBit = cAllBits);

        // Remove all the operands and clear the logic of this
        // Qnary operation
        virtual void releaseOperands();

        // convert operation expression into a string
        virtual string toString(bool decomposed = false, size_t forBit = cAllBits) const;

        // Set solution values from the evaluation set for this Qnary-operation
        virtual void add(const Qevaluations& evaluations){
            Qop::add(evaluations);
        };

        // Returns a string representation of a solution value of this
        // Qnary-operation for an evaluation at 'atEvltn'
        virtual string solution(size_t atEvltn) const{
            return Qop::solution(atEvltn);
        };

        // Resets the Qnary-operation into its initial state without solutions
        // by clear all solution samples
        virtual void reset(){
            Qop::reset();
        };

    protected:
        // Override to refresh the Q-nary operation cells according to the derived
        // operation logic
        virtual void refresh(){
            refreshOnInputs();
            refreshOnOutput();
        };

        // Override to refresh the Q-nary operation cells according to the derived
        // operation logic on inputs assignment
        virtual void refreshOnInputs() = 0;

        // Override to refresh the Q-nary operation cells according to the derived
        // operation logic on output assignment
        virtual void refreshOnOutput();
    private:
    };

    // An std::vector containing a list of shared pointers pointing to
    // Q-nary operations
    class QnaryOps : public vector <QnaryOp::Sp>
    {
    public:
        QnaryOps(const Qdefs& list){
            for(auto pDef: list)
                push_back(dynamic_pointer_cast<QnaryOp>(pDef));
        };
    };

};
