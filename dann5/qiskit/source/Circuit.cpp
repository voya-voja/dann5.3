#include <Circuits.h>
#include <QcellOps.h>

using namespace dann5;
using namespace dann5::qiskit;

/**** Circuit ****/
Circuit::Sp Circuit::circuit(const IoPorts& ports) const
{
    return Circuit::Sp( new Circuit(*this) );
}

string Circuit::draw() const
{
    return "";
}

std::ostream& dann5::qiskit::operator << (std::ostream& out, const Circuit& circuit)
{
    out << circuit.draw();
    return out;
}

/**** Measure Circuit ****/
Circuit::Instruction MeasureCircuit::Measure(const IoPort& arg, const IoPort& cl)
{
    return Instruction("measure",
        { Qubit(QuantumRegister(1, arg.name()), arg.at()) },
        { Clbit(ClassicalRegister(1, cl.name()), cl.at()) });
}

MeasureCircuit::MeasureCircuit() 
    :Circuit({ MeasureCircuit::Measure() }) 
{}

string MeasureCircuit::draw() const
{
    string drawing = "\t   a_0: -M-";
    drawing += "\t         |";
    drawing += "\tcl: 1 /= +=";
    drawing += "\t         0";
    return drawing;
}

/**** Hadamard Circuit ****/
Circuit::Instruction HadamardCircuit::Hadamard(const IoPort& arg)
{
    return Instruction("h",
        { Qubit(QuantumRegister(1, arg.name()), arg.at()) });
}

HadamardCircuit::HadamardCircuit() 
    :Circuit({ HadamardCircuit::Hadamard() })
{
}

string HadamardCircuit::draw() const
{
    string drawing = "\t  a_0: - H -";
    return drawing;
}

/**** Reset Circuit ****/
Circuit::Instruction ResetCircuit::Reset(const IoPort& arg) 
{
    return Instruction("reset",
        { Qubit(QuantumRegister(1, arg.name()), arg.at()) });
}

ResetCircuit::ResetCircuit() : Circuit({ ResetCircuit::Reset() }) 
{
}

string ResetCircuit::draw() const
{
    string drawing = "\t  a_0: -|0>-";
    return drawing;
}

/**** Invert Circuit ****/
Circuit::Instruction InvertCircuit::Not(const IoPort& arg) 
{
    return Instruction("x",
        { Qubit(QuantumRegister(1, arg.name()), arg.at()) });
}

InvertCircuit::InvertCircuit() 
    : Circuit({ InvertCircuit::Not() }) 
{
}

string InvertCircuit::draw() const
{
    string drawing = "\t  a_0: - X -";
    return drawing;
}

/**** Operator Circuit ****/
Circuit::Instruction OperatorCircuit::ControlledNot(const IoPort& in, const IoPort& out) 
{
    return Instruction("x",
        { Qubit(QuantumRegister(1, in.name()), in.at()),
            Qubit(QuantumRegister(1, out.name()), out.at()) });
}

Circuit::Instruction OperatorCircuit::Swap(const IoPort& in, const IoPort& out)
{
    return Instruction("swap",
        { Qubit(QuantumRegister(1, in.name()), in.at()),
            Qubit(QuantumRegister(1, out.name()), out.at()) });
}

/**** Binary Operation Circuit ****/
Circuit::Instruction BinaryOpCircuit::Toffoli(const IoPort& in, const IoPort& out) 
{
    return Instruction("x",
        { Qubit(QuantumRegister(2, in.name()), in.at()),
            Qubit(QuantumRegister(2, in.name()), in.at() + 1),
            Qubit(QuantumRegister(1, out.name()), out.at()) });
}

Circuit::Instruction BinaryOpCircuit::Toffoli(const IoPort& in0, const IoPort& in1, const IoPort& out) 
{
    return Instruction("x",
        { Qubit(QuantumRegister(1, in0.name()), in0.at()),
            Qubit(QuantumRegister(1, in1.name()), in1.at()),
            Qubit(QuantumRegister(1, out.name()), out.at()) });
}

/**** Equal Operator Circuit ****/
EqCircuit::EqCircuit() 
    :OperatorCircuit({ OperatorCircuit::ControlledNot() }) 
{
}

string EqCircuit::draw() const
{
    string 
        drawing = "\t  i_0: --+--";
        drawing += "\t         |";
        drawing += "\t  o_0: - X -";
    return drawing;
}

/**** Not-Equal Operator Circuit ****/
NeqCircuit::NeqCircuit() 
    : OperatorCircuit({ OperatorCircuit::ControlledNot(),
                        InvertCircuit::Not(IoPort("o")) })
{
}

string NeqCircuit::draw() const
{
    string 
        drawing = "\t  i_0: --+--";
        drawing += "\t         |";
        drawing += "\t  o_0: - X -- X -";
    return drawing;
}

/**** Less-Than Operator Circuit ****/
LtCircuit::LtCircuit()
    : OperatorCircuit({ OperatorCircuit::Swap(),
                        ResetCircuit::Reset(IoPort("o")),
                        InvertCircuit::Not(IoPort("o")),
                        OperatorCircuit::ControlledNot() }) 
{
}

string LtCircuit::draw() const
{
    string 
        drawing = "\t  i_0: -X-------------+--";
        drawing += "\t       |             | ";
        drawing += "\t o_0: -X--|0>-- X -- X -";
    return drawing;
};

/**** Less-Than-and-Equal Operator Circuit ****/
LeCircuit::LeCircuit() 
    : OperatorCircuit({ OperatorCircuit::Swap(),
                        OperatorCircuit::ControlledNot() }) 
{
}

string LeCircuit::draw() const
{
    string 
        drawing = "\t  i_0: --X---+-";
        drawing += "\t        |   |";
        drawing += "\t o_0: --X-- X -";
    return drawing;
}

/**** Greater-Than Operator Circuit ****/
GtCircuit::GtCircuit() 
    : OperatorCircuit({ InvertCircuit::Not(IoPort("o")),
                        OperatorCircuit::Swap(),
                        OperatorCircuit::ControlledNot(),
                        ResetCircuit::Reset(IoPort("o")) }) 
{
}

string GtCircuit::draw() const
{
    string 
        drawing = "\t  i_0: ------X---+-";
        drawing += "\t            |   | ";
        drawing += "\t o_0: - X --X-- X -----|0>-";
    return drawing;
}

/****  Greater-Than-and-Equal Operator Circuit ****/
GeCircuit::GeCircuit() 
    : OperatorCircuit({ InvertCircuit::Not(IoPort("o")),
                        OperatorCircuit::Swap(),
                        OperatorCircuit::ControlledNot() }) 
{
}

string GeCircuit::draw() const
{
    string 
        drawing = "\t  i_0: ------X---+-";
        drawing += "\t            |   | ";
        drawing += "\t o_0: - X --X-- X -";
    return drawing;
}

/****  And Operation Circuit ****/
AndCircuit::AndCircuit() 
    : BinaryOpCircuit({ BinaryOpCircuit::Toffoli() }) 
{
}

string AndCircuit::draw() const
{
    string 
        drawing = "\t  i_0: --+-";
        drawing += "\t        | ";
        drawing += "\t i_1: --+--";
        drawing += "\t        | ";
        drawing += "\t o_0: - X -";
    return drawing;
}

/****  Not-And Operation Circuit ****/
NandCircuit::NandCircuit() 
    : BinaryOpCircuit({ InvertCircuit::Not(IoPort("o")),
                        BinaryOpCircuit::Toffoli() }) 
{
}

string NandCircuit::draw() const
{
    string 
        drawing = "\t  i_0: -------+-";
        drawing += "\t             | ";
        drawing += "\t i_1: -------+--";
        drawing += "\t             | ";
        drawing += "\t o_0: - X -- X -";
    return drawing;
}

/**** Or Operation Circuit ****/
OrCircuit::OrCircuit() 
    : BinaryOpCircuit({ BinaryOpCircuit::Toffoli(IoPort("i"), IoPort("a")),
                        OperatorCircuit::ControlledNot(IoPort("i"), IoPort("a", 1)),
                        OperatorCircuit::ControlledNot(IoPort("i", 1), IoPort("a", 1)),
                        OperatorCircuit::ControlledNot(IoPort("a"), IoPort("o")),
                        OperatorCircuit::ControlledNot(IoPort("a", 1), IoPort("o")) }) 
{
}

string OrCircuit::draw() const
{
    string 
        drawing = "\t  i_0: --+----+--------------------";
        drawing += "\t        |    |                    ";
        drawing += "\t i_1: --+---------+---------------";
        drawing += "\t        |    |    |               ";
        drawing += "\t a_0: - X ----------------+-------";
        drawing += "\t             |    |       |       ";
        drawing += "\t a_1: ------ X -- X -----------+--";
        drawing += "\t                          |    |  ";
        drawing += "\t o_0: ------------------- X -- X -";
    return drawing;
}

/****  Not-Or Operation Circuit ****/
NorCircuit::NorCircuit() 
    : BinaryOpCircuit({ BinaryOpCircuit::Toffoli(IoPort("i"), IoPort("a")),
                        OperatorCircuit::ControlledNot(IoPort("i"), IoPort("a", 1)),
                        OperatorCircuit::ControlledNot(IoPort("i", 1), IoPort("a", 1)),
                        OperatorCircuit::ControlledNot(IoPort("a")),
                        OperatorCircuit::ControlledNot(IoPort("a", 1)),
                        InvertCircuit::Not(IoPort("o")) }) 
{
}

string NorCircuit::draw() const
{
    string 
        drawing = "\t  i_0: --+----+--------------------";
        drawing += "\t        |    |                    ";
        drawing += "\t i_1: --+---------+---------------";
        drawing += "\t        |    |    |               ";
        drawing += "\t a_0: - X ----------------+-------";
        drawing += "\t             |    |       |       ";
        drawing += "\t a_1: ------ X -- X -----------+--";
        drawing += "\t                          |    |  ";
        drawing += "\t o_0: ------------------- X -- X -- X -";
    return drawing;
};

/**** Xor Operation Circuit ****/
XorCircuit::XorCircuit() 
    : BinaryOpCircuit({ OperatorCircuit::ControlledNot(),
                        OperatorCircuit::ControlledNot(IoPort("i", 1)) }) 
{
}

string XorCircuit::draw() const
{
    string 
        drawing = "\t  i_0: --+-------";
        drawing += "\t        |       ";
        drawing += "\t i_1: -------+--";
        drawing += "\t        |    |  ";
        drawing += "\t o_0: - X -- X -";
    return drawing;
}

/****  Not-Xor Operation Circuit ****/
NxorCircuit::NxorCircuit() 
    : BinaryOpCircuit({ OperatorCircuit::ControlledNot(),
                        OperatorCircuit::ControlledNot(IoPort("i", 1)),
                        InvertCircuit::Not(IoPort("o")) }) 
{
}

string NxorCircuit::draw() const
{
    string 
        drawing = "\t  i_0: --+-------";
        drawing += "\t        |       ";
        drawing += "\t i_1: -------+--";
        drawing += "\t        |    |  ";
        drawing += "\t o_0: - X -- X -- X -";
    return drawing;
}

/****  Half-Adder Operation Circuit ****/
HalfAdder::HalfAdder() 
    : BinaryOp2OutCircuit({ OperatorCircuit::ControlledNot(),
                            OperatorCircuit::ControlledNot(IoPort("i", 1)),
                            BinaryOpCircuit::Toffoli(IoPort("i"), IoPort("c")) }) 
{
}

string HalfAdder::draw() const
{
    string 
        drawing = "\t  i_0: --+---------+--";
        drawing += "\t        |         |  ";
        drawing += "\t i_1: -------+----+--";
        drawing += "\t        |    |    |  ";
        drawing += "\t o_0: - X -- X ------";
        drawing += "\t                  |  ";
        drawing += "\t c_0: ----------- X -";
    return drawing;
}

/****  Adder Operation Circuit ****/
AdderCircuit::AdderCircuit() 
    : Circuit({ // half adder { out: a0 = i0 ^ i1; carry: a[1] = i[0] & i[1] }
                OperatorCircuit::ControlledNot(IoPort("i"), IoPort("a")),
                OperatorCircuit::ControlledNot(IoPort("i", 1), IoPort("a")),
                BinaryOpCircuit::Toffoli(IoPort("i"), IoPort("a", 1)),
                // half adder { out: o0 = a0 ^ i2; carry: a[2] = a[0] & i[2] }
                OperatorCircuit::ControlledNot(IoPort("a"), IoPort("o")),
                OperatorCircuit::ControlledNot(IoPort("i", 2) , IoPort("o+", 1)),
                BinaryOpCircuit::Toffoli(IoPort("a"), IoPort("i", 2), IoPort("a", 2)),
                // c0 = a1 | a2
                BinaryOpCircuit::Toffoli(IoPort("a", 1), IoPort("a", 3)),       // AND
                OperatorCircuit::ControlledNot(IoPort("a", 1), IoPort("a", 4)),
                OperatorCircuit::ControlledNot(IoPort("a", 2), IoPort("a", 4)), // XOR
                OperatorCircuit::ControlledNot(IoPort("i", 1), IoPort("c")),
                OperatorCircuit::ControlledNot(IoPort("i", 1), IoPort("c")) })  // XOR
{
}

string AdderCircuit::draw() const
{
    string 
        drawing = "\t  i_0: - H ------+---------+-----------M-----------------------------------";
        drawing += "\t                |         |           |";
        drawing += "\t i_1: - H -----------+----+--------------M--------------------------------";
        drawing += "\t                |    |    |           |  |";
        drawing += "\t i_3: - H -----------------------------------+----+-----------M-----------";
        drawing += "\t                |    |    |           |  |   |    |           |";
        drawing += "\t a_0: --------- X -- X -----------+---------------+-----------------------";
        drawing += "\t                          |       |   |  |   |    |           |";
        drawing += "\t a_1: ------------------- X ------------------------------+-------+-------";
        drawing += "\t                                  |   |  |   |    |       |   |   |";
        drawing += "\t a_2: ------------------------------------------- X ------+-----------------+-------";
        drawing += "\t                                  |   |  |   |            |   |   |         |";
        drawing += "\t a_3: --------------------------------------------------- X -----------+------------";
        drawing += "\t                                  |   |  |   |                |   |    |    |";
        drawing += "\t a_4: ----------------------------------------------------------- X ------- X ---+--";
        drawing += "\t                                  |   |  |   |                |        |         |";
        drawing += "\t o_0: --------------------------- X -------- X -------------------------------------";
        drawing += "\t                                      |  |                    |        |         |  ";
        drawing += "\t c_0: ---------------------------------------------------------------- X ------- X -";
    return drawing;
};

