#include "local_stream_server.h"
#include "local_stream_session.h"
#include "local_stream_data_handler.h"
#include <boost/bind.hpp>


local_stream_server::local_stream_server(boost::asio::io_service& ios, const std::string& file)
	:m_io_service(ios)
	, m_acceptor(ios, boost::asio::local::stream_protocol::endpoint(file))
	, m_started(false)
	, m_comm_mode(ECM_ASYNC)
{
}


local_stream_server::~local_stream_server()
{
	stop();
}

local_stream_data_handler_ptr local_stream_server::get_data_handler()
{
	return m_p_data_handler;
}

void local_stream_server::set_data_handler(local_stream_data_handler_ptr newVal)
{
	m_p_data_handler = newVal;
}

void local_stream_server::start()
{
	set_started();
	if (get_comm_mode() == ECM_ASYNC)
	{
		start_accept();
	}
}

bool local_stream_server::is_started() const
{
	return m_started;
}

void local_stream_server::stop()
{
	if (m_acceptor.is_open())
	{
		boost::system::error_code ec;
		m_acceptor.close(ec);
	}

	set_started(false);
}

void local_stream_server::start_accept()
{
	local_stream_session_ptr new_session;

	new_session = local_stream_session_ptr(new local_stream_session(m_io_service));


	new_session->set_comm_mode(get_comm_mode());

	m_acceptor.async_accept(new_session->get_socket(),
		boost::bind(&local_stream_server::handle_accept, shared_from_this(), new_session, boost::asio::placeholders::error));
}

enum_communicate_mode local_stream_server::get_comm_mode() const
{
	return m_comm_mode;
}

void local_stream_server::set_comm_mode(enum_communicate_mode newVal){
	assert(!is_started() && "Can't change mode since server has been started!");

	m_comm_mode = newVal;
}

void local_stream_server::handle_accept(local_stream_session_ptr& session, const boost::system::error_code & error)
{
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
		local_stream_data_handler_ptr hdl_ptr = get_data_handler();
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

void local_stream_server::set_started(bool newVal /*= true*/)
{
	m_started = newVal;
}

local_stream_session_ptr local_stream_server::accept(int& err_code, std::string& err_msg)
{
	assert(is_started() && "Server should be started first!");
	assert(get_comm_mode() == ECM_SYNC && "Can't call this function in async mode");

	boost::system::error_code error;

	local_stream_session_ptr new_session(new local_stream_session(m_io_service));
	new_session->set_comm_mode(get_comm_mode());

	m_acceptor.accept(new_session->get_socket(), error);
	if (boost::system::errc::success != error)
	{
		err_code = error.value();
		err_msg = error.message();
		return local_stream_session_ptr();
	}

	//启动会话
	new_session->start();

	return new_session;
}
