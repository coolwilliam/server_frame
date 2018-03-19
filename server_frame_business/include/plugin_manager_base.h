///////////////////////////////////////////////////////////
//  plugin_manager_base.h
//  Implementation of the Class plugin_manager_base
//  Created on:      17-十月-2016 10:28:38
//  Original author: wuht
///////////////////////////////////////////////////////////

#if !defined(EA_C30B8C0A_CD18_44bf_A629_289483A91CDA__INCLUDED_)
#define EA_C30B8C0A_CD18_44bf_A629_289483A91CDA__INCLUDED_

#include <vector>

#include <boost/unordered_map.hpp>

#include "common_macro.h"
#include "plugin_module_ptr_define.h"
#include "business_exports.h"

/**
 * 插件管理器接口
 */
class SERVER_FRAME_BUSINESS_API plugin_manager_base
{

public:
	typedef std::vector<std::string> vect_plugin_names_t;

	typedef boost::unordered_map<std::string, plugin_ptr> map_plugins_t;

	/*
	 * 获取主程序所在路径
	 */
	virtual std::string get_app_path() const = 0;
	/**
	 * 获取所有的插件
	 */
	virtual const plugin_manager_base::map_plugins_t get_all_plugins() const = 0;
	/**
	 * 获取指定插件
	 */
	virtual const plugin_ptr get_plugin(const std::string& str_plugin_name) const = 0;
	/**
	 * 加载插件
	 */
	virtual bool load_plugins(const std::string& str_config_path, const std::string& str_plugins_path) = 0;
	/**
	 * post消息到指定的插件
	 */
	virtual bool post_message(const std::string& msg_id, int i_value = 0, void* p_value = NULL, plugin_manager_base::vect_plugin_names_t plugin_names = vect_plugin_names_t()) = 0;
	/**
	 * send消息到指定的插件
	 */
	virtual bool send_message(const std::string& msg_id, int i_value = 0, void* p_value = NULL, plugin_manager_base::vect_plugin_names_t plugin_names = vect_plugin_names_t()) = 0;

protected:
	plugin_manager_base(){}
	virtual ~plugin_manager_base(){}

private:
	DISABLE_COPY(plugin_manager_base)

};
#endif // !defined(EA_C30B8C0A_CD18_44bf_A629_289483A91CDA__INCLUDED_)
