///////////////////////////////////////////////////////////
//  thread_obj.h
//  Implementation of the Class thread_obj
//  Created on:      16-����-2016 10:18:29
//  Original author: wuht
///////////////////////////////////////////////////////////

#if !defined(EA_6098D382_BD3F_4843_B9A6_E1B088F6EE4E__INCLUDED_)
#define EA_6098D382_BD3F_4843_B9A6_E1B088F6EE4E__INCLUDED_

#include <queue>

#include <boost/thread.hpp>
#include <boost/thread/condition_variable.hpp>


#include "business_ptr_define.h"
#include "common_macro.h"
#include "business_exports.h"

/**
 * �̶߳���
 */
class SERVER_FRAME_BUSINESS_API thread_obj
{
public:
	thread_obj();
	virtual ~thread_obj();

	/**
	 * ��ȡ��ǰ������
	 */
	unsigned int get_task_count();
	/**
	 * �ȴ��߳����˳���������Դ
	 */
	void join();
	/**
	 * ��������
	 */
	bool push_task(task_obj_ptr task);
	/**
	 * �����߳�
	 */
	void start();

	/**
	* ֹͣ�߳�
	*/
	void stop();

private:
	typedef std::queue<task_obj_ptr> queue_task_t;
	/**
	 * �߳��˳���ʶ
	 */
	bool m_b_exit;
	boost::condition_variable_any m_condition_get;
	/**
	 * ���������
	 */
	unsigned int m_max_task_count;
	/**
	 * �������������
	 */
	boost::mutex m_mutex;
	thread_obj::queue_task_t m_queue_task;
	boost::shared_ptr<boost::thread> m_thread;

	/**
	 * �Ӷ����л�ȡ����
	 */
	task_obj_ptr get_task();
	/**
	 * ��������Ƿ��ѿ�
	 */
	bool is_empty();
	/**
	 * ��������Ƿ�����
	 */
	bool is_full();
	/**
	 * �̰߳󶨵���������
	 */
	void start_work();

private:
	DISABLE_COPY(thread_obj)
};
#endif  // !defined(EA_6098D382_BD3F_4843_B9A6_E1B088F6EE4E__INCLUDED_)
