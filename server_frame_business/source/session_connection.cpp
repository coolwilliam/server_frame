///////////////////////////////////////////////////////////
//  session_connection.cpp
//  Implementation of the Class session_connection
//  Created on:      09-八月-2016 11:35:02
//  Original author: Administrator
///////////////////////////////////////////////////////////

#include "session_connection.h"
#include "common_session.h"
#include "data_buffer.h"

session_connection::session_connection()
	:m_session_id(0)
{

}


session_connection::session_connection(common_session_ptr first, common_session_ptr second, session_id_t id) 
	: m_pair(first, second)
	, m_session_id(id)
{

}

session_connection::~session_connection()
{
	destroy();
}

int session_connection::close_session(common_session_ptr session){

	if (m_pair.first == session)
	{
		m_pair.first.reset();
	}
	else if (m_pair.second == session)
	{
		m_pair.second.reset();
	}
	else
	{
		assert(false && "the session cannot be found in this connection!");
	}

	if (NULL != session)
	{
		session->close();
	}


	return 0;
}


void session_connection::destroy(){

	//清空缓存
	m_list_cache.clear();

	//关闭连接
	if (NULL != m_pair.first)
	{
		m_pair.first->close();
	}

	if (NULL != m_pair.second)
	{
		m_pair.second->close();
	}
}


bool session_connection::find_session(common_session_ptr session) const {

	if (NULL == session)
	{
		return false;
	}

	if (session == m_pair.first 
		|| session == m_pair.second)
	{
		return true;
	}

	return false;
}


common_session_ptr session_connection::get_other(common_session_ptr session) const {

	if (NULL == session)
	{
		return  common_session_ptr();
	}

	if (m_pair.first == session)
	{
		return m_pair.second;
	}
	else if (m_pair.second == session)
	{
		return m_pair.first;
	}
	else
	{
		//在键值对里面不存在传入的session
		return common_session_ptr();
	}

}


session_connection::session_id_t session_connection::get_session_id() const
{
	return  m_session_id;
}


session_connection::session_pair_t session_connection::get_sesson_pair() const
{
	return  m_pair;
}


bool session_connection::push_cache(data_buffer_ptr data, common_session_ptr src){

	assert(find_session(src) && "can't push to cache when source session is not found in this connection !");

	m_list_cache.push_back(make_pair(data, src));

	return true;
}


bool session_connection::send_cache()
{
	//遍历缓存链表，依次发送
	list_pair_cache_t::iterator it_begin = m_list_cache.begin();
	for (; it_begin != m_list_cache.end();)
	{
		//获取另一端会话
		common_session_ptr other = get_other(it_begin->second);
		if (NULL != other)
		{
			string str_data;
			str_data.resize(it_begin->first->get_data_length());
			//str_data.insert(0, it_begin->first->get_data(), it_begin->first->get_data_length());
			memcpy((void*)(str_data.data()), it_begin->first->get_data(), it_begin->first->get_data_length());
			other->send_msg(str_data);

			//发送后从链表中移除
			it_begin = m_list_cache.erase(it_begin);
		}
		else
		{
			//如果另一端为空，则指向下一个元素，继续发送
			it_begin++;
		}
	}

	return true;
}


void session_connection::send_data(data_buffer_ptr data, common_session_ptr src){

	//判断是否是属于该联接的会话
	if (false == find_session(src))
	{
		assert(false && "the session is not belong to this connection!");
		return;
	}

	//获取另一个会话
	common_session_ptr other = get_other(src);

	//如果另一个会话为空，则推到发送缓存中去
	if (NULL == other)
	{
		push_cache(data, src);
	}
	else
	{
		string str_data;
		str_data.resize(data->get_data_length());
		memcpy((void*)(str_data.data()), data->get_data(), data->get_data_length());

		other->send_msg(str_data);
	}
}


void session_connection::set_session(common_session_ptr session){

	if (NULL == session)
	{
		return;
	}

	if (session != m_pair.first
		&& m_pair.second == NULL)
	{
		m_pair.second = session;
	}
	else if (session != m_pair.second
		&& NULL == m_pair.first	)
	{
		m_pair.first = session;
	}
	else
	{
		assert(false && "can't set session to a full session_connection!");
	}
}


void session_connection::set_session_id(session_connection::session_id_t id){

	assert(m_session_id == 0 && "session id can't be set for more than once !");

	m_session_id = id;
}