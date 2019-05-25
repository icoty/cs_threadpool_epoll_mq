 ///
 /// @file    Pthread.hpp
 /// @author  https://icoty.github.io
 ///
#ifndef __YANGYU_PTHREAD_H__
#define __YANGYU_PTHREAD_H__

#include "Noncopyable.hpp"

#include <pthread.h>
#include <functional>
using std::function;

namespace yangyu
{
//private Noncopyable 实现继承
//public Noncopyable 接口继承
class Thread:private Noncopyable{
	typedef function<void()> ThreadCallback;
public:
	Thread(ThreadCallback cb);
	~Thread();
	void start();
	void join();
	static void *threadFunc(void *arg);
	pthread_t getId();

private:
	pthread_t _pthId; //oo_thread
	bool _isRunning;
	ThreadCallback _cb;
};

}
#endif
