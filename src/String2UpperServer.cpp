 ///
 /// @file    String2UpperServer.cpp
 /// @author  https://icoty.github.io
 ///
#include "Log4func.hpp"
#include "FileName.hpp"
#include "String2UpperServer.hpp"
#include "String2Upper.hpp"
#include <unistd.h>
#include <string>
using std::string;

namespace
{

string AbsolutelyPath(const string &s)
{
	char *cur=getcwd(NULL,0);
	int loc=s.rfind('/');
	
	string path=s.substr(0,loc);
	chdir(path.c_str());
	string cfgAbsPath=getcwd(NULL,0);
	cfgAbsPath.append("/").append(s.substr(loc+1));
	chdir(cur);
	return cfgAbsPath;
}

void Trim(string &str)
{
	//去除首尾空格
	str[str.size()-1]=' ';//去除最后一个换行符
	str.erase(str.find_last_not_of(' ')+1);
	str.erase(0,str.find_first_not_of(' '));
	for(size_t idx=0;idx!=str.size();++idx)//转换成小写
		if(str[idx]>='A'&&str[idx]<='Z')
			str[idx]+=32;
}

}

namespace yangyu
{

String2UpperServer::String2UpperServer(const string &cfgFileName)
:_conf(AbsolutelyPath(cfgFileName))
,_wordquery(_conf)
,_threadpool(5,10) // 创建5个计算线程，消息队列大小为10，并发高的话可以把数值改大
,_tcpserver(_conf.getConfigMap()[IP],
		    atoi(_conf.getConfigMap()[PORT].c_str()))
{ }

void String2UpperServer::start()
{
	_threadpool.start();
	
	_tcpserver.setConnectCallback(std::bind(&String2UpperServer::onConnection,this,std::placeholders::_1));
	_tcpserver.setMessageCallback(std::bind(&String2UpperServer::onMessage,this,std::placeholders::_1));
	_tcpserver.setCloseCallback(std::bind(&String2UpperServer::onClose,this,std::placeholders::_1));

	_tcpserver.start();
}

void String2UpperServer::onConnection(TcpConnectionPtr conn)
{
	LogInfo(conn->toString() + ".");
	conn->send("hello, welcome to Chat Server.");
}

void String2UpperServer::onMessage(TcpConnectionPtr conn)
{
	std::string queryWord(conn->receive());
	Trim(queryWord);//预处理
	
	_threadpool.addTask(std::bind(&yangyu::String2UpperServer::doQuery,this,conn,queryWord));
}

void String2UpperServer::onClose(TcpConnectionPtr conn)
{
	LogInfo(conn->toString() + " close.");
}

void String2UpperServer::doQuery(TcpConnectionPtr & conn,const string & query)
{
	string result=_wordquery.doQuery(query);
	conn->sendInLoop(result);	
}

}

