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
	* 消息处理器
	*/
	void set_data_handler(local_stream_data_handler_ptr newVal);
	local_stream_data_handler_ptr get_data_handler() const;

	/**
	* 是否已经启动
	*/
	bool is_started() const;

	/**
	* 通信方式
	*/
	void set_comm_mode(enum_communicate_mode newVal);
	enum_communicate_mode get_comm_mode() const;

	/**
	* 启动客户端通讯
	*/
	bool start(int& err_code, std::string& err_msg);

	/**
	* 客户端会话
	*/
	local_stream_session_ptr get_session() const { return m_session; }

	/**
	* 关闭客户端通讯
	*/
	void close();
private:
	/**
	* 是否已经启动
	*/
	void set_started(bool newVal = true);

	/**
	* 客户端会话
	*/
	void set_session(local_stream_session_ptr val) { m_session = val; }


private:
	/**
	* 通信方式
	*/
	enum_communicate_mode	m_comm_mode;

	/**
	* 消息处理器
	*/
	local_stream_data_handler_ptr		m_data_handler;

	/**
	* 客户端会话，每个客户端只有一个
	*/
	local_stream_session_ptr		m_session;

	/**
	* 是否已经启动
	*/
	bool					m_started;

	/**
	* boost IO服务
	*/
	boost::asio::io_service&				m_ios;

	/**
	* 服务主机
	*/
	std::string					m_file_path;
};

