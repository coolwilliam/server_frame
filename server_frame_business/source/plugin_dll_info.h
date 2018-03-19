///////////////////////////////////////////////////////////
//  plugin_dll_info.h
//  Implementation of the Class plugin_dll_info
//  Created on:      17-ʮ��-2016 10:28:38
//  Original author: wuht
///////////////////////////////////////////////////////////

#if !defined(EA_6210B2C0_2937_4915_AF35_A1ACAB09BE34__INCLUDED_)
#define EA_6210B2C0_2937_4915_AF35_A1ACAB09BE34__INCLUDED_

#include <string>

#include "plugin_module_ptr_define.h"

/**
 * �����̬����Ϣ
 */
class plugin_dll_info
{

public:
	plugin_dll_info();
	~plugin_dll_info();

	typedef void* dll_handler_t;

	dll_handler_t get_dll_handler();
	plugin_ptr get_plugin();
	plugin_factory_base_ptr get_plugin_factory();
	std::string get_plugin_name();
	bool is_enabled();
	void set_dll_handler(dll_handler_t newVal);
	void set_enabled(bool newVal);
	void set_plugin(plugin_ptr newVal);
	void set_plugin_factory(plugin_factory_base_ptr newVal);
	void set_plugin_name(const std::string& newVal);

private:
	/**
	 * �����̬����
	 */
	dll_handler_t m_dll_handler;
	/**
	 * �Ƿ����
	 */
	bool m_enabled;
	/**
	 * �������
	 */
	plugin_ptr m_plugin;
	/**
	 * �������
	 */
	plugin_factory_base_ptr m_plugin_factory;
	/**
	 * �������
	 */
	std::string m_plugin_name;

};

#endif // !defined(EA_6210B2C0_2937_4915_AF35_A1ACAB09BE34__INCLUDED_)
