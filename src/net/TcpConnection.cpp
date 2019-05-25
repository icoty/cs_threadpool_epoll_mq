///
/// @file    TcpConnection.cpp
/// @author  https://icoty.github.io
///
#include "Log4func.hpp"
#include "TcpConnection.hpp"
#include "EpollPoller.hpp"

namespace yangyu
{

TcpConnection::TcpConnection(int sockfd,EpollPoller *p)
: _sockfd(sockfd)
,_sockIO(sockfd)
,_localAddr(Socket::getLocalAddr(sockfd))
,_peerAddr(Socket::getPeerAddr(sockfd))
,_isShutdownWrite(false)
,_epollPoller(p)
{ }

TcpConnection::~TcpConnection()
{
	if(!_isShutdownWrite)
	{
		shutdown();
	}
}

//conn->handleConnectCalback()
#if 1
void TcpConnection::handleConnectCallback()
{
	if(onConnectCallback_)
	{
		onConnectCallback_(shared_from_this());
	}
}

void TcpConnection::handleMessageCallback()
{
	if(onMessageCallback_)
	{
		onMessageCallback_(shared_from_this());
	}
}

void TcpConnection::handleCloseCallback()
{
	if(onCloseCallback_)
	{
		onCloseCallback_(shared_from_this());
	}
}
#endif

ssize_t TcpConnection::readn(char *buf, size_t count)
{
	ssize_t ret = _sockIO.readn(buf, count);
	if(ret == -1)
	{
		LogError("stderr TcpConnection readn!");
		exit(EXIT_FAILURE);
	}
	return ret;
}

ssize_t TcpConnection::writen(const char *buf, size_t count)
{
	ssize_t ret = _sockIO.writen(buf, count);
	if(ret == -1)
	{
		LogError("stderr TcpConnection writen!");
		exit(EXIT_FAILURE);
	}
	return ret;
}

ssize_t TcpConnection::readLine(char *usrbuf, size_t maxlen)
{
	ssize_t ret = _sockIO.readline(usrbuf, maxlen);
	if(ret == -1)
	{
		LogError("stderr TcpConnection readLine!");
		exit(EXIT_FAILURE);
	}
	return ret;
}

std::string TcpConnection::receive()
{
	char str[65535] = {0};//64kB
	int ret = readLine(str, sizeof str);
	if(ret == 0)
		return std::string();
	else
		return std::string(str);
}

void TcpConnection::send(const std::string &s)
{
	writen(s.c_str(), s.size());
}

void TcpConnection::sendInLoop(const std::string &s)
{
	_epollPoller->runInLoop(std::bind(&TcpConnection::send,shared_from_this(),s));
}

std::string TcpConnection::toString() const
{
	char text[100];
	snprintf(text, sizeof text, "%s:%d -> %s:%d", 
			_localAddr.toIp().c_str(),
			_localAddr.toPort(),
			_peerAddr.toIp().c_str(),
			_peerAddr.toPort());

	return text;
}

}
