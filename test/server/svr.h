#pragma once
#include "common_server.h"
class svr
{
public:
	svr();
	~svr();

	bool start(boost::asio::io_service& _ios);

	common_server_ptr m_svr;
};

