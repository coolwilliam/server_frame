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
#include "io_service_pool.h"

common_server::common_server(io_service& ios, const tcp::endpoint& ep_bind)
	:m_ios_pool(io_service_pool_ptr())
	, m_io_service(ios)
	, m_acceptor(ios, ep_bind)
	, m_comm_mode(ECM_ASYNC)
	, m_started(false)
{
}

common_server::common_server(io_service_pool_ptr iosp, const tcp::endpoint& ep_bind)
	:m_ios_pool(iosp)
	, m_io_service(m_ios_pool->get_io_service())
	, m_acceptor(m_io_service, ep_bind)
	, m_comm_mode(ECM_ASYNC)
	, m_started(false)
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

		//通知监听者 有新的连接
		get_data_handler().get()->on_session_accept(session);

		//启动会话
		session->start();
	}
	else
	{
		data_handler_ptr hdl_ptr = get_data_handler();
		if (NULL != hdl_ptr)
		{
			//如果出错了就响应错误处理，并退出监听
			hdl_ptr->on_session_error(session, error.value(), error.message());
			return;
		}
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
	set_started();

	if (get_comm_mode() == ECM_ASYNC)
	{
		start_accept();
	}	
}


void common_server::start_accept()
{
	common_session_ptr new_session;
	if (NULL == m_ios_pool)
	{
		new_session = common_session_ptr(new common_session(m_io_service));
	}
	else
	{
		new_session = common_session_ptr(new common_session(m_ios_pool->get_io_service()));
	}

	new_session->set_comm_mode(get_comm_mode());

	m_acceptor.async_accept(new_session->get_socket(),
		boost::bind(&common_server::handle_accept, this, new_session, boost::asio::placeholders::error));
}

enum_communicate_mode common_server::get_comm_mode() const
{
	return m_comm_mode;
}

void common_server::set_comm_mode(enum_communicate_mode newVal){
	assert(!is_started() && "Can't change mode since server has been started!");
	assert(newVal == ECM_SYNC && m_ios_pool == NULL && "Can't change to ECM_SYNC mode since ios_pool is useful");

	m_comm_mode = newVal;
}

bool common_server::is_started() const
{
	return m_started;
}

common_session_ptr common_server::accept(int& err_code, string& err_msg)
{
	assert(is_started() && "Server should be started first!");
	assert(get_comm_mode() == ECM_SYNC && "Can't call this function in async mode");

	system::error_code error;

	common_session_ptr new_session(new common_session(m_io_service));
	new_session->set_comm_mode(get_comm_mode());

	m_acceptor.accept(new_session->get_socket(), error);
	if (boost::system::errc::success != error)
	{
		err_code = error.value();
		err_msg = error.message();
		return common_session_ptr();
	}

	//启动会话
	new_session->start();

	return new_session;
}

void common_server::set_started(bool newVal){
	m_started = newVal;
}

