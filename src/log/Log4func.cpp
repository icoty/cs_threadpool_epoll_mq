///
/// @file    Log4func.cpp
/// @author  https://icoty.github.io
///
#include "Log4func.hpp"
#include <log4cpp/OstreamAppender.hh>
#include <log4cpp/FileAppender.hh>
#include <log4cpp/PatternLayout.hh>
#include <log4cpp/Priority.hh>

Mylog * Mylog::_pInstance=NULL;

Mylog * Mylog::getInstance()
{
	if(NULL==_pInstance)
	{
		_pInstance = new Mylog;
	}
	return _pInstance;
}

void Mylog::destroy()
{
	if(_pInstance){
		Category::shutdown();
		delete _pInstance;
	}
}

Mylog::Mylog()
:_cat(Category::getRoot().getInstance("mycat"))
{
	PatternLayout *ptn1=new PatternLayout();
	ptn1->setConversionPattern("%d:%c %p %x:%m%n");
	PatternLayout *ptn2=new PatternLayout();
	ptn2->setConversionPattern("%d:%c %p %x:%m%n");

	OstreamAppender *osApp=new OstreamAppender("osApp",&cout);
	osApp->setLayout(ptn1);

	FileAppender *fileApp=new FileAppender("fileApp","./log/log4test.log");
	fileApp->setLayout(ptn2);

	_cat.addAppender(osApp);
	_cat.addAppender(fileApp);

	_cat.setPriority(Priority::DEBUG);
}

Mylog::~Mylog()
{}

void Mylog::warn(const string & msg)
{
	_cat.warn(msg.c_str());//c++11切换至c标准
}


void Mylog::debug(const string & msg)
{
	_cat.debug(msg.c_str());
}


void Mylog::info(const string & msg)
{
	_cat.info(msg.c_str());
}


void Mylog::error(const string & msg)
{
	_cat.error(msg.c_str());
}

