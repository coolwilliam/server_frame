///////////////////////////////////////////////////////////
//  common_client.h
//  Implementation of the Class common_client
//  Created on:      11-һ��-2017 9:35:17
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
 * ͨ�ÿͻ���
 */
class SERVER_FRAME_NETWORK_API common_client
{

public:
	common_client(const std::string& hostname, unsigned short port, boost::asio::io_service& ios);
	virtual ~common_client();

	/**
	 * ��Ϣ������
	 */
	void set_data_handler(data_handler_ptr newVal);
	data_handler_ptr get_data_handler() const;

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
	common_session_ptr get_session() const { return m_session; }

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
	void set_session(common_session_ptr val) { m_session = val; }


private:
	/**
	 * ͨ�ŷ�ʽ
	 */
	enum_communicate_mode	m_comm_mode;

	/**
	 * ��Ϣ������
	 */
	data_handler_ptr		m_data_handler;

	/**
	 * �ͻ��˻Ự��ÿ���ͻ���ֻ��һ��
	 */
	common_session_ptr		m_session;

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
	std::string					m_server_host;

	/**
	 * ����˿�
	 */
	unsigned short			m_server_port;
private:
	DISABLE_COPY(common_client)
};
#endif // !defined(EA_BD838CAB_7940_4214_B276_0D7FF8432A18__INCLUDED_)
