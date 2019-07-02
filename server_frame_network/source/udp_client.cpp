#include "udp_client.h"
#include "udp_data_handler.h"
#include "udp_session.h"
#include <boost/bind.hpp>

udp_client::udp_client(boost::asio::io_service& ios, boost::asio::ip::udp::endpoint svr_ep)
	:m_ios(ios)
	, m_svr_ep(svr_ep)
	, m_sock(ios)
	, m_receive_cache_size(receive_cache_size_default)
{
}

udp_client::~udp_client()
{
}

void udp_client::set_data_handler(udp_data_handler_ptr hd)
{
	m_data_handler = hd;
	if (NULL != m_session)
	{
		m_session->set_data_handler(m_data_handler);
	}
}

void udp_client::set_recv_cache_size(size_t val)
{
	assert(0 != val && "can't set cache size to 0!");

	m_receive_cache_size = val;
}

udp_data_handler_ptr udp_client::get_data_handler() const
{
	return m_data_handler;
}

void udp_client::start_receive()
{
	udp_buffer_ptr new_buff = udp_buffer_ptr(new udp_buffer);
	new_buff->resize(get_recv_cache_size());

	udp_session_ptr new_session = udp_session_ptr(new udp_session(m_sock));
	new_session->set_data_handler(get_data_handler());
	new_session->set_remote_endpoint(m_svr_ep);

	m_sock.async_receive_from(boost::asio::buffer((char*)(new_buff->data()), new_buff->size()),
		new_session->get_endpoint(),
		boost::bind(&udp_client::handle_receive, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred, new_buff, new_session));

}

void udp_client::handle_receive(const boost::system::error_code& error, std::size_t size, udp_buffer_ptr p_buf, udp_session_ptr session)
{
	boost::mutex::scoped_lock lck(session->get_socket_mutex());

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
		if (NULL != p_handler)
		{
			p_handler->on_session_error(session, error.value(), error.message());
		}

		return;
	}
}

void udp_client::start()
{
	m_sock.open(boost::asio::ip::udp::v4());
	m_sock.set_option(boost::asio::socket_base::reuse_address(true));
	m_session = udp_session_ptr(new udp_session(m_sock));
	m_session->set_remote_endpoint(m_svr_ep);
	m_session->set_data_handler(m_data_handler);
	start_receive();
}
