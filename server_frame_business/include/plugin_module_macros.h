#ifndef plugin_module_macros_h__
#define plugin_module_macros_h__

#include "common_macro.h"

#define DECLARE_MSG_MAP_BASE() \
	protected:\
	virtual bool reg_msg_map(){return true;}\

#define DECLARE_MSG_MAP() \
	protected:\
	bool reg_msg_map();\

#define BEGIN_MSG_MAP(CLASS) \
	bool CLASS::reg_msg_map()\
	{\
		typedef CLASS this_class;\
		plugin_base::plugin_msg_func_info_t __msg_engtries[]=\
		{\

#define  ON_MSG(str_msg_reg, mem_func) \
{ new std::string(str_msg_reg), \
(static_cast<plugin_base::on_msg_function_t>(&this_class::mem_func))\
},\

#define  END_MSG_MAP() {\
	new std::string(), \
	NULL}};\
	int icount = _countof(__msg_engtries);\
	for (int i = 0; i < icount-1; ++i)\
	{\
		plugin_base::insert_msg_func_info(__msg_engtries[i]);\
	}\
	return true;\
}\


#endif // plugin_module_macros_h__
