#include "plugin_backtrace.h"
#include "backtrace_func.h"
#include "plugin_log_adapter.h"


plugin_backtrace::plugin_backtrace()
{
}


plugin_backtrace::~plugin_backtrace()
{
}

bool plugin_backtrace::init()
{

	//��ʼ����־���������
	plugin_log_adapter::instance().set_plugin_mgr(get_plugin_mgr());

	backtrace_func::setup_signals();

	return true;
}
