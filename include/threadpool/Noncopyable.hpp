///
/// @file    Noncopyable.hpp
/// @author  https://icoty.github.io
///
#ifndef __YANGYU_NONCOPYABLE_H__
#define __YANGYU_NONCOPYABLE_H__

namespace yangyu
{

class Noncopyable{
protected:
	Noncopyable(){ }
	~Noncopyable(){ }
private:
	Noncopyable(const Noncopyable &);
	Noncopyable & operator=(const Noncopyable &);
};

}
#endif
