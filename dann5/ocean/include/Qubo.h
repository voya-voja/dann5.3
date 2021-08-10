#pragma once
#include <pybind11/pybind11.h>

#include <utility>
#include <Eigen/Dense>
#include <map>
#include <vector>

using namespace Eigen;
using namespace std;

namespace dann5{
	namespace ocean {
		// Qubo is a dictionary (map) of a solution graph for a binary objective
		// function where a node pair is a key (Qkey) mapped into its bias, which
		// corespondes to Quantum energy values for that node 
		// Qubo corresponds to DWave's dimod.BinaryQuadraticModel class, represented 
		// as an upper-diagonal matrix Q, where diagonal terms are the linear
		// coefficients (key pairs with the same Quantum node name) and the nonzero
		// off-diagonal terms the quadratic coefficients(graph branches, with
		// different mode names
		typedef std::pair<string, string> Qkey;
		typedef std::map<Qkey, double> Qubo;

		// Addition function returns a reference of an updated left operand with added
		// elements of right Qubo object. The left Qubo object is updated/extended by
		// adding biases for elements that exist in both left and right Qubo obects.
		// The elements that exist only in right Qubo object are concatenated to the 
		// elements in the left object
		Qubo& operator+=(Qubo& left, const Qubo& right);

		// Sends a representation of Qubo into output stream
		std::ostream& operator << (std::ostream&, const Qubo&);


		// Quantum value is in S-uperposition state for any value except 0 and 1 
		typedef unsigned char Qvalue;

		// Quantum default superposition value, 
		// though any Q bit value except 0 and 1 will be considered superposition
		static const Qvalue cSuperposition = 'S'; // value 83

		// an array of Quantum values
		typedef vector<Qvalue> Qvalues;

		// An abstraction of predefined Qubo tables 
		class QuboTable
		{
		public:
			// a shared pointer to a specific Qubo table
			typedef shared_ptr<QuboTable> Sp;

			struct IoPort {
				string name;
				Qvalue value;

				IoPort() : name(""), value(cSuperposition) {};
				IoPort(string n, Qvalue v) : name(n), value(v) {};
			};

			typedef vector<IoPort> IoPorts;

			// Defines a Qubo table size
			struct Size {
				unsigned nRows;
				unsigned nCols;

				Size(unsigned n) : nRows(n), nCols(n) {};
			};

			// An insertion operator (<<) to be used in combination with list operator (,)
			// as per example, where qt is particualar implementation of QuboTable and "a"
			// and "r" represent its Qubo nodes:
			//		qt << "a", "r"; 
			typedef Eigen::CommaInitializer<
						Eigen::Matrix<std::basic_string<char, std::char_traits<char>, 
						std::allocator<char> >, -1, 1, 0, -1, 1> > VariableComma;
			VariableComma operator << (const string& variable);

			// An insertion operator (<<) to be used in combination with list operator (,)
			// as per example, where qt is particualar implementation of QuboTable with 
			// 2 nodes:
			//		qt <<  1,  -2,
			//			   0,   1;
			typedef Eigen::CommaInitializer<Eigen::Matrix<double, -1, -1, 0, -1, -1> > ElementComma;
			ElementComma operator << (const double& element);

			// returns default qubo object based on this QuboTable rule using default argument names
			Qubo qubo() const;

			// Expects a list of argument names to be provided in order specified by particual
			// implementation, which is controled by overriding format(...) method.
			// returns qubo object based on this Qobo rule using provided argument list
			// if finalized, it will return qubo with substitue deterministic ports based 
			// on their value, i.e. for port value 0, itselements will be removes, and for
			// port with value 1, its quadratic branches will become leaner elements, e.g.
			// element pair (a/S/, b/1/) is substituted by (a/S/, a/S/) with the same energy 
			Qubo qubo(const IoPorts& ports, bool finalized = true) const;

			~QuboTable();

			// Sends a representation of QuboTable into output stream
			friend std::ostream& operator << (std::ostream&, const QuboTable&);

		protected:

			// The Qubo table column (row) labels are 1 dimensional vector
			typedef Matrix<string, Dynamic, 1> Labels;

			// A Qubo table can be implementated by specific specialization defining
			// a generic Qubo solution graph
			QuboTable(Size);

		private:

			Labels			mVariables;	// Qubo table defualt node names
			MatrixXd		mElements;	// Qubo Table value matrix
		};

		// Operator specialization of QuboTable has one input and one output port
		class OperatorQT : public QuboTable
		{
		private:
		protected:
			OperatorQT();

		public:
		};


		// Binary operation specialization of QuboTable has two input and one
		// output port
		class BinaryOpQT : public QuboTable
		{
		private:
		protected:
			BinaryOpQT();

		public:
		};


		// Binary operation specialization of QuboTable has two input and two
		// output ports
		class BinaryOp2OutQT : public QuboTable
		{
		private:
		protected:
			BinaryOp2OutQT();

		public:
		};


		// Equal operator specialization of Operator QuboTable defined by equal 
		// mark and name
		class EqQT : public OperatorQT
		{
		private:
		protected:
		public:
			static const string cMark;	// "=="
			static const string cName;	// "equal"

			EqQT();
		};

		// Not equal operator specialization of Operator QuboTable defined by 
		// not-equal mark (sign) and name
		class NeqQT : public OperatorQT
		{
		private:
		protected:
		public:
			static const string cMark;	// "!="
			static const string cName;	// "notequal"

			NeqQT();
		};

		// Less than operator specialization of Operator QuboTable defined by 
		// less-than mark (sign) and name
		class LtQT : public OperatorQT
		{
		private:
		protected:
		public:
			static const string cMark;	// "<"
			static const string cName;	// "lessthan"

			LtQT();
		};


		// Less eqal operator specialization of Operator QuboTable defined by 
		// less-eqal mark (sign) and name
		class LeQT : public OperatorQT
		{
		private:
		protected:
		public:
			static const string cMark;	// "<="
			static const string cName;	// "lessequal"

			LeQT();
		};


		// Greater-than operator specialization of Operator QuboTable defined by 
		// reater-than mark (sign) and name
		class GtQT : public OperatorQT
		{
		private:
		protected:
		public:
			static const string cMark;	// ">"
			static const string cName;	// "greaterthan"

			GtQT();
		};


		// Greater-eqal operator specialization of Operator QuboTable defined by 
		// greater-eqal mark (sign) and name
		class GeQT : public OperatorQT
		{
		private:
		protected:
		public:
			static const string cMark;	// ">="
			static const string cName;	// "greaterequal"

			GeQT();
		};


		// And specialization of Binary Operation QuboTable defined by 
		// bitwise-and mark (sign) and name
		class AndQT : public BinaryOpQT
		{
		private:
		protected:
		public:
			static const string cMark;	// "&"
			static const string cName;	// "and"

			AndQT();
		};


		// Not and specialization of Binary Operation QuboTable defined by 
		// bitwise-inverted-and mark (sign) and name
		class NandQT : public BinaryOp2OutQT
		{
		private:
		protected:
		public:
			static const string cMark;	// "~&"
			static const string cName;	// "nand"

			NandQT();
		};


		// Or specialization of Binary Operation QuboTable defined by 
		// bitwise-or mark (sign) and name
		class OrQT : public BinaryOpQT
		{
		private:
		protected:
		public:
			static const string cMark;	// "|"
			static const string cName;	// "or"

			OrQT();
		};


		// Not or specialization of Binary Operation QuboTable defined by 
		// bitwise-inverted-or mark (sign) and name
		class NorQT : public BinaryOp2OutQT
		{
		private:
		protected:
		public:
			static const string cMark;	// "~|"
			static const string cName;	// "nor"

			NorQT();
		};

		// Not-left-or-right specialization of Binary Operation with 2 outputs 
		// QuboTable defined by a mark (sign) and name
		class NotLeftOrRightQT : public BinaryOp2OutQT
		{
		private:
		protected:
		public:
			static const string cMark;	// "~l|r"
			static const string cName;	// "notleftorright"

			NotLeftOrRightQT();
		};

		// DWave's not-left-orright specialization of Binary Operation with 2 outputs 
		// QuboTable has different values with than NotLeftOrRightQT, but resulting 
		// Qubo has same solutions
		class DwNotLeftOrRightQT : public BinaryOp2OutQT
		{
		private:
		protected:
		public:
			static const string cMark;	// "d~l|r"
			static const string cName;	// "dwnotleftorright"

			DwNotLeftOrRightQT();
		};


		// Xor specialization of Binary Operation with 2 outputs QuboTable
		// defined by bitwise-xor mark (sign) and name
		class XorQT : public BinaryOp2OutQT
		{
		private:
		protected:
		public:
			static const string cMark;	// "^"
			static const string cName;	// "xor"

			XorQT();
		};

		// Not xor specialization of Binary Operation QuboTable defined by 
		// bitwise-inverted-xor mark (sign) and name
		class NxorQT : public BinaryOp2OutQT
		{
		private:
		protected:
		public:
			static const string cMark;	// "~^"
			static const string cName;	// "nxor"

			NxorQT();
		};

		// Half adder QuboTable is same sa Xor
		class  Adder05QT : public XorQT
		{
		private:
		protected:
		public:
			static const string cMark;	// ""h+"
			static const string cName;	// "halfadder"

			Adder05QT() {};
		};

		// Adder is trinary operation specialization of QuboTable with two outputs 
		// defined arithmetic-add mark (sign) and name
		class AdderQT : public QuboTable
		{
		private:
		protected:
			Labels format(const Labels&) const;
		public:
			static const string cMark;	// "+"
			static const string cName;	// "adder"

			AdderQT();

			// A carry is an output defined for this adder
			class Carry
			{
			public:
				static const string cMark;	// "#"
				static const string cName;	// "carry"

				// Return converted given string to Q carry definition, e.g. add-tag 
				// 'a0 ^ b0' is converted to '#[a0 ^ b0]'
				static string Symbol(const string& addTag);
			};
		};
	};
};