#pragma once
#include <string>

#include <boost/asio.hpp>

#include "network_ptr_define.h"
#include "common_macro.h"
#include "network_exports.h"
#include "network_define.h"

class SERVER_FRAME_NETWORK_API local_stream_client
{
public:
	local_stream_client(boost::asio::io_service& ios, const std::string& file_path);
	~local_stream_client();
	/**
	* ��Ϣ������
	*/
	void set_data_handler(local_stream_data_handler_ptr newVal);
	local_stream_data_handler_ptr get_data_handler() const;

	/**
	* �Ƿ��Ѿ�����
	*/
	bool is_started() const;

	/**
	* ͨ�ŷ�ʽ
	*/
	void set_comm_mode(enum_communicate_mode newVal);
	enum_communicate_mode get_comm_mode() const;

	/**
	* �����ͻ���ͨѶ
	*/
	bool start(int& err_code, std::string& err_msg);

	/**
	* �ͻ��˻Ự
	*/
	local_stream_session_ptr get_session() const { return m_session; }

	/**
	* �رտͻ���ͨѶ
	*/
	void close();
private:
	/**
	* �Ƿ��Ѿ�����
	*/
	void set_started(bool newVal = true);

	/**
	* �ͻ��˻Ự
	*/
	void set_session(local_stream_session_ptr val) { m_session = val; }


private:
	/**
	* ͨ�ŷ�ʽ
	*/
	enum_communicate_mode	m_comm_mode;

	/**
	* ��Ϣ������
	*/
	local_stream_data_handler_ptr		m_data_handler;

	/**
	* �ͻ��˻Ự��ÿ���ͻ���ֻ��һ��
	*/
	local_stream_session_ptr		m_session;

	/**
	* �Ƿ��Ѿ�����
	*/
	bool					m_started;

	/**
	* boost IO����
	*/
	boost::asio::io_service&				m_ios;

	/**
	* ��������
	*/
	std::string					m_file_path;
};

