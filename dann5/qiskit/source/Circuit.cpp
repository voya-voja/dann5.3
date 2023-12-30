#include <Circuits.h>
#include <QcellOps.h>

using namespace dann5;
using namespace dann5::qiskit;

/**** Circuit ****/

Circuit::Circuit()
{
}

Circuit::~Circuit() 
{
}

Circuit Circuit::circuit(const IoPorts& ports) const
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

std::ostream& dann5::qiskit::operator<< (std::ostream& stream, const Circuit& right)
{
	stream << "  " << right.mVariables.transpose() << std::endl;
	stream << right.mElements;
	return(stream);
}

/**** Operator Circuit ****/
OperatorCircuit::OperatorCircuit()
	:Circuit(Circuit::Size(2))
{
}

/**** Binary operation Circuit ****/
BinaryOpCircuit::BinaryOpCircuit()
	:Circuit(Circuit::Size(3))
{
}

/**** Binary operation Circuit with two outputs ****/
BinaryOp2OutCircuit::BinaryOp2OutCircuit()
	:Circuit(Circuit::Size(4))
{
}

/**** Invertoperator Circuit ****/
InvertCircuit::InvertCircuit()
    :OperatorCircuit()
{
    *this << "i", "o";
    *this << -1,   2,
              0,  -1;
}

/**** Equal operator Circuit ****/
EqCircuit::EqCircuit()
	:OperatorCircuit()
{
	*this << "i", "o";
	*this <<  1,  -2,
			  0,   1;
}

/**** Not equal operator Circuit ****/
NeqCircuit::NeqCircuit()
	:OperatorCircuit()
{
	*this << "i", "o";
	*this << -1,   2, 
			  0,  -1;
}

/**** Greater-than operator Circuit ****/
GtCircuit::GtCircuit()
	:OperatorCircuit()
{
	*this << "i", "o";
//	*this <<  2,   4,
//			  0,  -6;
	*this << 0.5,  1,
			 0,   -1.5;
}

/**** Greater-than-or-equal operator Circuit ****/
GeCircuit::GeCircuit()
	:OperatorCircuit()
{
	*this << "i", "o";
//	*this <<  4,  -4,
	* this << 1,  -1,
			  0,   0;
}

/**** Less-than operator Circuit ****/
LtCircuit::LtCircuit()
	:OperatorCircuit()
{
	*this << "i", "o";
	*this << -6,   4,
			  0,   2;
}

/**** Less-than-or-equal operator Circuit ****/
LeCircuit::LeCircuit()
	:OperatorCircuit()
{
	*this << "i", "o";
	*this <<  0,  -4,
			  0,   4;
}

/**** And binary operation Circuit ****/
AndCircuit::AndCircuit()
	:BinaryOpCircuit()
{
	*this << "i0", "i1", "o";
	*this <<   0,    1,  -2,
			   0,    0,  -2,
			   0,    0,   3;
}

/**** Nand binary operation Circuit with two outputs****/
NandCircuit::NandCircuit()
	:BinaryOp2OutCircuit()
{
	// assuming x = 1 - ab
	*this << "i0", "i1", "o0", "o1";
	*this <<   0,    5,    0,   -7,
			   0,    0,    0,   -8,
			   0,    0,   -5,   10,
			   0,    0,    0,    5;
}

/**** Or binary operation Circuit ****/
OrCircuit::OrCircuit()
	:BinaryOpCircuit()
{
	*this << "i0", "i1", "o";
	*this <<   1,    1,  -2,
			   0,    1,  -2,
			   0,    0,   1;
}

/**** Nand binary operation Circuit with two outputs****/
NorCircuit::NorCircuit()
	:BinaryOp2OutCircuit()
{
	// assuming x = 1 - ab
	*this << "i0", "i1", "o0", "o1";
	*this <<  -1,    2,    2,   -2,
			   0,   -1,    2,   -2,
			   0,    0,   -1,   -1,
			   0,    0,    0,    3;
}

/**** Not-left-or-right binary operation Circuit with two outputs****/
NotLeftOrRightCircuit::NotLeftOrRightCircuit()
	:BinaryOp2OutCircuit()
{
	*this << "i0", "i1", "o0", "o1";
	*this <<  -1,    1,    2,   -3,
			   0,    0,    0,   -2,
			   0,    0,   -1,   -2,
			   0,    0,    0,    5;
}

/**** DWave not-left-or-right binary operation Circuit with two outputs****/
DwNotLeftOrRightCircuit::DwNotLeftOrRightCircuit()
	:BinaryOp2OutCircuit()
{
	*this << "i0", "i1", "o0", "o1";
	*this <<  -1,    4,    2,   -6,
			   0,    0,    0,   -6,
			   0,    0,   -1,   -2,
			   0,    0,    0,    9;
}

/**** Xor binary operation Circuit with two outputs****/
XorCircuit::XorCircuit()
	:BinaryOp2OutCircuit()
{
	*this << "i0", "i1", "o0", "o1";
	*this <<   1,    2,   -2,   -4,
			   0,    1,   -2,   -4,
			   0,    0,    1,    4,
			   0,    0,    0,    4; 
}

/**** Nxor binary operation Circuit with two outputs****/
NxorCircuit::NxorCircuit()
	:BinaryOp2OutCircuit()
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

/**** Adder trinary operation Circuit with two outputs****/
AdderCircuit::AdderCircuit()
	:Circuit(Circuit::Size(5))
{
	*this << "i0", "i1", "i3", "o0", "o1";
	*this <<   1,    2,    2,   -2,   -4,
			   0,    1,    2,   -2,   -4,
			   0,    0,    1,   -2,   -4,
			   0,    0,    0,    1,    4,
			   0,    0,    0,    0,    4;
}

Circuit::Labels AdderCircuit::format(const Circuit::Labels& args) const
{
	if (args.rows() == 5) return args;

	Circuit::Labels fAdderArgs(5);
	if (args.rows() == 4)
		fAdderArgs << args(0), args(1), args(2), args(3),
                        Qaddition::Carry::Symbol(args(3));
	else
		fAdderArgs << args(0), args(1),
                        Qaddition::Carry::Symbol(args(0)), args(2),
                        Qaddition::Carry::Symbol(args(2));

	return(fAdderArgs);
}
