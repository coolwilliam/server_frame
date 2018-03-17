///////////////////////////////////////////////////////////
//  thread_pool.cpp
//  Implementation of the Class thread_pool
//  Created on:      16-°ËÔÂ-2016 10:18:30
//  Original author: wuht
///////////////////////////////////////////////////////////

#include "thread_pool.h"
#include <stdio.h>
#include "thread_obj.h"



thread_pool::thread_pool(){

}



thread_pool::~thread_pool(){

}


thread_pool* thread_pool::m_instance = NULL;


thread_pool* thread_pool::get_instance()
{
	if (NULL == m_instance)
	{
		m_instance = new thread_pool;
	}

	return m_instance;
}

void thread_pool::release_instance()
{
	if (NULL != m_instance)
	{
		delete m_instance;
		m_instance = NULL;
	}
}

bool thread_pool::add_task(task_obj_ptr task)
{
	thread_obj_ptr temp;
	if (!task)
	{
		printf("task do not exist\n");
		return false;
	}
	temp = get_spare_thread();
	temp->push_task(task);
	return true;
}


bool thread_pool::compare_task_num(thread_obj_ptr first, thread_obj_ptr second){

	return (first->get_task_count() < second->get_task_count());
}


unsigned int thread_pool::get_pool_size(){

	return m_pool_size;
}


thread_obj_ptr thread_pool::get_spare_thread()
{
	vect_thread_t::iterator itr;
	if (m_vect_thread.begin() != m_vect_thread.end())
	{
		itr = std::min_element(m_vect_thread.begin(), m_vect_thread.end(), boost::bind(&thread_pool::compare_task_num, this, _1, _2));
		return (*itr);
	}
	else
	{
		return  thread_obj_ptr();

	}
}



void thread_pool::set_pool_size(unsigned int newVal){

	m_pool_size = newVal;
}


bool thread_pool::start_threads(){

	for (unsigned int index = 0; index < m_pool_size; index++)
	{
		thread_obj_ptr ptr = thread_obj_ptr(new thread_obj);
		ptr->start();
		m_vect_thread.push_back(ptr);
	}
	return false;
}


void thread_pool::stop_threads(){

	for (vect_thread_t::iterator itr = m_vect_thread.begin(); itr != m_vect_thread.end(); ++itr)
	{
		(*itr)->stop();
	}
}