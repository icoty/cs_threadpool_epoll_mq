 ///
 /// @file    Condition.hpp
 /// @author  https://icoty.github.io
 ///
#ifndef _WD_CONDITION_H__
#define _WD_CONDITION_H__

#include "Noncopyable.hpp"
#include <pthread.h>

namespace yangyu
{

class MutexLock;//前向声明

class Condition:private Noncopyable{
public:
	Condition(MutexLock &mutex);
	~Condition();
	void wait();
	void notify();
	void notifyall();

private:
	pthread_cond_t _cond;
	MutexLock & _mutex;
};

}
#endif
