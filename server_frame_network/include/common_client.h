///////////////////////////////////////////////////////////
//  common_client.h
//  Implementation of the Class common_client
//  Created on:      11-一月-2017 9:35:17
//  Original author: wuht
///////////////////////////////////////////////////////////

#if !defined(EA_BD838CAB_7940_4214_B276_0D7FF8432A18__INCLUDED_)
#define EA_BD838CAB_7940_4214_B276_0D7FF8432A18__INCLUDED_

#include <string>

#include <boost/asio/io_service.hpp>

#include "network_ptr_define.h"
#include "common_macro.h"
#include "network_exports.h"
#include "network_define.h"

/**
 * 通用客户端
 */
class SERVER_FRAME_NETWORK_API common_client
{

public:
	common_client(const std::string& hostname, unsigned short port, boost::asio::io_service& ios);
	virtual ~common_client();

	/**
	 * 消息处理器
	 */
	void set_data_handler(data_handler_ptr newVal);
	data_handler_ptr get_data_handler() const;

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
	common_session_ptr get_session() const { return m_session; }

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
	void set_session(common_session_ptr val) { m_session = val; }


private:
	/**
	 * 通信方式
	 */
	enum_communicate_mode	m_comm_mode;

	/**
	 * 消息处理器
	 */
	data_handler_ptr		m_data_handler;

	/**
	 * 客户端会话，每个客户端只有一个
	 */
	common_session_ptr		m_session;

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
	std::string					m_server_host;

	/**
	 * 服务端口
	 */
	unsigned short			m_server_port;
private:
	DISABLE_COPY(common_client)
};
#endif // !defined(EA_BD838CAB_7940_4214_B276_0D7FF8432A18__INCLUDED_)
