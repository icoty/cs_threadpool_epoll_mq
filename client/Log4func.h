///
/// @file    Log4func.h
/// @author  https://icoty.github.io
///

#ifndef __LOG4FUN_H__
#define __LOG4FUN_H__

#include <iostream>
#include <string>
#include <log4cpp/Category.hh>

using std::cout;
using std::endl;
using std::string;
using namespace log4cpp;

class Mylog{
public:
	static Mylog * getInstance();
	static void destroy();

	void warn(const string & msg);
	void error(const string & msg);
	void info(const string & msg);
	void debug(const string & msg);
private:
	Mylog();
	~Mylog();
private:
	static Mylog * _pInstance;
	Category &_cat;
};

inline string int2str(int num){
	std::ostringstream oss;
	oss << num;
	return oss.str();
}

 #define postfix(msg) \
	string(msg).append("[").append(__FILE__)\
			   .append(":").append(__FUNCTION__)\
			   .append(":").append(int2str(__LINE__)).append("]")

inline void logWarn(const string & msg){
	Mylog * plog = Mylog::getInstance();
	plog->warn(msg);
}

inline void logInfo(const string & msg){
	Mylog * plog = Mylog::getInstance();
	plog->info(msg);
}

inline void logDebug(const string & msg){
	Mylog * plog = Mylog::getInstance();
	plog->debug(msg);
}

inline void logError(const string & msg){
	Mylog * plog = Mylog::getInstance();
	plog->error(msg);
}

#define LogWarn(msg) logWarn(postfix(msg))
#define LogError(msg) logError(postfix(msg))
#define LogInfo(msg) logInfo(postfix(msg))
#define LogDebug(msg) logDebug(postfix(msg))

#endif

