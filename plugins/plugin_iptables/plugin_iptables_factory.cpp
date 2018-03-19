#include "plugin_iptables_factory.h"
#include "plugin_iptables.h"

plugin_iptables_factory::plugin_iptables_factory()
{
}


plugin_iptables_factory::~plugin_iptables_factory()
{
}

plugin_ptr plugin_iptables_factory::create_plugin(plugin_manager_base_ptr plugin_mgr)
{
	return (new plugin_iptables);
}


plugin_factory_base_ptr create_plugin_factory(plugin_manager_base_ptr plugin_mgr)
{
	return (new plugin_iptables_factory);
}
