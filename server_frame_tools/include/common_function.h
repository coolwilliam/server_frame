#ifndef common_function_h__
#define common_function_h__
#include <string>
#include <vector>

#include "tool_exports.h"

class SERVER_FRAME_TOOL_API common_function
{
public:
	/////////////////////获取主程序存放路径/////////////////////////////////////////////////////
	/************************************
	* 函数名：	get_module_dir
	* 功  能：	获取当前模块所在目录
	* 参  数：
	*			local_addr 传入的模块全局地址
	* 返回值:	std::string
	************************************/
	static std::string get_module_dir(void* local_addr = NULL);
	/************************************
	* 函数名：	get_module_fullpath
	* 功  能：	获取当前模块全路径名称
	* 参  数：
	*			fullpath
	*			local_addr 传入的模块全局地址
	* 返回值:	void
	************************************/
	static void get_module_fullpath(char* fullpath, void* local_addr = NULL);
	//////////////////////////////////////////////////////////////////////////

	/************************************
	* 函数名:   set_proc_title
	* 功  能:	设置当前进程名称（只影响ps查看，不影响top）
	* 参  数: 	
	*			argc	main函数传入的参数个数
	*			argv	main函数传入的参数列表
	*			title	需要设置的进程名称
	* 返回值:   void
	************************************/
	static void set_proc_title(int argc, char* argv[], const std::string& title);

	typedef std::vector<int> vect_cpu_id_t;

	/************************************
	* 函数名:   set_proc_on_cpu
	* 功  能:	设置当前进程绑定到指定的cpu id上运行
	* 参  数: 	
	*			bind_cpu_id_array
	* 返回值:   void
	************************************/
	static int set_proc_on_cpu(const vect_cpu_id_t& bind_cpu_id_array);

	/************************************
	* 函数名:   get_configured_cpu_count
	* 功  能:	获取本机配置的cpu核数
	* 参  数: 	
	* 返回值:   int
	************************************/
	static int get_configured_cpu_count();

	/************************************
	* 函数名:   get_enabled_cpu_count
	* 功  能:	获取本机可用的cpu核数
	* 参  数: 	
	* 返回值:   int
	************************************/
	static int get_enabled_cpu_count();
};


#endif  // common_function_h__
