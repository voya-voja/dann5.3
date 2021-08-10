#include <QnaryOp.h>
#include <QcellOps.h>
#include <Qbin.h>

using namespace dann5::ocean;

QnaryOp::QnaryOp(const string& id, size_t noArguments)
	: Qnary(id), Qop(noArguments)
{ }

void QnaryOp::resize(size_t size, Qvalue value)
{
	Qcells& logic = cells();
	size_t oSize = logic.size();
	logic.resize(size);
	for (size_t at = oSize; at < size; at++)
		logic[at] = Qcell::Sp(new QnullCellOp());
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
	if (forBit != cAllBits) return logic[forBit]->toString(decomposed, forBit);

	size_t size = noqbs();
	string rStr("");
	for (size_t atBit = 0; atBit < size; atBit++)
	{
		rStr += logic[atBit]->toString(decomposed) + "\n";
	}
	return rStr;
}

Qubo QnaryOp::qubo(bool finalized, size_t forBit) const
{
	const Qcells& logic = cells();
	if (forBit != cAllBits)
	{
		Qop::Sp pCellOp = dynamic_pointer_cast<Qop>(logic[forBit]);
		if (pCellOp != nullptr)
			return pCellOp->qubo(finalized);
		else
			throw logic_error("The cell is not an operation");
	}
	size_t size = noqbs();
	Qubo aQubo;
	for (size_t atCell = 0; atCell < size; atCell++)
	{
		Qop::Sp pCellOp = dynamic_pointer_cast<Qop>(logic[atCell]);
		if (pCellOp != nullptr)
			aQubo += pCellOp->qubo(finalized);
		else
			throw logic_error("The cell is not an operation");
	}
	return aQubo;
}

void QnaryOp::solutions(const Qsolver::Samples& samples)
{
	Qop::solutions(samples);
}

string QnaryOp::solution(size_t sampleId) const
{
	return Qop::solution(sampleId);
}
