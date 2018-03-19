#pragma once
#include "data_handler.h"
#include "boost/unordered_map.hpp"

class server_data_handler :
	public data_handler
{
public:
	server_data_handler();
	~server_data_handler();

	virtual bool on_received_data(common_session_ptr  session, const string & str_data);
	virtual bool on_session_accept(common_session_ptr  new_session);
	virtual int on_session_close(common_session_ptr  session, const string & str_remote_ip);
	virtual int on_session_error(common_session_ptr  session, int err_code, const string& err_msg);

	boost::unordered_map<common_session_ptr, common_session_ptr> map_sessions;

};

