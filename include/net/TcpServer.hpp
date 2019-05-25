///
/// @file    TcpSever.hpp
/// @author  https://icoty.github.io
///
#ifndef __YANGYU_TCPSERVER_H__
#define __YANGYU_TCPSERVER_H__

#include "Noncopyable.hpp"
#include "EpollPoller.hpp"
#include "Socket.hpp"
#include "InetAddress.hpp"
#include <string>
using std::string;

namespace yangyu
{

class TcpServer : Noncopyable
{
public:
	typedef EpollPoller::EpollCallback TcpServerCallback;

	TcpServer(const string & ip, unsigned short port);

	void start()
	{ _poller.loop(); }

	void setConnectCallback(TcpServerCallback cb)
	{ _poller.setConnectCallback(std::move(cb)); }
	void setMessageCallback(TcpServerCallback cb)
	{ _poller.setMessageCallback(std::move(cb)); }
	void setCloseCallback(TcpServerCallback cb)
	{ _poller.setCloseCallback(std::move(cb)); }

private:
	InetAddress _addr;
	Socket _sockfd;
	EpollPoller _poller;
};

}
#endif
