#include "iptables_function.h"
#include "common_macro.h"
#include <sstream>
#include <stdio.h>
#include <algorithm>

#include "plugin_log_adapter.h"

struct enum_discr
{
	unsigned int		em_value;
	const char*			em_discr;
};

enum_discr g_prot_discr[ep_max] =
{
	{ ep_unknown, "unknown" },
	{ ep_all, "all" },
	{ ep_tcp, "tcp" },
	{ ep_udp, "udp" },
	{ ep_icmp, "icmp" },
	{ ep_esp, "esp" },
	{ ep_ah, "ah" }
};

enum_discr g_table_discr[eit_max] =
{
	{ eit_unknown, "unknown" },
	{ eit_filter, "filter" },
	{ eit_nat, "nat" },
	{ eit_mangle, "mangle" },
	{ eit_raw, "raw" }
};

enum_discr g_target_discr[etv_max] =
{
	{ etv_unknown, "unknown" },
	{ etv_accept, "ACCEPT" },
	{ etv_drop, "DROP" },
	{ etv_queue, "QUEUE" },
	{ etv_return, "RETURN" }
};

enum_discr g_chain_discr[eic_max] =
{
	{ eic_unknown, "unknown" },
	{ eic_prerouting, "PREROUTING" },
	{ eic_output, "OUTPUT" },
	{ eic_forward, "FORWARD" },
	{ eic_input, "INPUT" },
	{ eic_postrouting, "POSTROUTING" }
};

enum_discr g_rule_opt_discr[ero_max] =
{
	{ ero_unknown, "unknown" },
	{ ero_append, "-A" },
	{ ero_insert, "-I" },
	{ ero_delete, "-D" }
};

enum_discr g_tcp_flag[] =
{
	{ etf_none, "NONE" },
	{ etf_syn, "SYN" },
	{ etf_ack, "ACK" },
	{ etf_fin, "FIN" },
	{ etf_rst, "RST" },
	{ etf_urg, "URG" },
	{ etf_psh, "PSH" },
	{ etf_all, "ALL" }
};

const char* g_space = " ";

bool iptables_function::generate_cmd(const rule_opt_t& opt, std::string& cmd)
{
	std::ostringstream os_cmd;
	os_cmd << "iptables" << g_space;

	bool result = false;

	do
	{
		if (opt.ro > ero_unknown && opt.ro < ero_max)
		{
			os_cmd << g_rule_opt_discr[opt.ro].em_discr << g_space;

			if (opt.rule.chain < eic_max && opt.rule.chain > eic_unknown)
			{
				os_cmd << g_chain_discr[opt.rule.chain].em_discr << g_space;
			}
			else
			{
				LOG_ERROR("Invalid chain [" << opt.rule.chain << "]");
				break;
			}
		}
		else
		{
			LOG_ERROR("Invalid rule option [" << opt.ro << "]");
			break;
		}

		if (opt.rule.p < ep_max && opt.rule.p > ep_unknown)
		{
			os_cmd << "-p" << g_space << g_prot_discr[opt.rule.p].em_discr << g_space;
		}

		if (strcmp(opt.rule.s, "") != 0)
		{
			os_cmd << "-s" << g_space << opt.rule.s << g_space;
		}

		if (strcmp(opt.rule.d, "") != 0)
		{
			os_cmd << "-d" << g_space << opt.rule.d << g_space;
		}

		if (opt.rule.j < etv_max && opt.rule.j > etv_unknown)
		{
			os_cmd << "-j" << g_space << g_target_discr[opt.rule.j].em_discr << g_space;
		}

		if (strcmp(opt.rule.i, "") != 0)
		{
			os_cmd << "-i" << g_space << opt.rule.i << g_space;
		}

		if (strcmp(opt.rule.o, "") != 0)
		{
			os_cmd << "-o" << g_space << opt.rule.o << g_space;
		}

		if (opt.rule.sport[0] != 0 && opt.rule.sport[1] != 0)
		{
			os_cmd << "--sport" << g_space;
			if (opt.rule.sport[0] == opt.rule.sport[1])
			{
				os_cmd << opt.rule.sport[0];
			}
			else
			{
				os_cmd << std::min(opt.rule.sport[0], opt.rule.sport[1]) << ":" << std::max(opt.rule.sport[0], opt.rule.sport[1]);
			}

			os_cmd << g_space;
		}

		if (opt.rule.dport[0] != 0 && opt.rule.dport[1] != 0)
		{
			os_cmd << "--dport" << g_space;
			if (opt.rule.dport[0] == opt.rule.dport[1])
			{
				os_cmd << opt.rule.dport[0];
			}
			else
			{
				os_cmd << std::min(opt.rule.dport[0], opt.rule.dport[1]) << ":" << std::max(opt.rule.dport[0], opt.rule.dport[1]);
			}

			os_cmd << g_space;
		}

		size_t flg_count = _countof(g_tcp_flag);

		if (opt.rule.tf == etf_all)
		{
			os_cmd << "--tcp-flags " << g_tcp_flag[flg_count - 1].em_discr << g_space;
		}
		else
		{
			bool first_match = false;
			std::string str_flg = "";
			for (size_t i = 1; i < flg_count - 1; ++i)
			{
				if (opt.rule.tf & g_tcp_flag[i].em_value)
				{
					if (false == first_match)
					{
						first_match = true;
					}
					else
					{
						str_flg += ",";
					}

					str_flg += g_tcp_flag[i].em_discr;
				}
			}

			if (str_flg.empty() == false)
			{
				os_cmd << "--tcp-flags" << g_space << str_flg;
			}
		}

		result = true;
	} while (0);

	cmd = os_cmd.str();

	return result;
}

bool iptables_function::excute_cmd(const std::string& cmd)
{
	FILE* fstream = NULL;

	char buff[1024] = { 0 };

	if (NULL == (fstream = popen(cmd.c_str(), "r")))
	{
		LOG_ERROR("Do command [" << cmd << "] failed!");
		return false;
	}

	std::ostringstream var_oss;
	bool result = false;

	while (NULL != fgets(buff, sizeof(buff), fstream))
	{
		var_oss << buff;
		memset(buff, 0x00, sizeof(buff));
	}
	std::string str_res = var_oss.str();
	if (str_res.find(PLUGIN_IPTABLES) != std::string::npos)
	{
		LOG_ERROR("Command: " << cmd << std::endl
			<< "Result: " << str_res);
		result = false;
	}
	else
	{
		LOG_TRACE("Command: " << cmd << std::endl);

		result = true;
	}

	pclose(fstream);

	return result;
}
