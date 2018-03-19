#include "server_data_handler.h"
#include "plugin_log_adapter.h"
#include "common_session.h"

server_data_handler::server_data_handler()
{
}


server_data_handler::~server_data_handler()
{
}

bool server_data_handler::on_received_data(common_session_ptr session, const string & str_data)
{
	LOG_INFO("Session:" << session.get() << ", data received: " << str_data);

	session->send_msg(std::string("Server Received"));

	return true;
}

bool server_data_handler::on_session_accept(common_session_ptr new_session)
{
	LOG_TRACE("New session accepted : " << new_session.get());
	map_sessions.insert(make_pair(new_session, new_session));

	new_session->send_msg("Server accept");

	return true;
}

int server_data_handler::on_session_close(common_session_ptr session, const string & str_remote_ip)
{
	LOG_ERROR("Session closed : " << session.get());

	map_sessions.erase(session);

	return 0;
}

int server_data_handler::on_session_error(common_session_ptr session, int err_code, const string& err_msg)
{
	LOG_ERROR("Session error : ptr:" << session.get() << ", code:" << err_code << ", msg: " << err_msg);
	return 0;
}
