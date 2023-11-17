#include <math.h>

#include <Qop.h>
#include <Qexpr.h>

using namespace dann5;


/*** Operation ***/
map<string, unsigned long long> Qop::gOutputIndices;

string Qop::Id(const string& outType)
{
	if (gOutputIndices.find(outType) == gOutputIndices.end())
		gOutputIndices[outType] = 0;
	unsigned long long outIndex = gOutputIndices[outType]++;
	return to_string(outIndex);
}

Qop::Qop(size_t noArgs)
	:mNoInputs(noArgs), mpOutput(nullptr)
{
}

Qop::Qop(const Qop& right)
	: mNoInputs(right.mNoInputs), mpOutput(right.mpOutput)
{
	for (auto& pOp : right.mInputs)
		mInputs.push_back(pOp);
}

Qop::~Qop()
{
}

std::size_t Qop::noqbs() const noexcept
{
	size_t size = 0;
	for (auto pIn : mInputs)
	{
		size_t aNobs = pIn->noqbs();
		if (aNobs > size) size = aNobs;
	}
	if (mpOutput != nullptr)
	{
		size_t aNobs = mpOutput->noqbs();
		if (aNobs > size) size = aNobs;
	}
	return(size);
}

void Qop::releaseArguments()
{
	while (mInputs.size() > 0)
		mInputs.pop_back();
}

void Qop::inputs(const Qdefs& operands)
{
	if (operands.size() != mNoInputs)
		throw invalid_argument("Arguments number is " + to_string(operands.size())
			+ " instead of " + to_string(noInputs()));
	for (auto& pOp : operands)
		mInputs.push_back(pOp);
}

void Qop::append(Qdef::Sp argument)
{
	if (mInputs.size() == mNoInputs)
		throw invalid_argument("Arguments number will exceed defined size of "
			+ to_string(noInputs()));
	mInputs.push_back(argument);
}

Qdef::Sp Qop::output(size_t forBit) const
{
	if(forBit == cAllBits) return mpOutput;

	Qnary::Sp pNary = dynamic_pointer_cast<Qnary>(mpOutput);
	if(pNary == nullptr) return mpOutput;
	return as_const(*pNary)[forBit];
}

void Qop::output(const Qdef::Sp& pOut, size_t forBit)
{
	if (forBit == cAllBits)
	{
		mpOutput = pOut;
		Qnary::Sp pNary = dynamic_pointer_cast<Qnary>(mpOutput);
		size_t size = noqbs();
		if (pNary != nullptr && size != pNary->noqbs())
		{
			pNary->resize(size);
		}
	}
	else
	{
		Qnary::Sp pNary = dynamic_pointer_cast<Qnary>(mpOutput);
		Qcell::Sp pCell = dynamic_pointer_cast<Qcell>(pOut);
		if (pNary != nullptr && pCell != nullptr)
			pNary->set(pCell, forBit);
		else
			mpOutput = pOut;
	}
}

void Qop::compile(Qcompiler& compiler) const
{
    compiler.compile(*this);
}

string Qop::toString(bool decomposed, size_t forBit) const
{
	size_t size = mInputs.size();
	string rStr(""), rest("");
	if (!decomposed)
		rStr += "(";
	else
		rStr += output(forBit)->toString(decomposed, forBit) + " = ";
	for (size_t atArg = 0; atArg < size; atArg++)
	{
		Qdef::Sp pArg = mInputs[atArg];
		string aStr("?");
		if (pArg != nullptr)
		{
			aStr = pArg->toString(decomposed, forBit);
			if (decomposed)
			{
				// if operand is a sub-operation
				Qop::Sp pOp = dynamic_pointer_cast<Qop>(pArg);
				if (pOp != nullptr)
				{
					if (!pOp->asDefinition())
					{
						if (rest != "") rest += "; ";
						rest += aStr;
					}
					aStr = pOp->output(forBit)->toString(decomposed, forBit); // extract sub-operation output
				}
			}
		}
		rStr += aStr;
		if (atArg != size - 1)
			rStr += " " + identifier() + " ";
	}
	if (!decomposed)
		rStr += ")";
	else if (rest != "")
		rStr += "; " + rest;
		
	return rStr;
}

void Qop::add(const Qevaluations& evaluations)
{
	for (auto pInput : mInputs)
	{
		pInput->add(evaluations);
	}
	mpOutput->add(evaluations);
}

string Qop::solution(size_t sampleId) const
{
	string osStr(mpOutput->solution(sampleId));
	for (auto pInput : mInputs)
	{
		osStr += "; " + pInput->solution(sampleId);
	}
	return osStr;
}

void Qop::reset()
{
	for (auto pInput : mInputs)
	{
		pInput->reset();
	}
	if(mpOutput != nullptr)
		mpOutput->reset();
}