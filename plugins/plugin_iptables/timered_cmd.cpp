#include "timered_cmd.h"
#include "plugin_log_adapter.h"
#include <errno.h>
#include <string.h>

void timered_cmd::wait(long timeout_ms)
{
	struct timespec spec;
	spec.tv_sec = timeout_ms / 1000;
	spec.tv_nsec = (timeout_ms % 1000) * 1000000;
	nanosleep(&spec, NULL);
}

timered_cmd::timered_cmd(int sec, int usec)
{
	_tim_ticks.it_value.tv_sec = sec;
	_tim_ticks.it_value.tv_usec = usec;
	_tim_ticks.it_interval.tv_sec = sec;
	_tim_ticks.it_interval.tv_usec = usec;
}

void timered_cmd::set_handler(timer_handler_t handler)
{
	sigemptyset(&_act.sa_mask);
	_act.sa_flags = 0;
	_act.sa_handler = handler;
}

bool timered_cmd::reset()
{
	int res = sigaction(SIGALRM, &_act, &_oldact);
	if (res) {
		LOG_ERROR("Fail to install handle: " << strerror(errno));
		return false;
	}

	res = setitimer(ITIMER_REAL, &_tim_ticks, 0);

	if (res){

		LOG_ERROR("Fail to set timer: " << strerror(errno));

		sigaction(SIGALRM, &_oldact, 0);

		return false;
	}

	return true;
}

