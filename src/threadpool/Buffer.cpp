 ///
 /// @file    Buffer.cpp
 /// @author  https://icoty.github.io
 ///
#include "Buffer.hpp" 
#include "MutexLock.hpp"

namespace yangyu
{

Buffer::Buffer(size_t size)
:_mutex()
,_notFull(_mutex)
,_notEmpty(_mutex)
,_size(size)
,_flag(true)
{ }

bool Buffer::full()
{	return _que.size()==_size;	}

bool Buffer::empty()
{	return _que.size()==0;	}

//运行于生产者线程
void Buffer::push(ElemType elem)
{
	MutexLockGuard guard(_mutex);//不能创建临时变量,避免异常退出导致死锁
	while(full())     //while 代替 if可避免条件变量被异常唤醒
		_notFull.wait();
	_que.push(elem);
	_notEmpty.notify();
}

//运行于消费者线程
ElemType Buffer::pop()
{
	MutexLockGuard guard(_mutex);//不能创建临时变量,避免异常退出导致死锁
	while(_flag && empty())
		_notEmpty.wait();
	
	if(_flag){
		ElemType ret = _que.front();
		_que.pop();
		_notFull.notify();
		return ret;
	}else
		return NULL;
}

void Buffer::wakeup()
{
	_flag = false;
	_notEmpty.notifyall();
}

}
