#ifndef plugin_iptables_msg_h__
#define plugin_iptables_msg_h__
#include <string.h>
#include <string>

#define PLUGIN_IPTABLES		"iptables"
#define PLUGIN_IPTABLES_MSG	"PLUGIN_IPTABLES_MSG"

//内建表
enum internal_table
{
	eit_unknown = 0,
	eit_filter,
	eit_nat,
	eit_mangle,
	eit_raw,
	eit_max
};

//内建工具链
enum internal_chain
{
	eic_unknown = 0,
	eic_prerouting,
	eic_output,
	eic_forward,
	eic_input,
	eic_postrouting,
	eic_max
};

//目标值
enum target_value
{
	etv_unknown = 0,
	etv_accept,			//允许防火墙接收数据包
	etv_drop,			//防火墙丢弃
	etv_queue,			//防火墙将数据包移交到用户空间
	etv_return,			//停止执行当前链中的后续规则，返回到调用链
	etv_max
};

//协议
enum protocal
{
	ep_unknown = 0,
	ep_all,
	ep_tcp,
	ep_udp,
	ep_icmp,
	ep_esp,
	ep_ah,
	ep_max
};

//规则操作
enum rule_opt
{
	ero_unknown = 0,
	ero_append,			//追加
	ero_insert,			//添加
	ero_delete,			//删除
	ero_max
};

enum tcp_flag
{
	etf_none = 0,
	etf_syn = 0x01,
	etf_ack = 0x01 << 1,
	etf_fin = 0x01 << 2,
	etf_rst = 0x01 << 3,
	etf_urg = 0x01 << 4,
	etf_psh = 0x01 << 5,
	etf_all = unsigned(-1)
};

enum icmp_type
{
	eicmp_type_reply = 0,
	eicmp_type_echo = 8
};

//规则
typedef struct __rule
{
	/*-------------基本参数-----------------*/
	protocal		p;			//协议
	internal_chain	chain;		//内建链
	char			s[16];		//源地址
	char			d[16];		//目的地址
	target_value	j;			//执行目标
	char			i[16];		//输入接口
	char			o[16];		//输出接口

	/*-------------扩展参数-----------------*/
	unsigned short	sport[2];	//源端口
	unsigned short	dport[2];	//目的端口
	tcp_flag		tf;			//TCP标志
	icmp_type		it;			//icmp类型

	__rule()
	{
		init();
	}
	
	~__rule()
	{
		release();
	}
	
	void init()
	{
		//To do initialization here
		memset(this, 0x00, sizeof(__rule));
	}
	
	void release()
	{
		//To do release space here
		memset(this, 0x00, sizeof(__rule));
	}
	
	__rule(const __rule& obj_copy)
	{
		init();
		
		*this = obj_copy;
	}
	
	const __rule& operator=(const __rule& obj_copy)
	{
		if (this == &obj_copy)
		{
			return *this;
		}
		
		release();
		
		//To do copy item from obj_copy to this.
		memcpy(this, &obj_copy, sizeof(__rule));

		return *this;
	}
}rule_t, *rule_ptr;

//iptables规则操作
typedef struct __rule_opt
{
	rule_opt	ro;
	rule_t		rule;

	__rule_opt()
	{
		init();
	}
	
	~__rule_opt()
	{
		release();
	}
	
	void init()
	{
		//To do initialization here
		memset(this, 0x00, sizeof(__rule_opt));
	}
	
	void release()
	{
		//To do release space here
		memset(this, 0x00, sizeof(__rule_opt));
	}
	
	__rule_opt(const __rule_opt& obj_copy)
	{
		init();
		
		*this = obj_copy;
	}
	
	const __rule_opt& operator=(const __rule_opt& obj_copy)
	{
		if (this == &obj_copy)
		{
			return *this;
		}
		
		release();
		
		//To do copy item from obj_copy to this.

		memcpy(this, &obj_copy, sizeof(__rule_opt));
		
		return *this;
	}
}rule_opt_t, *rule_opt_ptr;

#endif // plugin_iptables_msg_h__
