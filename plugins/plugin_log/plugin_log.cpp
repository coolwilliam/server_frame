#include "plugin_log.h"
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <log4cplus/logger.h>
#include <log4cplus/configurator.h>
#include <log4cplus/helpers/stringhelper.h>
#include <log4cplus/fileappender.h>
#include <log4cplus/consoleappender.h>
#include <log4cplus/layout.h>
#include <log4cplus/loggingmacros.h>

#include "plugin_log_msg.h"
#include "debug_printer.h"
#include "customloglevel.h"

const char* g_log_config_file = "urconfig.properties";

BEGIN_MSG_MAP(plugin_log)
	ON_MSG(PLUGIN_LOG_MSG, on_log_msg)
END_MSG_MAP()

bool plugin_log::init()
{
	plugin_manager_base_ptr p_mgr = get_plugin_mgr();
	if (NULL == p_mgr)
	{
		DEBUG_ERROR("plugin manager is null");

		return false;
	}
	else
	{
		const std::string& str_app_path = p_mgr->get_app_path();
		if (str_app_path.empty() == true)
		{
			DEBUG_ERROR("app path is empty");

			return false;
		}
		else
		{
			std::string str_config_path = str_app_path + std::string("/") + std::string(g_log_config_file);
			std::string str_log_dir = str_app_path + std::string("/log");
			if (0 != access(str_config_path.c_str(), F_OK))
			{
				DEBUG_ERROR("config file " << str_config_path.c_str() << " is missing !");
				return false;
			}

			if (0 != access(str_log_dir.c_str(), F_OK))
			{
				mkdir(str_log_dir.c_str(),  0777);
			}

			log4cplus::tstring tstr_config_path = LOG4CPLUS_STRING_TO_TSTRING(str_config_path);
			log4cplus::PropertyConfigurator::doConfigure(tstr_config_path);
		}
	}
	return true;
}
void plugin_log::on_log_msg(int ivalue, void* pvalue)
{
	if (NULL != pvalue)
	{
		log_msg_ptr p_log = (log_msg_ptr)pvalue;
		switch (p_log->log_level)
		{
		case ELL_TRACE:
			LOG4CPLUS_TRACE(log4cplus::Logger::getRoot(), p_log->log_content.c_str());
			break;
		case ELL_ERROR:
			LOG4CPLUS_ERROR(log4cplus::Logger::getRoot(), p_log->log_content.c_str());
			break;
		case ELL_CRITICAL:
			LOG4CPLUS_CRITICAL(log4cplus::Logger::getRoot(), p_log->log_content.c_str());
			break;
		case ELL_INFO:
			LOG4CPLUS_INFO(log4cplus::Logger::getRoot(), p_log->log_content.c_str());
			break;
		default:
			break;
		}
	}
}
