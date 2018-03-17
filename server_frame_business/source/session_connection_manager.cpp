///////////////////////////////////////////////////////////
//  session_connection_manager.cpp
//  Implementation of the Class session_connection_manager
//  Created on:      09-°ËÔÂ-2016 11:35:03
//  Original author: Administrator
///////////////////////////////////////////////////////////

#include "session_connection_manager.h"


session_connection_manager::session_connection_manager(){

}



session_connection_manager::~session_connection_manager(){

}

session_connection_manager* session_connection_manager::m_p_instance = NULL;


session_connection_manager* session_connection_manager::get_instance()
{
	if (NULL == m_p_instance)
	{
		m_p_instance = new session_connection_manager;
	}

	return  m_p_instance;
}



void session_connection_manager::release_instance(){

	if (NULL != m_p_instance)
	{
		delete m_p_instance;
		m_p_instance = NULL;
	}
}



bool session_connection_manager::add_session_connection(session_connection_ptr sc){

	assert(NULL != sc && "can't add null to session_connection_manager!");

	if (NULL != find_session_connection(sc->get_session_id()))
	{
		return false;
	}
	else
	{
		m_map_sc.insert(make_pair(sc->get_session_id(), sc));
	}
	

	return true;
}


bool session_connection_manager::delete_session_connection(session_connection::session_id_t id){

	if (NULL == find_session_connection(id))
	{
		return false;
	}
	else
	{
		m_map_sc.erase(id);
	}


	return true;
}


session_connection_ptr session_connection_manager::find_session_connection(session_connection::session_id_t id){

	map_session_connection_t::iterator it_find = m_map_sc.find(id);
	if (it_find != m_map_sc.end())
	{
		return it_find->second;
	}
	else
	{
		return  session_connection_ptr();
	}
}


session_connection_ptr session_connection_manager::find_session_connection(common_session_ptr session){

	map_session_connection_t::iterator it = m_map_sc.begin();

	session_connection_ptr scp;
	
	for (; it != m_map_sc.end(); ++it)
	{
		if (true == it->second->find_session(session))
		{
			scp = it->second;
			break;
		}
	}

	return  scp;
}

