///////////////////////////////////////////////////////////
//  data_handler.h
//  Implementation of the Interface data_handler
//  Created on:      09-����-2016 11:35:01
//  Original author: Administrator
///////////////////////////////////////////////////////////

#if !defined(EA_12C4770D_7ABF_4e02_AACB_F6E7AB117B3F__INCLUDED_)
#define EA_12C4770D_7ABF_4e02_AACB_F6E7AB117B3F__INCLUDED_

#include "network_ptr_define.h"
#include <string>
using namespace std;

class data_handler
{ 

public:
	virtual ~data_handler() {

	}

	/************************************
	* ������:   	on_received_data
	* ��  ��:	��Ӧ��Ϣ����
	* ��  ��: 	
	*			session		������Ϣ�ĻỰ
	*			str_data	���յ�����
	* ����ֵ:   	bool
	************************************/
	virtual bool on_received_data(common_session_ptr  session, const string & str_data) =0;
	
	/************************************
	* ������:   	on_session_accept
	* ��  ��:	��Ӧ�»Ự����
	* ��  ��: 	
	*			new_session	�µĻỰ
	* ����ֵ:   	bool
	************************************/
	virtual bool on_session_accept(common_session_ptr  new_session) =0;
	
	/************************************
	* ������:   	on_session_close
	* ��  ��:	��Ӧ�Ự�ر�
	* ��  ��: 	
	*			session			���رյĻỰ
	*			str_remote_ip	�Ự��Զ��IP
	* ����ֵ:   	int
	************************************/
	virtual int on_session_close(common_session_ptr  session, const string & str_remote_ip) =0;

};
#endif // !defined(EA_12C4770D_7ABF_4e02_AACB_F6E7AB117B3F__INCLUDED_)
