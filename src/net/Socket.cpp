 ///
 /// @file    Socket.cpp
 /// @author  https://icoty.github.io
 ///
#include "Socket.hpp"
#include "Log4func.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>
#include <netdb.h>

namespace yangyu
{
	
Socket::Socket(int sockfd)
:_sockfd(sockfd)
{ }

Socket::~Socket()
{
	::close(_sockfd);
}

#if 0//tcpserver类在main.cc中完成此部分工作
void Socket::ready(const InetAddress &inetAddr)
{
	setReuseAddr(true);
	setReusePort(true);
	setKeepAlive(false);
	setTcpNoDelay(false);
	bindAddress(inetAddr);
	listen();
}
#endif
void Socket::bindAddress(const InetAddress &addr)
{
	if(::bind(_sockfd, (SA*)addr.getSockAddrInet(), sizeof(addr)) == -1)
	{
		LogError("stderr bind address error!");
		exit(EXIT_FAILURE);
	}
}

int Socket::accept()
{
	int fd = ::accept(_sockfd, NULL, NULL);//新方法
	if(fd == -1){
		LogError("stderr accept error!");
		exit(EXIT_FAILURE);
	}
	return fd;
}

void Socket::shutdownWrite()
{
	if(::shutdown(_sockfd, SHUT_WR) == -1)
	{
		LogError("stderr shutdown error!");
		exit(EXIT_FAILURE);
	}
}

void Socket::setTcpNoDelay(bool on)
{
	int optval = on ? 1 : 0;
	if(::setsockopt(_sockfd,   //????????
					IPPROTO_TCP,
					TCP_NODELAY,
					&optval, 
					static_cast<socklen_t>(sizeof optval)) == -1)
	{
		LogError("stderr setTcpNoDelay error!");
		exit(EXIT_FAILURE);
	}
}

void Socket::setReuseAddr(bool on)
{
	int optval = on ? 1 : 0;
	if(::setsockopt(_sockfd,
					SOL_SOCKET,
					SO_REUSEADDR,
					&optval, 
					static_cast<socklen_t>(sizeof optval)) == -1)
	{
		LogError("stderr setTcpNoDelay error!");
		exit(EXIT_FAILURE);
	}
}

//??????????????
void Socket::setReusePort(bool on)
{
#ifdef SO_REUSEPORT
	int optval = on ? 1 : 0;
	int ret = ::setsockopt(_sockfd,SOL_SOCKET,SO_REUSEADDR,&optval, 
							static_cast<socklen_t>(sizeof optval));
	if(ret < 0){
		LogError("stderr setTcpNoDelay error!");
		exit(EXIT_FAILURE);
	}
#else
	if(on)
		LogError("stderr SO_REUSEPORT is not supported!");
#endif
}

void Socket::setKeepAlive(bool on)
{
	int optval = on ? 1 : 0;
	if(::setsockopt(_sockfd,
					SOL_SOCKET,
					SO_KEEPALIVE,
					&optval, 
					static_cast<socklen_t>(sizeof optval)) == -1)
	{
		LogError("stderr setTcpNoDelay error!");
		exit(EXIT_FAILURE);
	}
}

InetAddress Socket::getLocalAddr(int sockfd)
{
	struct sockaddr_in addr;
	socklen_t len = sizeof addr;
	if(::getsockname(sockfd, (SA*)&addr, &len) == -1)//?????????????
	{
		LogError("stderr getLocalAddress!");
		exit(EXIT_FAILURE);
	}
	return InetAddress(addr);//返回本身，this指针
}

InetAddress Socket::getPeerAddr(int sockfd)
{
	struct sockaddr_in addr;
	socklen_t len = sizeof addr;
	if(::getpeername(sockfd, (SA*)&addr, &len) == -1)//?????????????
	{
		LogError("stderr getPeerAddress!");
		exit(EXIT_FAILURE);
	}
	return InetAddress(addr);//返回本身，this指针
}

void Socket::listen()
{
	 if(::listen(_sockfd, 5) == -1)
	 {
	 	LogError("stderr listen address error!");
	 	exit(EXIT_FAILURE);
	 }
}

int Socket::fd() const 
{ 
	return _sockfd; 
}

}
