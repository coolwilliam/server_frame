///////////////////////////////////////////////////////////
//  common_server.cpp
//  Implementation of the Class common_server
//  Created on:      09-八月-2016 11:35:01
///////////////////////////////////////////////////////////

#include "common_server.h"
#include "common_session.h"
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include "data_handler.h"

common_server::common_server(io_service& ios, const tcp::endpoint& ep_bind)
	:m_io_service(ios)
	, m_acceptor(ios, ep_bind)
{

}

common_server::~common_server(){

}


data_handler_ptr common_server::get_data_handler(){

	return m_p_data_handler;
}


void common_server::handle_accept(common_session_ptr& session, const boost::system::error_code & error){

	if (error == boost::system::errc::success)
	{
		//设置数据处理器
		session->set_data_handler(get_data_handler());

		//启动会话
		session->start();

		//通知监听者 有新的连接
		get_data_handler().get()->on_session_accept(session);

	}

	//再次启动异步监听
	start_accept();

	return;
}


void common_server::set_data_handler(data_handler_ptr newVal){

	m_p_data_handler = newVal;
}


void common_server::start()
{
	start_accept();
}


void common_server::start_accept()
{
	common_session_ptr new_session(new common_session(m_io_service));

	m_acceptor.async_accept(new_session->get_socket(),
		boost::bind(&common_server::handle_accept, this, new_session, boost::asio::placeholders::error));

}