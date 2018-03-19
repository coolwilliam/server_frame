#include "backtrace_func.h"
#include <cstdio>
#include <cstdlib>
#include <csignal>
#include <cstring>
#include <sstream>
#include <iostream>
#include "plugin_log_adapter.h"
#include "common_macro.h"
#include "debug_printer.h"

#include <unistd.h>
#include <execinfo.h>

//信号信息
typedef struct __signal_info
{
	//信号名称
	const char* str_signal_name;
	//信号编号
	int signal_id;
}signal_info_t, *signal_info_ptr;

#define DEFINE_SIGNAL_ARRAY_BEGIN(signal_array) signal_info_t signal_array[]=\
{\

#define ADD_SIGNAL(signal_id)	{#signal_id, signal_id},

#define DEFINE_SIGNAL_ARRAY_END() {NULL, 0}};

#define BACKTRACE_DEPTH_MAX 100


DEFINE_SIGNAL_ARRAY_BEGIN(g_watched_signals)
ADD_SIGNAL(SIGSEGV)
ADD_SIGNAL(SIGABRT)
ADD_SIGNAL(SIGBUS)
ADD_SIGNAL(SIGILL)
ADD_SIGNAL(SIGFPE)
DEFINE_SIGNAL_ARRAY_END()

bool backtrace_func::m_b_setup = false;
backtrace_func::map_sign_name_t backtrace_func::m_map_sig_name;

int backtrace_func::setup_signals()
{
	if (m_b_setup == true)
	{
		return 0;
	}

    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));

	stack_t sigstack;

	//分配可变栈并通知内核，防止在堆栈溢出时，没有空间运行处理函数
	sigstack.ss_sp = malloc(SIGSTKSZ);
	if (sigstack.ss_sp == NULL)
	{
		perror("malloc stack failed");
		DEBUG_ERROR("malloc stack failed");
		exit(EXIT_FAILURE);
	}

	sigstack.ss_size = SIGSTKSZ;
	sigstack.ss_flags = 0;
	if (sigaltstack(&sigstack, NULL) == -1) {
		perror("sigaltstack failed");
		DEBUG_ERROR("sigaltstack failed");
		exit(EXIT_FAILURE);
	}

	sa.sa_handler = sig_handle;
    sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_ONSTACK;
   
	size_t sig_count = _countof(g_watched_signals);
	for (size_t i = 0; i < sig_count - 1; ++i)
	{
		if (sigaction(g_watched_signals[i].signal_id, &sa, 0) < 0) 
		{
			std::ostringstream oss;
			oss << "Set up signal [" << g_watched_signals[i].str_signal_name << " : " << g_watched_signals[i].signal_id << "] failed !";
			LOG_ERROR(oss.str());
			DEBUG_ERROR(oss.str());
		}

		m_map_sig_name.insert(make_pair(g_watched_signals[i].signal_id, g_watched_signals[i].str_signal_name));
	}

	m_b_setup = true;

    return 0;
}

void backtrace_func::dump_stack()
{
	void *bufs[BACKTRACE_DEPTH_MAX];
	int n = backtrace(bufs, BACKTRACE_DEPTH_MAX);
    char **infos = backtrace_symbols(bufs, n);

	std::ostringstream oss;

	if (!infos)
	{
		std::ostringstream oss_failed;
		oss_failed << "Call backtrace_symbols failed";
		LOG_CRITICAL(oss_failed.str());
		DEBUG_ERROR(oss_failed.str());
		return;
	}

	oss << "==================" << std::endl;
	oss << "Frame info:" << std::endl;

    for(int i = 0; i < n; ++i) {
		oss << infos[i] << std::endl;
		oss << (void*)(bufs[i]) << std::endl;
    }
/*
	char cmd[512];
	int len = snprintf(cmd, sizeof(cmd),
	"addr2line -ifC -e ./bt_test");
	char *p = cmd + len;

    fprintf(log, "src info:\n");

    FILE *fp;
    char buf[128];
    if((fp = popen(cmd, "r"))) {
        while(fgets(buf, sizeof(buf), fp))
            fprintf(log, "%s", buf);
        pclose(fp);
    }
*/
	free(infos);

	oss << "==================" << std::endl;

	DEBUG_ERROR(std::endl << oss.str());

	LOG_CRITICAL("dump:" << std::endl << oss.str());

    // same as:
    //backtrace_symbols_fd(bufs, n, STDOUT_FILENO);
}

void backtrace_func::sig_handle(int sig_id)
{
	map_sign_name_t::iterator it_find = m_map_sig_name.find(sig_id);
	if (it_find != m_map_sig_name.end())
	{
		std::ostringstream oss_msg;
		oss_msg << "Signal [" << it_find->second << " : " << it_find->first << "] catched !";
		LOG_CRITICAL(oss_msg.str());
		DEBUG_ERROR(oss_msg.str());
	}

    dump_stack();

    exit(127 + sig_id);
}

