#pragma once
#include <string>

#include "business_exports.h"

class SERVER_FRAME_BUSINESS_API single_app_instance
{
public:
	single_app_instance(const std::string& lock_file);
	~single_app_instance();

	bool is_running();

private:
	bool set_lock(int i_fd);

	std::string m_str_lock_file;
};

