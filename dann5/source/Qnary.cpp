#include <Qnary.h>
#include <Qbit.h>

using namespace dann5;

const Bits Qnary::cUnknown = ULLONG_MAX;
const string Qnary::cUnknownSign = "U";

Qnary::Qnary(const string& id, const Qcells& cells)
	:Qdef(id)
{
	initCells(cells);
}

Qnary::Qnary(const Qnary& right) 
	:Qdef(right) 
{
	initCells(right.mCells);
}

inline void Qnary::initCells(const Qcells& cells)
{
	for (auto pCell : cells)
		mCells.push_back(static_pointer_cast<Qcell>(pCell->clone()));
}

Qnary::operator Bits()
{
	return(operator const Bits());
}

Qnary::operator const Bits() const
{
	if (any()) return cUnknown;

	Bits bits(noqbs());
	size_t at = 0;
	for (auto cell : mCells)
	{
		bits[at] = cell->value();
		at++;
	}
	return(bits);
}

Qnary::operator QdefVector()
{
	return(operator const QdefVector());
}

Qnary::operator const QdefVector() const
{
	QdefVector vector(noqbs());
	size_t at = 0;
	for (auto cell : mCells)
	{
		vector(at) = cell;
		at++;
	}
	return(vector);
}

Qcell::Sp Qnary::create(Qvalue value, size_t atLevel) const
{
	return Qcell::Sp(new Qbit(Qdef::id() + to_string(atLevel), value));
}

void Qnary::id(const string& id)
{
	Qdef::id(id);
	size_t at = 0;
	for (auto pCell : mCells)
	{
		pCell->id(id + to_string(at));
		at++;
	}
}

void Qnary::resize(size_t size, Qvalue value)
{
    Qvalue aValue = value;
    size_t oSize = mCells.size();

    if(size > oSize && aValue == cSuperposition && noqbs() != 0 && !any())
        aValue = 0; // this is a constant

    mCells.resize(size);
	for (size_t at = oSize; at < size; at++)
		mCells[at] = create( aValue, at);
}

bool Qnary::all(Qvalue value) const
{
	size_t at = 0, size = mCells.size();
    if(size == 0) return false;
	while (at < size && mCells[at]->value() == value) at++;
	return(at == size);
}

bool Qnary::any(Qvalue value) const
{
	size_t at = 0, size = mCells.size();
	while (at < size && mCells[at]->value() != value) at++;
	return(at < size);
}

bool Qnary::none(Qvalue value) const
{
	size_t at = 0, size = mCells.size();
	while (at < size && mCells[at]->value() != value) at++;
	return(at == size);
}

size_t Qnary::count(Qvalue value) const
{
	size_t count = 0;
	for (auto v : mCells)
		if (v->value() == value) count++;
	return count;
}

string Qnary::toString(bool decomposed, size_t forBit) const
{
	string valueStr = "";
	size_t size = noqbs();
	if (decomposed)
	{
		if(forBit == cAllBits)
		{
			for(size_t at = size; at > 0; at--)
				valueStr += mCells[at - 1]->toString(decomposed) + ";";
		}
		else
		{
			return (*this)[forBit]->toString(decomposed);
		}
	}
	else if(size > 0)
	{
		valueStr = "0";
		if (!all(0))
		{
			valueStr = cUnknownSign;
			Bits value = (*this);
			if (value != cUnknown)
			{
				valueStr = value.to_string();
				valueStr = valueStr.substr(value.size() - size);
			}
		}
	}
	string id = Qdef::toString(decomposed);
	return id + "\\" + to_string(size) + "b:" + valueStr + "\\";
}

void Qnary::add(const Qevaluations& evaluations)
{
	Qcells& _cells = cells();
	for (auto pCell : _cells)
		pCell->add(evaluations);
}

string Qnary::solution(size_t atEvltn) const
{
	const Qcells& _cells = cells();
	size_t size = noqbs();
	Bits value;
	for (size_t at = 0; at < size; at++)
		value[at] = _cells[at]->solutionValue(atEvltn);

	string valueStr = value.to_string();
	valueStr = valueStr.substr(value.size() - size);
	string id = Qdef::toString();
	return id + "\\" + to_string(size) + ":" + valueStr + "\\";
}

void Qnary::reset()
{
	Qcells& _cells = cells();
	for (auto pCell : _cells)
		pCell->reset();
}

Qcell::Sp Qnary::operator[](size_t pos) const noexcept
{
	size_t size = mCells.size();
	if (pos >= size)
		return Qcell::Sp(new Value0cell(Qdef::id() + to_string(pos)));
	/*		throw invalid_argument("Position " + to_string(pos) +
			" is out of range for " + toString());
*/
	return mCells[pos]; 
}

Qcell& Qnary::operator[](size_t pos) 
{ 
	size_t size = mCells.size();
	if (pos >= size)
		resize(pos + 1);
	return *mCells[pos];
}

Qnary::Sp Qnary::operator<<(size_t pos) const
{
	Qnary::Sp result = static_pointer_cast<Qnary>(clone());
	(*result) <<= pos;
	return result;
}

Qnary& Qnary::operator<<=(size_t pos)
{
	size_t s = noqbs();
	for (size_t at = s; at > 0; at--)
	{
		if (at - 1 >= pos)
			mCells[at - 1] = create(mCells[at - pos - 1]->value(), at - 1);
		else
			mCells[at - 1] = create(cSuperposition, at - 1);
	}
	return (*this);
}

Qnary::Sp Qnary::operator>>(size_t pos) const
{
	Qnary::Sp result = static_pointer_cast<Qnary>(clone());
	(*result) >>= pos;
	return result;
}

Qnary& Qnary::operator>>=(size_t pos)
{
	size_t size = mCells.size();
	for (size_t at = 0; at < size; at++)
	{
		if (at < size - pos)
			mCells[at] = create(mCells[at + pos]->value(), at);
		else
			mCells[at] = create(cSuperposition, at);
	}
	return (*this);
}

Qnary::Sp Qnary::operator<<(Qvalue value) const
{
	Qnary::Sp result = static_pointer_cast<Qnary>(clone());
	(*result) <<= value;
	return result;
}

Qnary& Qnary::operator<<=(Qvalue value)
{
	(*this) <<= size_t(1);
	mCells[0]->value(value);
	return (*this);
}

Qnary::Sp Qnary::operator>>(Qvalue value) const
{
	Qnary::Sp result = static_pointer_cast<Qnary>(clone());
	(*result) >>= value;
	return result;
}

Qnary& Qnary::operator>>=(Qvalue value)
{
	(*this) >>= size_t(1);
	mCells[mCells.size() - 1]->value(value);
	return (*this);
}
