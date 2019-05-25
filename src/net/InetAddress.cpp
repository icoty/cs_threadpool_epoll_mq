 ///
 /// @file    InetAddress.cpp
 /// @author  https://icoty.github.io
 ///
#include "Log4func.hpp"
#include "InetAddress.hpp"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

namespace yangyu
{
//??????????????
static_assert(sizeof(InetAddress) == sizeof(struct sockaddr_in), "InetAddress Error");

InetAddress::InetAddress(unsigned short port)
{
	bzero(&_addr,sizeof _addr);
	_addr.sin_family = AF_INET;
	_addr.sin_addr.s_addr = INADDR_ANY;
	_addr.sin_port = htons(port);
}

InetAddress::InetAddress(const string &ip, unsigned short port)
{
	bzero(&_addr,sizeof _addr);
	_addr.sin_family = AF_INET;
	_addr.sin_port = htons(port);
	if(inet_aton(ip.c_str(), &_addr.sin_addr) == 0) //新方法
	{
		LogError("stderr ip invalid!");
		exit(EXIT_FAILURE);
	}
}

InetAddress::InetAddress(const struct sockaddr_in &addr)
:_addr(addr)
{ }

string InetAddress::toIp() const
{
	return inet_ntoa(_addr.sin_addr);//字符串
}

unsigned short InetAddress::toPort() const
{
	return ntohs(_addr.sin_port);
}

const struct sockaddr_in * InetAddress::getSockAddrInet()const
{
	return & _addr;
}

}
