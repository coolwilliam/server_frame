///////////////////////////////////////////////////////////
//  common_server.h
//  Implementation of the Class common_server
//  Created on:      09-����-2016 11:35:01
///////////////////////////////////////////////////////////

#if !defined(EA_367432EF_52BD_46e0_B3CF_56ECE9ACFB34__INCLUDED_)
#define EA_367432EF_52BD_46e0_B3CF_56ECE9ACFB34__INCLUDED_

#include <string>
using namespace std;

#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/tcp.hpp>
using namespace boost::asio;
using namespace boost::asio::ip;
using boost::asio::ip::tcp;

#include "network_ptr_define.h"
#include "common_macro.h"
#include "network_exports.h"
#include "network_define.h"

class SERVER_FRAME_NETWORK_API common_server
{
public:
	common_server(io_service& ios, const tcp::endpoint& ep_bind);
	common_server(io_service_pool_ptr iosp, const tcp::endpoint& ep_bind);
	virtual ~common_server();

	/************************************
	* ������:   	get_data_handler
	* ��  ��:	��ȡ���ݴ�����
	* ��  ��: 	
	* ����ֵ:   	data_handler *
	************************************/
	data_handler_ptr get_data_handler();
	/************************************
	* ������:   	set_data_handler
	* ��  ��:	�������ݴ�����
	* ��  ��: 	
	*			newVal
	* ����ֵ:   	void
	************************************/
	void set_data_handler(data_handler_ptr newVal);
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
	* ����ֵ:   	common_session_ptr
	************************************/
	common_session_ptr accept(int& err_code, string& err_msg);

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
	void handle_accept(common_session_ptr& session, const boost::system::error_code & error);

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
	*	io_service_pool�����̣߳�
	*/
	io_service_pool_ptr m_ios_pool;

	/**
	 * boost IO�������̣߳�
	 */
	io_service & m_io_service;

	/**
	 * �������ӽ�����
	 */
	tcp::acceptor m_acceptor;

	/**
	 * ���ݴ�����
	 */
	data_handler_ptr m_p_data_handler;

	/**
	 * ͨ�ŷ�ʽ
	 */
	enum_communicate_mode m_comm_mode;

	/**
	* �Ƿ��Ѿ�����
	*/
	bool					m_started;

private:
	DISABLE_COPY(common_server)
};
#endif  // !defined(EA_367432EF_52BD_46e0_B3CF_56ECE9ACFB34__INCLUDED_)
