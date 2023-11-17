#pragma once
#include <utility>
#include <memory>

#include <iostream>
#include <fstream>
using namespace std;

namespace dann5 {
	class Logger
	{
	public:
		static Logger& Instance();

		void log_alloc(const void* pObject, const char* oType, bool deallocated = false);
		void log_alloc(const void* pObject, const char* oType, const string& txt, bool deallocated = false);
		void log(const void* pObject, const char* oType, const string& action, const string& txt);

	protected:
	private:
		static unique_ptr<Logger> gpSingleton;

		Logger();
		~Logger();

		ofstream mLogFile;

		friend class unique_ptr<Logger>;
                friend struct default_delete<Logger>;
	};
};

#ifdef _USE_DANN5_LOGGER
	#define _lc dann5::Logger::Instance().log_alloc(this, typeid(this).name())
	#define _ld dann5::Logger::Instance().log_alloc(this, typeid(this).name(), true)
	
	#define _lct(t) dann5::Logger::Instance().log_alloc(this, typeid(this).name(), t)
	#define _ldt(t) dann5::Logger::Instance().log_alloc(this, typeid(this).name(), t, true)

	#define _la(a)    dann5::Logger::Instance().log(this, typeid(this).name(), a, "")
	#define _lat(a,t) dann5::Logger::Instance().log(this, typeid(this).name(), a, t)

#else
	#define _lc 
	#define _ld 

	#define _lct(t) 
	#define _ldt(t) 

	#define _la(a)
	#define _lat(a,t)
#endif
