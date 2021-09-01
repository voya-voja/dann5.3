#pragma once

#include <pybind11/pybind11.h>

#include <utility>
#include <Qdef.h>
#include <Qubo.h>
#include <Qsolver.h>

using namespace std;

namespace dann5 {
	namespace ocean {

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

			// Return true, if this Qop object is identical to the object pointed by pRight
			bool isIdentical(const Qop::Sp& pRight) { return(this == pRight.get()); }

			// The Qop's number of inputs
			size_t noInputs() const { return mNoInputs; }

			// Return Q operation inputs
			const Qdefs& inputs() const { return mInputs; };

			// Set Q operation inputs
			// throw invalid_argument exception when # of inputs in the list is not
			// the same as value defined by noInputs() const
			virtual void inputs(const Qdefs&);

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

			// return Qubo presentation of this Q operation
			virtual Qubo qubo(bool finalized = true, size_t forBit = cAllBits) const;

			// Override to set solution values from the sample set for this deffinition
			virtual void add(const Qsolver::Samples& samples);

			// Override to return a string representation of a solution value for a sample id
			virtual string solution(size_t sampleId) const;

			// Clear all solution samples
			virtual void clearSolutions();

		protected:
			// Remove all the Qdefs from the list of inputs
			void releaseArguments();

			// The Qop's number of inputs
			void noInputs(size_t nIns) { mNoInputs = nIns; }

			// Return the list of inputs
			Qdefs& inputs() { return mInputs; };

		private:
			static map<string, unsigned long long> gOutputIndices;

			Qdefs		mInputs;
			size_t		mNoInputs;
			Qdef::Sp	mpOutput;
		};
	};
};