#include <Qmultiply.h>
#include <Qwhole.h>

using namespace dann5;

/*** Multiplication ***/

void Qmultiply::refreshOnInputs()
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
			QcellOp::Sp pOp = Factory<string, QcellOp>::Instance().create(Qand::cMark);
			Qbit out(pOp->createOutId());
            pOp->operands(out.clone(), {(*pLeft)[atRow].clone(), (*pRight)[atCol].clone()});
			xMatrix(atRow, atCol) = pOp;
		}
	}
	return xMatrix;
}

void Qmultiply::sumDiagonal(const QcellMatrix& matrix)
{
	// resize X cells to a # of diagonals + 1 = # of rows + # of cols of xMatrix
	Index size = matrix.rows() + matrix.cols();
	Qcells& xCells = cells();
	xCells.resize(size);

	// The right-most diagonal corresponding to 0 Qcell value has 1 element and
	// the offset = # of cols -1
	Index diagonalOffset = matrix.cols() - 1;
	// a stack of carry Q cells to be carried to sum of next Q cell
	Qdefs carryStack;	
	for (Index at = 0; at < size; at++)
	{
		// all of inputs to be summed up for at Q cell. 
		// It is initialized with carry Q cells, if any
		Qdefs ins(carryStack);
		carryStack.clear();	// reset carryStack to accumulate any carry Q cells

		// starting with right-most diagonal and inputs for at Q cell sum
		QcellMatrix diagonal = matrix.diagonal(diagonalOffset - at);
		Index nDiagElmns = diagonal.rows();
		for (Index atD = 0; atD < nDiagElmns; atD++)
		{
			ins.push_back(diagonal(atD));
		}

		// a Q addition to be used to sum up inputs for at Q cell
		Qaddition::Sp pAddition = nullptr;
		while (!ins.empty())
		{
			// initialize addition-inputs, with no more than 3 inputs
			Qdefs addIns;
			if (pAddition != nullptr)
				addIns.push_back(pAddition);
			do
			{
				addIns.push_back(ins.back());
				ins.pop_back();
			} while (!ins.empty() && addIns.size() < 3);

			if (addIns.size() == 1)
				xCells[at] = static_pointer_cast<Qcell>(addIns[0]);
			else
			{	// create addition object according to # of addition-inputs
				if (addIns.size() == 2)
					pAddition = static_pointer_cast<Qaddition>(Factory<string, QcellOp>::Instance().create(Qxor::cMark));
				else
					pAddition = static_pointer_cast<Qaddition>(Factory<string, QcellOp>::Instance().create(Qadder::cMark));
				pAddition->inputs(addIns);
				Qbit out(pAddition->createOutId());
				pAddition->output(out.clone());
				// stack carry-forward Q cell
				carryStack.push_back(as_const(*pAddition).carry());
				xCells[at] = pAddition;
			}
		}
	}
}
