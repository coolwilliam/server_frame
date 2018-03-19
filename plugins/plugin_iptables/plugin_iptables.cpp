#include "plugin_iptables.h"
#include "plugin_iptables_msg.h"
#include "plugin_log_adapter.h"
#include "iptables_function.h"
#include "timered_cmd.h"

BEGIN_MSG_MAP(plugin_iptables)
	ON_MSG(PLUGIN_IPTABLES_MSG, on_iptables_msg)
END_MSG_MAP()

plugin_iptables::list_cmd_t plugin_iptables::m_list_cmd;

plugin_iptables::plugin_iptables()
{
}


plugin_iptables::~plugin_iptables()
{
}

bool plugin_iptables::init()
{
	//throw std::logic_error("The method or operation is not implemented.");
	plugin_log_adapter::instance().set_plugin_mgr(get_plugin_mgr());

	//启动计时器
	//5秒检测一次
	timered_cmd tc(5, 0);
	tc.set_handler(plugin_iptables::on_cmd_timer);
	tc.reset();

	return true;
}

void plugin_iptables::on_iptables_msg(int ivalue, void* p_value)
{
	if (NULL != p_value)
	{
		rule_opt_ptr p_opt = (rule_opt_ptr)p_value;

		std::string str_cmd;
		bool b_gc = iptables_function::generate_cmd(*p_opt, str_cmd);
		if (false == b_gc)
		{
			LOG_ERROR("Generic rule option command failed!");
			return;
		}

		str_cmd += " 2>&1";

		if (p_opt->ro == ero_delete)
		{
			m_list_cmd.push_back(str_cmd);
		}
		else
		{
			std::string str_del_cmd;
			p_opt->ro = ero_delete;
			//先执行删除，再执行增加
			iptables_function::generate_cmd(*p_opt, str_del_cmd);
			iptables_function::excute_cmd(str_del_cmd);
			iptables_function::excute_cmd(str_cmd);
		}
	}
}

void plugin_iptables::on_cmd_timer(int time_id)
{
	if (false == m_list_cmd.empty())
	{
		list_cmd_t::iterator::value_type& str_cmd = m_list_cmd.front();

		bool b_exec = iptables_function::excute_cmd(str_cmd);
		if (true == b_exec)
		{
			//如果执行成功，则再次加入执行队列中，直到失败（因为目前只执行规则删除的操作）
			m_list_cmd.push_back(str_cmd);
		}

		m_list_cmd.pop_front();
	}

	timered_cmd tc(5, 0);
	tc.set_handler(plugin_iptables::on_cmd_timer);
	tc.reset();
}

