 ///
 /// @file    Socket.hpp
 /// @author  https://icoty.github.io
 ///
#ifndef __YANGYU_SOCKET_H__
#define __YANGYU_SOCKET_H__

#include "Noncopyable.hpp"
#include "InetAddress.hpp"

namespace yangyu
{

typedef struct sockaddr SA;

class Socket:Noncopyable{
public:
	explicit Socket(int sockfd);
	~Socket();

	//TcpServer类于main.cc中完成此部分工作
	//void ready(const InetAddress &);
	int fd()const;
	void bindAddress(const InetAddress &addr);
	void listen();
	int accept();
	
	void shutdownWrite();
	
	void setTcpNoDelay(bool on);
	void setReusePort(bool on);
	void setReuseAddr(bool on);
	void setKeepAlive(bool on);

	static InetAddress getLocalAddr(int sockfd);
	static InetAddress getPeerAddr(int sockfd);
private:
	const int _sockfd;
};

}
#endif
