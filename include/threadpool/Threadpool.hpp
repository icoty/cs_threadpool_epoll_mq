 ///
 /// @file    Threadpool.hpp
 /// @author  https://icoty.github.io
 ///
#ifndef __YANGYU_THREADPOOL_H__
#define __YANGYU_THREADPOOL_H__
#include "Buffer.hpp"
#include "Pthread.hpp"
#include <iostream>
#include <vector>
#include <memory>
#include <functional>
using std::vector;
using std::shared_ptr;
using std::cout;
using std::endl;

namespace yangyu
{

class Thread;

typedef std::function<void()> Task;

class Threadpool{
public:
	Threadpool(size_t threadNum,size_t bufSize);
	~Threadpool();
	void start();
	void stop();
	void addTask(Task task);

private:
	Task getTask();
	void processTask();
private:
	bool _isExit;
	size_t  _threadNum;
	size_t _bufSize;
	Buffer _buff;
	vector<shared_ptr<Thread>> _threads;
};

}
#endif
