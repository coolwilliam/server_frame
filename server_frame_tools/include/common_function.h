#ifndef common_function_h__
#define common_function_h__
#include <string>
using namespace std;

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
};


#endif // common_function_h__
