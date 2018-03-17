///////////////////////////////////////////////////////////
//  thread_pool.h
//  Implementation of the Class thread_pool
//  Created on:      16-八月-2016 10:18:30
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
 * 任务线程池
 */
class SERVER_FRAME_BUSINESS_API thread_pool
{

public:
	typedef vector<thread_obj_ptr> vect_thread_t;
	/**
	 *	获取单实例
	 */
	static thread_pool* get_instance();

	/**
	*	释放单实例
	*/
	static void release_instance();

	/**
	 * 添加任务到线程池中
	 */
	bool add_task(task_obj_ptr task);
	/**
	 * 线程池大小
	 */
	unsigned int get_pool_size();
	/**
	 * 线程池大小
	 */
	void set_pool_size(unsigned int newVal);
	/**
	 * 启动指定数量的线程
	 */
	bool start_threads();
	/**
	 * 停止所有线程
	 */
	void stop_threads();

private:
	thread_pool();
	virtual ~thread_pool();

	/**
	* 比较线程的任务数，是否是first < second
	*/
	bool compare_task_num(thread_obj_ptr first, thread_obj_ptr second);
	/**
	* 获取相对空闲的线程对象
	*/
	thread_obj_ptr get_spare_thread();

private:
	/**
	 *	单实例对象
	 */
	static thread_pool* m_instance;

	/**
	 * 线程池大小
	 */
	unsigned int m_pool_size;
	/**
	 * 线程链表
	 */
	thread_pool::vect_thread_t m_vect_thread;

private:
	DISABLE_COPY(thread_pool)

};
#endif // !defined(EA_474432E6_2AAE_4923_8623_F2B537A53168__INCLUDED_)
