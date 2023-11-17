#pragma once

#include <pybind11/pybind11.h>

#include <map>
#include <iostream>

#include <Qvalue.h>

using namespace std;

namespace dann5 {
    // A quantum sample is defined as a dictionary (map) of definition
    // nodes and their values. The node names are defined by coresponding
    // compiled quantum statement object sent to a quantum solver
    typedef map<string, Qvalue> Qsample;
    
    // A quantum evaluation is a pair of a sample and its assessed energy
    // by quantum solver for a compiled quantum statement
    class Qevaluation
    {
    public:
        constexpr static const double cOoRenergy = MAXFLOAT;   // Out of Range Energy
        
        // default constructor
        Qevaluation() : mEnergy(cOoRenergy) {};
        
        // copy constructor
        Qevaluation(const Qevaluation& right): mSample(right.mSample), mEnergy(right.mEnergy) {};
        
        // construct evaluation by pairing a sample and assessed energy
        Qevaluation(const Qsample& sample, double energy): mSample(sample), mEnergy(energy) {};
        Qevaluation(double energy, const Qsample& sample): mSample(sample), mEnergy(energy) {};
        
        // reset this evaluation to initial values
        void reset() {
            mEnergy = cOoRenergy;
            mSample.clear();
        }
        
        // the evaluation is valid if the sample set has elements
        bool isValid() { return mSample.size() > 0; };

        // merge this and right evaluation with same or different elements
        Qevaluation& operator+=(const Qevaluation& right);
        
        // return a quantum evaluation with merge elements from this and
        // right object
        Qevaluation operator+(const Qevaluation& right) const;
        
        // return sample object
        Qsample& sample() { return mSample; };
        const Qsample& sample() const { return mSample; };
        
        // return energy value
        double& energy() { return mEnergy; };
        double energy() const { return mEnergy; };

    protected:
    private:
        Qsample mSample;    // the evaluation sample
        double mEnergy;     // the assessed energy
    };

    // A list of quantum evaluations by quantum solver for a compiled quantum statement
    typedef vector<Qevaluation> Qevaluations;

    // merges two quantum evaluations vectors with same and/or different elements
    Qevaluations operator+(const Qevaluations& left, const Qevaluations& right);
    
    // Sends a representation of Qevaluations into output stream
    std::ostream& operator << (std::ostream&, const Qevaluations&);
};
