#include <math.h> 
#include <Qstatement.h>
#include <Utility.h>

#include <Logger.h>

using namespace dann5::ocean;

/*** QuboEquation ***/


Qstatement::Qstatement()
	:mpExpression(nullptr)
{
//	mpReduct = createReduct();
}

Qstatement::Qstatement(const Qstatement& right)
	: mpExpression(right.mpExpression)
{
/*	mpReduct = createReduct();
	reduct().init();
*/}

Qstatement::Qstatement(const Qexpression& expression)
	: mpExpression(Qexpression::Sp(expression.clone()))
{
/*	mpReduct = createReduct();
	reduct().init();
*/}

Qstatement::~Qstatement()
{
//	delete mpReduct;
}


Qubo Qstatement::createRawQubo(bool finalized, Index level) const
{
	bool iterateAll = level == Eigen::Infinity;
	Index last = (iterateAll ? noqbs() - 1 : level);
	Qubo rawQubo;
	for (Index at = 0; at <= last; at++)
	{
/*		if (iterateAll || (!iterateAll && at == level))
		{
			Qubo qubo = expression()(at)->qubo(finalized);
			rawQubo += qubo;
		}
*/	}
	return rawQubo;
}

Qubo Qstatement::qubo(bool finalized, Index level) const
{
	Qubo rawQubo = createRawQubo(finalized, level);
	return(reduceRawQubo(rawQubo, finalized));
}

Qubo Qstatement::reduceRawQubo(Qubo& rawQubo, bool finalized) const
{
	Qubo qubo = initQubo(finalized);	// Initialize Qubo object
	for (auto at = rawQubo.begin(); at != rawQubo.end(); at++)
	{
/*		// remove all quadratic elements with bias 0
		if (((*at).first.first != (*at).first.second) && ((*at).second == 0))
			continue;

		Qkey key = (*mpReduct)((*at).first, finalized);
		if (key != Reduct::cSkip)
		{
			// Using a correct key add bias for the buinary quadratic element in updated Qubo 
			qubo[key] += (*at).second;
		}
*/	}
	return(qubo);	// return updated Qubo
}

/*
Qstatement& Qstatement::operator=(const Qstatement& right)
{
	arguments().clear();
	arguments().insert(arguments().end(), right.arguments().begin(), right.arguments().end());
	expression() = right.expression();
	Index size = expression().nobs();
	reduct().init();
	return(*this);
}

void Qstatement::reduct(Reduct* pReduct) 
{
	if (mpReduct != nullptr)
		delete mpReduct;
	mpReduct = pReduct; 
}

void Qstatement::add(Qsolver::Sample& sample)
{
	mSolutions.push_back(sample);
}

void Qstatement::set(Qsolver::Samples& samples)
{
	_lat("solutions", to_string(samples.size()) + " of samples");
	mSolutions = samples;
}

string Qstatement::solutions() const
{
	string values("");
	for (auto sample : mSolutions)
	{
		const Qvars& args = arguments();
		for (auto arg : args)
		{
			Qdef definition = as_const(arg).definition();
			Qwhole v = as_const(arg).value();
			Qwhole value(v.nobs());
			if (v.value() != Qwhole::cUnknown)
			{
				value = v;
			}
			else
			{
				int atBit = 0;
				for (auto bitSymbol : definition)
				{
					value[atBit++] = sample[bitSymbol->identity()];
				}
			}
			values += definition.name() + " = " + value.toString() + "; ";
		}
		values += "\n";
	}
	_lat("solutions", values);
	return(values);
}
*/

/*** Qstatement::Reduct code***/
/*
const Qkey Qstatement::Reduct::cSkip("skip", "skip");

Qstatement::Reduct::Reduct(Qstatement& statement)
	:mStatement(statement)
{
	_lc;
}

Qstatement::Reduct::~Reduct()
{
	_ld;
}

void Qstatement::Reduct::init()
{
	// capture reductions for arguments with values different from gSuperposition (i.e. 0 or 1)
	for (auto atArg = mStatement.arguments().cbegin(); atArg != mStatement.arguments().cend(); atArg++)
	{
		for (Index at = 0; at < (*atArg).definition().nobs(); at++)
		{
			Qbit value = (*atArg).value()(at);
			if (value < 2)
			{
				string definition = (*atArg).definition()(at)->identity();
				Reduction reduction(definition, value);
				mReductions[definition] = reduction;
			}
		}
	}
}

Qkey Qstatement::Reduct::operator() (const Qkey& original, bool finalized) const
{
	Qkey key = original;
	Reductions::const_iterator found1stAt = mReductions.find(original.first);
	Reductions::const_iterator found2ndAt = mReductions.find(original.second);
	Reductions::const_iterator end = mReductions.cend();
	if (found1stAt != end)
	{
		// The 1st half of a key of a quadratic element  should be updated with replacement definition
		Reduction correct1st = (*found1stAt).second;
		if (found2ndAt != end)
		{
			// The 2nd half of a key of a quadratic element should be updated with replacement definition
			Reduction correct2nd = (*found2ndAt).second;
			if (!finalized || (correct1st.second == Qbit::gSuperposition && correct2nd.second == Qbit::gSuperposition))
			{
				// Always just replace 1st and 2nd symbols without applying condition
				// or replace 1st and 2nd symbols in binaryquadratic element when condition values are not defined (not 0 or 1)
				key = Qkey(correct1st.first, correct2nd.first);
			}
			else if (correct1st.second == Qbit::gSuperposition && correct2nd.second == 1)
			{
				// otherwise replace with a linear element using 1st replacement when its condition value is 1
				key = Qkey(correct1st.first, correct1st.first);
			}
			else if (correct1st.second == 1 && correct2nd.second == Qbit::gSuperposition)
			{
				// otherwise replace with a linear element using 2nd replacement when its condition value is 1
				key = Qkey(correct2nd.first, correct2nd.first);
			}
			else
			{
				// all other combinations should be skipped as result is 0 or 1 constant
				key = cSkip;
			}
		}
		else
		{
			// Just 1st half of a key of a quadratic element should be updated with replacement definition
			if (!finalized || correct1st.second == Qbit::gSuperposition)
			{
				// Always just replace 1st definition without applying condition
				// or replace 1st definition in binaryquadratic element when condition value is not defined (not 0 or 1)
				key = Qkey(correct1st.first, original.second);
			}
			else if (correct1st.second == 1)
			{
				// otherwise replace with a linear element using 2nd half of a key when condition value is 1
				key = Qkey(original.second, original.second);
			}
			else
			{
				// all other combinations should be removed as result is 0
				key = cSkip;
			}
		}
	}
	else if (found2ndAt != end)
	{
		// Just 2st half of a key of a quadratic element should be updated with replacement definition
		Reduction reduction = (*found2ndAt).second;
		if (!finalized || reduction.second == Qbit::gSuperposition)
		{
			key = Qkey(original.first, reduction.first);
		}
		else if (reduction.second == 1)
		{
			// otherwise replace with a linear element using 1st half of a key when condition value is 1
			key = Qkey(original.first, original.first);
		}
		else
		{
			// all other combinations should be removed as result is 0
			key = cSkip;
		}
	}
	return key;
}
*/