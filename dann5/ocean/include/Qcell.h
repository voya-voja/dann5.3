#pragma once

#include <pybind11/pybind11.h>

#include <utility>
#include <Eigen/Dense>

#include <Qdef.h>

using namespace std;
using namespace Eigen;

namespace dann5 {
	namespace ocean {

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

			// type conversion operator
			operator const Qvalue() const { return value(); };
			operator const Qvalue() { return value(); };

			// set and get the value of this Q cell
			virtual void value(Qvalue v) = 0;
			virtual Qvalue value() const = 0;
			virtual Qvalue value() = 0;

			// Override to return a string representation of a solution value for a sample id
			virtual string solution(size_t sampleId) const;

			Qvalue solutionValue(size_t sampleId) const;

		protected:
			Qvalues& solutionValues() { return mSolutions; };

		private:
			Qvalues	mSolutions;
		};

		// Quantum vector of Q cell references
		typedef vector<Qcell::Sp> Qcells;

		// Quantum Eigen vector of Q cell references
		typedef Matrix<Qcell::Sp, Dynamic, 1> QcellVector;

		// Quantum Eigen matrix of Q cell references
		typedef Matrix<Qcell::Sp, Dynamic, Dynamic> QcellMatrix;
	};
};