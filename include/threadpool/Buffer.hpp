 ///
 /// @file    Buffer.hpp
 /// @author  https://icoty.github.io
 ///
#ifndef __YANGYU_BUFFER_H__
#define __YANGYU_BUFFER_H__

#include "MutexLock.hpp"
#include "Condition.hpp"

#include <queue>
#include <functional>

namespace yangyu
{

typedef std::function<void()> ElemType;

class Buffer{
public:
	Buffer(size_t size);
	void push(ElemType elem);
	ElemType pop();
	bool empty();
	bool full();
	void wakeup();
private:
	MutexLock _mutex;
	Condition _notFull;
	Condition _notEmpty;
	size_t _size;
	bool _flag;
	std::queue<ElemType> _que;
};

}
#endif
