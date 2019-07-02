#pragma once
#include <list>
#include <string>

#include <boost/asio.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/thread/mutex.hpp>

#include "network_ptr_define.h"
#include "tool_ptr_define.h"
#include "common_macro.h"
#include "network_exports.h"
#include "network_define.h"

class SERVER_FRAME_NETWORK_API local_stream_session
	:public boost::enable_shared_from_this<local_stream_session>
{
public:
	typedef boost::asio::local::stream_protocol::socket session_socket_t;
	local_stream_session(boost::asio::io_service & ios);
	~local_stream_session();
	/**
	* �رջỰ
	*/
	void close();

	/**
	* ��ȡ�Ự�׽���
	*/
	session_socket_t& get_socket();

	/*
	*	��������
	*/
	int post_msg(const std::string & str_data);

	/**
	* �������ݴ�����
	*/
	void set_data_handler(local_stream_data_handler_ptr new_val);

	/**
	* ��ȡ���ݴ�����
	*/
	local_stream_data_handler_ptr get_data_handler() const;

	/**
	* �����Ự
	*/
	void start();

	/**
	*	���ý��ջ����С
	*/
	void set_recv_cache_size(std::size_t val);

	/**
	*	��ȡ���ջ����С
	*/
	std::size_t get_recv_cache_size() const { return m_receive_cache_size; }

	/**
	* ͨ�ŷ�ʽ
	*/
	enum_communicate_mode get_comm_mode() const;

	/**
	* �Ƿ��Ѿ�����
	*/
	bool is_started() const;

	/*
	* ͬ������
	*/
	size_t receive(std::string& str_data_recv, int& err_code, std::string& err_msg);

	/*
	* ͬ������
	*/
	size_t send_msg(const std::string& str_data_send, int& err_code, std::string& err_msg);
private:

	friend class local_stream_server;
	friend class local_stream_client;

	/**
	* �������͵�����д�����绺����
	*/
	void data_write();

	/*
	*	��ȡԶ��IP
	*/
	std::string get_remote_ip();

	/*
	*	����Զ��IP
	*/
	void set_remote_ip(const std::string& new_val);

	/**
	* ��������
	*/
	bool start_receive();

	/**
	* �����������
	*/
	void handle_read(boost::system::error_code error, std::size_t size, boost::shared_ptr<std::string> p_cache);

	/**
	* �����͵�����
	*/
	void handle_write(const boost::system::error_code& error, std::size_t size);

	/**
	* ͨ�ŷ�ʽ
	*/
	void set_comm_mode(enum_communicate_mode newVal);

	/**
	* �Ƿ��Ѿ�����
	*/
	void set_started(bool newVal = true);

	/*�رջỰ*/
	void __close();
private:
	enum
	{
		//Ĭ�Ͻ��ջ����С
		receive_cache_size_default = 10 * 1024
	};

	typedef std::list<data_buffer_ptr> list_buffer_t;

	/**
	* ��������͵����ݣ���֤ÿ�����ݶ�������
	*/
	local_stream_session::list_buffer_t m_list_send;

	/**
	* ���ݴ�����
	*/
	local_stream_data_handler_ptr m_p_dataHandler;

	/**
	* �Ự�׽���
	*/
	session_socket_t m_socket;

	/**
	* �Ự��Ӧ�ĶԶ�IP
	*/
	std::string m_str_remote_ip;

	/**
	*	�������ݻ����С
	*/
	std::size_t m_receive_cache_size;

	/**
	* ͨ�ŷ�ʽ
	*/
	enum_communicate_mode m_comm_mode;

	/**
	* �Ƿ��Ѿ�����
	*/
	bool m_started;

	/**
	* ���ڱ�֤����socket������
	*/
	boost::asio::io_service::strand m_strand;

	/**
	* ����ͬ����socket�Ĳ���
	*/
	boost::mutex m_mtx_socket;
private:
	DISABLE_COPY(local_stream_session)
};

