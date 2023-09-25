#pragma once

#include <pybind11/pybind11.h>

#include <Qwhole.h>

namespace dann5 {
	namespace ocean {

        // Prime Number Generator
		class PNGen
		{
		public:
            PNGen(size_t noQbits);
            Qwhole opeator() { return execute(); };

            const Qwhole& prime() const { return mPrime; };
            const Qwhole& factor() const { return mFactor; };

		protected:
            virtual Qwhole execute() = 0;
            Qwhole execute(const Qstatement& qCode);

            size_t primeNoQbits() const { return mPrimeNoQbits; };
            size_t factorNoQbits() const { return mFactorNoQbits; };
        private:
            Qwhole mPrime;
            size_t mPrimeNoQbits;
            Qwhole mFactor;
            size_t mFactorNoQbits;
            const Qwhole m6;
            const Qwhole m1;
        };

        class PNGbase6algorithm : public PNGen
        {
        public:
        };
	};
};

