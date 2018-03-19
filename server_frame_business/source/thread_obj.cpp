///////////////////////////////////////////////////////////
//  thread_obj.cpp
//  Implementation of the Class thread_obj
//  Created on:      16-°ËÔÂ-2016 10:18:30
//  Original author: wuht
///////////////////////////////////////////////////////////

#include "thread_obj.h"
#include "task_obj.h"


thread_obj::thread_obj()
	:m_max_task_count(0xFFFFFFFF)
{
	m_b_exit = false;
}



thread_obj::~thread_obj(){
	m_b_exit = true;
}





task_obj_ptr thread_obj::get_task()
{
	task_obj_ptr task;
	boost::mutex::scoped_lock slock(m_mutex);
	m_condition_get.wait(m_mutex, !boost::bind(&thread_obj::is_empty, this));
	task = m_queue_task.front();
	m_queue_task.pop();
	return  task;
}


unsigned int thread_obj::get_task_count(){
	return m_queue_task.size();
}


bool thread_obj::is_empty(){
	return m_queue_task.empty();
}


bool thread_obj::is_full(){
	return (m_queue_task.size() == m_max_task_count);
}


void thread_obj::join(){
	m_thread->join();
}


bool thread_obj::push_task(task_obj_ptr task){
	if (task)
	{
		boost::mutex::scoped_lock slock(m_mutex);

		if (is_full())
		{
			printf("task full\n");
			return false;
		}
		m_queue_task.push(task);
		m_condition_get.notify_one();
		return true;
	}
	else
	{
		return false;
	}
}


void thread_obj::start(){
	m_thread = boost::shared_ptr<boost::thread>(new boost::thread(boost::bind(&thread_obj::start_work, this)));
}


void thread_obj::start_work(){
	while (!m_b_exit)
	{
		task_obj_ptr task = get_task();
		if (task)
		{
			task->start();
		}
		boost::this_thread::interruption_point();
	}
}


void thread_obj::stop(){
	m_thread->interrupt();
}
