#pragma once
#include <string>
#include <sys/time.h>
#include <signal.h>

class timered_cmd
{
	typedef void(*timer_handler_t)(int);
protected:

	struct sigaction  _act;

	struct sigaction  _oldact;

	struct itimerval  _tim_ticks;

	void wait(long timeout_ms);


public:

	timered_cmd(int sec, int usec);

	void set_handler(timer_handler_t handler);

	bool reset();
};

