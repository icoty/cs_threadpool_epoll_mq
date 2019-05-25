///
/// @file    SockIO.cpp
/// @author  https://icoty.github.io
///
#include "SockIO.hpp"
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>

namespace yangyu
{

ssize_t SocketIO::readn(char *buf, size_t count)
{
	size_t nleft = count;    //剩余的字节数
	ssize_t nread;           //用作返回值
	char *bufp = (char*)buf; //缓冲区的偏移
	while(nleft > 0)
	{
		nread = ::read(_sockfd, bufp, nleft);
		if(nread == -1){
			if(errno == EINTR)
				continue;
			return -1;       // ERROR
		}else if(nread == 0) //EOF
			break;
		nleft -= nread;
		bufp += nread;
	}
	return (count - nleft);
}

ssize_t SocketIO::writen(const char *buf, size_t count)
{
	size_t nleft = count;
	ssize_t nwrite;
	const char *bufp = buf;
	while(nleft > 0)
	{
		nwrite = ::write(_sockfd, bufp, nleft);
		if(nwrite <= 0){
			if(nwrite == -1 && errno == EINTR)
				continue;
			return -1;
		}
		nleft -= nwrite;
		bufp += nwrite;
	}
	return count;
}

//预览内核缓冲区数据
ssize_t SocketIO::recv_peek(char *buf, size_t len)
{
	int nread;
	do{
		nread = ::recv(_sockfd, buf, len, MSG_PEEK);
	}while(nread == -1 && errno == EINTR);
	
	return nread;
}

ssize_t SocketIO::readline(char *usrbuf, size_t maxlen)
{
	size_t nleft = maxlen - 1;
	char *bufp = usrbuf;  //缓冲区位置
	size_t total = 0;     //读取的字节数
	ssize_t nread;
	while(nleft > 0)
	{
		//预读取
		nread = recv_peek(bufp, nleft);
		if(nread <= 0)
			return nread;

		//检查\n
		int i;
		for(i = 0; i < nread; ++i)
		{
			if(bufp[i] == '\n')
			{
				//找到\n
				size_t nsize = i+1;
				if(readn(bufp, nsize) != static_cast<ssize_t>(nsize))
					return -1;
				bufp += nsize;
				total += nsize;
				*bufp = 0;
				return total;
			}
		}

		//没找到\n
		if(readn(bufp, nread) != nread)
			return -1;
		bufp += nread;
		total += nread;
		nleft -= nread;
	}
	*bufp = 0;
	return maxlen - 1;
}

}
