///
/// @file    EpollPoller.hpp
/// @author  https://icoty.github.io
///
#ifndef __YANGYU_EPOLLPOLLER_H__
#define __YANGYU_EPOLLPOLLER_H__
#include "Noncopyable.hpp"
#include "MutexLock.hpp"
#include "TcpConnection.hpp"
#include <vector>
#include <map>
#include <sys/epoll.h>
#include <functional>

namespace yangyu
{

class EpollPoller : Noncopyable{
public:
	typedef TcpConnection::TcpConnectionCallback EpollCallback;
	typedef std::function<void()> Function;

	explicit EpollPoller(int listenfd);
	~EpollPoller();
	void loop(); //启动epoll
	void unloop(); //关闭epoll
	void setConnectCallback(EpollCallback cb)
	{ _onConnectCallback = std::move(cb); }
	void setMessageCallback(EpollCallback cb)
	{ _onMessageCallback = std::move(cb); }
	void setCloseCallback(EpollCallback cb)
	{ _onCloseCallback = std::move(cb); }

	void runInLoop(Function cb);

private:
	void waitEpollFd(); //循环
	void handleConnection(); //处理accept
	void handleMessage(int peerfd); //处理msg

	void handleRead();//处理_eventfd
	void wakeup();//激活_enentfd
	void doPendingFunctors();//_eventfd被激活之后，执行回调函数

private:
	const int _epollfd;
	const int _listenfd; 
	const int _eventfd;
	bool _isLooping; //是否在运行
	
	MutexLock _mutex;
	std::vector<Function> _pendingFunctors;

	typedef std::vector<struct epoll_event> EventList;
	EventList _events;  //保存活跃的fd
	typedef std::map<int, TcpConnectionPtr> ConnectionList;//保存连接的sfd和tcpconnection对
	ConnectionList _lists; //实现fd到conn的映射
	EpollCallback _onConnectCallback;
	EpollCallback _onMessageCallback;
	EpollCallback _onCloseCallback;
};

}
#endif
