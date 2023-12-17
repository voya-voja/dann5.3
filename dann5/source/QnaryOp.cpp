#include <QnaryOp.h>
#include <QcellOps.h>
#include <Qbin.h>

using namespace dann5;

QnaryOp::QnaryOp(const string& id, size_t noArguments)
	: Qnary(id), Qop(noArguments)
{ }

void QnaryOp::resize(size_t size, Qvalue value)
{
	Qcells& logic = cells();
	size_t oSize = logic.size();
	logic.resize(size);
	Qnary::Sp pNaryOut = dynamic_pointer_cast<Qnary>(outputs()[0]);
	if (pNaryOut != nullptr)
	{
		for (size_t at = oSize; at < size; at++)
		{
			QnullCellOp::Sp pNullOp(new QnullCellOp());
			logic[at] = pNullOp;
			pNullOp->output(as_const(*pNaryOut)[at], at);
		}
	}
}

void QnaryOp::operands(const Qdef::Sp& pOut, const Qdefs& ins)
{
    Qop::operands(pOut, ins);
    refresh();
}

void QnaryOp::inputs(const Qdefs& args)
{
	Qop::inputs(args);
    refreshOnInputs();
}

void QnaryOp::append(Qdef::Sp argument)
{
	Qop::append(argument);
	if (Qop::inputs().size() == noInputs())
		refreshOnInputs();
}

Qdef::Sp QnaryOp::output(size_t forBit) const
{
	Qdef::Sp pOut = Qop::output(forBit);
	if (forBit != cAllBits)
	{
		Qnary::Sp pNaryOutput = dynamic_pointer_cast<Qnary>(pOut);
		if (pNaryOutput != nullptr)
			pOut = as_const(*pNaryOutput)[forBit];
	}
	return pOut;
}

void QnaryOp::output(const Qdef::Sp& pOut, size_t forBit)
{
	if (forBit == cAllBits)
		Qop::output(pOut, forBit);
	else
	{
		Qnary::Sp pNaryOutput = static_pointer_cast<Qnary>(Qop::output());
        Qcell::Sp pCellOut = dynamic_pointer_cast<Qcell>(pOut);
        if(pCellOut == nullptr)
            throw logic_error("ERROR @QnaryOp: Output is not Qcell or is not defined!");
		pNaryOutput->set(pCellOut, forBit);
	}
    refreshOnOutput();
}

void QnaryOp::releaseOperands()
{
    Qop::releaseOperands();
    cells().clear();
}

void QnaryOp::refreshOnOutput()
{
    Qnary::Sp pNaryOut = dynamic_pointer_cast<Qnary>(Qop::output());
    if (pNaryOut == nullptr)
        throw logic_error("ERROR @QnaryOp: Output is not Qnary or is not defined!");
    size_t size = noqbs(), outSize = pNaryOut->noqbs();
    if(size > outSize)
        pNaryOut->resize(size);
    else
        resize(outSize);
    Qcells& logic = cells();
    for (size_t atBit = 0; atBit < size; atBit++)
    {
        Qop::Sp pOp = dynamic_pointer_cast<Qop>(logic[atBit]);
        if (pOp == nullptr)
        {
            QnullCellOp::Sp pNullOp(new QnullCellOp());
            logic[atBit] = pNullOp;
            pOp = pNullOp;
        }
        pOp->output(as_const(*pNaryOut)[atBit], atBit);
    }
}

string QnaryOp::toString(bool decomposed, size_t forBit) const
{
	if (!decomposed) return Qop::toString();
    const Qcells& logic = cells();
	if (forBit != cAllBits)
	{
		size_t size = logic.size();
		if (forBit < size)
			return logic[forBit]->toString(decomposed, forBit);
		else // if logic doesn't have requested cell return empty string
			return "";
	}

	size_t size = noqbs();
	string rStr("");
	for (size_t atBit = 0; atBit < size; atBit++)
	{
		rStr += logic[atBit]->toString(decomposed) + "; ";
	}
	return rStr;
}

