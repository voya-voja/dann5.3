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

void QnaryOp::inputs(const Qdefs& args)
{
	Qop::inputs(args);
	refresh();
}

void QnaryOp::append(Qdef::Sp argument)
{
	Qop::append(argument);
	if (Qop::inputs().size() == noInputs())
		refresh();
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
	{
		Qop::output(pOut, forBit);
		Qnary::Sp pNaryOut = dynamic_pointer_cast<Qnary>(pOut);
		if (pNaryOut != nullptr)
		{
			Qcells& logic = cells();
			size_t size = noqbs();
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
	}
	else
	{
		Qnary::Sp pNaryOutput = dynamic_pointer_cast<Qnary>(Qop::output());
		pNaryOutput->set(dynamic_pointer_cast<Qcell>(pOut), forBit);
	}
}

string QnaryOp::toString(bool decomposed, size_t forBit) const
{
	const Qcells& logic = cells();
	if (!decomposed) return Qop::toString(decomposed, forBit);
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

void QnaryOp::compile(Qcompiler& compiler) const
{
	const Qcells& logic = cells();
	size_t size = noqbs();
	for (size_t atCell = 0; atCell < size; atCell++)
	{
		Qop::Sp pCellOp = dynamic_pointer_cast<Qop>(logic[atCell]);
		if (pCellOp != nullptr)
			pCellOp->compile(compiler);
		else
			throw logic_error("Error@QnaryOp: The cell is not an operation");
	}
}

void QnaryOp::add(const Qevaluations& evaluations)
{
	Qop::add(evaluations);
}

string QnaryOp::solution(size_t evaluationId) const
{
	return Qop::solution(evaluationId);
}

void QnaryOp::reset()
{
	Qop::reset();
}