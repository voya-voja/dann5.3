#pragma once
#pragma once

#include <pybind11/pybind11.h>

#include <bitset>
#include <Eigen/Dense>

#include <Qcell.h>

using namespace Eigen;
using namespace std;

namespace dann5 {
	namespace ocean {
		// coresponds to a bitset of the same size and unsigned long long
		typedef bitset<64> Bits;

		// Quantum-nary is an abstraction of Q type, operations and constructs which 
		// are an array of Q cells.
		class Qnary : public Qdef
		{
		public:
			// Q array shared pointer 
			typedef shared_ptr<Qnary> Sp;

			// Qnary is unknown, if any of its Qcells is in superposition state
			static const Bits cUnknown;

			// Qnary unknown string representation is "(U)"
			static const string cUnknownSign;

			// Initialize 0 size Q array
			Qnary(const string& id) : Qdef(id), mCells(0) {};

			// Initialize Q array of a specified size
			Qnary(size_t size, const string& id) : Qdef(id), mCells(size) {};

			// Initialize Q array with a given Qcells
			Qnary(const string& id, const Qcells& value);

			// copy constructor
			Qnary(const Qnary& right) :Qdef(right), mCells(right.mCells) {};

			// type conversion operator to a std::vertor of Qcells
			operator Qcells() { return mCells; };
			operator const Qcells() const { mCells; };

			// type conversion operator to a Bits object
			operator Bits();
			operator const Bits() const;

			// type conversion operator to a Eigen::vector of Qbits
			operator QdefVector();
			operator const QdefVector() const;

			// by defualt returns Qcell::Sp to a created Qbit with a value and apporopriate id 
			// override to create appropriate Qcell object for a given value at givel level
			virtual Qcell::Sp create(Qvalue value, size_t atLevel) const;

			// Set a new Q definition identity
			virtual void id(const string& id);

			// Returns the number of Q bits that the Q binary holds
			virtual size_t noqbs() const noexcept { return mCells.size(); };

			// Resizes the Q array to contain a specified size of elements. If the size is
			// smaller than the current Q array size, the content is reduced to its
			// first size elements, removing those beyond (and destroying them). If the size
			// is greater than the current container size, the content is expanded by 
			// inserting at the end as many elements as needed to reach the new size.
			// If value is specified, the new elements are initialized as its copies, 
			// otherwise, they are 0 - initialized. If size is also greater than the 
			// current container capacity, an automatic reallocation of the allocated 
			// storage space takes place.
			virtual void resize(size_t size, Qvalue value = 0);

			// Return a Q nary string representation when forBit is cAllBits. 
			// Otherwise, return a string representation of a Q cell at the specified 
			// forBit
			virtual string toString(bool decomposed = false,
								size_t forBit = cAllBits) const;

			// Override to set solution values from the sample set for this deffinition
			virtual void add(const Qsolver::Samples& samples);

			// Override to return a string representation of a solution value for a sample id
			virtual string solution(size_t sampleId) const;

			// Reset the quantum array of cells into its initial state without solutions by 
			// clearing all solution samples
			virtual void reset();

			void set(const Qcell::Sp& pCell, size_t forBit) {
				mCells[forBit] = pCell;
			}

			// Accesses the Q bit at position pos. The first version returns the value 
			// of the Q bit, the second version returns a reference to a Q bit object 
			// that allows modification of the value.
			Qcell::Sp operator[](size_t pos) const noexcept;
			Qcell& operator[](size_t pos);

			// Checks if all, any or none of the bits are set to a value
			// by default the functions will check for supperposition state
			bool all(Qvalue value = cSuperposition) const;
			bool any(Qvalue value = cSuperposition) const;
			bool none(Qvalue value = cSuperposition) const;

			// Returns the number of bits that are set to a value
			// by default the functions will check for supperposition state
			size_t count(Qvalue value = cSuperposition) const;

			//Performs binary shift left and binary shift right. 
			// 1 - 2) Performs binary shift left with zeroes shifted in.
			Qnary::Sp operator<<(size_t pos) const;
			// The(2) version is performs the shift to the current object.
			Qnary& operator<<=(size_t pos);
			// 3 - 4) Performs binary shift left with zeroes shifted in.
			Qnary::Sp operator>>(size_t pos) const;
			// The(4) version is performs the shift to the current object.
			Qnary& operator>>=(size_t pos);
			// 5 - 6) Performs binary shift left with specified Q bit value shifted in.
			Qnary::Sp operator<<(Qvalue value) const;
			// The(6) version is performs the shift to the current object.
			Qnary& operator<<=(Qvalue value);
			// 7 - 8) Performs binary shift left with specified Q bit value shifted in.
			Qnary::Sp operator>>(Qvalue value) const;
			// The(8) version is performs the shift to the current object.
			Qnary& operator>>=(Qvalue value);

		protected:
			// return a reference to a value
			Qcells& cells() { return mCells; };
			const Qcells& cells() const { return mCells; };

			// set a new value
			void cells(const Qcells& value) { mCells = value; };

		private:
			class Value0cell : public Qcell
			{
			public:
				Value0cell(const string& id) : Qcell(id) {};
				Value0cell(const Value0cell& right) : Qcell(right) {};

				virtual void value(Qvalue v) {};
				virtual Qvalue value() const { return 0; };
				virtual Qvalue value() { return 0; };

				virtual void add(const Qsolver::Samples& samples) {};

				virtual Qdef::Sp clone() const { return Qdef::Sp(new Value0cell(*this)); };

				// Reset the cell into its initial state without solutions by clearing 
				// all solution samples
				virtual void reset() {};

			protected:
			private:
			};

			Qcells mCells;
		};
	};
};