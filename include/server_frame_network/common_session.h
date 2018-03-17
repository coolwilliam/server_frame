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
#include <boost/thread.hpp>
using namespace boost;
using asio::io_service;
using boost::asio::ip::tcp;


#include "network_ptr_define.h"
#include "tool_ptr_define.h"
#include "common_macro.h"
#include "network_exports.h"


class SERVER_FRAME_NETWORK_API common_session
	:public boost::enable_shared_from_this<common_session>
{

public:

	common_session(io_service & ios);
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
private:

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
	 *	可写锁
	 */
//	 boost::mutex m_mtx_can_write;
	 
	/**
	 *	可写标识
	 */
//	 bool m_b_can_write;

private:
	DISABLE_COPY(common_session)
};
#endif // !defined(EA_D744ED6F_9FF6_4fa1_9C4E_04045BEB0927__INCLUDED_)
