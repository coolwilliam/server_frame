///////////////////////////////////////////////////////////
//  session_connection.h
//  Implementation of the Class session_connection
//  Created on:      09-����-2016 11:35:02
//  Original author: Administrator
///////////////////////////////////////////////////////////

#if !defined(EA_4E3F0DD7_1F36_4f24_9E98_8769316DA052__INCLUDED_)
#define EA_4E3F0DD7_1F36_4f24_9E98_8769316DA052__INCLUDED_

#include <utility>
#include <list>

#include "business_ptr_define.h"
#include "network_ptr_define.h"
#include "tool_ptr_define.h"
#include "common_macro.h"
#include "business_exports.h"

/**
 * �Ự����
 */
class SERVER_FRAME_BUSINESS_API session_connection
{
public:
	typedef std::pair<common_session_ptr, common_session_ptr> session_pair_t;

	typedef unsigned int session_id_t;


	session_connection();
	session_connection(common_session_ptr first, common_session_ptr second, session_connection::session_id_t id);
	~session_connection();


	/************************************
	* ������:   	close_session
	* ��  ��:	�ر�ָ���Ự
	* ��  ��: 	
	*			session ���رյĻỰ
	* ����ֵ:   	int
	************************************/
	int close_session(common_session_ptr session);

	/************************************
	* ������:   	destroy
	* ��  ��:	���ٻỰ����
	* ��  ��: 	
	* ����ֵ:   	void
	************************************/
	void destroy();

	/************************************
	* ������:   	find_session
	* ��  ��:	�����Ƿ���ָ���ĻỰ
	* ��  ��: 	
	*			session
	* ����ֵ:   	bool
	************************************/
	bool find_session(common_session_ptr session) const;

	/************************************
	* ������:   	get_other
	* ��  ��:	ͨ������һ���Ự�õ���һ���Ự
	* ��  ��: 	
	*			session		����һ���Ự
	* ����ֵ:   	common_session*
	************************************/
	common_session_ptr get_other(common_session_ptr session) const;

	/************************************
	* ������:   	get_session_id
	* ��  ��:	��ȡ�ỰID
	* ��  ��: 	
	* ����ֵ:   	session_connection::session_id_t
	************************************/
	session_connection::session_id_t get_session_id() const;

	/************************************
	* ������:   	get_session_pair
	* ��  ��:	��ȡ�Ự���ֵ
	* ��  ��: 	
	* ����ֵ:   	session_connection::session_pair_t
	************************************/
	session_connection::session_pair_t get_session_pair() const;

	/************************************
	* ������:   	send_cache
	* ��  ��:	�������е����ݷ���
	* ��  ��: 	
	* ����ֵ:   	bool
	************************************/
	bool send_cache();

	/************************************
	* ������:   	send_data
	* ��  ��:	������src�Ự�����ݴ��͵���һ���Ự��
	* ��  ��: 	
	*			data	�����͵�����
	*			src		��Դ�Ự
	* ����ֵ:   	void
	************************************/
	void send_data(data_buffer_ptr data, common_session_ptr src);

	/************************************
	* ������:   	set_session
	* ��  ��:	���ûỰ
	* ��  ��: 	
	*			session	�Ự
	* ����ֵ:   	bool
	************************************/
	bool set_session(common_session_ptr session);

	/************************************
	* ������:   	set_session_id
	* ��  ��:	���ûỰID
	* ��  ��: 	
	*			id	�ỰID
	* ����ֵ:   	void
	************************************/
	void set_session_id(session_connection::session_id_t id);

	/************************************
	* ������:   	is_close_on_destroy
	* ��  ��:	�Ƿ����ټ��ر�
	* ��  ��: 	
	* ����ֵ:   	bool
	************************************/
	bool is_close_on_destroy() const;

	/************************************
	* ������:   	set_close_on_destroy
	* ��  ��:	�������ټ��ر�
	* ��  ��: 	
	*			val
	* ����ֵ:   	void
	************************************/
	void set_close_on_destroy(bool val = true);
private:
	typedef std::pair<data_buffer_ptr, common_session_ptr> pair_cache_t;
	typedef std::list<pair_cache_t> list_pair_cache_t;

	// �������
	list_pair_cache_t m_list_cache;

	// �Ự��ֵ��
	session_connection::session_pair_t m_pair;

	// �ỰID
	session_connection::session_id_t m_session_id;

	// ��������ʱ���ر���صĻỰ
	bool m_close_on_destroy;

private:
	/************************************
	* ������:   	push_cache
	* ��  ��:	�������Ƶ�����������
	* ��  ��: 	
	*			data	�����͵�����
	*			src		��Դ�Ự
	* ����ֵ:   	bool
	************************************/
	bool push_cache(data_buffer_ptr data, common_session_ptr src);

	DISABLE_COPY(session_connection)
};
#endif  // !defined(EA_4E3F0DD7_1F36_4f24_9E98_8769316DA052__INCLUDED_)
