#ifndef common_function_h__
#define common_function_h__
#include <string>
#include <vector>

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

	/************************************
	* ������:   set_proc_title
	* ��  ��:	���õ�ǰ�������ƣ�ֻӰ��ps�鿴����Ӱ��top��
	* ��  ��: 	
	*			argc	main��������Ĳ�������
	*			argv	main��������Ĳ����б�
	*			title	��Ҫ���õĽ�������
	* ����ֵ:   void
	************************************/
	static void set_proc_title(int argc, char* argv[], const std::string& title);

	typedef std::vector<int> vect_cpu_id_t;

	/************************************
	* ������:   set_proc_on_cpu
	* ��  ��:	���õ�ǰ���̰󶨵�ָ����cpu id������
	* ��  ��: 	
	*			bind_cpu_id_array
	* ����ֵ:   void
	************************************/
	static int set_proc_on_cpu(const vect_cpu_id_t& bind_cpu_id_array);

	/************************************
	* ������:   get_configured_cpu_count
	* ��  ��:	��ȡ�������õ�cpu����
	* ��  ��: 	
	* ����ֵ:   int
	************************************/
	static int get_configured_cpu_count();

	/************************************
	* ������:   get_enabled_cpu_count
	* ��  ��:	��ȡ�������õ�cpu����
	* ��  ��: 	
	* ����ֵ:   int
	************************************/
	static int get_enabled_cpu_count();
};


#endif  // common_function_h__
