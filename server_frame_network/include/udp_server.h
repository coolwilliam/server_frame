#pragma once
#include <boost/asio.hpp>
#include <boost/enable_shared_from_this.hpp>

#include "network_ptr_define.h"
#include "common_macro.h"
#include "network_exports.h"
#include "network_define.h"

class SERVER_FRAME_NETWORK_API udp_server :public boost::enable_shared_from_this<udp_server>
{
public:
	udp_server(boost::asio::io_service& ios, const boost::asio::ip::udp::endpoint& ep_bind, bool reuse_address = true);
	udp_server(boost::asio::io_service& ios);
	virtual ~udp_server();
	void start();
	void set_data_handler(udp_data_handler_ptr dh);
	udp_data_handler_ptr get_data_handler() const;
	boost::asio::ip::udp::socket& get_socket();
private:
	void start_receive();

	void handle_receive(const boost::system::error_code& error, std::size_t size, udp_buffer_ptr p_buf, udp_session_ptr session);
	
private:
	boost::asio::io_service&					m_ios;
	const boost::asio::ip::udp::endpoint		m_ep_bind;
	boost::asio::ip::udp::socket				m_sock;
	udp_data_handler_ptr						m_dh;
private:
	DISABLE_COPY(udp_server)
};

