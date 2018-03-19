#include "plugin_log_adapter.h"
#include "plugin_manager.h"
#include "plugin_log_msg.h"

void plugin_log_adapter::set_plugin_mgr(plugin_manager_base_ptr p_plugin_mgr)
{
	m_p_plugin_mgr = p_plugin_mgr;
}

void plugin_log_adapter::log_trace(const std::string& str_log)
{
//	boost::mutex::scoped_lock(m_mtx_log);
	if (NULL == m_p_plugin_mgr)
	{
		return;
	}

	log_msg_t log_msg;
	log_msg.log_level = ELL_TRACE;
	log_msg.log_content = str_log;
	plugin_manager_base::vect_plugin_names_t vect_plugins;
	vect_plugins.push_back(PLUGIN_LOG_NAME);

	m_p_plugin_mgr->send_message(PLUGIN_LOG_MSG, 0, &log_msg, vect_plugins);
}

void plugin_log_adapter::log_error(const std::string& str_log)
{
//	boost::mutex::scoped_lock(m_mtx_log);

	if (NULL == m_p_plugin_mgr)
	{
		return;
	}

	log_msg_t log_msg;
	log_msg.log_level = ELL_ERROR;
	log_msg.log_content = str_log;
	plugin_manager_base::vect_plugin_names_t vect_plugins;
	vect_plugins.push_back(PLUGIN_LOG_NAME);

	m_p_plugin_mgr->send_message(PLUGIN_LOG_MSG, 0, &log_msg, vect_plugins);
}

void plugin_log_adapter::log_info(const std::string& str_log)
{
//	boost::mutex::scoped_lock(m_mtx_log);

	if (NULL == m_p_plugin_mgr)
	{
		return;
	}

	log_msg_t log_msg;
	log_msg.log_level = ELL_INFO;
	log_msg.log_content = str_log;
	plugin_manager_base::vect_plugin_names_t vect_plugins;
	vect_plugins.push_back(PLUGIN_LOG_NAME);

	m_p_plugin_mgr->send_message(PLUGIN_LOG_MSG, 0, &log_msg, vect_plugins);
}

void plugin_log_adapter::log_critical(const std::string& str_log)
{
//	boost::mutex::scoped_lock(m_mtx_log);

	if (NULL == m_p_plugin_mgr)
	{
		return;
	}

	log_msg_t log_msg;
	log_msg.log_level = ELL_CRITICAL;
	log_msg.log_content = str_log;
	plugin_manager_base::vect_plugin_names_t vect_plugins;
	vect_plugins.push_back(PLUGIN_LOG_NAME);

	m_p_plugin_mgr->send_message(PLUGIN_LOG_MSG, 0, &log_msg, vect_plugins);
}

plugin_log_adapter::plugin_log_adapter()
	: m_p_plugin_mgr(NULL)
{

}

plugin_log_adapter::~plugin_log_adapter()
{

}

bool plugin_log_adapter::has_log_plugin() const
{
	if (NULL == m_p_plugin_mgr)
	{
		return false;
	}

	plugin_ptr p_plugin = m_p_plugin_mgr->get_plugin(PLUGIN_LOG_NAME);
	if (NULL == p_plugin)
	{
		return false;
	}

	return true;
}
