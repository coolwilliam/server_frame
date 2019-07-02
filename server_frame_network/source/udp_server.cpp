#include "udp_server.h"
#include "udp_session.h"
#include <boost/bind.hpp>
#include "udp_data_handler.h"

udp_server::udp_server(boost::asio::io_service& ios, const boost::asio::ip::udp::endpoint& ep_bind, bool reuse_address/* = true*/)
	:m_ios(ios)
	, m_ep_bind(ep_bind)
	, m_sock(ios, ep_bind)
{
	m_sock.set_option(boost::asio::ip::udp::socket::reuse_address(reuse_address));
}

udp_server::udp_server(boost::asio::io_service& ios)
	:m_ios(ios)
	, m_sock(ios)
{

}


udp_server::~udp_server()
{
}

void udp_server::start()
{
	start_receive();
}

void udp_server::set_data_handler(udp_data_handler_ptr dh)
{
	m_dh = dh;
}

udp_data_handler_ptr udp_server::get_data_handler() const
{
	return m_dh;
}

void udp_server::start_receive()
{
	udp_buffer_ptr new_buff = udp_buffer_ptr(new udp_buffer);

	new_buff->resize(1024);

	udp_session_ptr new_session = udp_session_ptr(new udp_session(m_sock));
	m_sock.async_receive_from(boost::asio::buffer((char*)(new_buff->data()), new_buff->size()), 
		new_session->get_endpoint(), 
		new_session->get_strand().wrap(boost::bind(&udp_server::handle_receive, shared_from_this(), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred, new_buff, new_session)));
}

void udp_server::handle_receive(const boost::system::error_code& error, std::size_t size, udp_buffer_ptr p_buf, udp_session_ptr session)
{
	if (boost::system::errc::success == error)
	{
		if (0 == size)
		{
			start_receive();
			return;
		}

		udp_data_handler_ptr p_handler = get_data_handler();
		if (NULL != p_handler)
		{
			std::string str_data;
			str_data.resize(size);
			memcpy((void*)(str_data.data()), p_buf->data(), size);

			if (false == p_handler->on_received_data(session, str_data))
			{
				start_receive();
				return;
			}
		}

		start_receive();
	}
	else
	{
		udp_data_handler_ptr p_handler = get_data_handler();
		if (NULL !=  p_handler)
		{
			p_handler->on_session_error(session, error.value(), error.message());
		}

		return;
	}
}

boost::asio::ip::udp::socket& udp_server::get_socket()
{
	return m_sock;
}
