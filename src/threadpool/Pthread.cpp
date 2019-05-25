///
/// @file    Pthread.cpp
/// @author  https://icoty.github.io
///
#include "Pthread.hpp"
#include <iostream>
using std::cout;
using std::endl;

namespace yangyu
{

Thread::Thread(ThreadCallback cb)
:_pthId(0)
,_isRunning(false)
,_cb(cb)
{  }

void Thread::start()
{
	pthread_create(&_pthId,NULL,threadFunc,this);
	_isRunning = true;
}

void Thread::join()
{
	if(_isRunning){
		pthread_join(_pthId,NULL);
		_isRunning = false;
	}
}

void * Thread::threadFunc(void* arg)
{
	Thread* p=static_cast<Thread*>(arg);
	if(p)
		p->_cb();
	return NULL;
}

Thread::~Thread()
{
	//LogInfo("Thread::~Thread().");
	if(_isRunning){
		pthread_detach(_pthId);//资源回收交给主线程进行托管
		_isRunning=false;
	}
}

pthread_t Thread::getId()
{
	return pthread_self();
}

}
