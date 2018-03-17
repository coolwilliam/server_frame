#ifndef common_function_h__
#define common_function_h__
#include <string>
using namespace std;

#include "tool_exports.h"

class SERVER_FRAME_TOOL_API common_function
{
public:
	/////////////////////��ȡ��������·��/////////////////////////////////////////////////////
	/************************************
	* ��������	get_module_dir
	* ��  �ܣ�	��ȡ��ǰģ������Ŀ¼
	* ��  ����
	*			local_addr �����ģ��ȫ�ֵ�ַ
	* ����ֵ:	std::string
	************************************/
	static std::string get_module_dir(void* local_addr = NULL);
	/************************************
	* ��������	get_module_fullpath
	* ��  �ܣ�	��ȡ��ǰģ��ȫ·������
	* ��  ����
	*			fullpath
	*			local_addr �����ģ��ȫ�ֵ�ַ
	* ����ֵ:	void
	************************************/
	static void get_module_fullpath(char* fullpath, void* local_addr = NULL);
	//////////////////////////////////////////////////////////////////////////
};


#endif // common_function_h__
