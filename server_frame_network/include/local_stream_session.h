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
	* 关闭会话
	*/
	void close();

	/**
	* 获取会话套接字
	*/
	session_socket_t& get_socket();

	/*
	*	发送数据
	*/
	int post_msg(const std::string & str_data);

	/**
	* 设置数据处理器
	*/
	void set_data_handler(local_stream_data_handler_ptr new_val);

	/**
	* 获取数据处理器
	*/
	local_stream_data_handler_ptr get_data_handler() const;

	/**
	* 启动会话
	*/
	void start();

	/**
	*	设置接收缓存大小
	*/
	void set_recv_cache_size(std::size_t val);

	/**
	*	获取接收缓存大小
	*/
	std::size_t get_recv_cache_size() const { return m_receive_cache_size; }

	/**
	* 通信方式
	*/
	enum_communicate_mode get_comm_mode() const;

	/**
	* 是否已经启动
	*/
	bool is_started() const;

	/*
	* 同步接收
	*/
	size_t receive(std::string& str_data_recv, int& err_code, std::string& err_msg);

	/*
	* 同步发送
	*/
	size_t send_msg(const std::string& str_data_send, int& err_code, std::string& err_msg);
private:

	friend class local_stream_server;
	friend class local_stream_client;

	/**
	* 将待发送的数据写入网络缓存中
	*/
	void data_write();

	/*
	*	获取远端IP
	*/
	std::string get_remote_ip();

	/*
	*	设置远端IP
	*/
	void set_remote_ip(const std::string& new_val);

	/**
	* 启动接收
	*/
	bool start_receive();

	/**
	* 处理接收数据
	*/
	void handle_read(boost::system::error_code error, std::size_t size, boost::shared_ptr<std::string> p_cache);

	/**
	* 处理发送的数据
	*/
	void handle_write(const boost::system::error_code& error, std::size_t size);

	/**
	* 通信方式
	*/
	void set_comm_mode(enum_communicate_mode newVal);

	/**
	* 是否已经启动
	*/
	void set_started(bool newVal = true);

	/*关闭会话*/
	void __close();
private:
	enum
	{
		//默认接收缓存大小
		receive_cache_size_default = 10 * 1024
	};

	typedef std::list<data_buffer_ptr> list_buffer_t;

	/**
	* 保存待发送的数据，保证每批数据独立发送
	*/
	local_stream_session::list_buffer_t m_list_send;

	/**
	* 数据处理器
	*/
	local_stream_data_handler_ptr m_p_dataHandler;

	/**
	* 会话套接字
	*/
	session_socket_t m_socket;

	/**
	* 会话对应的对端IP
	*/
	std::string m_str_remote_ip;

	/**
	*	接收数据缓存大小
	*/
	std::size_t m_receive_cache_size;

	/**
	* 通信方式
	*/
	enum_communicate_mode m_comm_mode;

	/**
	* 是否已经启动
	*/
	bool m_started;

	/**
	* 用于保证操作socket不乱序
	*/
	boost::asio::io_service::strand m_strand;

	/**
	* 用于同步对socket的操作
	*/
	boost::mutex m_mtx_socket;
private:
	DISABLE_COPY(local_stream_session)
};

