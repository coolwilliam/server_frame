///////////////////////////////////////////////////////////
//  task_obj.h
//  Implementation of the Class task_obj
//  Created on:      16-八月-2016 10:18:29
//  Original author: wuht
///////////////////////////////////////////////////////////

#if !defined(EA_8DC2A168_3235_472b_9762_6BD1B46CC9FD__INCLUDED_)
#define EA_8DC2A168_3235_472b_9762_6BD1B46CC9FD__INCLUDED_

#include "common_macro.h"

/**
 * 任务对象
 */
class task_obj
{
public:
	task_obj(){}

	/**
	 * 启动任务
	 */
	virtual int start() = 0;

protected:
	virtual ~task_obj(){}



private:
	DISABLE_COPY(task_obj)
};
#endif  // !defined(EA_8DC2A168_3235_472b_9762_6BD1B46CC9FD__INCLUDED_)
