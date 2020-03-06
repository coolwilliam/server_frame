///////////////////////////////////////////////////////////
//  thread_pool.cpp
//  Implementation of the Class thread_pool
//  Created on:      16-八月-2016 10:18:30
//  Original author: wuht
///////////////////////////////////////////////////////////

#include "thread_pool.h"
#include <stdio.h>
#include "thread_obj.h"

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
		ptr->max_task_count(m_max_task_per_thread);
		ptr->start();
		m_vect_thread.push_back(ptr);
	}
	return false;
}


void thread_pool::stop_threads(bool clear/* = false*/){
	for (vect_thread_t::iterator itr = m_vect_thread.begin(); itr != m_vect_thread.end(); ++itr)
	{
		(*itr)->stop();
	}

	// 是否释放相关对象
	if (clear)
	{
		m_vect_thread.clear();
	}
	
}

thread_pool::thread_pool()
	:m_max_task_per_thread(default_max_task_cache_count)
{

}

unsigned int thread_pool::max_task_per_thread() const
{
	return m_max_task_per_thread;
}

void thread_pool::max_task_per_thread(unsigned int val)
{
	m_max_task_per_thread = val;
}

thread_pool_ptr thread_pool::new_instance()
{
	return new thread_pool;
}

size_t thread_pool::task_count() const
{
	size_t task_count = 0;
	vect_thread_t::const_iterator it = m_vect_thread.begin();
	for (; it != m_vect_thread.end(); ++it)
	{
		task_count += it->get()->get_task_count();
	}

	return task_count;
}
