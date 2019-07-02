///////////////////////////////////////////////////////////
//  thread_obj.h
//  Implementation of the Class thread_obj
//  Created on:      16-八月-2016 10:18:29
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
 * 线程对象
 */
class SERVER_FRAME_BUSINESS_API thread_obj
{
public:
	thread_obj();
	virtual ~thread_obj();

	/**
	 * 获取当前任务数
	 */
	unsigned int get_task_count();
	/**
	 * 等待线程体退出并回收资源
	 */
	void join();
	/**
	 * 推送任务
	 */
	bool push_task(task_obj_ptr task);
	/**
	 * 启动线程
	 */
	void start();

	/**
	* 停止线程
	*/
	void stop();

private:
	typedef std::queue<task_obj_ptr> queue_task_t;
	/**
	 * 线程退出标识
	 */
	bool m_b_exit;
	boost::condition_variable_any m_condition_get;
	/**
	 * 最大任务数
	 */
	unsigned int m_max_task_count;
	/**
	 * 用于锁任务队列
	 */
	boost::mutex m_mutex;
	thread_obj::queue_task_t m_queue_task;
	boost::shared_ptr<boost::thread> m_thread;

	/**
	 * 从队列中获取任务
	 */
	task_obj_ptr get_task();
	/**
	 * 任务队列是否已空
	 */
	bool is_empty();
	/**
	 * 任务队列是否已满
	 */
	bool is_full();
	/**
	 * 线程绑定的启动函数
	 */
	void start_work();

private:
	DISABLE_COPY(thread_obj)
};
#endif  // !defined(EA_6098D382_BD3F_4843_B9A6_E1B088F6EE4E__INCLUDED_)
