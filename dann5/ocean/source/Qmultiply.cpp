#include <Qmultiply.h>
#include <Qwhole.h>

using namespace dann5::ocean;

/*** Multiplication ***/

void Qmultiply::refresh()
{
	const Qdefs& args = Qop::inputs();
	QcellMatrix xMatrix = x(args);	// this * right vectors => xMatrix
	// Rotate xMatrix left-90-digrees to change the values on main diagonal
	QcellMatrix matrix = xMatrix.rowwise().reverse();
	sumDiagonal(matrix);
}

QcellMatrix Qmultiply::x(const Qdefs& args) const
{
	Qnary::Sp pLeft = dynamic_pointer_cast<Qnary>(args[0]),
		pRight = dynamic_pointer_cast<Qnary>(args[1]);
	size_t rows = pLeft->noqbs(), columns = pRight->noqbs();
	QcellMatrix xMatrix(rows, columns);
	for (size_t atRow = 0; atRow < rows; atRow++)
	{
		for (size_t atCol = 0; atCol < columns; atCol++)
		{
			QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(AndQT::cMark);
			pOp->inputs({ (*pLeft)[atRow].clone(), (*pRight)[atCol].clone() });
			Qbit out(pOp->outId());
			pOp->output(out.clone());
			xMatrix(atRow, atCol) = pOp;
		}
	}
	return xMatrix;
}

void Qmultiply::sumDiagonal(const QcellMatrix& matrix)
{
	// resize this vector to a # of diagonals + 1 of xMatrix
	Index size = matrix.rows() + matrix.cols();
	Qcells& xCells = cells();
	xCells.resize(size);
	Index last = size - 1;

	// Value of 0 Q bit is the sum of right-most diagonal
	Index diagonalOffset = matrix.cols() - 1;

	// add functional object to add diagonal vectors' bit symbols
	Qaddition::Sp pAddition = nullptr;
	for (Index at = 0; at < last; at++)
	{
		QcellMatrix diagonal = matrix.diagonal(diagonalOffset - at);
		Index nDiagElmns = diagonal.rows();
		for (Index atD = 0; atD < nDiagElmns; atD++)
		{
			if (atD == 0)
			{
				xCells[at] = diagonal(atD);
			}
			else
			{
				if (pAddition == nullptr)
				{	// if there is no previous addition use Qxor
					pAddition = dynamic_pointer_cast<Qaddition>(Factory<string, QcellOp>::Instance().create(XorQT::cMark));
					pAddition->inputs({ xCells[at], diagonal(atD) });
				}
				else
				{	// add any carry-forward operand using Qadder
					Qaddition::Carry::Sp pCarry = as_const(*pAddition).carry();
					pAddition = dynamic_pointer_cast<Qaddition>(Factory<string, QcellOp>::Instance().create(AdderQT::cMark));
					pAddition->inputs({ xCells[at], diagonal(atD), pCarry });
				}
				Qbit out(pAddition->outId());
				pAddition->output(out.clone());
				xCells[at] = pAddition;
			}
		}
	}
	if (pAddition != nullptr)
	{	// add a previous carry Q bit to the 'last - 1' cell
		Qaddition::Carry::Sp pCarry = as_const(*pAddition).carry();
		pAddition = dynamic_pointer_cast<Qaddition>(Factory<string, QcellOp>::Instance().create(XorQT::cMark));
		pAddition->inputs({ xCells[last - 1], pCarry });
		xCells[last - 1] = pAddition;
		// last Q bit of result is a carry Q bit of previous addition
		xCells[last] = as_const(*pAddition).carry();
	}
	else
	{
		xCells.resize(last);
	}
}
