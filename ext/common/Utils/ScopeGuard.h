/*
 *  Phusion Passenger - https://www.phusionpassenger.com/
 *  Copyright (c) 2010, 2011, 2012 Phusion
 *
 *  "Phusion Passenger" is a trademark of Hongli Lai & Ninh Bui.
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in
 *  all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *  THE SOFTWARE.
 */
#ifndef _PASSENGER_SCOPE_GUARD_H_
#define _PASSENGER_SCOPE_GUARD_H_

#include <boost/noncopyable.hpp>
#include <boost/function.hpp>
#include <boost/thread.hpp>
#include <oxt/system_calls.hpp>
#include <cstdio>

namespace Passenger {

using namespace boost;
using namespace oxt;


#ifndef _PASSENGER_SAFELY_CLOSE_DEFINED_
	#define _PASSENGER_SAFELY_CLOSE_DEFINED_
	void safelyClose(int fd, bool ignoreErrors = false);
#endif


/**
 * Guard object for making sure that a certain function is going to be
 * called when the object goes out of scope. To avoid the function from
 * being called, call clear().
 */
class ScopeGuard: public noncopyable {
private:
	function<void ()> func;
	bool interruptable;
	
public:
	ScopeGuard() { }
	
	ScopeGuard(const function<void ()> &func, bool interruptable = false) {
		this->func = func;
		this->interruptable = interruptable;
	}
	
	~ScopeGuard() {
		if (func) {
			if (interruptable) {
				func();
			} else {
				this_thread::disable_interruption di;
				this_thread::disable_syscall_interruption dsi;
				func();
			}
		}
	}
	
	void clear() {
		func = function<void()>();
	}
	
	void runNow() {
		function<void ()> oldFunc = func;
		func = function<void()>();
		if (interruptable) {
			oldFunc();
		} else {
			this_thread::disable_interruption di;
			this_thread::disable_syscall_interruption dsi;
			oldFunc();
		}
	}
};

class StdioGuard: public noncopyable {
private:
	FILE *f;

public:
	StdioGuard()
		: f(0)
		{ }

	StdioGuard(FILE *_f)
		: f(_f)
		{ }
	
	~StdioGuard() {
		if (f != NULL) {
			fclose(f);
		}
	}
};

class FdGuard: public noncopyable {
private:
	int fd;
	bool ignoreErrors;

public:
	FdGuard(int _fd, bool _ignoreErrors = false)
		: fd(_fd),
		  ignoreErrors(_ignoreErrors)
		{ }
	
	~FdGuard() {
		if (fd != -1) {
			safelyClose(fd, ignoreErrors);
		}
	}

	void clear() {
		fd = -1;
	}
};


} // namespace Passenger

#endif /* _PASSENGER_SCOPE_GUARD_H_ */
