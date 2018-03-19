#pragma once
#include "plugin_base.h"
#include <list>
#include <string>

class plugin_iptables :
	public plugin_base
{
public:
	typedef std::list<std::string> list_cmd_t;

	plugin_iptables();
	~plugin_iptables();

	virtual bool init();

protected:
	DECLARE_MSG_MAP()

private:
	void on_iptables_msg(int ivalue, void* p_value);

	static void on_cmd_timer(int time_id);

private:

	static list_cmd_t m_list_cmd;
};

