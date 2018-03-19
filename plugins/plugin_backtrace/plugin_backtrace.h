#pragma once
#include "plugin_base.h"

class plugin_backtrace :
	public plugin_base
{
public:
	plugin_backtrace();
	~plugin_backtrace();
	bool init();
};

