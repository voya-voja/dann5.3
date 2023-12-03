#include <math.h> 
#include <Qroutine.h>
#include <Utility.h>
#include <QlogicCompiler.h>

#include <Logger.h>

using namespace dann5;

/*** Qroutine ***/
string Qroutine::declaration() const
{
    string dclrtn = mName + " ";
    
    size_t size = mArguments.size();
    if(size > 1) dclrtn += "(";
    for(size_t at = 0; at < size; at++)
    {
        dclrtn += mArguments[at]->toString();
        if(at != size - 1) dclrtn += ", ";
    }
    if(size > 1) dclrtn += ")";
    return dclrtn;
}

string Qroutine::toString(bool decomposed, size_t forBit) const
{
    if(!decomposed)
    {
        return(declaration() + ": " + mLogic.toString(decomposed, forBit));
    }
    return(mLogic.toString(decomposed, forBit));
}

void Qroutine::add(const Qevaluations& evaluations)
{
    mArguments.add(evaluations);
    mLogic.add(evaluations);
    Qstatement::add(evaluations);
}

void Qroutine::reset()
{
    mArguments.reset();
    Qstatement::reset();
}

void Qroutine::arguments(const Qdefs& args)
{
    for(auto pArg: args)
        mArguments << *pArg;
}

Qroutine& Qroutine::operator<<(const Qdef& arg)
{
    mArguments << arg;
    return(*this);
}

Qroutine Qroutine::operator<<(const Qdef& arg) const
{
    Qroutine result(*this);
    result << arg;
    return(result);
}

Qroutine& Qroutine::operator<<(const Qstatement& statement)
{
	mLogic << statement;
	return(*this);
}

Qroutine Qroutine::operator<<(const Qstatement& statement) const
{
	Qroutine result(*this);
	result << statement;
	return(result);
}

Qroutine::CommaOp Qroutine::operator=(const Qstatement& statement)
{
	mLogic << statement;
	return(CommaOp(this));
}

ostream& dann5::operator << (std::ostream& out, const Qroutine& r)
{
	out << r.toString();
	return out;
}
