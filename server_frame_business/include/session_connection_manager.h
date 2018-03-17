///////////////////////////////////////////////////////////
//  session_connection_manager.h
//  Implementation of the Class session_connection_manager
//  Created on:      09-����-2016 11:35:02
//  Original author: Administrator
///////////////////////////////////////////////////////////

#if !defined(EA_3CB22261_BFBB_4bf7_989B_9F6194F8F7BA__INCLUDED_)
#define EA_3CB22261_BFBB_4bf7_989B_9F6194F8F7BA__INCLUDED_

#include "session_connection.h"
#include "business_ptr_define.h"
#include <map>
using namespace std;

#include "common_macro.h"
#include "business_exports.h"

/**
 * �Ự���ӹ�����
 */
class SERVER_FRAME_BUSINESS_API session_connection_manager
{

public:
	/************************************
	* ������:   	get_instance
	* ��  ��:	��ȡ����
	* ��  ��: 	
	* ����ֵ:   	session_connection_manager*
	************************************/
	static session_connection_manager* get_instance();
	/************************************
	* ������:   	release_instance
	* ��  ��:	�ͷŵ���
	* ��  ��: 	
	* ����ֵ:   	void
	************************************/
	static void release_instance();

	/************************************
	* ������:   	add_session_connection
	* ��  ��:	���ӻỰ����
	* ��  ��: 	
	*			sc
	* ����ֵ:   	bool
	************************************/
	bool add_session_connection(session_connection_ptr sc);

	/************************************
	* ������:   	delete_session_connection
	* ��  ��:	ɾ���Ự����
	* ��  ��: 	
	*			id
	* ����ֵ:   	bool
	************************************/
	bool delete_session_connection(session_connection::session_id_t id);

	/************************************
	* ������:   	find_session_connection
	* ��  ��:	���һỰ����
	* ��  ��: 	
	*			id
	* ����ֵ:   	session_connection_ptr
	************************************/
	session_connection_ptr find_session_connection(session_connection::session_id_t id);

	/************************************
	* ������:   	find_session_connection
	* ��  ��:	���һỰ����
	* ��  ��: 	
	*			session
	* ����ֵ:   	session_connection_ptr
	************************************/
	session_connection_ptr find_session_connection(common_session_ptr session);

private:
	session_connection_manager();
	~session_connection_manager();

private:
	typedef map<session_connection::session_id_t, session_connection_ptr> map_session_connection_t;
	
	//�Ự�����ֵ�
	session_connection_manager::map_session_connection_t m_map_sc;

	//����
	static session_connection_manager* m_p_instance;

private:
	DISABLE_COPY(session_connection_manager)

};
#endif // !defined(EA_3CB22261_BFBB_4bf7_989B_9F6194F8F7BA__INCLUDED_)
