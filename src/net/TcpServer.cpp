///
/// @file    TcpSever.cpp
/// @author  https://icoty.github.io
///
#include "Log4func.hpp"
#include "TcpServer.hpp"

namespace yangyu
{

int createSocketFd()
{
	int fd = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);//????????????
	if(fd == -1)
	{
		LogError("stderr create socket fd error!");
		exit(EXIT_FAILURE);
	}

	return fd;
}

TcpServer::TcpServer(const string & ip, unsigned short port)
: _addr(ip, port)
, _sockfd(createSocketFd())
, _poller(_sockfd.fd())
{
	_sockfd.setTcpNoDelay(false);
	_sockfd.setReusePort(true);
	_sockfd.setReuseAddr(true);
	_sockfd.setKeepAlive(false);

	_sockfd.bindAddress(_addr);
	_sockfd.listen();
}

}
