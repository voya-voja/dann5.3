//
//  Qcompiler.h
//  d5o_Xc
//
//  Created by Nebojsa Vojinovic on 2023-11-12.
//

#ifndef Qcompiler_h
#define Qcompiler_h

namespace dann5
{
    class Qop;

    class Qcompiler
    {
    public:
        Qcompiler() {};
        
        virtual void compile(const Qop&) = 0;

    protected:
    private:
    };
};

#endif /* Qcompiler_h */
