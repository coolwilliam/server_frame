///////////////////////////////////////////////////////////
//  session_connection.h
//  Implementation of the Class session_connection
//  Created on:      09-八月-2016 11:35:02
//  Original author: Administrator
///////////////////////////////////////////////////////////

#if !defined(EA_4E3F0DD7_1F36_4f24_9E98_8769316DA052__INCLUDED_)
#define EA_4E3F0DD7_1F36_4f24_9E98_8769316DA052__INCLUDED_

#include "business_ptr_define.h"
#include "network_ptr_define.h"
#include "tool_ptr_define.h"
#include <utility>
#include <list>
using namespace std;

#include "common_macro.h"
#include "business_exports.h"

/**
 * 会话联接
 */
class SERVER_FRAME_BUSINESS_API session_connection
{

public:
	typedef pair<common_session_ptr, common_session_ptr> session_pair_t;

	typedef unsigned int session_id_t;


	session_connection();
	session_connection(common_session_ptr first, common_session_ptr second, session_connection::session_id_t id);
	~session_connection();


	/************************************
	* 函数名:   	close_session
	* 功  能:	关闭指定会话
	* 参  数: 	
	*			session 待关闭的会话
	* 返回值:   	int
	************************************/
	int close_session(common_session_ptr session);

	/************************************
	* 函数名:   	destroy
	* 功  能:	销毁会话联接
	* 参  数: 	
	* 返回值:   	void
	************************************/
	void destroy();

	/************************************
	* 函数名:   	find_session
	* 功  能:	查找是否含有指定的会话
	* 参  数: 	
	*			session
	* 返回值:   	bool
	************************************/
	bool find_session(common_session_ptr session) const;

	/************************************
	* 函数名:   	get_other
	* 功  能:	通过其中一个会话得到另一个会话
	* 参  数: 	
	*			session		其中一个会话
	* 返回值:   	common_session*
	************************************/
	common_session_ptr get_other(common_session_ptr session) const;

	/************************************
	* 函数名:   	get_session_id
	* 功  能:	获取会话ID
	* 参  数: 	
	* 返回值:   	session_connection::session_id_t
	************************************/
	session_connection::session_id_t get_session_id() const;

	/************************************
	* 函数名:   	get_sesson_pair
	* 功  能:	获取会话配对值
	* 参  数: 	
	* 返回值:   	session_connection::session_pair_t
	************************************/
	session_connection::session_pair_t get_sesson_pair() const;

	/************************************
	* 函数名:   	send_cache
	* 功  能:	将缓存中的数据发送
	* 参  数: 	
	* 返回值:   	bool
	************************************/
	bool send_cache();

	/************************************
	* 函数名:   	send_data
	* 功  能:	将来自src会话的数据传送到另一个会话中
	* 参  数: 	
	*			data	待发送的数据
	*			src		来源会话
	* 返回值:   	void
	************************************/
	void send_data(data_buffer_ptr data, common_session_ptr src);

	/************************************
	* 函数名:   	set_session
	* 功  能:	设置会话
	* 参  数: 	
	*			session	会话
	* 返回值:   	void
	************************************/
	void set_session(common_session_ptr session);

	/************************************
	* 函数名:   	set_session_id
	* 功  能:	设置会话ID
	* 参  数: 	
	*			id	会话ID
	* 返回值:   	void
	************************************/
	void set_session_id(session_connection::session_id_t id);

private:
	typedef pair<data_buffer_ptr, common_session_ptr> pair_cache_t;
	typedef list<pair_cache_t> list_pair_cache_t;

	//缓存队列
	list_pair_cache_t m_list_cache;

	//会话键值对
	session_connection::session_pair_t m_pair;

	//会话ID
	session_connection::session_id_t m_session_id;

private:
	/************************************
	* 函数名:   	push_cache
	* 功  能:	将数据推到缓存链表中
	* 参  数: 	
	*			data	待发送的数据
	*			src		来源会话
	* 返回值:   	bool
	************************************/
	bool push_cache(data_buffer_ptr data, common_session_ptr src);

	DISABLE_COPY(session_connection)

};
#endif // !defined(EA_4E3F0DD7_1F36_4f24_9E98_8769316DA052__INCLUDED_)
