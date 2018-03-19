#include "plugin_backtrace_factory.h"
#include "plugin_backtrace.h"

plugin_backtrace_factory::plugin_backtrace_factory()
{
}


plugin_backtrace_factory::~plugin_backtrace_factory()
{
}


plugin_ptr plugin_backtrace_factory::create_plugin(plugin_manager_base_ptr plugin_mgr)
{
	plugin_ptr plugin_obj = new plugin_backtrace;

	return plugin_obj;
}

plugin_factory_base_ptr create_plugin_factory(plugin_manager_base_ptr plugin_mgr)
{
	return (new plugin_backtrace_factory);
}

