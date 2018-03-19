#ifndef plugin_iptables_msg_h__
#define plugin_iptables_msg_h__
#include <string.h>
#include <string>

#define PLUGIN_IPTABLES		"iptables"
#define PLUGIN_IPTABLES_MSG	"PLUGIN_IPTABLES_MSG"

//�ڽ���
enum internal_table
{
	eit_unknown = 0,
	eit_filter,
	eit_nat,
	eit_mangle,
	eit_raw,
	eit_max
};

//�ڽ�������
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

//Ŀ��ֵ
enum target_value
{
	etv_unknown = 0,
	etv_accept,			//�������ǽ�������ݰ�
	etv_drop,			//����ǽ����
	etv_queue,			//����ǽ�����ݰ��ƽ����û��ռ�
	etv_return,			//ִֹͣ�е�ǰ���еĺ������򣬷��ص�������
	etv_max
};

//Э��
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

//�������
enum rule_opt
{
	ero_unknown = 0,
	ero_append,			//׷��
	ero_insert,			//���
	ero_delete,			//ɾ��
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

//����
typedef struct __rule
{
	/*-------------��������-----------------*/
	protocal		p;			//Э��
	internal_chain	chain;		//�ڽ���
	char			s[16];		//Դ��ַ
	char			d[16];		//Ŀ�ĵ�ַ
	target_value	j;			//ִ��Ŀ��
	char			i[16];		//����ӿ�
	char			o[16];		//����ӿ�

	/*-------------��չ����-----------------*/
	unsigned short	sport[2];	//Դ�˿�
	unsigned short	dport[2];	//Ŀ�Ķ˿�
	tcp_flag		tf;			//TCP��־
	icmp_type		it;			//icmp����

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

//iptables�������
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
