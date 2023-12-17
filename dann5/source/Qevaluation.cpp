#include <Qevaluation.h>

#include <iostream>
#include <utility>

using namespace dann5;

/**** Q evaluation ****/
Qevaluation& Qevaluation::operator+=(const Qevaluation& right)
{
    mEnergy += right.mEnergy;
    bool mergeable = true;
    Qsample::const_iterator dEnd = mSample.cend();
    for(auto elmntR : right.sample())
    {   // if left and right evaluation have the same element their values should be the same
        Qsample::const_iterator at = mSample.find(elmntR.first);
        bool dffrntElmnt =  at == dEnd;
        mergeable = dffrntElmnt || (*at).second == elmntR.second;
        if(!mergeable)
        {
            reset();
            break;
        }
        if(dffrntElmnt) // when mergable and different element
            mSample[elmntR.first] = elmntR.second;
    }
    return(*this);
}

Qevaluation Qevaluation::operator+(const Qevaluation& right) const
{
    Qevaluation rslt(*this);
    rslt += right;
    return(rslt);
}

Qevaluations dann5::operator+(const Qevaluations& left, const Qevaluations& right)
{
    Qevaluations evaltns;
    for(auto evltnL : left)
        for(auto evltnR : right)
        {
            Qevaluation evltn = evltnL + evltnR;
            if(evltn.isValid())
                evaltns.push_back(evltn);
        }
    return evaltns;
}

std::ostream& dann5::operator << (std::ostream& out, const Qevaluations& evaluations)
{
    bool tableHeader = true;
    for (auto evltn : evaluations)
    {
        out << endl;
        if (tableHeader)
        {
            for (auto element : evltn.sample())
                out << element.first << "\t";
            out << endl;
            tableHeader = false;
        }
        for (auto element : evltn.sample())
            out << to_string(element.second) << "\t";
        out << "-->\t" << evltn.energy();
    }
    return out;
}


