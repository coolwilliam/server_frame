#include "svr.h"
#include "tool_ptr_define.h"
#include "simple_kv_config.h"
#include "config_keys.h"
#include "server_data_handler.h"
#include "plugin_log_adapter.h"

#include <string>

extern simple_kv_config_ptr gl_nvram_ptr;

svr::svr()
{
}


svr::~svr()
{
}

bool svr::start(boost::asio::io_service& _ios)
{
	unsigned short us_com_port = 0;
	bool b_get = gl_nvram_ptr->get(std::string(SERVER_PORT), us_com_port);
	if (false == b_get)
	{
		LOG_ERROR("Get config [" << SERVER_PORT << "] failed!");
		return false;
	}
	boost::asio::ip::tcp::endpoint end_com(boost::asio::ip::tcp::v4(), us_com_port);

	m_svr = common_server_ptr(new common_server(_ios, end_com));

	m_svr->set_data_handler(data_handler_ptr(new server_data_handler()));

	m_svr->start();
}
