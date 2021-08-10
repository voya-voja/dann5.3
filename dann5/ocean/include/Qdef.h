#pragma once

#include <pybind11/pybind11.h>

#include <utility>
#include <Eigen/Dense>

#include <Qubo.h>
#include <Qsolver.h>

using namespace std;
using namespace Eigen;

namespace dann5 {
	namespace ocean {

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

			// return string presentation of this Qoperand
			virtual string toString(bool decomposed = false, 
									size_t forBit = cAllBits) const
			{ 
				return mID; 
			};

			// Override to set solution values from the sample set for this deffinition
			virtual void solutions(const Qsolver::Samples& samples) = 0;

			// Override to return a string representation of a solution value for a sample id
			virtual string solution(size_t sampleId) const = 0;

		protected:
		private:
			string mID;	// The identity of a Q definition
		};

		// An std::vector containing a list of shared pointers pointing to Q definitions
		typedef vector <Qdef::Sp> Qdefs;

		// Quantum Eigen vector of Q cell references
		typedef Matrix<Qdef::Sp, Dynamic, 1> QdefVector;

		// Quantum Eigen matrix of Q cell references
		typedef Matrix<Qdef::Sp, Dynamic, Dynamic> QdefMatrix;
	};
};