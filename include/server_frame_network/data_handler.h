///////////////////////////////////////////////////////////
//  data_handler.h
//  Implementation of the Interface data_handler
//  Created on:      09-八月-2016 11:35:01
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
	* 函数名:   	on_received_data
	* 功  能:	相应消息接收
	* 参  数: 	
	*			session		接收消息的会话
	*			str_data	接收的数据
	* 返回值:   	bool
	************************************/
	virtual bool on_received_data(common_session_ptr  session, const string & str_data) =0;
	
	/************************************
	* 函数名:   	on_session_accept
	* 功  能:	响应新会话连接
	* 参  数: 	
	*			new_session	新的会话
	* 返回值:   	bool
	************************************/
	virtual bool on_session_accept(common_session_ptr  new_session) =0;
	
	/************************************
	* 函数名:   	on_session_close
	* 功  能:	响应会话关闭
	* 参  数: 	
	*			session			待关闭的会话
	*			str_remote_ip	会话的远端IP
	* 返回值:   	int
	************************************/
	virtual int on_session_close(common_session_ptr  session, const string & str_remote_ip) =0;

};
#endif // !defined(EA_12C4770D_7ABF_4e02_AACB_F6E7AB117B3F__INCLUDED_)
