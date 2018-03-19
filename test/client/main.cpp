#include "common_client.h"
#include "tool_ptr_define.h"
#include "simple_kv_config.h"
#include "config_keys.h"
#include "common_function.h"
#include "plugin_base.h"
#include "plugin_manager.h"
#include "plugin_log_adapter.h"
#include "common_session.h"

#include <iostream>

#include <boost/asio.hpp>

#define CONF_FILE_NAME 		"nvram_save.tmp"
#define PLUGINS_XML_NAME	"plugins.xml"
#define PLUGIN_DIR_NAME		"plugins"

int main(int argc, char* argv[])
{
	boost::asio::io_service _ios;

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


	simple_kv_config_ptr kv_config = simple_kv_config_ptr(new simple_kv_config);

	//读取配置文件
	std::string str_full_config = common_function::get_module_dir((void*)main) + CONF_FILE_NAME;
	if (false == kv_config->load_file(str_full_config))
	{
		LOG_ERROR("Load file %s failed!\n", str_full_config.c_str());
		return -1;
	}


	unsigned short conn_port = 0;
	bool b_get = kv_config->get(SERVER_PORT, conn_port);
	if (false == b_get)
	{
		LOG_ERROR("Get config [" << SERVER_PORT << "] failed!");
		return -1;
	}

	common_client_ptr client_obj = common_client_ptr(new common_client("localhost", conn_port, _ios));

	client_obj->set_comm_mode(ECM_SYNC);
	int err_code = 0;
	std::string str_err_msg = "";
	bool b_start = client_obj->start(err_code, str_err_msg);
	if (false == b_start)
	{
		LOG_ERROR("Client start failed, error_code:" << err_code << ", err_msg:" << str_err_msg);
		return -1;
	}

	while (true)
	{
		common_session_ptr cs = client_obj->get_session();
		if (NULL != cs)
		{
			std::string str_recv;
			size_t recv_size = cs->receive(str_recv, err_code, str_err_msg);
			if (recv_size > 0)
			{
				sleep(2);
				static int recv_count = 1;
				ostringstream send_buff;
				send_buff << "Client recv " << recv_count;
				cs->send(send_buff.str(), err_code, str_err_msg);
				if (err_code != 0)
				{
					LOG_ERROR("Send error: code:" << err_code << ", err_msg: " << str_err_msg);
					break;
				}
			}

			if (err_code != 0)
			{
				LOG_ERROR("Recv error: code:" << err_code << ", err_msg: " << str_err_msg);
				break;
			}
		}
	}

	return 0;
}