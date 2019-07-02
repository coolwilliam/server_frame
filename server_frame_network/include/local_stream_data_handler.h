#pragma once
#include "network_ptr_define.h"
#include <string>

class local_stream_data_handler
{
public:
	virtual ~local_stream_data_handler(){}
	/************************************
	* ������:   	on_received_data
	* ��  ��:	��Ӧ��Ϣ����
	* ��  ��:
	*			session		������Ϣ�ĻỰ
	*			str_data	���յ�����
	* ����ֵ:   	bool
	************************************/
	virtual bool on_received_data(local_stream_session_ptr  session, const std::string & str_data) = 0;

	/************************************
	* ������:   	on_session_accept
	* ��  ��:	��Ӧ�»Ự����
	* ��  ��:
	*			new_session	�µĻỰ
	* ����ֵ:   	bool
	************************************/
	virtual bool on_session_accept(local_stream_session_ptr  new_session) = 0;

	/************************************
	* ������:   	on_session_close
	* ��  ��:	��Ӧ�Ự�ر�
	* ��  ��:
	*			session			���رյĻỰ
	*			str_remote_ip	�Ự��Զ��IP
	* ����ֵ:   	int
	************************************/
	virtual int on_session_close(local_stream_session_ptr  session, const std::string & str_remote_ip) = 0;

	/************************************
	* ������:   	on_session_error
	* ��  ��:	��Ӧ�Ự����
	* ��  ��:
	*			session		�Ự
	*			err_code	������
	*			err_msg		������Ϣ
	* ����ֵ:   	int
	************************************/
	virtual int on_session_error(local_stream_session_ptr  session, int err_code, const std::string& err_msg) { return 0; }
};

