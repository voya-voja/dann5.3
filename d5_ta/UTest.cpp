#include "UTest.h"

#include <fstream>
#include <thread>
#include <utility>
#include <future>
#include <string>

using namespace std;
using namespace dann5test;

string UTest::mFileDirectory = "test//log//";

UTest::UTest()
{
}

string UTest::run()
{
	string filePath = mFileDirectory + filePrefix() +"_test.log";
	future<string> ret = async(&UTest::runTest, this, filePath);
	return filePrefix() + "-test: " + ret.get();
}

string UTest::runTest(const string& filePath)
{
	ofstream outFstream;
	outFstream.open(filePath);
	bool fo = outFstream.is_open();
	if (!fo)
		return "ERROR: " + filePath + " could not create!";
	runAll(outFstream);
	outFstream.close();
	return compareBaseline(filePath);
}

string UTest::compareBaseline(const string& filePath)
{
	string baselineFilePath("test//" + filePrefix() + "_baseline.txt");
	ifstream baselineFstream;
	baselineFstream.open(baselineFilePath);
	if(!baselineFstream.is_open())
		return "OK: no baseline comparison!";
	else
	{
		ifstream logFstream;
		logFstream.open(filePath);
		if (!logFstream.is_open())
		{
			baselineFstream.close();
			return "ERROR: input " + filePath + " could not opened!";
		}
		string baseline, logline;
		size_t lineCounter = 0, previousCounter = -1, startCounter = -1;
		string errorLines("");
		while (getline(baselineFstream, baseline) && getline(logFstream, logline))
		{
			lineCounter++;
			if (baseline != logline)
			{
				previousCounter = lineCounter;
				if(startCounter == -1)
					startCounter = lineCounter;
			}
			else if (startCounter != -1)
			{
				if(previousCounter == startCounter)
					errorLines += to_string(startCounter) + ", ";
				else
					errorLines += to_string(startCounter) + "-" + to_string(previousCounter) + ", ";
				previousCounter = -1;
				startCounter = -1;
			}
		}
		if (errorLines != "")
			return "ERROR: lines " + errorLines;
		if(!baselineFstream.eof())
			return "ERROR: file " + baselineFilePath + " @ line " + to_string(lineCounter);
		else
		{
			getline(logFstream, logline);
			if (!logFstream.eof())
				return "ERROR: file " + filePath + " @ line " + to_string(lineCounter);
		}
	}

	return "OK";
}