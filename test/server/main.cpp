#include "tool_ptr_define.h"
#include "common_function.h"
#include "simple_kv_config.h"
#include "plugin_base.h"
#include "plugin_manager.h"
#include "plugin_log_adapter.h"
#include "svr.h"

#include <iostream>

#include <boost/asio.hpp>

#define CONF_FILE_NAME 		"nvram_save.tmp"
#define PLUGINS_XML_NAME	"plugins.xml"
#define PLUGIN_DIR_NAME		"plugins"

simple_kv_config_ptr gl_nvram_ptr;

int main(int argc, char* argv[])
{
	boost::asio::io_service _ios;
	std::string str_full_config = "";
	gl_nvram_ptr = simple_kv_config_ptr(new simple_kv_config);

	//读取配置文件
	str_full_config = common_function::get_module_dir((void*)main) + CONF_FILE_NAME;
	if (false == gl_nvram_ptr->load_file(str_full_config))
	{
		printf("load file %s failed!\n", str_full_config.c_str());
		return -1;
	}

	//初始化插件管理器
	bool b_mgr_init = plugin_manager::get_instance()->init(&_ios);
	if (false == b_mgr_init)
	{
		std::cout << "init plugin manager failed!" << std::endl;
		return -1;
	}

	const std::string& str_app_dir = common_function::get_module_dir((void*)main);

	//设置主程序所在路径
	plugin_manager::get_instance()->set_app_path(str_app_dir);

	//加载插件
	bool b_load_plugin = plugin_manager::get_instance()->load_plugins(str_app_dir + std::string("/"PLUGINS_XML_NAME), str_app_dir + std::string("/"PLUGIN_DIR_NAME));
	if (false == b_load_plugin)
	{
		std::cout << "load plugins failed!" << std::endl;
		return -1;
	}

	//输出已经加载的插件
	const plugin_manager_base::map_plugins_t& plugins = plugin_manager::get_instance()->get_all_plugins();
	std::cout << "plugins size:" << plugins.size() << std::endl;
	std::cout << "plugins:" << std::endl << "{" << std::endl;
	plugin_manager_base::map_plugins_t::const_iterator it_plugin = plugins.begin();
	for (; it_plugin != plugins.end(); ++it_plugin)
	{
		std::cout << "\tplugin name:" << it_plugin->second->get_plugin_name().c_str() << std::endl;
	}
	std::cout << "}" << std::endl;

	svr* p_svr = new svr;
	p_svr->start(_ios);

	try
	{
		_ios.run();
	}
	catch (std::exception &e)
	{
		LOG_CRITICAL("Exception happened in io_service.run: " << string(e.what()));
		return false;
	}
	

	return 0;
}