///
/// @file    SockIO.hpp
/// @author  https://icoty.github.io
///
#ifndef __YANGYU_SOCKETIO_H__
#define __YANGYU_SOCKETIO_H__

#include "Noncopyable.hpp"
#include <sys/types.h>

namespace yangyu
{

class SocketIO : Noncopyable{
public: 
	explicit SocketIO(int sockfd)
	: _sockfd(sockfd)
	{ }

	ssize_t readn(char *buf, size_t count);
	ssize_t writen(const char *buf, size_t count);
	ssize_t readline(char *usrbuf, size_t maxlen);
private:
	ssize_t recv_peek(char *buf, size_t len);//??
	const int _sockfd;
};

}
#endif

