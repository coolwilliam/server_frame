///////////////////////////////////////////////////////////
//  common_client.cpp
//  Implementation of the Class common_client
//  Created on:      11-һ��-2017 9:35:17
//  Original author: wuht
///////////////////////////////////////////////////////////

#include "common_client.h"

#include <boost/asio/ip/tcp.hpp>
#include <boost/lexical_cast.hpp>
using boost::asio::ip::tcp;

#include "common_session.h"

common_client::common_client(const string& hostname, unsigned short port, io_service& ios)
	: m_comm_mode(ECM_ASYNC)
	, m_started(false)
	, m_ios(ios)
	, m_server_host(hostname)
	, m_server_port(port)
{

}



common_client::~common_client(){
	close();
}

enum_communicate_mode common_client::get_comm_mode() const
{
	return m_comm_mode;
}


data_handler_ptr common_client::get_data_handler() const
{
	return m_data_handler;
}


bool common_client::is_started() const
{
	return m_started;
}


void common_client::set_comm_mode(enum_communicate_mode newVal){

	assert(!is_started() && "Can't change mode since client has been started!");

	m_comm_mode = newVal;
}


void common_client::set_data_handler(data_handler_ptr newVal){

	m_data_handler = newVal;
}


void common_client::set_started(bool newVal){

	m_started = newVal;
}


bool common_client::start(int& err_code, string& err_msg){

	set_started();

	bool ret = true;

	//�����Ự
	common_session_ptr new_session = common_session_ptr(new common_session(m_ios));

	tcp::resolver resolver(m_ios);
	boost::system::error_code ec;
	tcp::resolver::query query(m_server_host, boost::lexical_cast<string, unsigned short>(m_server_port));

	tcp::resolver::iterator iter = resolver.resolve(query, ec);
	tcp::resolver::iterator end;

	if (boost::system::errc::success == ec)
	{
		if (iter != end)
		{
			tcp::endpoint ep = *iter;

			tcp::socket& _socket = new_session->get_socket();

			//���ӷ����
			_socket.connect(ep, ec);

			if (boost::system::errc::success != ec)
			{
				//����ʧ��
				ret = false;
				err_code = ec.value();
				err_msg = ec.message();
			}
			else
			{
				//���ӳɹ�

				//�������ݴ�����
				new_session->set_data_handler(get_data_handler());

				//����ͨѶģʽ
				new_session->set_comm_mode(get_comm_mode());

				//����ɹ���session���õ��ͻ���
				set_session(new_session);

				//�����Ự
				new_session->start();
			}
		}
		else
		{
			ostringstream oss;
			oss << "Resolve endpoint for host:" << m_server_host << ", port:" << m_server_port << " failed!";
			err_msg = oss.str();
			ret = false;
		}
	}
	else
	{
		//����ʧ��
		ret = false;
		err_code = ec.value();
		err_msg = ec.message();
	}
	
	return ret;
}

void common_client::close()
{
	if (NULL != m_session)
	{
		//�رջỰ�������ٻỰ����
		m_session->close();
		m_session.reset();
	}

	//״̬��Ϊδ����
	set_started(false);
}
