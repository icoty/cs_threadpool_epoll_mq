 ///
 /// @file    String2UpperServer.hpp
 /// @author  https://icoty.github.io
 ///
#ifndef __YANGYU_WORDQUERYSERVER_H__
#define __YANGYU_WORDQUERYSERVER_H__

#include "TcpServer.hpp"
#include "Threadpool.hpp"
#include "TcpConnection.hpp"
#include "Configuration.hpp"
#include "String2Upper.hpp"
#include <memory>

namespace yangyu
{

class String2UpperServer
:public std::enable_shared_from_this<String2UpperServer>
{
public:
	String2UpperServer(const string &cfgFileName);
	void start();
	void onConnection(TcpConnectionPtr conn);
	void onMessage(TcpConnectionPtr conn);
	void onClose(TcpConnectionPtr conn);
	void doQuery(TcpConnectionPtr & conn,const string & query);
private:
	Configuration    _conf;
	String2Upper		_wordquery;
	Threadpool		_threadpool;
	TcpServer		_tcpserver;
};

}
#endif
