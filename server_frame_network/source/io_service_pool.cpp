///////////////////////////////////////////////////////////
//  io_service_pool.cpp
//  Implementation of the Class io_service_pool
//  Created on:      04-����-2017 15:49:45
//  Original author: wuht
///////////////////////////////////////////////////////////

#include "io_service_pool.h"
#include <boost/thread/thread.hpp>
#include <boost/bind.hpp>

io_service_pool::io_service_pool(size_t pool_size)
	: m_next_io_service_index(0)
{
	assert(0 != pool_size && "io_service pool size can't be 0");

	//�����е�io_service�����蹤����������run()�����˳���������������stop()
	for (size_t i = 0; i < pool_size; ++i)
	{
		io_service_ptr io_service(new boost::asio::io_service);
		work_ptr work(new boost::asio::io_service::work(*io_service));
		m_io_services.push_back(io_service);
		m_works.push_back(work);
	}
}

void io_service_pool::run(){
	//����һ���̳߳أ������������е�io_service
	typedef boost::shared_ptr<boost::thread> thread_ptr;
	typedef std::vector<thread_ptr> vect_thread_t;

	vect_thread_t threads;
	for (vect_io_service_t::size_type i = 0; i < m_io_services.size(); ++i)
	{
		thread_ptr thrd(new boost::thread(boost::bind(&boost::asio::io_service::run, m_io_services[i])));
		threads.push_back(thrd);
	}

	//�ȴ��̳߳��е������߳��˳�
	for (vect_thread_t::size_type i = 0; i < threads.size(); ++i)
	{
		threads[i]->join();
	}
}


void io_service_pool::stop(){
	//��ʽֹͣ����io_service
	for (vect_io_service_t::size_type i = 0; i < m_io_services.size(); ++i)
	{
		m_io_services[i]->stop();
	}
}


boost::asio::io_service& io_service_pool::get_io_service(){

	//ʹ��ѭ��˳���ȡ�ķ�ʽ��ѡȡһ��io_service
	boost::asio::io_service& io_service = *m_io_services[m_next_io_service_index];
	
	++m_next_io_service_index;
	if (m_next_io_service_index == m_io_services.size())
	{
		m_next_io_service_index = 0;
	}

	return io_service;
}