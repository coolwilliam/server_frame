#include "local_stream_client.h"
#include "local_stream_session.h"


local_stream_client::local_stream_client(boost::asio::io_service& ios, const std::string& file_path)
	: m_comm_mode(ECM_ASYNC)
	, m_started(false)
	, m_ios(ios)
	, m_file_path(file_path)
{
}


local_stream_client::~local_stream_client()
{
	close();
}

void local_stream_client::set_data_handler(local_stream_data_handler_ptr newVal)
{
	m_data_handler = newVal;
}

local_stream_data_handler_ptr local_stream_client::get_data_handler() const
{
	return m_data_handler;
}

bool local_stream_client::is_started() const
{
	return m_started;
}

void local_stream_client::set_comm_mode(enum_communicate_mode newVal)
{
	assert(!is_started() && "Can't change mode since client has been started!");

	m_comm_mode = newVal;
}

enum_communicate_mode local_stream_client::get_comm_mode() const
{
	return m_comm_mode;
}

bool local_stream_client::start(int& err_code, std::string& err_msg)
{
	set_started();

	bool ret = true;

	//�����Ự
	local_stream_session_ptr new_session = local_stream_session_ptr(new local_stream_session(m_ios));

	boost::system::error_code ec;
	{
		boost::asio::local::stream_protocol::endpoint ep(m_file_path);

		local_stream_session::session_socket_t& _socket = new_session->get_socket();

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
	return ret;
}

void local_stream_client::close()
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

void local_stream_client::set_started(bool newVal /*= true*/)
{
	m_started = newVal;
}
