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

#include <boost/asio/ip/tcp.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/system/error_code.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/asio/strand.hpp>
#include <boost/thread/mutex.hpp>

#include "network_ptr_define.h"
#include "tool_ptr_define.h"
#include "common_macro.h"
#include "network_exports.h"
#include "network_define.h"


class SERVER_FRAME_NETWORK_API common_session
	:public boost::enable_shared_from_this<common_session>
{
public:
	explicit common_session(boost::asio::io_service & ios);
	virtual ~common_session();

	/**
	* �رջỰ
	*/
	void close();

	/**
	* ��ȡ�Ự�׽���
	*/
	boost::asio::ip::tcp::socket& get_socket();

	/*
	 *	��������
	 */
	int send_msg(const std::string & str_data);

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
	void set_recv_cache_size(std::size_t val);

	/**
	*	��ȡ���ջ����С
	*/
	std::size_t get_recv_cache_size() const { return m_receive_cache_size; }

	/**
	* ͨ�ŷ�ʽ
	*/
	enum_communicate_mode get_comm_mode() const;

	/**
	* �Ƿ��Ѿ�����
	*/
	bool is_started() const;

	/*
	* ͬ������
	*/
	size_t receive(std::string& str_data_recv, int& err_code, std::string& err_msg);

	/*
	* ͬ������
	*/
	size_t send(const std::string& str_data_send, int& err_code, std::string& err_msg);

	/*
	*	�첽�ر�
	*/
	void post_close();
private:

	friend class common_server;
	friend class common_client;

	/**
	* �������͵�����д�����绺����
	*/
	void data_write();

	/*
	 *	��ȡԶ��IP
	 */
	std::string get_remote_ip();

	/*
	 *	����Զ��IP
	 */
	void set_remote_ip(const std::string& new_val);

	/**
	* ��������
	*/
	bool start_receive();

	/**
	* �����������
	*/
	void handle_read(boost::system::error_code error, std::size_t size, boost::shared_ptr<std::string> p_cache);

	/**
	* �����͵�����
	*/
	void handle_write(const boost::system::error_code& error, std::size_t size);

	/**
	* ͨ�ŷ�ʽ
	*/
	void set_comm_mode(enum_communicate_mode newVal);

	/**
	* �Ƿ��Ѿ�����
	*/
	void set_started(bool newVal = true);

	/*�رջỰ*/
	void __close();

	data_buffer_ptr front();
	void push_back(data_buffer_ptr buff, std::size_t& list_size_out);
	void pop_front(std::size_t& list_size_out);
private:
	enum
	{
		//Ĭ�Ͻ��ջ����С
		receive_cache_size_default = 10*1024
	};

	typedef std::list<data_buffer_ptr> list_buffer_t;

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
	boost::asio::ip::tcp::socket m_socket;

	/**
	 * �Ự��Ӧ�ĶԶ�IP
	 */
	std::string m_str_remote_ip;

	/**
	 *	�������ݻ����С
	 */
	std::size_t m_receive_cache_size;

	/**
	 * ͨ�ŷ�ʽ
	 */
	enum_communicate_mode m_comm_mode;

	/**
	* �Ƿ��Ѿ�����
	*/
	bool m_started;

	/**
	* ���ڱ�֤����socket������
	*/
	boost::asio::io_service::strand m_strand;

	/**
	* ����ͬ����socket�Ĳ���
	*/
	boost::mutex m_mtx_socket;

	/**
	*	����ͬ�������б�
	*/
	boost::mutex m_mtx_list;
private:
	DISABLE_COPY(common_session)
};
#endif  // !defined(EA_D744ED6F_9FF6_4fa1_9C4E_04045BEB0927__INCLUDED_)
