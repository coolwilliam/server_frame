///////////////////////////////////////////////////////////
//  session_connection_manager.h
//  Implementation of the Class session_connection_manager
//  Created on:      09-八月-2016 11:35:02
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
 * 会话联接管理器
 */
class SERVER_FRAME_BUSINESS_API session_connection_manager
{

public:
	/************************************
	* 函数名:   	get_instance
	* 功  能:	获取单件
	* 参  数: 	
	* 返回值:   	session_connection_manager*
	************************************/
	static session_connection_manager* get_instance();
	/************************************
	* 函数名:   	release_instance
	* 功  能:	释放单件
	* 参  数: 	
	* 返回值:   	void
	************************************/
	static void release_instance();

	/************************************
	* 函数名:   	add_session_connection
	* 功  能:	增加会话联接
	* 参  数: 	
	*			sc
	* 返回值:   	bool
	************************************/
	bool add_session_connection(session_connection_ptr sc);

	/************************************
	* 函数名:   	delete_session_connection
	* 功  能:	删除会话联接
	* 参  数: 	
	*			id
	* 返回值:   	bool
	************************************/
	bool delete_session_connection(session_connection::session_id_t id);

	/************************************
	* 函数名:   	find_session_connection
	* 功  能:	查找会话联接
	* 参  数: 	
	*			id
	* 返回值:   	session_connection_ptr
	************************************/
	session_connection_ptr find_session_connection(session_connection::session_id_t id);

	/************************************
	* 函数名:   	find_session_connection
	* 功  能:	查找会话联接
	* 参  数: 	
	*			session
	* 返回值:   	session_connection_ptr
	************************************/
	session_connection_ptr find_session_connection(common_session_ptr session);

private:
	session_connection_manager();
	~session_connection_manager();

private:
	typedef map<session_connection::session_id_t, session_connection_ptr> map_session_connection_t;
	
	//会话联接字典
	session_connection_manager::map_session_connection_t m_map_sc;

	//单件
	static session_connection_manager* m_p_instance;

private:
	DISABLE_COPY(session_connection_manager)

};
#endif // !defined(EA_3CB22261_BFBB_4bf7_989B_9F6194F8F7BA__INCLUDED_)
