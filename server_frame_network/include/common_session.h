///////////////////////////////////////////////////////////
//  common_session.h
//  Implementation of the Class common_session
//  Created on:      09-����-2016 11:35:01
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
	* �رջỰ
	*/
	void close();

	/**
	* ��ȡ�Ự�׽���
	*/
	tcp::socket& get_socket();

	/*
	 *	��������
	 */
	int send_msg(const string & str_data);

	/**
	* �������ݴ�����
	*/
	void set_data_handler(data_handler_ptr new_val);

	/**
	* ��ȡ���ݴ�����
	*/
	data_handler_ptr get_data_handler() const;
	
	/**
	* �����Ự
	*/
	void start();

	/**
	 *	���ý��ջ����С
	 */
	void set_recv_cache_size(size_t val);

	/**
	*	��ȡ���ջ����С
	*/
	size_t get_recv_cache_size() const { return m_receive_cache_size; }
private:

	/**
	* �������͵�����д�����绺����
	*/
	void data_write();

	/*
	 *	��ȡԶ��IP
	 */
	string get_remote_ip();

	/*
	 *	����Զ��IP
	 */
	void set_remote_ip(const string& new_val);

	/**
	* ��������
	*/
	bool start_receive();

	/**
	* �����������
	*/
	void handle_read(system::error_code error, size_t size, boost::shared_ptr<string> p_cache);
	
	/**
	* �����͵�����
	*/
	void handle_write(const system::error_code& error, size_t size);

private:

	enum
	{
		//Ĭ�Ͻ��ջ����С
		receive_cache_size_default = 10*1024
	};

	typedef list<data_buffer_ptr> list_buffer_t;

	/**
	 * ��������͵����ݣ���֤ÿ�����ݶ�������
	 */
	common_session::list_buffer_t m_list_send;

	/**
	 * ���ݴ�����
	 */
	data_handler_ptr m_p_dataHandler;
	
	/**
	 * �Ự�׽���
	 */
	tcp::socket m_socket;

	/**
	 * �Ự��Ӧ�ĶԶ�IP
	 */
	string m_str_remote_ip;

	/**
	 *	�������ݻ����С
	 */
	size_t m_receive_cache_size;
	
	/**
	 *	��д��
	 */
//	 boost::mutex m_mtx_can_write;
	 
	/**
	 *	��д��ʶ
	 */
//	 bool m_b_can_write;

private:
	DISABLE_COPY(common_session)
};
#endif // !defined(EA_D744ED6F_9FF6_4fa1_9C4E_04045BEB0927__INCLUDED_)
