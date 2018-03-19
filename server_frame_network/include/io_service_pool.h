///////////////////////////////////////////////////////////
//  io_service_pool.h
//  Implementation of the Class io_service_pool
//  Created on:      04-����-2017 15:49:45
//  Original author: wuht
///////////////////////////////////////////////////////////

#if !defined(EA_32B19472_624F_4a7d_8CB6_B829AAAB6C6A__INCLUDED_)
#define EA_32B19472_624F_4a7d_8CB6_B829AAAB6C6A__INCLUDED_

#include <boost/shared_ptr.hpp>
#include <vector>
#include <boost/asio.hpp>
#include <boost/noncopyable.hpp>

#include "network_exports.h"

/**
 * io��������
 */
class SERVER_FRAME_NETWORK_API io_service_pool :private boost::noncopyable
{

public:
	explicit io_service_pool(size_t pool_size);

	/**
	 * ����io_service��
	 */
	void run();
	/**
	 * ֹͣ����io_service�������������߳�
	 */
	void stop();
	/**
	 * ˳���ȡһ��io_service
	 */
	boost::asio::io_service& get_io_service();

private:
	typedef boost::shared_ptr<boost::asio::io_service> io_service_ptr;

	typedef std::vector<io_service_ptr> vect_io_service_t;

	typedef boost::shared_ptr<boost::asio::io_service::work> work_ptr;

	typedef std::vector<work_ptr> vect_work_t;

	/**
	 * io_service��
	 */
	io_service_pool::vect_io_service_t m_io_services;
	/**
	 * �����߳�
	 */
	io_service_pool::vect_work_t m_works;
	/**
	 * ָ����һ��io_service������ֵ
	 */
	size_t m_next_io_service_index;

};
#endif // !defined(EA_32B19472_624F_4a7d_8CB6_B829AAAB6C6A__INCLUDED_)
