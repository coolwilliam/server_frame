#include "plugin_factory_base.h"

class plugin_log_factory:public plugin_factory_base
{
public:
	plugin_log_factory(){}
	~plugin_log_factory(){}
	plugin_ptr create_plugin(plugin_manager_base_ptr plugin_mgr);

protected:
private:
};

extern "C" plugin_factory_base_ptr create_plugin_factory(plugin_manager_base_ptr plugin_mgr);

