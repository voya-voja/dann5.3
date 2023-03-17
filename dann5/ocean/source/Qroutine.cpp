#include <math.h> 
#include <Qroutine.h>
#include <Utility.h>

#include <Logger.h>

using namespace dann5::ocean;

/*** QuboEquation ***/
Qubo Qroutine::qubo(bool finalized, size_t forBit) const
{
	return(mBlock.qubo(finalized, forBit));
}

string Qroutine::toString(bool decomposed, size_t forBit) const
{
	return(id() + ": " + mBlock.toString(decomposed, forBit));
}

Qroutine& Qroutine::operator<<(const Qstatement& statement)
{
	mBlock << statement;
	return(*this);
}

Qroutine Qroutine::operator<<(const Qstatement& statement) const
{
	Qroutine result(*this);
	result.block() << statement;
	return(result);
}


Qroutine::CommaOp Qroutine::operator=(const Qstatement& statement)
{
	mBlock << statement;
	return(CommaOp(this));
}

ostream& dann5::ocean::operator << (std::ostream& out, const Qroutine& r)
{
	out << r.toString();
	return out;
}