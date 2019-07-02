#include "udp_session.h"
#include "data_buffer.h"
#include <boost/bind.hpp>
#include "udp_data_handler.h"

udp_session::udp_session(boost::asio::ip::udp::socket& sk)
	:m_sock(sk)
	, m_strand(sk.get_io_service())
{

}

udp_session::~udp_session()
{
}

void udp_session::post_message(const std::string& msg)
{
	data_buffer_ptr buff = data_buffer_ptr(new data_buffer);
	buff->write_byte_array(msg.data(), msg.size());

	bool b_write = m_list_send.empty();

	m_list_send.push_back(buff);

	if (b_write)
	{
		data_write();
	}

	return;
}

boost::asio::ip::udp::endpoint& udp_session::get_endpoint()
{
	return m_remote_endpoint;
}

void udp_session::data_write()
{
	//判断链表是否为空
	if (true == m_list_send.empty())
	{
		return;
	}

	data_buffer_ptr data_ptr = m_list_send.front();

	m_sock.async_send_to(
		boost::asio::buffer(reinterpret_cast<void*>(data_ptr->get_data() + data_ptr->get_readpos()), data_ptr->get_data_length() - data_ptr->get_readpos()),
		m_remote_endpoint,
		m_strand.wrap(boost::bind(&udp_session::handle_write,
		shared_from_this(),
		boost::asio::placeholders::error,
		boost::asio::placeholders::bytes_transferred)));
}

void udp_session::handle_write(const boost::system::error_code& error, size_t size)
{
	boost::mutex::scoped_lock lck(m_mtx_socket);
	
	if (boost::system::errc::success == error)
	{
		data_buffer_ptr data_ptr = m_list_send.front();

		size_t left_size = data_ptr->get_data_length() - data_ptr->get_readpos();

		if (left_size == size
			&& 0 != size)
		{
			//如果发送的大小等于数据缓存大小，则从队列中删除
			m_list_send.pop_front();
		}
		else if (left_size > size)
		{
			//如果发送的大小小于数据缓存大小，则偏移下一次发送的位置
			data_ptr->set_readpos(data_ptr->get_readpos() + size);
		}

		data_write();
	}
	else
	{
		m_list_send.pop_front();

		udp_data_handler_ptr p_handler = get_data_handler();
		if (NULL != p_handler)
		{
			p_handler->on_session_error(shared_from_this(), error.value(), error.message());
		}

		//close();
	}

	return;
}

udp_data_handler_ptr udp_session::get_data_handler() const
{
	return m_data_handler;
}

void udp_session::close()
{
	boost::mutex::scoped_lock lck(m_mtx_socket);

	__close();
}

boost::asio::io_service::strand& udp_session::get_strand()
{
	return m_strand;
}

void udp_session::set_data_handler(udp_data_handler_ptr hd)
{
	m_data_handler = hd;
}

void udp_session::set_remote_endpoint(boost::asio::ip::udp::endpoint& ep)
{
	m_remote_endpoint = ep;
}

void udp_session::__close()
{
	if (m_sock.is_open())
	{
		udp_data_handler_ptr p_data_handler = get_data_handler();

		boost::system::error_code ec;
		m_sock.shutdown(boost::asio::socket_base::shutdown_both, ec);
		m_sock.close(ec);
		if (ec)
		{
			if (NULL != p_data_handler)
			{
				p_data_handler->on_session_error(shared_from_this(), ec.value(), ec.message());
			}
		}
	}
}

boost::mutex& udp_session::get_socket_mutex()
{
	return m_mtx_socket;
}

