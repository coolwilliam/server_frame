#pragma once
#include <boost/asio.hpp>
#include <boost/enable_shared_from_this.hpp>

#include "network_ptr_define.h"
#include "common_macro.h"
#include "network_exports.h"
#include "network_define.h"

class SERVER_FRAME_NETWORK_API local_stream_server:public boost::enable_shared_from_this<local_stream_server>
{
public:
	local_stream_server(boost::asio::io_service& ios, const std::string& file);
	~local_stream_server();

	/************************************
	* ������:   	get_data_handler
	* ��  ��:	��ȡ���ݴ�����
	* ��  ��:
	* ����ֵ:   	local_stream_data_handler_ptr
	************************************/
	local_stream_data_handler_ptr get_data_handler();

	/************************************
	* ������:   	set_data_handler
	* ��  ��:	�������ݴ�����
	* ��  ��:
	*			newVal
	* ����ֵ:   	void
	************************************/
	void set_data_handler(local_stream_data_handler_ptr newVal);

	/************************************
	* ������:   	start
	* ��  ��:	��������
	* ��  ��:
	* ����ֵ:   	void
	************************************/
	void start();

	/************************************
	* ������:   	get_comm_mode
	* ��  ��:	��ȡͨ�ŷ�ʽ
	* ��  ��:
	* ����ֵ:   	enum_communicate_mode
	************************************/
	enum_communicate_mode get_comm_mode() const;

	/************************************
	* ������:   	set_comm_mode
	* ��  ��:	����ͨ�ŷ�ʽ
	* ��  ��:
	*			newVal
	* ����ֵ:   	void
	************************************/
	void set_comm_mode(enum_communicate_mode newVal);

	/************************************
	* ������:   	is_started
	* ��  ��:	�Ƿ��Ѿ�����
	* ��  ��:
	* ����ֵ:   	bool
	************************************/
	bool is_started() const;

	/************************************
	* ������:   	accept
	* ��  ��:	��������
	* ��  ��:
	*			err_code
	*			err_msg
	* ����ֵ:   	local_stream_session_ptr
	************************************/
	local_stream_session_ptr accept(int& err_code, std::string& err_msg);

	/************************************
	* ������:   	stop
	* ��  ��:	ֹͣ����
	* ��  ��:
	* ����ֵ:   	void
	************************************/
	void stop();

private:
	/************************************
	* ������:   	start_accept
	* ��  ��:	��ʼ������������
	* ��  ��:
	* ����ֵ:   	void
	************************************/
	void start_accept();
	/************************************
	* ������:   	handle_accept
	* ��  ��:	�����µ���������
	* ��  ��:
	*			session
	* ����ֵ:   	int
	************************************/
	void handle_accept(local_stream_session_ptr& session, const boost::system::error_code & error);

	/************************************
	* ������:   	set_started
	* ��  ��:	�Ƿ��Ѿ�����
	* ��  ��:
	*			newVal
	* ����ֵ:   	void
	************************************/
	void set_started(bool newVal = true);
private:
	/**
	* boost IO�������̣߳�
	*/
	boost::asio::io_service & m_io_service;

	/**
	* �������ӽ�����
	*/
	boost::asio::local::stream_protocol::acceptor m_acceptor;

	/**
	* ���ݴ�����
	*/
	local_stream_data_handler_ptr m_p_data_handler;

	/**
	* �Ƿ��Ѿ�����
	*/
	bool					m_started;

	/**
	* ͨ�ŷ�ʽ
	*/
	enum_communicate_mode m_comm_mode;
private:
	DISABLE_COPY(local_stream_server)
};

