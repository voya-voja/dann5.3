#include <Circuits.h>
#include <QcellOps.h>

using namespace dann5;
using namespace dann5::qiskit;

/**** Instruction ****/
void Instruction::append(const Qubits& qbts, const Clbits& clbts)
{
    mQubits.insert(mQubits.end(), qbts.begin(), qbts.end());
    mClbits.insert(mClbits.end(), clbts.begin(), clbts.end());
}

Instruction::operator string() const
{
    string str("CircuitInstruction(operation=Instruction('");
    str += name() + "', num_qubits=";
    str += to_string(num_qubits()) + ", num_clbits=";
    str += to_string(num_clbits()) + ", params=[]), qubits=(";
    for (auto qbt : mQubits)
        str += QuantumBit(qbt);
    str += "), clbits=(";
    for (auto clbt : mClbits)
        str += ClassicalBit(clbt);
    str += "))";
    return str;
}

ostream& dann5::qiskit::operator<< (ostream& out, const Instruction& right) 
{
    out << string(right);
    return out;
}

/**** Instructions ****/
Instructions& dann5::qiskit::operator+=(Instructions& left, const Instructions& right)
{
    left.insert(left.end(), right.begin(), right.end());
    return left;
}

Instructions dann5::qiskit::operator+(const Instructions& left, const Instructions& right)
{
    Instructions result(left);
    result += right;
    return result;
}

std::ostream& dann5::qiskit::operator<< (std::ostream& out, const Instructions& right)
{
    for (auto instruction : right)
        out << instruction << endl;
    return out;
}

/**** Circuit ****/
std::ostream& dann5::qiskit::operator << (std::ostream& out, const Circuit& circuit)
{
    out << circuit.draw();
    return out;
}

/**** Circuits ****/
Circuits& dann5::qiskit::operator+=(Circuits& left, const Circuits& right)
{
    left.insert(left.end(), right.begin(), right.end());
    return left;
}

Circuits dann5::qiskit::operator+(const Circuits& left, const Circuits& right)
{
    Circuits result(left);
    result += right;
    return result;
}

std::ostream& dann5::qiskit::operator<< (std::ostream& out, const Circuits& right)
{
    for (auto circuit : right)
        out << circuit;
    return out;
}

/**** Measure Circuit ****/
/*
Instruction MeasureCircuit::Measure(const IoPort& arg, const IoPort& cl)
{
    return Instruction("measure",
        { Qubit(QuantumRegister(1, arg.name()), arg.at()) },
        { Clbit(ClassicalRegister(1, cl.name()), cl.at()) });
}
*/
Instruction MeasureCircuit::Measure(const Qubit& arg, const Clbit& cl)
{
    return Instruction("measure", { arg }, { cl });
}

MeasureCircuit::MeasureCircuit()
//    :Circuit({ MeasureCircuit::Measure() }) 
{}

Instructions MeasureCircuit::instructions(const Qubits& arguments) const
{
    return Circuit::instructions();
}

string MeasureCircuit::draw() const
{
    string drawing = "\t   a_0: -M-";
    drawing += "\t         |";
    drawing += "\tcl: 1 /= +=";
    drawing += "\t         0";
    return drawing;
}

/**** Hadamard Circuit ****/
/*
Instruction HadamardCircuit::Hadamard(const IoPort& arg)
{
    return Instruction("h",
        { Qubit(QuantumRegister(1, arg.name()), arg.at()) });
}
*/
Instruction HadamardCircuit::Hadamard(const Qubit& arg)
{
    return Instruction("h", { arg });
}

HadamardCircuit::HadamardCircuit()
//    :Circuit({ HadamardCircuit::Hadamard() })
{
}

Instructions HadamardCircuit::instructions(const Qubits& arguments) const
{
    if (arguments.size() != 1)
        throw logic_error(
                "Error @HadamardCircuit::instructions: arguments size is "
                + arguments.size());
    return { Hadamard(arguments[0]) };
}

string HadamardCircuit::draw() const
{
    string drawing = "\t  a_0: - H -";
    return drawing;
}

/**** Reset Circuit ****/
/*
Instruction ResetCircuit::Reset(const IoPort& arg) 
{
    return Instruction("reset",
        { Qubit(QuantumRegister(1, arg.name()), arg.at()) });
}
*/
Instruction ResetCircuit::Reset(const Qubit& arg)
{
    return Instruction("reset", { arg });
}

ResetCircuit::ResetCircuit()
//    : Circuit({ ResetCircuit::Reset() }) 
{
}

Instructions ResetCircuit::instructions(const Qubits& arguments) const
{
    if (arguments.size() != 1)
        throw logic_error(
            "Error @HadamardCircuit::instructions: arguments size is "
            + arguments.size());
    return { Reset(arguments[0]) };
}

string ResetCircuit::draw() const
{
    string drawing = "\t  a_0: -|0>-";
    return drawing;
}

/**** Invert Circuit ****/
/*
Instruction InvertCircuit::Not(const IoPort& arg) 
{
    return Instruction("x",
        { Qubit(QuantumRegister(1, arg.name()), arg.at()) });
}
*/
Instruction InvertCircuit::Not(const Qubit& arg)
{
    return Instruction("x", { arg });
}

InvertCircuit::InvertCircuit()
//    : Circuit({ InvertCircuit::Not() }) 
{
}

Instructions InvertCircuit::instructions(const Qubits& arguments) const
{
    if (arguments.size() != 1)
        throw logic_error(
            "Error @HadamardCircuit::instructions: arguments size is "
            + arguments.size());
    return { Not(arguments[0]) };
}

string InvertCircuit::draw() const
{
    string drawing = "\t  a_0: - X -";
    return drawing;
}

/**** Operator Circuit ****/
/*
Instruction OperatorCircuit::ControlledNot(const IoPort& in, const IoPort& out) 
{
    return Instruction("x",
        { Qubit(QuantumRegister(1, in.name()), in.at()),
            Qubit(QuantumRegister(1, out.name()), out.at()) });
}
*/
Instruction OperatorCircuit::ControlledNot(const Qubit& in, const Qubit& out)
{
    return Instruction("cx", { in, out });
}
/*
Instruction OperatorCircuit::Swap(const IoPort& in, const IoPort& out)
{
    return Instruction("swap",
        { Qubit(QuantumRegister(1, in.name()), in.at()),
            Qubit(QuantumRegister(1, out.name()), out.at()) });
}
*/
Instruction OperatorCircuit::Swap(const Qubit& in, const Qubit& out)
{
    return Instruction("swap", { in, out });
}

/**** Binary Operation Circuit ****/
/*
Instruction BinaryOpCircuit::Toffoli(const IoPort& in, const IoPort& out) 
{
    return Instruction("x",
        { Qubit(QuantumRegister(2, in.name()), in.at()),
            Qubit(QuantumRegister(2, in.name()), in.at() + 1),
            Qubit(QuantumRegister(1, out.name()), out.at()) });
}

Instruction BinaryOpCircuit::Toffoli(const IoPort& in0, const IoPort& in1,
                                                            const IoPort& out)
{
    return Instruction("x",
        { Qubit(QuantumRegister(1, in0.name()), in0.at()),
            Qubit(QuantumRegister(1, in1.name()), in1.at()),
            Qubit(QuantumRegister(1, out.name()), out.at()) });
}
*/
Instruction BinaryOpCircuit::Toffoli(const Qubit& in0, const Qubit& in1, 
                                                            const Qubit& out)
{
    return Instruction("ccx", { in0, in1, out });
}
/**** Equal Operator Circuit ****/
EqCircuit::EqCircuit() 
//
{
}

Instructions EqCircuit::instructions(const Qubits& arguments) const
{
    if (arguments.size() != 2)
        throw logic_error(
            "Error @HadamardCircuit::instructions: arguments size is "
            + arguments.size());
    return { ControlledNot(arguments[0], arguments[1]) };
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
/* :OperatorCircuit({OperatorCircuit::ControlledNot(),
                                 InvertCircuit::Not() }) */
{
}

Instructions NeqCircuit::instructions(const Qubits& arguments) const
{
    if (arguments.size() != 2)
        throw logic_error(
            "Error @HadamardCircuit::instructions: arguments size is "
            + arguments.size());
    Instructions instructions = { ControlledNot(arguments[0], arguments[1]),
                                 InvertCircuit::Not(arguments[1]) };
    return instructions;
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
/* : OperatorCircuit({OperatorCircuit::Swap(),
                        ResetCircuit::Reset(IoPort("o")),
                        InvertCircuit::Not(IoPort("o")),
                        OperatorCircuit::ControlledNot() }) */
{
}

Instructions LtCircuit::instructions(const Qubits& arguments) const
{
    if (arguments.size() != 2)
        throw logic_error(
            "Error @HadamardCircuit::instructions: arguments size is "
            + arguments.size());
    Instructions instructions = { OperatorCircuit::Swap(arguments[0], 
                                                                arguments[1]),
                                  ResetCircuit::Reset(arguments[1]),
                                 InvertCircuit::Not(arguments[1]),
                                  ControlledNot(arguments[0], arguments[1]) };
    return instructions;
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
/* : OperatorCircuit({OperatorCircuit::Swap(),
                        OperatorCircuit::ControlledNot() }) */
{
}

Instructions LeCircuit::instructions(const Qubits& arguments) const
{
    if (arguments.size() != 2)
        throw logic_error(
            "Error @HadamardCircuit::instructions: arguments size is "
            + arguments.size());
    Instructions instructions = { OperatorCircuit::Swap(arguments[0],
                                                                arguments[1]),
                                  ControlledNot(arguments[0], arguments[1]) };
    return instructions;
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
/* : OperatorCircuit({InvertCircuit::Not(IoPort("o")),
                        OperatorCircuit::Swap(),
                        OperatorCircuit::ControlledNot(),
                        ResetCircuit::Reset(IoPort("o")) }) */
{
}

Instructions GtCircuit::instructions(const Qubits& arguments) const
{
    if (arguments.size() != 2)
        throw logic_error(
            "Error @HadamardCircuit::instructions: arguments size is "
            + arguments.size());
    Instructions instructions = { InvertCircuit::Not(arguments[1]),
                                  OperatorCircuit::Swap(arguments[0],
                                                                arguments[1]),
                                  ControlledNot(arguments[0], arguments[1]),
                                  ResetCircuit::Reset(arguments[1]) };
    return instructions;
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
/* : OperatorCircuit({InvertCircuit::Not(IoPort("o")),
                        OperatorCircuit::Swap(),
                        OperatorCircuit::ControlledNot() }) */
{
}

Instructions GeCircuit::instructions(const Qubits& arguments) const
{
    if (arguments.size() != 2)
        throw logic_error(
            "Error @HadamardCircuit::instructions: arguments size is "
            + arguments.size());
    Instructions instructions = { InvertCircuit::Not(arguments[1]),
                                  OperatorCircuit::Swap(arguments[0],
                                                                arguments[1]),
                                  ControlledNot(arguments[0], arguments[1]) };
    return instructions;
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
//    : BinaryOpCircuit({ BinaryOpCircuit::Toffoli() }) 
{
}

Instructions AndCircuit::instructions(const Qubits& arguments) const
{
    if (arguments.size() != 3)
        throw logic_error(
            "Error @HadamardCircuit::instructions: arguments size is "
            + arguments.size());
    Instructions instructions = { Toffoli(arguments[0], arguments[1],
                                                                arguments[2]) };
    return instructions;
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
/* : BinaryOpCircuit({InvertCircuit::Not(IoPort("o")),
                        BinaryOpCircuit::Toffoli() }) */
{
}

Instructions NandCircuit::instructions(const Qubits& arguments) const
{
    if (arguments.size() != 3)
        throw logic_error(
            "Error @HadamardCircuit::instructions: arguments size is "
            + arguments.size());
    Instructions instructions = { InvertCircuit::Not(arguments[2]),
                                  Toffoli(arguments[0], arguments[1],
                                                                arguments[2]) };
    return instructions;
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
/* : BinaryOpCircuit({BinaryOpCircuit::Toffoli(IoPort("i"), IoPort("a")),
                        OperatorCircuit::ControlledNot(IoPort("i"), IoPort("a", 1)),
                        OperatorCircuit::ControlledNot(IoPort("i", 1), IoPort("a", 1)),
                        OperatorCircuit::ControlledNot(IoPort("a"), IoPort("o")),
                        OperatorCircuit::ControlledNot(IoPort("a", 1), IoPort("o")) }) */
{
}

Instructions OrCircuit::instructions(const Qubits& arguments) const
{
    if (arguments.size() != 3)
        throw logic_error(
            "Error @HadamardCircuit::instructions: arguments size is "
            + arguments.size());
    QuReg aux(2, "_or_a_");
    Instructions instructions = { Toffoli(arguments[0], arguments[1], 
                                                                arguments[2]),
                OperatorCircuit::ControlledNot(arguments[0], arguments[2]),
                OperatorCircuit::ControlledNot(arguments[1], arguments[2]) };
    return instructions;
}

string OrCircuit::draw() const
{
    string 
        drawing = "\t  i_0: --+----+-------";
        drawing += "\t        |    |";
        drawing += "\t i_1: --+---------+--";
        drawing += "\t        |    |    |";
        drawing += "\t o_0: - X -- X -- X -";
    return drawing;
}

/****  Not-Or Operation Circuit ****/
NorCircuit::NorCircuit() 
/* : BinaryOpCircuit({BinaryOpCircuit::Toffoli(IoPort("i"), IoPort("a")),
                        OperatorCircuit::ControlledNot(IoPort("i"), IoPort("a", 1)),
                        OperatorCircuit::ControlledNot(IoPort("i", 1), IoPort("a", 1)),
                        OperatorCircuit::ControlledNot(IoPort("a")),
                        OperatorCircuit::ControlledNot(IoPort("a", 1)),
                        InvertCircuit::Not(IoPort("o")) }) */
{
}

Instructions NorCircuit::instructions(const Qubits& arguments) const
{
    OrCircuit orCrct;
    Instructions instructions = orCrct.instructions(arguments);
    instructions += { InvertCircuit::Not(arguments[2]) };
    return instructions;
}

string NorCircuit::draw() const
{
    string 
        drawing = "\t  i_0: --+----+-------";
        drawing += "\t        |    |";
        drawing += "\t i_1: --+---------+--";
        drawing += "\t        |    |    |";
        drawing += "\t o_0: - X -- X -- X -- X -";
    return drawing;
};

/**** Xor Operation Circuit ****/
XorCircuit::XorCircuit() 
/* : BinaryOpCircuit({OperatorCircuit::ControlledNot(),
                        OperatorCircuit::ControlledNot(IoPort("i", 1)) }) */
{
}

Instructions XorCircuit::instructions(const Qubits& arguments) const
{
    if (arguments.size() < 3)
        throw logic_error(
            "Error @HadamardCircuit::instructions: arguments size is "
            + arguments.size());
    Instructions instructions = { 
                OperatorCircuit::ControlledNot(arguments[0], arguments[2]),
                OperatorCircuit::ControlledNot(arguments[1], arguments[2]) };
    return instructions;
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
/* : BinaryOpCircuit({OperatorCircuit::ControlledNot(),
                        OperatorCircuit::ControlledNot(IoPort("i", 1)),
                        InvertCircuit::Not(IoPort("o")) }) */
{
}

Instructions NxorCircuit::instructions(const Qubits& arguments) const
{
    XorCircuit xorCrct;
    Instructions instructions = xorCrct.instructions(arguments);
    instructions += { InvertCircuit::Not(arguments[2]) };
    return instructions;
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
HalfAdderCircuit::HalfAdderCircuit()
/* : BinaryOp2OutCircuit({OperatorCircuit::ControlledNot(),
                            OperatorCircuit::ControlledNot(IoPort("i", 1)),
                            BinaryOpCircuit::Toffoli(IoPort("i"), IoPort("c")) }) */
{
}

Instructions HalfAdderCircuit::instructions(const Qubits& arguments) const
{
    if (arguments.size() != 4)
        throw logic_error(
            "Error @HadamardCircuit::instructions: arguments size is "
            + arguments.size());
    XorCircuit xorCrct;
    Instructions instructions = xorCrct.instructions({ arguments[0], arguments[1], arguments[2] });
    instructions += { BinaryOpCircuit::Toffoli(arguments[0], arguments[1], arguments[3]) };
    return instructions;
}

string HalfAdderCircuit::draw() const
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
/* : Circuit({ // half adder { out: a0 = i0 ^ i1; carry: a[1] = i[0] & i[1] }
                OperatorCircuit::ControlledNot(IoPort("i"), IoPort("a")),
                OperatorCircuit::ControlledNot(IoPort("i", 1), IoPort("a")),
                BinaryOpCircuit::Toffoli(IoPort("i"), IoPort("a", 1)),
                // half adder { out: o0 = a0 ^ i2; carry: a[2] = a[0] & i[2] }
                OperatorCircuit::ControlledNot(IoPort("a"), IoPort("o")),
                OperatorCircuit::ControlledNot(IoPort("i", 2) , IoPort("o", 1)),
                BinaryOpCircuit::Toffoli(IoPort("a"), IoPort("i", 2), IoPort("a", 2)),
                // c0 = a1 | a2
                BinaryOpCircuit::Toffoli(IoPort("a", 1), IoPort("a", 3)),       // AND
                OperatorCircuit::ControlledNot(IoPort("a", 1), IoPort("a", 4)),
                OperatorCircuit::ControlledNot(IoPort("a", 2), IoPort("a", 4)), // XOR
                OperatorCircuit::ControlledNot(IoPort("i", 1), IoPort("c")),
                OperatorCircuit::ControlledNot(IoPort("i", 1), IoPort("c")) })  // XOR */
{
}

Instructions AdderCircuit::instructions(const Qubits& arguments) const
{
    if (arguments.size() != 5)
        throw logic_error(
            "Error @HadamardCircuit::instructions: arguments size is "
            + arguments.size());
    QuReg aux(3, "_add_a_");
    // half adder { out: a0 = i0 ^ i1; carry: a[1] = i[0] & i[1] }
    HalfAdderCircuit haCrct;
    QuantumBit aux0(aux, 0), aux1(aux, 1);
    Instructions instructions = haCrct.instructions({ arguments[0], arguments[1], aux0, aux1 });
    // half adder { out: o0 = a0 ^ i2; carry: a[2] = a[0] & i[2] }
    QuantumBit aux2(aux, 2);
    instructions += haCrct.instructions({ aux0, arguments[2], arguments[3], aux2 });
    // c0 = a1 | a2
    OrCircuit orCrct;
    instructions += orCrct.instructions({ aux1, aux2, arguments[4] });
    return instructions;
}

string AdderCircuit::draw() const
{
    string 
        drawing = "\t  i_0: - H ------+---------+--------------------------------------";
        drawing += "\t                |         |          ";
        drawing += "\t i_1: - H -----------+----+--------------------------------------";
        drawing += "\t                |    |    |          ";
        drawing += "\t i_3: - H -----------------------------+----+--------------------";
        drawing += "\t                |    |    |            |    |          ";
        drawing += "\t a_0: --------- X -- X -----------+---------+--------------------";
        drawing += "\t                          |       |    |    |          ";
        drawing += "\t a_1: ------------------- X ------------------------+----+-------";
        drawing += "\t                                  |    |    |       |     |";
        drawing += "\t a_2: ------------------------------------- X ------+--------------+-------";
        drawing += "\t                                  |    |            |     |         |";
        drawing += "\t a_3: --------------------------------------------- X --------+------------";
        drawing += "\t                                  |    |                  |    |    |";
        drawing += "\t a_4: -------------------------------------------------- X ------- X ---+--";
        drawing += "\t                                  |    |                      |         |";
        drawing += "\t o_0: --------------------------- X -- X ----------------------------------";
        drawing += "\t                                                              |         |  ";
        drawing += "\t c_0: ------------------------------------------------------- X ------- X -";
    return drawing;
}

/****  dann5 custom Circuit ****/
D5circuit::D5circuit()
    : Circuit()  
{
}

void D5circuit::declare(const Qop& op)
{
    const Qdefs& ins = op.inputs();
    size_t numClbits = 0;
    for (auto pIn : ins)
    {
        QcellOp::Sp pOp = dynamic_pointer_cast<QcellOp>(pIn);
        if (pOp != nullptr)
            pIn = pOp->Qop::output();
        QuReg quReg(pIn->noqbs(), pIn->id());
        if(mIns.find(quReg.mName) == mIns.end())
        {
            mIns[quReg.mName] = quReg;
            numClbits += pIn->noqbs();
        }
    }

    const Qdefs& outs = op.outputs();
    for (auto pOut : outs)
    {
        QcellOp::Sp pOp = dynamic_pointer_cast<QcellOp>(pOut);
        if (pOp != nullptr)
            pOut = pOp->Qop::output();
        QuReg quReg(pOut->noqbs(), pOut->id());
        if (mOuts.find(quReg.mName) == mOuts.end())
        {
            mOuts[quReg.mName] = quReg;
            numClbits += pOut->noqbs();
        }
    }
}

QuantumBit D5circuit::input(const Qcell::Sp& pOperand)
{
    string id = pOperand->id();
    Qvalue value = pOperand->value();
    QuOperand quOperand;
    for (auto inReg : mIns)
    {
        if (id == inReg.first)
        {
            // quOperand.QuBit.QuantumRegister = outReg.QuantumRegister
            quOperand.first.first = inReg.second;
            // quOperand.QuBit.at = operands bit position
            quOperand.first.second = 0;
            // quOperand.Qvalue = value
            quOperand.second = pOperand->value();
            break;
        }
    }
    if (mOperands.find(QuReg(quOperand.first.first).mName) == mOperands.end())
    {
        mOperands[QuReg(quOperand.first.first).mName] = quOperand;
        // only input operands to be added
        mInitOperands[QuReg(quOperand.first.first).mName] = quOperand;
    }
    return quOperand.first;
}

QuantumBit D5circuit::output(const Qcell::Sp& pOperand)
{
    string id = pOperand->id();
    Qvalue value = pOperand->value();
    QuOperand quOperand;
    for (auto outReg : mOuts)
    {
        if (id == outReg.first)
        {
            // quOperand.QuBit.QuantumRegister = outReg.QuantumRegister
            quOperand.first.first = outReg.second;
            // quOperand.QuBit.at = operands bit position
            quOperand.first.second = 0;
            // quOperand.Qvalue = value
            quOperand.second = pOperand->value();
            break;
        }
    }
    if (mOperands.find(QuReg(quOperand.first.first).mName) == mOperands.end())
        mOperands[QuReg(quOperand.first.first).mName] = quOperand;
    return quOperand.first;
}
void D5circuit::measure()
{
    ClReg clReg(mOperands.size(), "cl");
    size_t numClbits = 0;
    for (auto operand : mOperands)
    {
        if (numClbits == clReg.mNumClbits)
            throw logic_error("ERROR @D5circuit::measure: clReg["
                + to_string(numClbits)
                + "] is smaller than input QuRegs!");

        ClassicalBit clBit(clReg, numClbits);
        Instruction measure = MeasureCircuit::Measure(operand.second.first, clBit);
        Circuit::instructions().push_back(measure);
        numClbits++;
    }
}

string D5circuit::draw() const
{
    string drawing = "";
    return drawing;
}

size_t D5circuit::nodesNo() const
{
    size_t noNodes = 0;
    for (auto in : mIns)
        noNodes += QuReg(in.second).mNumQubits;
    for (auto out : mOuts)
        noNodes += QuReg(out.second).mNumQubits;
    return noNodes;
}

Instructions D5circuit::instructions(const Qubits& arguments) const
{
    return Circuit::instructions();
}

Instructions D5circuit::initialize() const
{
    Instructions initInstrcts;
    for(auto operand : mInitOperands)
        // if operand.Qvalue == cSuperposition
        if (operand.second.second == cSuperposition)
            initInstrcts.push_back(HadamardCircuit::Hadamard(operand.second.first));
        // if operand.Qvalue == 1
        else if(operand.second.second == 1)
            initInstrcts.push_back(InvertCircuit::Not(operand.second.first));
    return initInstrcts;
}
