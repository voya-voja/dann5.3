#include <math.h> 
#include <Qroutine.h>
#include <Utility.h>

#include <Logger.h>

using namespace dann5;

/*** Qroutine ***/
void Qroutine::compile(Qcompiler& compile) const
{
	return(mBlock.compile(compile));
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

ostream& dann5::operator << (std::ostream& out, const Qroutine& r)
{
	out << r.toString();
	return out;
}
