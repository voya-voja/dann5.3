#include <QcellOps.h>
#include <Qbit.h>

using namespace dann5;

/*** Q cell operation ***/

QcellOp::QcellOp(const string& id, size_t size) 
	: Qcell(id), Qop(size)
{}

void QcellOp::inputs(const Qdefs& ins)
{
	Qop::inputs(ins);
	Qvalue v = value();
	if (v != cSuperposition)
	{
		Qcell::Sp pOut = dynamic_pointer_cast<Qcell>(Qop::output());
		if (pOut != nullptr)
		{
			Qvalue outV = pOut->value();
			if (outV == cSuperposition && v != outV)// && pOut->id()[0] == '_')
				pOut->value(v);
		}
	}
}

void QcellOp::output(const Qdef::Sp& pOut, size_t forBit)
{
	Qvalue v = value();
	if (v != cSuperposition)
	{
		Qcell::Sp pCellOut = dynamic_pointer_cast<Qcell>(pOut);
		if (pCellOut != nullptr)
		{
			Qvalue outV = pCellOut->value();
			if (outV == cSuperposition && v != outV)// && pCellOut->id()[0] == '_')
				pCellOut->value(v);
		}
	}
	Qop::output(pOut);
}

void QcellOp::value(Qvalue v)
{
	Qcell::Sp pOut = dynamic_pointer_cast<Qcell>(outputs()[0]);
	if (pOut != nullptr)
		pOut->value(v);
	else
		throw std::logic_error("Error@QcellOp: Setting value of a Qcell operation without output argument!");
}

Qvalue QcellOp::value() const
{
	Qcells ins = Qop::inputs();
	size_t iSize = ins.size();
	Qvalues values;
	for (int at = 0; at < iSize; at++)
	{
		Qcell::Sp pIn = ins[at];
		if (pIn != nullptr)
		{
			Qvalue v = pIn->value();
			if (v == cSuperposition)
				return(cSuperposition);
			values.push_back(v);
		}
	}
	return(calculate(values));
}

/*** Q operator ***/

Qdef::Sp Qoperator::output(size_t forBit) const
{
	Qdef::Sp pOut = Qop::output();
	Qop::Sp pOop = dynamic_pointer_cast<Qop>(pOut);
	if (pOop != nullptr)
		pOut = pOop->output();
	return(pOut);
}

string Qoperator::toString(bool decomposed, size_t forBit) const
{
	string str("?"), rest(""), rStr("");
	if (!decomposed) rStr += "(";
    Qdef::Sp pOut = Qop::output();
    if (pOut != nullptr)
    {
        str = pOut->toString(decomposed);
        if (decomposed)
        {
            // if operand is a sub-operation
            Qop::Sp pOp = dynamic_pointer_cast<Qop>(pOut);
            if (pOp != nullptr)
            {
                if (rest != "") rest += "; ";
                rest += str;
                str = pOp->output()->toString(decomposed); // extract sub-operation output
            }
        }
    }
	rStr += str + " " + identifier() + " ";
    Qdef::Sp pIn = Qop::inputs()[0];
    if (pIn != nullptr)
    {
        str = pIn->toString(decomposed);
        if (decomposed)
        {
            // if operand is a sub-operation
            Qop::Sp pOp = dynamic_pointer_cast<Qop>(pIn);
            if (pOp != nullptr)
            {
                rest += str;
                str = pOp->output()->toString(decomposed); // extract sub-operation output
            }
        }
        rStr += str;
    }
	if (!decomposed)
		rStr += ")";
	else if (rest != "")
		rStr += "; " + rest;
	return rStr;
}

/*** Q invert operation ***/

Qvalue Qinvert::calculate(const Qvalues& values) const
{
    Qcell::Sp pOut = dynamic_pointer_cast<Qcell>(Qop::output());
    if (pOut == nullptr || pOut->value() == cSuperposition)
        return(cSuperposition);
    return(Qvalue(values[0] != pOut->value()));
}

/*** Q eq operation ***/
void Qeq::operands(const Qdef::Sp& pOut, const Qdefs& ins)
{
	validate(dynamic_pointer_cast<Qcell>(pOut), Qcells(ins)[0]);
	Qoperator::operands(pOut, ins);
}

void Qeq::output(const Qdef::Sp& pOut, size_t forBit)
{
	Qdefs ins = Qop::inputs();
	if(ins.size() == 1)
		validate(dynamic_pointer_cast<Qcell>(pOut), Qcells(ins)[0]);
	QcellOp::output(pOut, forBit);
}

Qvalue Qeq::calculate(const Qvalues& values) const
{
	Qcell::Sp pOut = dynamic_pointer_cast<Qcell>(Qop::output());
	if (pOut == nullptr || pOut->value() == cSuperposition)
		return(cSuperposition);
	return(Qvalue(values[0] == pOut->value()));
}

void Qeq::validate(const Qcell::Sp& pOut, const Qcell::Sp& pIn)
{
	Qvalue outValue = pOut->value(), inValue = pIn->value();
	if (outValue != inValue)
	{
		if (outValue == cSuperposition)
			pOut->value(inValue);
		else if (inValue == cSuperposition)
			pIn->value(outValue);
		else
			throw logic_error("ERROR @Qeq::operands: output "
				+ pOut->id() + " = " + to_string(outValue)
				+ " can not ne equal to input "
				+ pIn->id() + " = " + to_string(outValue));
	}
}


/*** Q ne operation ***/

Qvalue Qneq::calculate(const Qvalues& values) const
{
	Qcell::Sp pOut = dynamic_pointer_cast<Qcell>(Qop::output());
	if (pOut == nullptr || pOut->value() == cSuperposition)
		return(cSuperposition);
	return(Qvalue(values[0] != pOut->value()));
}

/*** Q lt operation ***/

Qvalue Qlt::calculate(const Qvalues& values) const
{
	Qcell::Sp pOut = dynamic_pointer_cast<Qcell>(Qop::output());
	if (pOut == nullptr || pOut->value() == cSuperposition)
		return(cSuperposition);
	return(Qvalue(values[0] < pOut->value()));
}

/*** Q le operation ***/

Qvalue Qle::calculate(const Qvalues& values) const
{
	Qcell::Sp pOut = dynamic_pointer_cast<Qcell>(Qop::output());
	if (pOut == nullptr || pOut->value() == cSuperposition)
		return(cSuperposition);
	return(Qvalue(values[0] <= pOut->value()));
}

/*** Q gt operation ***/

Qvalue Qgt::calculate(const Qvalues& values) const
{
	Qcell::Sp pOut = dynamic_pointer_cast<Qcell>(Qop::output());
	if (pOut == nullptr || pOut->value() == cSuperposition)
		return(cSuperposition);
	return(Qvalue(values[0] > pOut->value()));
}

/*** Q ge operation ***/

Qvalue Qge::calculate(const Qvalues& values) const
{
	Qcell::Sp pOut = dynamic_pointer_cast<Qcell>(Qop::output());
	if (pOut == nullptr || pOut->value() == cSuperposition)
		return(cSuperposition);
	return(Qvalue(values[0] >= pOut->value()));
}

/*** Q and operation ***/
void Qand::operands(const Qdef::Sp& pOut, const Qdefs& ins)
{
	Qcell::Sp pCellOut = dynamic_pointer_cast<Qcell>(pOut);
	Qcells inCells = ins;
	Qvalues values;
	for (auto in : inCells)
		values.push_back(in->value());
	pCellOut->value(calculate(values));
	QcellOp::operands(pOut, ins);
}

void Qand::output(const Qdef::Sp& pOut, size_t forBit)
{
	Qcell::Sp pCellOut = dynamic_pointer_cast<Qcell>(pOut);
	if(pCellOut->value() == cSuperposition)
		pCellOut->value(value());
	QcellOp::output(pOut, forBit);
}

Qvalue Qand::value() const
{
	Qcells ins = Qop::inputs();
	Qvalues values;
	for (auto in : ins)
		values.push_back(in->value());
	return calculate(values);
}

Qvalue Qand::calculate(const Qvalues& values) const
{
	if (values[0] == 0 || values[1] == 0) return 0;
	if (values[0] == cSuperposition || values[1] == cSuperposition) return cSuperposition;
	return 1;
}

/*** Q cell 2 outputs operation ***/

Qcell2outOp::Qcell2outOp(const string& id, size_t size) 
	: QcellOp(id, size), mpAuxiliary(nullptr)
{}

void Qcell2outOp::operands(const Qdef::Sp& pOut, const Qdefs& ins)
{
    QcellOp::operands(pOut, ins);
    // if aux output is not initialized, initialize it with same Q type as output
    if (mpAuxiliary == nullptr)
        initialize(pOut);
}

void Qcell2outOp::output(const Qdef::Sp& pOut, size_t forBit)
{
	QcellOp::output(pOut);
	// if aux output is not initialized, initialize it with same Q type as output
    if (mpAuxiliary == nullptr)
        initialize(pOut);
}

inline void Qcell2outOp::initialize(const Qdef::Sp& pOut)
{
    mpAuxiliary = Qop::output()->clone();
    mpAuxiliary->id("?" + id() + Qop::Id("?" + id()));
}

Qdefs Qcell2outOp::outputs() const
{
	Qdefs outs = QcellOp::outputs();
	outs.push_back(mpAuxiliary);
	return outs;
}

/*** Q nAnd operation ***/

Qvalue Qnand::calculate(const Qvalues& values) const
{
	return(Qvalue(!(values[0] & values[1])));
}

/*** Q or operation ***/

Qvalue Qor::calculate(const Qvalues& values) const
{
	return(Qvalue(values[0] | values[1]));
}

/*** Q nOr operation ***/
void Qnor::operands(const Qdef::Sp& pOut, const Qdefs& ins)
{
	Qcell::Sp pCellOut = dynamic_pointer_cast<Qcell>(pOut);
	Qcells inCells = ins;
	Qvalues values;
	for (auto in : inCells)
		values.push_back(in->value());
	pCellOut->value(calculate(values));
	Qcell2outOp::operands(pOut, ins);
}

void Qnor::output(const Qdef::Sp& pOut, size_t forBit)
{
	Qcell::Sp pCellOut = dynamic_pointer_cast<Qcell>(pOut);
	if (pCellOut->value() == cSuperposition)
		pCellOut->value(value());
	Qcell2outOp::output(pOut, forBit);
}

Qvalue Qnor::value() const
{
	Qcells ins = Qop::inputs();
	Qvalues values;
	for (auto in : ins)
		values.push_back(in->value());
	return calculate(values);
}

Qvalue Qnor::calculate(const Qvalues& values) const
{
	if (values[0] == 1 || values[1] == 1) return 0;
	if (values[0] == cSuperposition || values[1] == cSuperposition) return cSuperposition;
	return 1;
}

/*** Q addition operation ***/

Qaddition::Qaddition(const string& id, size_t size, const Qdef::Sp pOut, const Qaddition::Carry::Sp pCarry)
	: QcellOp(id, size), mpCarry(pCarry)
{
	mpCarry->addition(this);
	output(pOut);
	pOut->id(createOutId());
}

void Qaddition::operands(const Qdef::Sp& pOut, const Qdefs& ins)
{
    QcellOp::operands(pOut, ins);
    mpCarry->inputs(ins);
    initialize();
}

void Qaddition::inputs(const Qdefs& ins)
{
	QcellOp::inputs(ins);
	mpCarry->inputs(ins);
}

void Qaddition::output(const Qdef::Sp& pOut, size_t forBit)
{
	QcellOp::output(pOut);
    initialize();
}

Qdefs Qaddition::outputs() const
{
	Qdefs outs = QcellOp::outputs();
	outs.push_back(mpCarry->Qop::output());
	return outs;
}

inline void Qaddition::initialize()
{
    Qdef::Sp pAdditionOut = Qop::output();
    // if carry output is not initialized, initialize it with same Q type as output
    Qdef::Sp pCarryOut = mpCarry->Qop::output();
    if(pCarryOut == nullptr)
    {
        pCarryOut = pAdditionOut->clone();
		dynamic_pointer_cast<Qcell>(pCarryOut)->value(mpCarry->value());
        mpCarry->output(pCarryOut);
    }
    // refresh carry out id to be consistent with addition out id
    pCarryOut->id(Carry::Symbol(pAdditionOut->id()));
}

/*** Carry Operation ***/
string Qaddition::Carry::Carry::Symbol(const string& addTag)
{
    return(cMark() + "[" + addTag + "]");
}

Qaddition::Carry::Carry(Qaddition* pAddition)
	: QcellOp(cMark(), pAddition->noInputs()), mpAddition(pAddition)
{}

Qaddition::Carry::Carry(const Carry& right)
	: QcellOp(right), mpAddition(right.mpAddition)
{}

Qaddition::Carry::~Carry()
{}

Qdef::Sp Qaddition::Carry::clone() const
{
    Sp pCarry = Sp(new Qaddition::Carry(*this));
    return pCarry;
}


void Qaddition::Carry::output(const Qdef::Sp& pOut, size_t forBit)
{
	Qop::output(pOut);
    Qaddition::Carry* pOldCarry = as_const(*mpAddition).carry().get();
    if(pOldCarry != this)
        pOldCarry->output(pOut);
}

string Qaddition::Carry::toString(bool decomposed, size_t forBit) const
{
	if (decomposed)
	{
		string cStr = Qop::output()->toString(decomposed) + " = ";

		Qcell::Sp pOut = static_pointer_cast<Qcell>(mpAddition->Qop::output());

		Qvalue value = pOut->value();
		string valueStr = "";
		if (value != cSuperposition) valueStr = to_string(value);
		else valueStr.append(1, cSuperposition);

		cStr += Symbol(pOut->id()) + "\\" + valueStr + "\\";
		return cStr;
	}
	return Symbol(mpAddition->Qop::toString(decomposed));
}

void Qaddition::Carry::add(const Qevaluations& evaluations)
{
	Qdef::Sp pOut = Qop::output();
	pOut->add(evaluations);
}

void Qaddition::Carry::addition(Qaddition* pAddition)
{
	mpAddition = pAddition;
}

Qvalue Qaddition::Carry::calculate(const Qvalues& values) const
{
	//const Qdefs& ins = as_const(*mpAddition).Qop::inputs();
	Qcells ins = as_const(*mpAddition).Qop::inputs();
	size_t iSize = ins.size();
	Qvalue carry = ins[0]->value() & ins[1]->value();
	if (iSize == 3)
		carry &= ins[2]->value();
	return carry;
}

/*** Q xor operation ***/

Qvalue Qxor::calculate(const Qvalues& values) const
{
	// returns one Qbit value without carry bit
	return(Qvalue((values[0] ^ values[1]) & 1));
}

/*** Q Nxor operation ***/

Qvalue Qnxor::calculate(const Qvalues& values) const
{
	// returns one Qbit value without carry bit
	return(Qvalue((values[0] ^ values[1]) & 0));
}

/*** Q adder operation ***/

Qvalue Qadder::calculate(const Qvalues& values) const
{
	// returns one Qbit value without carry bit
	return(Qvalue(values[0] + values[1] + values[2]) & 1);
}

/*** Q xor-adder operation ***/

QadjustAdder::Sp QadjustAdder::process(const Qdefs& ins)
{
    QadjustAdder::Sp pAdder;
	if (ins.size() == 3)
	{
        pAdder = QadjustAdder::Sp(new QadjustAdder(true));
        pAdder->inputs(ins);
	}
	else if (ins.size() == 2)
	{
		QadjustAdder::Sp pLAA(dynamic_pointer_cast<QadjustAdder>(ins[0])),
			pRAA(dynamic_pointer_cast<QadjustAdder>(ins[1]));
		if (pLAA == nullptr && pRAA == nullptr)
		{
            pAdder = QadjustAdder::Sp(new QadjustAdder());
            pAdder->inputs(ins);
		}
		else
		{
			size_t at = 1;
			if (pLAA != nullptr) 
				pAdder = pLAA;
			else
			{
				pAdder = pRAA;
				at = 0;
			}
			if(pAdder->isAdder())
            {
                Qaddition::inputs(ins);
                pAdder = Sp(new QadjustAdder(*this));
            }
			else
			{
				pAdder->id(Qadder::cMark());
				pAdder->noInputs(3);
				pAdder->append(ins[at]);
				pAdder->extended();
			}
		}
	}
	else
		throw invalid_argument("Arguments number is " + to_string(ins.size())
			+ " instead of 2 or 3, in case of xor or adder, respectfully ");
	return pAdder;
}

Qvalue QadjustAdder::calculate(const Qvalues& values) const
{
	size_t size = values.size();
	// returns one Qbit value without carry bit
	if(size == 2)
		return(Qvalue((values[0] + values[1]) & 1));

	return(Qvalue(values[0] + values[1] + values[2]) & 1);
}
