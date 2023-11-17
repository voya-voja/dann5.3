//
//  QuboCompiler.h
//  d5o_Xc
//
//  Created by Nebojsa Vojinovic on 2023-11-12.
//

#ifndef QuboCompiler_h
#define QuboCompiler_h

#include <Qcompiler.h>

#include <Qubo.h>
#include <QnaryOp.h>

namespace dann5{
    namespace ocean {
        class QuboCompiler : public Qcompiler
        {
        public:
            QuboCompiler(bool finalized = true) : mFinalized(finalized) {};
            
            virtual void compile(const Qop&);
            
            const Qubo& qubo() const { return mQubo; };
            
            void reset() { mQubo = Qubo(); };

            bool finalized() const { return mFinalized; };
            void finalized(bool fnlzd) {
                mFinalized = fnlzd;
                reset();
            };
            
        protected:
            virtual Qubo parse(const Qop& op, size_t forBit = cAllBits) const;
            virtual Qubo qubo(const Qop& op, size_t forBit = cAllBits) const;
            virtual Qubo qubo(const QnaryOp& op, size_t forBit = cAllBits) const;
            
        private:
            Qubo mQubo;
            bool mFinalized;
        };
    };
};
#endif /* QuboCompiler_h */
