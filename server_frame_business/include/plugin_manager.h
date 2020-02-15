///////////////////////////////////////////////////////////
//  plugin_manager.h
//  Implementation of the Class plugin_manager
//  Created on:      17-十月-2016 10:28:38
//  Original author: wuht
///////////////////////////////////////////////////////////

#if !defined(EA_8685F1A4_B46F_4f04_8E5D_D8E573B323ED__INCLUDED_)
#define EA_8685F1A4_B46F_4f04_8E5D_D8E573B323ED__INCLUDED_

#include "plugin_manager_base.h"
#include <boost/unordered_map.hpp>

#include "business_exports.h"
#include "plugin_module_ptr_define.h"
#include "common_singleton.hpp"

namespace boost
{
	namespace asio
	{
#ifndef io_service		
		class io_context io_service;
#endif
	}
}

/**
 * 插件管理器
 */
class SERVER_FRAME_BUSINESS_API plugin_manager : public plugin_manager_base, public common_singleton<plugin_manager>
{
public:
	/************************************
	* 函数名:   	init
	* 功  能:	初始化
	* 参  数: 	
	*			ios		io服务
	* 返回值:   	bool
	************************************/
	bool init(boost::asio::io_service* ios);


	virtual const plugin_manager_base::map_plugins_t get_all_plugins() const;


	virtual const plugin_ptr get_plugin(const std::string& str_plugin_name) const;


	virtual bool load_plugins(const std::string& str_config_path, const std::string& str_plugins_path);


	virtual bool post_message(const std::string& msg_id, int i_value = 0, void* p_value = NULL, plugin_manager_base::vect_plugin_names_t plugin_names = vect_plugin_names_t());


	virtual bool send_message(const std::string& msg_id, int i_value = 0, void* p_value = NULL, plugin_manager_base::vect_plugin_names_t plugin_names = vect_plugin_names_t());

	bool is_initialized() const { return m_initialized; }
	void set_initialized(bool val) { m_initialized = val; }

	virtual std::string get_app_path() const { return m_app_path; }
	void set_app_path(const std::string& val) { m_app_path = val; }
private:

	friend class common_singleton<plugin_manager>;

	plugin_manager();
	virtual ~plugin_manager();
	/************************************
	* 函数名:   	add_plugin
	* 功  能:	添加插件到字典表中
	* 参  数: 	
	*			plugin
	* 返回值:   	void
	************************************/
	bool add_plugin(plugin_dll_info_ptr plugin);

	bool is_plugins_loaded() const { return m_plugins_loaded; }
	void set_plugins_loaded(bool val = true) { m_plugins_loaded = val; }

private:
	typedef boost::unordered_map<std::string, plugin_dll_info_ptr> map_dll_info_t;

	/**
	 * 插件动态库信息集合
	 */
	plugin_manager::map_dll_info_t m_map_dll_info;

	/**
	 * io服务
	 */
	boost::asio::io_service* m_ios;

	/**
	 * 是否初始化
	 */
	bool m_initialized;

	/**
	 * 插件是否已经加载
	 */
	bool m_plugins_loaded;

	/**
	 * 主程序所在路径
	 */
	std::string m_app_path;

};
#endif // !defined(EA_8685F1A4_B46F_4f04_8E5D_D8E573B323ED__INCLUDED_)
