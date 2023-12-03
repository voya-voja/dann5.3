#include <math.h> 
#include <Qroutine.h>
#include <Utility.h>
#include <QlogicCompiler.h>

#include <Logger.h>

using namespace dann5;

/*** Qroutine ***/
void Qroutine::compile(Qcompiler& compile) const
{
	return(mBlock.compile(compile));
}

string Qroutine::declaration() const
{
    Qdef::Sp pOut = Qop::output();
    string dclrtn = pOut->toString() + " " + id() + " ";
    
    Qnaries ins(Qop::inputs());
    size_t size = ins.size();
    if(size > 1) dclrtn += "(";
    for(size_t at = 0; at < size; at++)
    {
        dclrtn += ins[at]->toString();
        if(at != size - 1) dclrtn += ", ";
    }
    if(size > 1) dclrtn += ")";
    return dclrtn;
}

string Qroutine::toString(bool decomposed, size_t forBit) const
{
    if(!decomposed)
    {
        return(declaration() + ": " + mBlock.toString(decomposed, forBit));
    }
    return(mBlock.toString(decomposed, forBit));
}

void Qroutine::add(const Qevaluations& evaluations)
{
    mBinder.add(evaluations);
    mBlock.add(evaluations);
    QnaryOp::add(evaluations);
}

string Qroutine::solution(size_t atEvltn) const
{
//    string sltnStr = mBlock.solution(atEvltn);
//    sltnStr += mBinder.solution(atEvltn);
    string sltnStr = QnaryOp::solution(atEvltn);
    return sltnStr;
}

void Qroutine::reset()
{
    mBinder.reset();
    QnaryOp::reset();
}

Qroutine& Qroutine::operator<<(const Qstatement& statement)
{
	mBlock << statement;
    updateLogic();
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
	mBlock << statement;
    updateLogic();
	return(CommaOp(this));
}

void Qroutine::refreshOnInputs()
{
    Qdefs ins(Qop::inputs());
    for(auto pIn: ins)
        mBinder << *pIn;
}

void Qroutine::refreshOnOutput()
{
    mBinder << *Qop::output();
}

void Qroutine::updateLogic()
{
    QlogicCompiler compiler;
    mBlock.compile(compiler);
    cells(compiler.logic());
}

ostream& dann5::operator << (std::ostream& out, const Qroutine& r)
{
	out << r.toString();
	return out;
}
