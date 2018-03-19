#include "plugin_log_factory.h"
#include "plugin_log.h"

plugin_ptr plugin_log_factory::create_plugin(plugin_manager_base_ptr plugin_mgr)
{
	plugin_ptr plugin_obj = new plugin_log;

//	plugin_test_obj->set_plugin_mgr(plugin_mgr);

	return plugin_obj;
}

plugin_factory_base_ptr create_plugin_factory(plugin_manager_base_ptr plugin_mgr)
{
	return (new plugin_log_factory);
}

