#include <Logger.h>
#include <Utility.h>

using namespace dann5;

Logger& Logger::Instance() 
{
	if (mpSingleton.get() == nullptr)
		mpSingleton.reset(new Logger);
	return(*mpSingleton);
}

#ifdef _DANN5_LOG_INTO_CSV
	#define _DANN5_LOG mLogFile
#else
	#ifdef _DANN5_LOG_INTO_TXT
		#define _DANN5_LOG mLogFile
	#else
		#define _DANN5_LOG std::cout
	#endif
#endif

Logger::Logger()
{
#ifdef _DANN5_LOG_INTO_CSV
	mLogFile.open("dann5_log.csv");
	mLogFile << "Object Type, Action, Address, Text, countIf(C:C_ C2), =COUNTIFS(B:B_\" deallocate\"_C:C_C2)" << endl;
#endif
#ifdef _DANN5_LOG_INTO_TXT
	mLogFile.open("dann5_log.txt");
	mLogFile << "Object Type, Action, Address, Text, countIf(C:C_ C2), =COUNTIFS(B:B_\" deallocate\"_C:C_C2)" << endl;
#endif
	log_alloc(this, typeid(this).name());
}

Logger::~Logger()
{
	log_alloc(this, typeid(this).name(), true);
#ifdef _DANN5_LOG_INTO_CSV
	mLogFile.close();
#endif
}

void Logger::log_alloc(const void* pObject, const char* oType, bool deallocated)
{
	string typeStr(oType);
#ifdef _DANN5_LOG_INTO_CSV
	replaceAll(typeStr, ",", "_");
#endif
	const char* typeTxt = typeStr._Unchecked_begin();
	_DANN5_LOG << "'" << typeTxt << "', " << (deallocated ? "deallocate" : "allocate") << ", "
		<< pObject << endl;
}

void Logger::log_alloc(const void* pObject, const char* oType, const string& txt, bool deallocated)
{
	string typeStr(oType);
	string txtStr(txt);
#ifdef _DANN5_LOG_INTO_CSV
	replaceAll(typeStr, ",", "_");
	replaceAll(txtStr, ",", "_");
	replaceAll(txtStr, "\t", ";");
	replaceAll(txtStr, "\n", ";");
#endif
	const char* typeTxt = typeStr._Unchecked_begin();
	const char* pTxt = txtStr._Unchecked_begin();
	_DANN5_LOG << "'" << typeTxt << "', " << (deallocated ? "deallocate" : "allocate") << ", "
		<< pObject << ", '" << pTxt << "'" << endl;
}


void Logger::log(const void* pObject, const char* oType, const string& action, const string& txt)
{
	string typeStr(oType);
	string txtStr(txt);
	string actStr(action);
#ifdef _DANN5_LOG_INTO_CSV
	replaceAll(typeStr, ",", "_");
	replaceAll(txtStr, ",", "_");
	replaceAll(txtStr, "\t", ";");
	replaceAll(txtStr, "\n", ";");
	replaceAll(actStr, ",", "_");
	replaceAll(actStr, "\t", ";");
	replaceAll(actStr, "\n", ";");
#endif
	const char* typeTxt = typeStr._Unchecked_begin();
	const char* pTxt = txtStr._Unchecked_begin();
	const char* actTxt = actStr._Unchecked_begin();
	_DANN5_LOG << "'" << typeTxt << "', " << actTxt << ", " << pObject << ", '" << pTxt << "'" << endl;
}
