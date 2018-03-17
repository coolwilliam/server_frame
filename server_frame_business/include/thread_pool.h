///////////////////////////////////////////////////////////
//  thread_pool.h
//  Implementation of the Class thread_pool
//  Created on:      16-����-2016 10:18:30
//  Original author: wuht
///////////////////////////////////////////////////////////

#if !defined(EA_474432E6_2AAE_4923_8623_F2B537A53168__INCLUDED_)
#define EA_474432E6_2AAE_4923_8623_F2B537A53168__INCLUDED_

#include "business_ptr_define.h"
#include <vector>
#include "common_macro.h"
#include "business_exports.h"
using namespace std;
/**
 * �����̳߳�
 */
class SERVER_FRAME_BUSINESS_API thread_pool
{

public:
	typedef vector<thread_obj_ptr> vect_thread_t;
	/**
	 *	��ȡ��ʵ��
	 */
	static thread_pool* get_instance();

	/**
	*	�ͷŵ�ʵ��
	*/
	static void release_instance();

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
	void stop_threads();

private:
	thread_pool();
	virtual ~thread_pool();

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
	 *	��ʵ������
	 */
	static thread_pool* m_instance;

	/**
	 * �̳߳ش�С
	 */
	unsigned int m_pool_size;
	/**
	 * �߳�����
	 */
	thread_pool::vect_thread_t m_vect_thread;

private:
	DISABLE_COPY(thread_pool)

};
#endif // !defined(EA_474432E6_2AAE_4923_8623_F2B537A53168__INCLUDED_)
