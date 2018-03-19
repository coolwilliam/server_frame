#ifndef COMMON_SINGLETON_HPP
#define COMMON_SINGLETON_HPP

#include <boost/noncopyable.hpp>

#include "business_exports.h"

// class singleton has the same goal as all singletons: create one instance of
// a class on demand, then dish it out as requested.

template <class T>
class SERVER_FRAME_BUSINESS_API common_singleton : private boost::noncopyable {
public:
	static T& instance() { static T the_inst; return the_inst; }
protected:
	common_singleton()  {}
	~common_singleton() {}
};

#endif // COMMON_SINGLETON_HPP
