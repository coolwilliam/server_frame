#pragma once
#include <string>
#include "plugin_iptables_msg.h"

class iptables_function
{
public:
	static bool generate_cmd(const rule_opt_t& opt, std::string& cmd);

	static bool excute_cmd(const std::string& cmd);
};

