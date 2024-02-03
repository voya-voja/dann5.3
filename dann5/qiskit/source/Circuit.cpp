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
    string str("CircuitInstruction(operation=Instruction(name='");
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
/**** QuOperandsMap ****/
QuOperandsMap& dann5::qiskit::operator+=(QuOperandsMap& left, const QuOperandsMap& right)
{
    left.insert(right.begin(), right.end());
    return left;
}

QuOperandsMap dann5::qiskit::operator+(const QuOperandsMap& left, const QuOperandsMap& right)
{
    QuOperandsMap result(left);
    result += right;
    return result;
}

/**** Circuit ****/
size_t Circuit::nodesNo() const
{
    size_t noNodes = 0;
    for (auto oprndMapElmnt : mOperands)
    {
        // oprndMapElmnt.operand.qubit.qureg
        QuReg oprndReg(oprndMapElmnt.second.first.first);
        noNodes += oprndReg.mNumQubits;
    }
    return noNodes;
}

string Circuit::draw() const
{
    vector<string> lines;
    size_t qbNameSize = 10;
    // draw qu(antum)-bits
    for (auto operand : mOperands)
    {
        QuantumBit qubit(operand.second.first);
        string line(qubit.mRegister.mName);
        line.insert(0, qbNameSize - line.size(), ' ');
        line += ": ";
        lines.push_back(line);
    }
    const Instructions& instrctns = Circuit::instructions();
    bool measuresMode = false;
    for (auto instruction : instrctns)
    {
        string instrctn = instruction.name();
        Qubits qubits = instruction.qubits();
        size_t lineCount = 0;
        for (auto operand : mOperands)
        {
            bool emptyLine = true;
            QuantumBit oprndQubit(operand.second.first);
            size_t atQubit = 0, lastQubit = qubits.size() - 1;
            for (auto qubit : qubits)
            {
                emptyLine = QuantumBit(qubit).mRegister.mName != oprndQubit.mRegister.mName;
                if (!emptyLine)
                {
                    if (lastQubit == atQubit)
                    {
                        if (instrctn == "measure")
                        {
                            ClassicalBit clbit(instruction.clbits()[0]);
                            lines[lineCount] += " " + to_string(clbit.mAt) + " ";
                            measuresMode = true;
                        }
                        else if (lastQubit == 0)
                        {
                            if (instrctn == "reset")
                                lines[lineCount] += "|0>";
                            else
                                lines[lineCount] += "-" + instrctn + "-";
                        }
                        else
                            lines[lineCount] += "-X-";
                    }
                    else if(instrctn == "swap")
                        lines[lineCount] += "-X-";
                    else
                        lines[lineCount] += "-+-";
                    break;
                }
                atQubit++;
            }
            if (emptyLine && !measuresMode)
                lines[lineCount] += "---";
            lineCount += 1;
        }
    }
    string drawing("");
    for (auto line : lines)
        drawing += line + "\n";
    return drawing;
}

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
Instruction MeasureCircuit::Measure(const Qubit& arg, const Clbit& cl)
{
    return Instruction("measure", { arg }, { cl });
}

MeasureCircuit::MeasureCircuit()
:mClbit(ClassicalBit(ClReg(1, "cl"), 0))
{}

MeasureCircuit::MeasureCircuit(const ClassicalBit& clbit)
    :mClbit(clbit)
{}

Instructions MeasureCircuit::create(const Qubits& arguments) const
{
    if (arguments.size() != 1)
        throw logic_error(
            "Error @MeasureCircuit::instructions: arguments size is "
            + arguments.size());
    return { Measure(arguments[0], mClbit) };
}

string MeasureCircuit::draw() const
{
    if (Circuit::operands().size() != 0)
        return Circuit::draw();
    string drawing = "\t  a_0: -M-";
    return drawing;
}

/**** Hadamard Circuit ****/
Instruction HadamardCircuit::Hadamard(const Qubit& arg)
{
    return Instruction("h", { arg });
}

HadamardCircuit::HadamardCircuit()
{
}

Instructions HadamardCircuit::create(const Qubits& arguments) const
{
    if (arguments.size() != 1)
        throw logic_error(
                "Error @HadamardCircuit::instructions: arguments size is "
                + arguments.size());
    return { Hadamard(arguments[0]) };
}

string HadamardCircuit::draw() const
{
    if (Circuit::operands().size() != 0)
        return Circuit::draw();
    string drawing = "\t  a_0: -h-";
    return drawing;
}

/**** Reset Circuit ****/
Instruction ResetCircuit::Reset(const Qubit& arg)
{
    return Instruction("reset", { arg });
}

ResetCircuit::ResetCircuit()
{
}

Instructions ResetCircuit::create(const Qubits& arguments) const
{
    if (arguments.size() != 1)
        throw logic_error(
            "Error @ResetCircuit::instructions: arguments size is "
            + arguments.size());
    return { Reset(arguments[0]) };
}

string ResetCircuit::draw() const
{
    if (Circuit::operands().size() != 0)
        return Circuit::draw();
    string drawing = "\t  a_0: |0>";
    return drawing;
}

/**** Invert Circuit ****/
Instruction InvertCircuit::Not(const Qubit& arg)
{
    return Instruction("x", { arg });
}

InvertCircuit::InvertCircuit()
{
}

Instructions InvertCircuit::create(const Qubits& arguments) const
{
    if (arguments.size() != 1)
        throw logic_error(
            "Error @InvertCircuit::instructions: arguments size is "
            + arguments.size());
    return { Not(arguments[0]) };
}

string InvertCircuit::draw() const
{
    if (Circuit::operands().size() != 0)
        return Circuit::draw();
    string drawing = "\t  a_0: -x-";
    return drawing;
}

/**** Operator Circuit ****/
Instruction OperatorCircuit::ControlledNot(const Qubit& in, const Qubit& out)
{
    // i_0: -+-
    // o_0: -X-
    return Instruction("cx", { in, out });
}

Instruction OperatorCircuit::Swap(const Qubit& in, const Qubit& out)
{
    // i_0: -X-
    // o_0: -X-
    return Instruction("swap", { in, out });
}

/**** Binary Operation Circuit ****/
Instruction BinaryOpCircuit::Toffoli(const Qubit& in0, const Qubit& in1, 
                                                            const Qubit& out)
{
    // i_0: -+-
    // i_1: -+-
    // o_0: -X-
    return Instruction("ccx", { in0, in1, out });
}

/**** Equal Operator Circuit ****/
EqCircuit::EqCircuit() 
{}

Instructions EqCircuit::create(const Qubits& arguments) const
{
    if (arguments.size() != 2)
        throw logic_error(
            "Error @HadamardCircuit::instructions: arguments size is "
            + arguments.size());
    return { ControlledNot(arguments[0], arguments[1]) };
}

string EqCircuit::draw() const
{
    if (Circuit::operands().size() != 0)
        return Circuit::draw();
    string
        drawing  = "\t  i_0: -+-";
        drawing += "\t  o_0: -X-";
    return drawing;
}

/**** Not-Equal Operator Circuit ****/
NeqCircuit::NeqCircuit() 
{
}

Instructions NeqCircuit::create(const Qubits& arguments) const
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
    if (Circuit::operands().size() != 0)
        return Circuit::draw();
    string
        drawing  = "\t  i_0: -+---";
        drawing += "\t  o_0: -X-x-";
    return drawing;
}

/**** Less-Than Operator Circuit ****/
LtCircuit::LtCircuit()
{
}

Instructions LtCircuit::create(const Qubits& arguments) const
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
    if (Circuit::operands().size() != 0)
        return Circuit::draw();
    string
        drawing  = "\t i_0: -X-------+-";
        drawing += "\t o_0: -X-|0>-x-X-";
    return drawing;
};

/**** Less-Than-and-Equal Operator Circuit ****/
LeCircuit::LeCircuit() 
{
}

Instructions LeCircuit::create(const Qubits& arguments) const
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
    if (Circuit::operands().size() != 0)
        return Circuit::draw();
    string
        drawing  = "\t i_0: -X-+-";
        drawing += "\t o_0: -X-X-";
    return drawing;
}

/**** Greater-Than Operator Circuit ****/
GtCircuit::GtCircuit() 
{
}

Instructions GtCircuit::create(const Qubits& arguments) const
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
    if (Circuit::operands().size() != 0)
        return Circuit::draw();
    string
        drawing = "\t  i_0: ---X-+----";
        drawing += "\t o_0: -x-X-X-|0>";
    return drawing;
}

/****  Greater-Than-and-Equal Operator Circuit ****/
GeCircuit::GeCircuit() 
{
}

Instructions GeCircuit::create(const Qubits& arguments) const
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
    if (Circuit::operands().size() != 0)
        return Circuit::draw();
    string
        drawing = "\t  i_0: ---X-+-";
        drawing += "\t o_0: -x-X-X-";
    return drawing;
}
/****  And Operation Circuit ****/
AndCircuit::AndCircuit() 
{
}

Instructions AndCircuit::create(const Qubits& arguments) const
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
    if (Circuit::operands().size() != 0)
        return Circuit::draw();
    string
        drawing = "\t  i_0: -+-";
        drawing += "\t i_1: -+-";
        drawing += "\t o_0: -X-";
    return drawing;
}

/****  Not-And Operation Circuit ****/
NandCircuit::NandCircuit() 
{
}

Instructions NandCircuit::create(const Qubits& arguments) const
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
        drawing = "\t  i_0: ---+-";
        drawing += "\t i_1: ---+-";
        drawing += "\t o_0: -x-X-";
    return drawing;
}

/**** Or Operation Circuit ****/
OrCircuit::OrCircuit() 
{
}

Instructions OrCircuit::create(const Qubits& arguments) const
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
    if (Circuit::operands().size() != 0)
        return Circuit::draw();
    string
        drawing = "\t  i_0: -+-+---";
        drawing += "\t i_1: -+---+-";
        drawing += "\t o_0: -X-X-X-";
    return drawing;
}

/****  Not-Or Operation Circuit ****/
NorCircuit::NorCircuit() 
{
}

Instructions NorCircuit::create(const Qubits& arguments) const
{
    OrCircuit orCrct;
    Instructions instructions = orCrct.instructions(arguments);
    instructions += { InvertCircuit::Not(arguments[2]) };
    return instructions;
}

string NorCircuit::draw() const
{
    if (Circuit::operands().size() != 0)
        return Circuit::draw();
    string
        drawing = "\t  i_0: -+-+-----";
        drawing += "\t i_1: -+---+---";
        drawing += "\t o_0: -X-X-X-x-";
    return drawing;
};

/**** Xor Operation Circuit ****/
XorCircuit::XorCircuit() 
{
}

Instructions XorCircuit::create(const Qubits& arguments) const
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
    if (Circuit::operands().size() != 0)
        return Circuit::draw();
    string
        drawing = "\t  i_0: -+---";
        drawing += "\t i_1: ---+-";
        drawing += "\t o_0: -X-X-";
    return drawing;
}

/****  Not-Xor Operation Circuit ****/
NxorCircuit::NxorCircuit() 
{
}

Instructions NxorCircuit::create(const Qubits& arguments) const
{
    XorCircuit xorCrct;
    Instructions instructions = xorCrct.instructions(arguments);
    instructions += { InvertCircuit::Not(arguments[2]) };
    return instructions;
}

string NxorCircuit::draw() const
{
    if (Circuit::operands().size() != 0)
        return Circuit::draw();
    string
        drawing  = "\t i_0: -+-----";
        drawing += "\t i_1: ---+---";
        drawing += "\t o_0: -X-X-x-";
    return drawing;
}

/****  Half-Adder Operation Circuit ****/
HalfAdderCircuit::HalfAdderCircuit()
{
}

Instructions HalfAdderCircuit::create(const Qubits& arguments) const
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
    if (Circuit::operands().size() != 0)
        return Circuit::draw();
    string
        drawing  = "\t i_0: -+---+-";
        drawing += "\t i_1: ---+-+-";
        drawing += "\t o_0: -X-X---";
        drawing += "\t c_0: -----X-";
    return drawing;
}

/****  Adder Operation Circuit ****/
size_t AdderCircuit::gAuxCounter = 0;

string AdderCircuit::AuxRegName()
{
    string name = "_+" + to_string(gAuxCounter++);
    return name;
}

AdderCircuit::AdderCircuit() 
{
    Qubits auxBits;
    string auxRegName = AuxRegName();
    QuReg aux(1, auxRegName + "_");
    auxBits.push_back(Qubit(aux, 0));
    aux.mName = auxRegName + "c0_";
    auxBits.push_back(Qubit(aux, 0));
    aux.mName = auxRegName + "c1_";
    auxBits.push_back(Qubit(aux, 0));
    QuOperandsMap& oprnds = operands();
    for (size_t count = 0; count < 3; count++)
    {
        QuantumBit qubit = auxBits[count];
        oprnds[qubit.mRegister.mName] = QuOperand(qubit, cSuperposition);
    }
}

Instructions AdderCircuit::create(const Qubits& arguments) const
{
    if (arguments.size() != 5)
        throw logic_error(
            "Error @HadamardCircuit::instructions: arguments size is "
            + arguments.size());
    
    Qubits auxs;
    for (auto auxPair : operands())
        auxs.push_back(auxPair.second.first);
    // half adder { out: a0 = i0 ^ i1; carry: a[1] = i[0] & i[1] }
    HalfAdderCircuit haCrct;
    Instructions instructions = 
                            haCrct.instructions({ arguments[0], arguments[1],
                                                    auxs[0], auxs[1] });
    // half adder { out: o0 = a0 ^ i2; carry: a[2] = a[0] & i[2] }
    instructions += haCrct.instructions({ auxs[0], arguments[2],
                                          arguments[3], auxs[2] });
    // c0 = a1 | a2
    OrCircuit orCrct;
    instructions += orCrct.instructions({ auxs[1], auxs[2], arguments[4] });
    return instructions;
}

string AdderCircuit::draw() const
{
    if (Circuit::operands().size() != 0)
        return Circuit::draw();
    string
        drawing  = "\t i_0: -+-----+-------------------";
        drawing += "\t i_1: ----+--+-------------------";
        drawing += "\t i_3: -------------+--+----------";
        drawing += "\t  _+: -X--X-----+-----+----------";
        drawing += "\t _c0: -------X-----------+--+----";
        drawing += "\t _c1: ----------------X--+-----+-";
        drawing += "\t o_0: ----------X--X-------------";
        drawing += "\t c_0: -------------------X--X--X-";
    return drawing;
}

/****  dann5 custom Circuit ****/
D5circuit::D5circuit()
    : Circuit()  
{
}

D5circuit::D5circuit(const D5circuit& right)
    :Circuit(right), mIns(right.mIns), mOuts(right.mOuts),
                    mInitOperands(right.mInitOperands)
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
    QuReg quReg(quOperand.first.first);
    QuOperandsMap& oprnds = operands();
    if (oprnds.find(quReg.mName) == oprnds.end())
    {
        oprnds[quReg.mName] = quOperand;
        // only input operands are added to the list of initialization operands
        mInitOperands[quReg.mName] = quOperand;
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
    QuReg quReg(quOperand.first.first);
    QuOperandsMap& oprnds= operands();
    if (oprnds.find(quReg.mName) == oprnds.end())
        oprnds[quReg.mName] = quOperand;
    return quOperand.first;
}

void D5circuit::initialize()
{
    if(!mInitialized)
    {
        Instructions initInstrcts;
        for (auto operand : mInitOperands)
            // if operand.Qvalue == cSuperposition
            if (operand.second.second == cSuperposition)
                initInstrcts.push_back(HadamardCircuit::Hadamard(operand.second.first));
        // if operand.Qvalue == 1
            else if (operand.second.second == 1)
                initInstrcts.push_back(InvertCircuit::Not(operand.second.first));
        Circuit::instructions() = initInstrcts + Circuit::instructions();
        mInitialized = true;
    }
}

void D5circuit::measure()
{
    if (!mMeasured)
    {
        ClReg clReg(operands().size(), "cl");
        size_t numClbits = 0;
        for (auto operand : operands())
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
        mMeasured = true;
    }
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

Instructions D5circuit::create(const Qubits& arguments) const
{
    return Circuit::instructions();
}