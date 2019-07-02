///////////////////////////////////////////////////////////
//  session_connection_manager.cpp
//  Implementation of the Class session_connection_manager
//  Created on:      09-°ËÔÂ-2016 11:35:03
//  Original author: Administrator
///////////////////////////////////////////////////////////

#include "session_connection_manager.h"

bool session_connection_manager::add_session_connection(session_connection_ptr sc){
	assert(NULL != sc && "can't add null to session_connection_manager!");

	if (NULL != find_session_connection(sc->get_session_id()))
	{
		return false;
	}
	else
	{
		boost::mutex::scoped_lock lock(m_mtx_sc);

		m_map_sc.insert(std::make_pair(sc->get_session_id(), sc));
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
		boost::mutex::scoped_lock lock(m_mtx_sc);

		m_map_sc.erase(id);
	}


	return true;
}


session_connection_ptr session_connection_manager::find_session_connection(session_connection::session_id_t id){
	boost::mutex::scoped_lock lock(m_mtx_sc);
	
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
	boost::mutex::scoped_lock lock(m_mtx_sc);

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

