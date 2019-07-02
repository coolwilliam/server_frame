#pragma once
#include <boost/asio.hpp>

#include "network_ptr_define.h"
#include "common_macro.h"
#include "network_exports.h"
#include "network_define.h"

class SERVER_FRAME_NETWORK_API udp_client
{
public:
	udp_client(boost::asio::io_service& ios, boost::asio::ip::udp::endpoint svr_ep);
	virtual ~udp_client();

	void start();

	void set_data_handler(udp_data_handler_ptr hd);

	/**
	*	设置接收缓存大小
	*/
	void set_recv_cache_size(size_t val);

	/**
	*	获取接收缓存大小
	*/
	size_t get_recv_cache_size() const { return m_receive_cache_size; }

	/**
	*	获取会话对象  
	*/
	udp_session_ptr get_session() const { return m_session; }

private:
	udp_data_handler_ptr get_data_handler() const;

	void start_receive();

	void handle_receive(const boost::system::error_code& error, std::size_t size, udp_buffer_ptr p_buf, udp_session_ptr session);

private:
	enum
	{
		//默认接收缓存大小
		receive_cache_size_default = 10 * 1024
	};


	boost::asio::io_service&		m_ios;
	boost::asio::ip::udp::endpoint	m_svr_ep;
	boost::asio::ip::udp::socket	m_sock;
	udp_data_handler_ptr			m_data_handler;
	/**
	*	接收数据缓存大小
	*/
	size_t m_receive_cache_size;

	udp_session_ptr					m_session;
private:
	DISABLE_COPY(udp_client)
};

