#pragma once
//
//  UTest.h
//  d5o_ta
//
//  Created by Nebojsa Vojinovic on 2024-04-13.
//

#ifndef UTest_h
#define UTest_h

#include <iostream>

using namespace std;

namespace dann5test
{
    // Unit test base class runs tests within separate thread
    // and output is saved into a specific .log file.
    // The ouput is compared with a content of the file with extension 
    // _baseline.log, if present.
    class UTest
    {
    public:
        UTest();
        // Creates a running thread to run the test and returns the status
        // message at the end
        string run();

        // override to execute all unit tests
        virtual void runAll(ostream& out) = 0;

    protected:
        // Provides a unique prefix of the test file name
        virtual string filePrefix() = 0;

    private:
        static string mFileDirectory;   // default is 'test//log//'

        // Runs the test, writing the output into the file provided by filePath.
        // The ouput is compared with a content of the file with extension 
        // _baseline.log, if present. Returns the status message.
        string runTest(const string& filePath);

        string compareBaseline(const string& filePath);
    };
};

#endif // UTest_h