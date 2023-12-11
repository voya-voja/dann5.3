#include <Qubo.h>
#include <QcellOps.h>

using namespace dann5;
using namespace dann5::ocean;

/**** Qubo ****/
std::ostream& dann5::ocean::operator<< (std::ostream& stream, const Qubo& right)
{
	for (auto at = right.cbegin(); at != right.cend(); at++)
		stream << "(" << at->first.first << ", " << at->first.second << "): " << at->second << "; ";
	return stream;
}

Qubo& dann5::ocean::operator+=(Qubo& left, const Qubo& right)
{
	for (auto at = right.cbegin(); at != right.cend(); at++)
	{
		Qkey key = (*at).first;
		Qubo::iterator item = left.find(key);
		if (item != left.end())
			(*item).second += (*at).second;
		else
		{
			Qkey mirrorKey(key.second, key.first);
			item = left.find(mirrorKey);
			if (item != left.end())
				(*item).second += (*at).second;
			else
				left[key] = (*at).second;
		}
	}
	return left;
}

Qubo dann5::ocean::operator+(const Qubo& left, const Qubo& right)
{
	Qubo result(left);
	result += right;
	return result;
}

Qubo& dann5::ocean::operator+=(Qubo& left, double scalar)
{
	for (auto at = left.begin(); at != left.end(); at++)
	{
		(*at).second += scalar;
	}
	return left;
}

Qubo dann5::ocean::operator+(double scalar, const Qubo& right)
{
	Qubo result(right);
	result += scalar;
	return result;
}

Qubo dann5::ocean::operator+(const Qubo& left, double scalar)
{
	Qubo result(left);
	result += scalar;
	return result;
}

Qubo& dann5::ocean::operator-=(Qubo& left, double scalar)
{
	for (auto at = left.begin(); at != left.end(); at++)
	{
		(*at).second -= scalar;
	}
	return left;
}

Qubo dann5::ocean::operator-(const Qubo& left, double scalar)
{
	Qubo result(left);
	result -= scalar;
	return result;
}

Qubo& dann5::ocean::operator*=(Qubo& left, double scalar)
{
	for (auto at = left.begin(); at != left.end(); at++)
	{
		(*at).second *= scalar;
	}
	return left;
}

Qubo dann5::ocean::operator*(double scalar, const Qubo& right)
{
	Qubo result(right);
	result *= scalar;
	return result;
}

Qubo dann5::ocean::operator*(const Qubo& left, double scalar)
{
	Qubo result(left);
	result *= scalar;
	return result;
}

Qubo& dann5::ocean::operator/=(Qubo& left, double scalar)
{
	for (auto at = left.begin(); at != left.end(); at++)
	{
		(*at).second /= scalar;
	}
	return left;
}

Qubo dann5::ocean::operator/(const Qubo& left, double scalar)
{
	Qubo result(left);
	result /= scalar;
	return result;
}

/**** Qubos ****/

Qubos& dann5::ocean::operator+=(Qubos& left, const Qubos& right)
{
    left.insert( left.end(), right.begin(), right.end() );
    return left;
}

Qubos dann5::ocean::operator+(const Qubos& left, const Qubos& right)
{
    Qubos result(left);
    result += right;
    return result;
}

/**** Qubo Table ****/

QuboTable::QuboTable(Size size)
	:mVariables(size.nCols), mElements(size.nRows, size.nCols)
{
}

QuboTable::~QuboTable() 
{
}

inline QuboTable::VariableComma QuboTable::operator << (const string& variable)
{
	return mVariables << variable;
}

inline QuboTable::ElementComma QuboTable::operator << (const double& element)
{
	return mElements << element;
}

Qubo QuboTable::qubo() const
{
	int c, r;
	Qkey key;
	Qubo dict;
	for (r = 0; r < mElements.rows(); r++)
	{
		key.first = mVariables(r);
		for (c = 0; c < mElements.cols(); c++)
		{
			key.second = mVariables(c);
			double element = mElements(r, c);
			if (element != 0 || key.first == key.second)
				dict[key] = element;
		}
	}
	return dict;
}

Qubo QuboTable::qubo(const IoPorts& ports, bool finalized) const
{
	Index cSize = mElements.cols(), rSize = mElements.rows();
	Qkey key;
	Qubo dict;
	// generate full qubo if not finalized
	for (Index r = 0; r < rSize; r++)
	{	// if finalized and 1st port value is 0, skip element
		if (!finalized || ports[r].value != 0)
		{
			key.first = ports[r].name;
			for (Index c = 0; c < cSize; c++)
			{	// if finalized and 1st port value is 1, use name of 2nd
				if (finalized && ports[r].value == 1)
				{
					if (ports[c].value == 1) continue;
					key.first = ports[c].name;
				}
				// if finalized and 2nd port value is 0, skip element
				if (!finalized || ports[c].value != 0)
				{	// if finalized and 2nd port value is 1, use name of 1st
					if (finalized && ports[c].value == 1)
						key.second = ports[r].name;
					else
						key.second = ports[c].name;

					// add 0 energy element only if linear element, i.e. 
					// 1st and 2nd name are the same
					double energy = mElements(r, c);
					if (energy != 0 || key.first == key.second)
						dict[key] += energy;
				}
			}
		}
	}
	return dict;
}

std::ostream& dann5::ocean::operator<< (std::ostream& stream, const QuboTable& right)
{
	stream << "  " << right.mVariables.transpose() << std::endl;
	stream << right.mElements;
	return(stream);
}

/**** Operator Qubo Table ****/
OperatorQT::OperatorQT()
	:QuboTable(QuboTable::Size(2))
{
}

/**** Binary operation Qubo Table ****/
BinaryOpQT::BinaryOpQT()
	:QuboTable(QuboTable::Size(3))
{
}

/**** Binary operation Qubo Table with two outputs ****/
BinaryOp2OutQT::BinaryOp2OutQT()
	:QuboTable(QuboTable::Size(4))
{
}

/**** Invertoperator Qubo Table ****/
InvertQT::InvertQT()
    :OperatorQT()
{
    *this << "i", "o";
    *this << -1,   2,
              0,  -1;
}

/**** Equal operator Qubo Table ****/
EqQT::EqQT()
	:OperatorQT()
{
	*this << "i", "o";
	*this <<  1,  -2,
			  0,   1;
}

/**** Not equal operator Qubo Table ****/
NeqQT::NeqQT()
	:OperatorQT()
{
	*this << "i", "o";
	*this << -1,   2, 
			  0,  -1;
}

/**** Greater-than operator Qubo Table ****/
GtQT::GtQT()
	:OperatorQT()
{
	*this << "i", "o";
//	*this <<  2,   4,
//			  0,  -6;
	*this << 0.5,  1,
			 0,   -1.5;
}

/**** Greater-than-or-equal operator Qubo Table ****/
GeQT::GeQT()
	:OperatorQT()
{
	*this << "i", "o";
//	*this <<  4,  -4,
	* this << 1,  -1,
			  0,   0;
}

/**** Less-than operator Qubo Table ****/
LtQT::LtQT()
	:OperatorQT()
{
	*this << "i", "o";
	*this << -6,   4,
			  0,   2;
}

/**** Less-than-or-equal operator Qubo Table ****/
LeQT::LeQT()
	:OperatorQT()
{
	*this << "i", "o";
	*this <<  0,  -4,
			  0,   4;
}

/**** And binary operation Qubo Table ****/
AndQT::AndQT()
	:BinaryOpQT()
{
	*this << "i0", "i1", "o";
	*this <<   0,    1,  -2,
			   0,    0,  -2,
			   0,    0,   3;
}

/**** Nand binary operation Qubo Table with two outputs****/
NandQT::NandQT()
	:BinaryOp2OutQT()
{
	// assuming x = 1 - ab
	*this << "i0", "i1", "o0", "o1";
	*this <<   0,    5,    0,   -7,
			   0,    0,    0,   -8,
			   0,    0,   -5,   10,
			   0,    0,    0,    5;
}

/**** Or binary operation Qubo Table ****/
OrQT::OrQT()
	:BinaryOpQT()
{
	*this << "i0", "i1", "o";
	*this <<   1,    1,  -2,
			   0,    1,  -2,
			   0,    0,   1;
}

/**** Nand binary operation Qubo Table with two outputs****/
NorQT::NorQT()
	:BinaryOp2OutQT()
{
	// assuming x = 1 - ab
	*this << "i0", "i1", "o0", "o1";
	*this <<  -1,    2,    2,   -2,
			   0,   -1,    2,   -2,
			   0,    0,   -1,   -1,
			   0,    0,    0,    3;
}

/**** Not-left-or-right binary operation Qubo Table with two outputs****/
NotLeftOrRightQT::NotLeftOrRightQT()
	:BinaryOp2OutQT()
{
	*this << "i0", "i1", "o0", "o1";
	*this <<  -1,    1,    2,   -3,
			   0,    0,    0,   -2,
			   0,    0,   -1,   -2,
			   0,    0,    0,    5;
}

/**** DWave not-left-or-right binary operation Qubo Table with two outputs****/
DwNotLeftOrRightQT::DwNotLeftOrRightQT()
	:BinaryOp2OutQT()
{
	*this << "i0", "i1", "o0", "o1";
	*this <<  -1,    4,    2,   -6,
			   0,    0,    0,   -6,
			   0,    0,   -1,   -2,
			   0,    0,    0,    9;
}

/**** Xor binary operation Qubo Table with two outputs****/
XorQT::XorQT()
	:BinaryOp2OutQT()
{
	*this << "i0", "i1", "o0", "o1";
	*this <<   1,    2,   -2,   -4,
			   0,    1,   -2,   -4,
			   0,    0,    1,    4,
			   0,    0,    0,    4; 
}

/**** Nxor binary operation Qubo Table with two outputs****/
NxorQT::NxorQT()
	:BinaryOp2OutQT()
{
	*this << "i0", "i1", "o0", "o1";
/*	*this <<  -1,    2,   -4,    2,
			   0,   -1,   -4,    2,
			   0,    0,    8,   -4,
			   0,    0,    0,   -1;
*/
	*this <<  -1,    2,    2,   -4,
			   0,   -1,    2,   -4,
			   0,    0,   -1,   -4,
			   0,    0,    0,    8;

}

/**** Adder trinary operation Qubo Table with two outputs****/
AdderQT::AdderQT()
	:QuboTable(QuboTable::Size(5))
{
	*this << "i0", "i1", "i3", "o0", "o1";
	*this <<   1,    2,    2,   -2,   -4,
			   0,    1,    2,   -2,   -4,
			   0,    0,    1,   -2,   -4,
			   0,    0,    0,    1,    4,
			   0,    0,    0,    0,    4;
}

QuboTable::Labels AdderQT::format(const QuboTable::Labels& args) const
{
	if (args.rows() == 5) return args;

	QuboTable::Labels fAdderArgs(5);
	if (args.rows() == 4)
		fAdderArgs << args(0), args(1), args(2), args(3),
                        Qaddition::Carry::Symbol(args(3));
	else
		fAdderArgs << args(0), args(1),
                        Qaddition::Carry::Symbol(args(0)), args(2),
                        Qaddition::Carry::Symbol(args(2));

	return(fAdderArgs);
}
