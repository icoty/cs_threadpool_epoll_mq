///
/// @file    EpollPoller.cpp
/// @author  https://icoty.github.io
///
#include "Log4func.hpp"
#include "EpollPoller.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <errno.h>
#include <assert.h>
#include <sys/eventfd.h>

//匿名空间，与系统匿名空间有何区别？？
//非类内成员，供类内调用，辅助初始化
namespace
{

int createEpollFd()  //free函数  全局函数
{
	int epollfd = ::epoll_create1(0);//?????????
	if(epollfd == -1)
	{
		LogError("create epoll fd error!");
		exit(EXIT_FAILURE);
	}
	return epollfd;
}

int createEventfd()
{
	int fd = ::eventfd(0,0);
	if(-1 == fd){
		LogError("create event fd error!");
		exit(EXIT_FAILURE);
	}
	return fd;
}

void addEpollReadFd(int epollfd, int fd)
{
	struct epoll_event ev;
	ev.data.fd = fd;
	ev.events = EPOLLIN;
	if(epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &ev) == -1)
	{
		LogError("add epoll fd error!");
		exit(EXIT_FAILURE);
	}
}

void delEpollReadFd(int epollfd, int fd)
{
	struct epoll_event ev;
	ev.data.fd = fd;
	//ev.events = EPOLLIN;
	if(epoll_ctl(epollfd, EPOLL_CTL_DEL, fd, &ev) == -1)
	{
		LogError("del epoll fd error!");
		exit(EXIT_FAILURE);
	}
}

int acceptConnFd(int listenfd)
{
	int peerfd = ::accept(listenfd, NULL, NULL);
	if(peerfd == -1)
	{
		LogError("accept conn fd!");
		exit(EXIT_FAILURE);
	}
	return peerfd;
}

//预览数据
ssize_t recvPeek(int sockfd, void *buf, size_t len)
{
	int nread;
	do
	{
		nread = ::recv(sockfd, buf, len, MSG_PEEK);
	}
	while(nread == -1 && errno == EINTR);

	return nread;
}

//通过预览数据 判断conn是否关闭
bool isConnectionClosed(int sockfd)
{
	char buf[1024];
	ssize_t nread = recvPeek(sockfd, buf, sizeof buf);
	if(nread == -1)
	{
		LogError("recvPeek!");
	}

	return (nread == 0);
}

}//end anonymous namespace


namespace yangyu
{

EpollPoller::EpollPoller(int listenfd)
: _epollfd(createEpollFd())
,_listenfd(listenfd)
,_eventfd(createEventfd())
,_isLooping(false)
,_events(1024)
{
	addEpollReadFd(_epollfd, listenfd);
	addEpollReadFd(_epollfd,_eventfd);
}

EpollPoller::~EpollPoller()
{
	::close(_epollfd);//内部关闭机制？？
}


void EpollPoller::waitEpollFd()
{
	int nready;
	do
	{
		nready = ::epoll_wait(_epollfd, 
				&(*_events.begin()), 
				static_cast<int>(_events.size()), 
				5000);
	}while(nready == -1 && errno == EINTR);

	if(nready == -1)
	{
		LogError("epoll wait error!");
		exit(EXIT_FAILURE);
	}
	else if(nready == 0)
	{
		//LogInfo("epoll timeout.");
	}
	else
	{
		//当vector满时，扩充内存
		if(nready == static_cast<int>(_events.size()))
		{
			_events.resize(_events.size() * 2);//动态扩容，调用vector.resize()
		}

		for(int ix = 0; ix != nready; ++ix)
		{
			if(_events[ix].data.fd == _listenfd)//data结构体内部成员，fd再内部
			{
				if(_events[ix].events & EPOLLIN)//struct epoll_event内部成员events ？？？
					handleConnection();
			}else if(_events[ix].data.fd == _eventfd){
				handleRead();
				doPendingFunctors();
			}else{
				if(_events[ix].events & EPOLLIN)
					handleMessage(_events[ix].data.fd);
			}
		}
	}
}

void EpollPoller::handleRead()
{
	uint64_t buff=0;
	int ret=::read(_eventfd,&buff,sizeof(buff));
	if(ret != sizeof(buff)){
		LogError("read eventfd error!");
	}
}

void EpollPoller::runInLoop(Function cb)
{
	{
		MutexLockGuard guard(_mutex);
		_pendingFunctors.push_back(cb);
	}
	wakeup();
}

void EpollPoller::wakeup()
{
	uint64_t one=1;
	int ret=::write(_eventfd,&one,sizeof(one));
	if(ret!=sizeof(one)){
		LogError("reaad eventfd error!");
	}
}

void EpollPoller::doPendingFunctors()
{
	//printf("> doPendingFunctors()\n");
	std::vector<Function> tmp;
	{
		MutexLockGuard guard(_mutex);
		tmp.swap(_pendingFunctors);
	}

	for(auto & func:tmp){
		if(func)
			func();
	}
}

void EpollPoller::handleConnection()
{
	int peerfd = acceptConnFd(_listenfd);
	addEpollReadFd(_epollfd, peerfd);

	//多??
	std::pair<ConnectionList::iterator, bool> ret;

	TcpConnectionPtr conn(new TcpConnection(peerfd,this));
	conn->setConnectCallback(_onConnectCallback);
	conn->setMessageCallback(_onMessageCallback);
	conn->setCloseCallback(_onCloseCallback);

	ret = _lists.insert(std::make_pair(peerfd, conn));
	assert(ret.second == true); //断言插入成功
	(void)ret; //消除ret 未使用的warning ?????

	conn->handleConnectCallback();
}

void EpollPoller::handleMessage(int peerfd)
{
	bool isClosed = isConnectionClosed(peerfd);
	ConnectionList::iterator it = _lists.find(peerfd);
	//断言
	assert(it != _lists.end());

	if(isClosed)
	{
		//调用conn的close事件handleCloseCalback
		it->second->handleCloseCallback();
		delEpollReadFd(_epollfd, peerfd);
		_lists.erase(it);
	}
	else
	{
		it->second->handleMessageCallback();
	}
}

void EpollPoller::loop()
{
	_isLooping = true;

	while(_isLooping){
		waitEpollFd();
	}

	LogInfo("Loop quit safely!");
}

void EpollPoller::unloop()
{
	_isLooping = false;
}

}
