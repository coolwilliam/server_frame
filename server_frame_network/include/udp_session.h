#pragma once
#include <list>

#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>
#include <boost/thread/mutex.hpp>

#include "tool_ptr_define.h"
#include "network_ptr_define.h"
#include "common_macro.h"
#include "network_exports.h"
#include "network_define.h"

class SERVER_FRAME_NETWORK_API udp_session :public boost::enable_shared_from_this<udp_session>
{
public:
	udp_session(boost::asio::ip::udp::socket& sk);
	~udp_session();
	void post_message(const std::string& msg);

	boost::asio::ip::udp::endpoint& get_endpoint();

	void set_data_handler(udp_data_handler_ptr hd);

	void close();

	boost::asio::io_service::strand& get_strand();

private:

	friend class udp_client;
	friend class udp_server;
	/**
	* �������͵�����д�����绺����
	*/
	void data_write();

	/**
	* �����͵�����
	*/
	void handle_write(const boost::system::error_code& error, size_t size);

	udp_data_handler_ptr get_data_handler() const;

	void set_remote_endpoint(boost::asio::ip::udp::endpoint& ep);

	/*�رջỰ*/
	void __close();

	boost::mutex& get_socket_mutex();
private:
	boost::asio::ip::udp::endpoint	m_remote_endpoint;
	boost::asio::ip::udp::socket&	m_sock;

	typedef std::list<data_buffer_ptr> list_buffer_t;

	/**
	* ��������͵����ݣ���֤ÿ�����ݶ�������
	*/
	udp_session::list_buffer_t m_list_send;

	/**
	* ���ڱ�֤����socket������
	*/
	boost::asio::io_service::strand m_strand;

	udp_data_handler_ptr m_data_handler;

	/**
	* ����ͬ����socket�Ĳ���
	*/
	boost::mutex m_mtx_socket;
private:
	DISABLE_COPY(udp_session)
};

