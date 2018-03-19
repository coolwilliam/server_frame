///////////////////////////////////////////////////////////
//  common_session.h
//  Implementation of the Class common_session
//  Created on:      09-八月-2016 11:35:01
//  Original author: wuht
///////////////////////////////////////////////////////////

#if !defined(EA_D744ED6F_9FF6_4fa1_9C4E_04045BEB0927__INCLUDED_)
#define EA_D744ED6F_9FF6_4fa1_9C4E_04045BEB0927__INCLUDED_

#include <list>
#include <string>
using namespace std;

#include <boost/asio/ip/tcp.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/system/error_code.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/asio/strand.hpp>
using namespace boost;
using asio::io_service;
using boost::asio::ip::tcp;


#include "network_ptr_define.h"
#include "tool_ptr_define.h"
#include "common_macro.h"
#include "network_exports.h"
#include "network_define.h"


class SERVER_FRAME_NETWORK_API common_session
	:public boost::enable_shared_from_this<common_session>
{
public:
	explicit common_session(io_service & ios);
	virtual ~common_session();

	/**
	* 关闭会话
	*/
	void close();

	/**
	* 获取会话套接字
	*/
	tcp::socket& get_socket();

	/*
	 *	发送数据
	 */
	int send_msg(const string & str_data);

	/**
	* 设置数据处理器
	*/
	void set_data_handler(data_handler_ptr new_val);

	/**
	* 获取数据处理器
	*/
	data_handler_ptr get_data_handler() const;

	/**
	* 启动会话
	*/
	void start();

	/**
	 *	设置接收缓存大小
	 */
	void set_recv_cache_size(size_t val);

	/**
	*	获取接收缓存大小
	*/
	size_t get_recv_cache_size() const { return m_receive_cache_size; }

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
	size_t receive(string& str_data_recv, int& err_code, string& err_msg);

	/*
	* 同步发送
	*/
	size_t send(const string& str_data_send, int& err_code, string& err_msg);
private:

	friend class common_server;
	friend class common_client;

	/**
	* 将待发送的数据写入网络缓存中
	*/
	void data_write();

	/*
	 *	获取远端IP
	 */
	string get_remote_ip();

	/*
	 *	设置远端IP
	 */
	void set_remote_ip(const string& new_val);

	/**
	* 启动接收
	*/
	bool start_receive();

	/**
	* 处理接收数据
	*/
	void handle_read(system::error_code error, size_t size, boost::shared_ptr<string> p_cache);

	/**
	* 处理发送的数据
	*/
	void handle_write(const system::error_code& error, size_t size);

	/**
	* 通信方式
	*/
	void set_comm_mode(enum_communicate_mode newVal);

	/**
	* 是否已经启动
	*/
	void set_started(bool newVal = true);
private:
	enum
	{
		//默认接收缓存大小
		receive_cache_size_default = 10*1024
	};

	typedef list<data_buffer_ptr> list_buffer_t;

	/**
	 * 保存待发送的数据，保证每批数据独立发送
	 */
	common_session::list_buffer_t m_list_send;

	/**
	 * 数据处理器
	 */
	data_handler_ptr m_p_dataHandler;

	/**
	 * 会话套接字
	 */
	tcp::socket m_socket;

	/**
	 * 会话对应的对端IP
	 */
	string m_str_remote_ip;

	/**
	 *	接收数据缓存大小
	 */
	size_t m_receive_cache_size;

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

private:
	DISABLE_COPY(common_session)
};
#endif  // !defined(EA_D744ED6F_9FF6_4fa1_9C4E_04045BEB0927__INCLUDED_)
