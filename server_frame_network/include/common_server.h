///////////////////////////////////////////////////////////
//  common_server.h
//  Implementation of the Class common_server
//  Created on:      09-八月-2016 11:35:01
///////////////////////////////////////////////////////////

#if !defined(EA_367432EF_52BD_46e0_B3CF_56ECE9ACFB34__INCLUDED_)
#define EA_367432EF_52BD_46e0_B3CF_56ECE9ACFB34__INCLUDED_

#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/tcp.hpp>
using namespace boost::asio;
using namespace boost::asio::ip;
using boost::asio::ip::tcp;

#include "network_ptr_define.h"
#include "common_macro.h"
#include "network_exports.h"

class SERVER_FRAME_NETWORK_API common_server
{

public:
	common_server(io_service& ios, const tcp::endpoint& ep_bind);
	virtual ~common_server();

	/************************************
	* 函数名:   	get_data_handler
	* 功  能:	获取数据处理器
	* 参  数: 	
	* 返回值:   	data_handler *
	************************************/
	data_handler_ptr get_data_handler();
	/************************************
	* 函数名:   	set_data_handler
	* 功  能:	设置数据处理器
	* 参  数: 	
	*			newVal
	* 返回值:   	void
	************************************/
	void set_data_handler(data_handler_ptr newVal);
	/************************************
	* 函数名:   	start
	* 功  能:	启动服务
	* 参  数: 	
	* 返回值:   	void
	************************************/
	void start();

private:
	/************************************
	* 函数名:   	start_accept
	* 功  能:	开始接收连接请求
	* 参  数: 	
	* 返回值:   	void
	************************************/
	void start_accept();
	/************************************
	* 函数名:   	handle_accept
	* 功  能:	处理新的连接请求
	* 参  数: 	
	*			session
	* 返回值:   	int
	************************************/
	void handle_accept(common_session_ptr& session, const boost::system::error_code & error);


private:
	/**
	 * boost IO服务
	 */
	io_service & m_io_service;

	/**
	 * 网络连接接收器
	 */
	tcp::acceptor m_acceptor;

	/**
	 * 数据处理器
	 */
	data_handler_ptr m_p_data_handler;

private:
	DISABLE_COPY(common_server)

};
#endif // !defined(EA_367432EF_52BD_46e0_B3CF_56ECE9ACFB34__INCLUDED_)
