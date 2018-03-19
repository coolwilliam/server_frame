#ifndef backtrace_funch_h__
#define backtrace_funch_h__
#include <map>
#include <string>

class backtrace_func
{
public:
	
	/************************************
	* 函数名:   	setup_signals
	* 功  能:	注册监视信号
	* 参  数: 	
	* 返回值:   	int
	************************************/
	static int setup_signals();
private:
	/************************************
	* 函数名:   	dump_stack
	* 功  能:	获取堆栈信息
	* 参  数: 	
	* 返回值:   	void
	************************************/
	static void dump_stack();
	/************************************
	* 函数名:   	sig_handle
	* 功  能:	信号处理函数
	* 参  数: 	
	*			sig_id
	* 返回值:   	void
	************************************/
	static void sig_handle(int sig_id);

private:
	typedef std::map<int, std::string> map_sign_name_t;
	//是否已经注册的监听信号
	static bool m_b_setup;
	//信号和名称字典
	static map_sign_name_t m_map_sig_name;
};

#endif // backtrace_funch_h__
