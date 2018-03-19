#pragma once
#include "plugin_factory_base.h"
class plugin_iptables_factory :
	public plugin_factory_base
{
public:
	plugin_iptables_factory();
	~plugin_iptables_factory();

	virtual plugin_ptr create_plugin(plugin_manager_base_ptr plugin_mgr);

};

extern "C" plugin_factory_base_ptr create_plugin_factory(plugin_manager_base_ptr plugin_mgr);
