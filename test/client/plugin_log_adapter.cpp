#include "plugin_log_adapter.h"
#include "plugin_manager.h"
#include "plugin_log_msg.h"

plugin_log_adapter_ptr plugin_log_adapter::m_instance = NULL;


plugin_log_adapter_ptr plugin_log_adapter::get_instance()
{
	if (NULL == m_instance)
	{
		m_instance = new plugin_log_adapter();
	}

	return m_instance;
}

void plugin_log_adapter::release_instance()
{
	if (NULL != m_instance)
	{
		delete m_instance;
		m_instance = NULL;
	}
}

void plugin_log_adapter::log_trace(const std::string& str_log)
{
	boost::mutex::scoped_lock(m_mtx_log);

	log_msg_t log_msg;
	log_msg.log_level = ELL_TRACE;
	log_msg.log_content = str_log;
	plugin_manager_base::vect_plugin_names_t vect_plugins;
	vect_plugins.push_back(PLUGIN_LOG_NAME);

	plugin_manager::get_instance()->send_message(PLUGIN_LOG_MSG, 0, &log_msg, vect_plugins);
}

void plugin_log_adapter::log_error(const std::string& str_log)
{
	boost::mutex::scoped_lock(m_mtx_log);

	log_msg_t log_msg;
	log_msg.log_level = ELL_ERROR;
	log_msg.log_content = str_log;
	plugin_manager_base::vect_plugin_names_t vect_plugins;
	vect_plugins.push_back(PLUGIN_LOG_NAME);

	plugin_manager::get_instance()->send_message(PLUGIN_LOG_MSG, 0, &log_msg, vect_plugins);
}

void plugin_log_adapter::log_info(const std::string& str_log)
{
	boost::mutex::scoped_lock(m_mtx_log);

	log_msg_t log_msg;
	log_msg.log_level = ELL_INFO;
	log_msg.log_content = str_log;
	plugin_manager_base::vect_plugin_names_t vect_plugins;
	vect_plugins.push_back(PLUGIN_LOG_NAME);

	plugin_manager::get_instance()->send_message(PLUGIN_LOG_MSG, 0, &log_msg, vect_plugins);
}

void plugin_log_adapter::log_critical(const std::string& str_log)
{
	boost::mutex::scoped_lock(m_mtx_log);

	log_msg_t log_msg;
	log_msg.log_level = ELL_CRITICAL;
	log_msg.log_content = str_log;
	plugin_manager_base::vect_plugin_names_t vect_plugins;
	vect_plugins.push_back(PLUGIN_LOG_NAME);

	plugin_manager::get_instance()->send_message(PLUGIN_LOG_MSG, 0, &log_msg, vect_plugins);
}

plugin_log_adapter::plugin_log_adapter()
{

}

plugin_log_adapter::~plugin_log_adapter()
{

}

bool plugin_log_adapter::has_log_plugin() const
{
	plugin_ptr p_plugin = plugin_manager::get_instance()->get_plugin("log");
	if (NULL == p_plugin)
	{
		return false;
	}

	return true;
}


