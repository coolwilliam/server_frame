///////////////////////////////////////////////////////////
//  thread_pool.h
//  Implementation of the Class thread_pool
//  Created on:      16-����-2016 10:18:30
//  Original author: wuht
///////////////////////////////////////////////////////////

#if !defined(EA_474432E6_2AAE_4923_8623_F2B537A53168__INCLUDED_)
#define EA_474432E6_2AAE_4923_8623_F2B537A53168__INCLUDED_

#include <vector>

#include "business_exports.h"
#include "common_macro.h"
#include "business_ptr_define.h"
#include "common_singleton.hpp"

/**
 * �����̳߳�
 */
class SERVER_FRAME_BUSINESS_API thread_pool : public common_singleton<thread_pool>
{
public:
	typedef std::vector<thread_obj_ptr> vect_thread_t;

	/**
	 * ��������̳߳���
	 */
	bool add_task(task_obj_ptr task);
	/**
	 * �̳߳ش�С
	 */
	unsigned int get_pool_size();
	/**
	 * �̳߳ش�С
	 */
	void set_pool_size(unsigned int newVal);
	/**
	 * ����ָ���������߳�
	 */
	bool start_threads();
	/**
	 * ֹͣ�����߳�
	 */
	void stop_threads(bool clear = false);

	/*
	 *	ÿ���߳���󻺴�������
	 */
	unsigned int max_task_per_thread() const;
	void max_task_per_thread(unsigned int val);
	enum {
		default_max_task_cache_count = 0xFFFFFFFF
	};

private:
	friend class common_singleton<thread_pool>;
	thread_pool();
	~thread_pool(){}

	/**
	* �Ƚ��̵߳����������Ƿ���first < second
	*/
	bool compare_task_num(thread_obj_ptr first, thread_obj_ptr second);
	/**
	* ��ȡ��Կ��е��̶߳���
	*/
	thread_obj_ptr get_spare_thread();

private:
	/**
	 * �̳߳ش�С
	 */
	unsigned int m_pool_size;
	/**
	 * �߳�����
	 */
	thread_pool::vect_thread_t m_vect_thread;

	/*
	 *	ÿ���̵߳�ͬʱ��󻺴�������
	 */
	unsigned int m_max_task_per_thread;
};
#endif  // !defined(EA_474432E6_2AAE_4923_8623_F2B537A53168__INCLUDED_)
