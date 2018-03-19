#ifndef backtrace_funch_h__
#define backtrace_funch_h__
#include <map>
#include <string>

class backtrace_func
{
public:
	
	/************************************
	* ������:   	setup_signals
	* ��  ��:	ע������ź�
	* ��  ��: 	
	* ����ֵ:   	int
	************************************/
	static int setup_signals();
private:
	/************************************
	* ������:   	dump_stack
	* ��  ��:	��ȡ��ջ��Ϣ
	* ��  ��: 	
	* ����ֵ:   	void
	************************************/
	static void dump_stack();
	/************************************
	* ������:   	sig_handle
	* ��  ��:	�źŴ�����
	* ��  ��: 	
	*			sig_id
	* ����ֵ:   	void
	************************************/
	static void sig_handle(int sig_id);

private:
	typedef std::map<int, std::string> map_sign_name_t;
	//�Ƿ��Ѿ�ע��ļ����ź�
	static bool m_b_setup;
	//�źź������ֵ�
	static map_sign_name_t m_map_sig_name;
};

#endif // backtrace_funch_h__
