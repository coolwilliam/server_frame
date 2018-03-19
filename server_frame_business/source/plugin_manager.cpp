///////////////////////////////////////////////////////////
//  plugin_manager.cpp
//  Implementation of the Class plugin_manager
//  Created on:      17-ʮ��-2016 10:28:38
//  Original author: wuht
///////////////////////////////////////////////////////////

#include "plugin_manager.h"
#include <iostream>
#include <dlfcn.h>

#include "plugin_dll_info.h"
#include "plugin_base.h"
#include "plugin_factory_base.h"
#include "tinyxml.h"
#include <boost/bind.hpp>
#include <boost/filesystem.hpp>
#include <boost/asio/io_service.hpp>

bool plugin_manager::init(boost::asio::io_service* ios)
{
	if (is_initialized())
	{
		return true;
	}
	else
	{
		assert(NULL != ios && "io_service can't be null");
		m_ios = ios;

		set_initialized(true);

		return true;
	}
}

const plugin_manager_base::map_plugins_t plugin_manager::get_all_plugins() const
{
	map_plugins_t map_plugins;
	map_dll_info_t::const_iterator it = m_map_dll_info.begin();
	for (; it != m_map_dll_info.end(); ++it)
	{
		if (it->second->is_enabled())
		{
			map_plugins.insert(make_pair(it->first, it->second->get_plugin()));
		}
	}

	return map_plugins;
}

const plugin_ptr plugin_manager::get_plugin(const std::string& str_plugin_name) const
{
	map_dll_info_t::const_iterator it = m_map_dll_info.find(str_plugin_name);
	if (it != m_map_dll_info.end())
	{
		if (it->second->is_enabled())
		{
			return it->second->get_plugin();
		}
		else
		{
			return NULL;
		}
	}
	else
	{
		return NULL;
	}
}

bool plugin_manager::load_plugins(const std::string& str_config_path, const std::string& str_plugins_path)
{
	//��ֹ��μ���
	if (is_plugins_loaded() == true)
	{
		return true;
	}

	if (is_initialized() == false)
	{
		throw std::logic_error("The plugin manager has not been initialized yet.");
	}

	//�ж��ļ��Ƿ����
	boost::filesystem::path config_path = str_config_path;
	boost::filesystem::path plugins_path = str_plugins_path;
	if (false == boost::filesystem::exists(config_path))
	{
		std::stringstream ss;
		ss << __FUNCTION__ << ":" << __LINE__ << ":--->" << "File:" << str_config_path << " does not exist!";
		throw std::logic_error(ss.str().c_str());
	}

	if (false == boost::filesystem::exists(plugins_path))
	{
		std::stringstream ss;
		ss << __FUNCTION__ << ":" << __LINE__ << ":--->" << "File:" << str_plugins_path << " does not exist!";
		throw std::logic_error(ss.str().c_str());
	}

	//ͨ������XML�����ļ������ز��
	TiXmlDocument doc;
	bool b_load = doc.LoadFile(str_config_path);
	if (false == b_load)
	{
		printf("[%s:%s:%d]:--->Error in %s: %s\n", __FILE__, __FUNCTION__, __LINE__, doc.Value(), doc.ErrorDesc());
		return false;
	}

	TiXmlNode* root_node = NULL;
	TiXmlNode* plugin_node = NULL;

	root_node = doc.RootElement();
	if (NULL == root_node)
	{
		printf("[%s:%s:%d]:--->Error no root element !\n", __FILE__, __FUNCTION__, __LINE__);

		return false;
	}

	const char* plugin_element_name = "plugin";
	const char* plugin_enable_name	= "enable";
	const char* plugin_name_name	= "name";

	plugin_node = root_node->FirstChild(plugin_element_name);
	if (NULL == plugin_node)
	{
		printf("[%s:%s:%d]:--->Error no %s element !\n", __FILE__, __FUNCTION__, __LINE__, plugin_element_name);

		return false;
	}

	typedef std::vector<plugin_dll_info_ptr> vct_dll_t;
	vct_dll_t vect_dll;

	int i_index = 0;

	while (NULL != plugin_node)
	{
		TiXmlElement* attr_element = plugin_node->ToElement();
		const std::string* attr_enable_string = attr_element->Attribute(std::string(plugin_enable_name));
		if (NULL == attr_enable_string || attr_enable_string->empty() == true)
		{
			printf("[%s:%s:%d]:--->Warning: Element %s[%d] missing attribute %s !\n", __FILE__, __FUNCTION__, __LINE__, plugin_element_name, i_index, plugin_enable_name);
			plugin_node = plugin_node->NextSibling(plugin_element_name);
			++i_index;
			continue;
		}

		const std::string* attr_name_string = attr_element->Attribute(std::string(plugin_name_name));
		if (NULL == attr_name_string || attr_name_string->empty() == true)
		{
			printf("[%s:%s:%d]:--->Warning: Element %s[%d] missing attribute %s !\n", __FILE__, __FUNCTION__, __LINE__, plugin_element_name, i_index, plugin_name_name);
			plugin_node = plugin_node->NextSibling(plugin_element_name);
			++i_index;
			continue;
		}

		plugin_dll_info_ptr dll_info = new plugin_dll_info();
		dll_info->set_plugin_name(*attr_name_string);
		dll_info->set_enabled(attr_enable_string->compare("0") == 0 ? false : true);

		//����ǻ�Ĳ���ӵ��б���
		if (dll_info->is_enabled())
		{
			vect_dll.push_back(dll_info);
		}
		else
		{
			delete dll_info;
			dll_info = NULL;
		}

		++i_index;

		plugin_node = plugin_node->NextSibling(plugin_element_name);
	}

	//����ָ��·���µĲ����̬��
	const char* dll_prefix	= "libplugin_";
	const char* dll_sufix   = ".so";

/*
	if (*(plugins_path.string().rbegin()) != boost::filesystem::path::preferred_separator)
	{
		char sz_sep[2] = { 0 };
		snprintf(sz_sep, sizeof(sz_sep), "%c", boost::filesystem::path::preferred_separator);
		plugins_path.append(sz_sep);
	}
*/

	for (size_t i = 0; i < vect_dll.size(); ++i)
	{
		plugin_dll_info_ptr& dll_info = vect_dll.at(i);

		std::string str_dll_full_path = plugins_path.string()
			+ boost::filesystem::path::preferred_separator
			+ std::string(dll_prefix)
			+ dll_info->get_plugin_name()
			+ std::string(dll_sufix);

		plugin_dll_info::dll_handler_t dll_handler = dlopen(str_dll_full_path.c_str(), RTLD_NOW);
		
		if (NULL == dll_handler)
		{
			//���ز����̬��ʧ��
			char* dl_err = dlerror();
			if (NULL != dl_err)
			{
				printf("[%s:%s:%d]:--->Error: load plugin %s failed!(%s) !\n", __FILE__, __FUNCTION__, __LINE__, str_dll_full_path.c_str(), dl_err);
			}

			dll_info->set_enabled(false);

			continue;
		}

		//���ö�̬����
		dll_info->set_dll_handler(dll_handler);

		//��ȡ��ں�����ַ
		create_plugin_factory_func_t create_plugin_factory_func = (create_plugin_factory_func_t)dlsym(dll_handler, CREATE_PLUGIN_FACTORY_FUNC_NAME);
		if (NULL == create_plugin_factory_func)
		{
			printf("[%s:%s:%d]:--->Error: load plugin %s failed! can't find symbo %s\n", __FILE__, __FUNCTION__, __LINE__, str_dll_full_path.c_str(), CREATE_PLUGIN_FACTORY_FUNC_NAME);

			dll_info->set_enabled(false);

			continue;
		}

		plugin_factory_base_ptr factory = create_plugin_factory_func(this);
		if (NULL == factory)
		{
			printf("[%s:%s:%d]:--->Error: load plugin %s failed! can't create factory\n", __FILE__, __FUNCTION__, __LINE__, str_dll_full_path.c_str());

			dll_info->set_enabled(false);

			continue;
		}

		//���ù���
		dll_info->set_plugin_factory(factory);

		plugin_ptr plugin_obj = factory->create_plugin(this);
		if (NULL == plugin_obj)
		{
			printf("[%s:%s:%d]:--->Error: load plugin %s failed! can't create plugin object\n", __FILE__, __FUNCTION__, __LINE__, str_dll_full_path.c_str());

			dll_info->set_enabled(false);

			continue;
		}

		//ע����Ϣӳ���
		plugin_obj->reg_msg_map();

		//�����������ò������������
		plugin_obj->set_plugin_mgr(this);

		//���ò������
		plugin_obj->set_plugin_name(dll_info->get_plugin_name());

		//���ò������
		dll_info->set_plugin(plugin_obj);

		//�������̬����Ϣ��ӵ��ֵ���
		if (false == add_plugin(dll_info))
		{
			delete dll_info;
			dll_info = NULL;
		}
	}

	//��ʼ�����
	for (size_t i = 0; i < vect_dll.size(); ++i)
	{
		plugin_dll_info_ptr& dll_info = vect_dll.at(i);

		if (NULL != dll_info
			&& true == dll_info->is_enabled()
			&& NULL != dll_info->get_plugin())
		{
			plugin_ptr plugin_obj = dll_info->get_plugin();
			bool b_init = plugin_obj->init();
			dll_info->set_enabled(b_init);
		}
	}

	//���ü���״̬Ϊtrue
	set_plugins_loaded();

	return true;
}

bool plugin_manager::post_message(const std::string& msg_id, int i_value /*= 0*/, void* p_value /*= NULL*/, plugin_manager_base::vect_plugin_names_t plugin_names /*= vect_plugin_names_t()*/)
{
	if (is_initialized() == false)
	{
		throw std::logic_error("The plugin manager has not been initialized yet.");
	}

	assert(m_ios != NULL && "io_service is NULL");

	m_ios->post(boost::bind(&plugin_manager::send_message, this, msg_id, i_value, p_value, plugin_names));

	return true;
}

bool plugin_manager::send_message(const std::string& msg_id, int i_value /*= 0*/, void* p_value /*= NULL*/, plugin_manager_base::vect_plugin_names_t plugin_names /*= vect_plugin_names_t()*/)
{
	if (is_initialized() == false)
	{
		throw std::logic_error("The plugin manager has not been initialized yet.");
	}

	if (true == plugin_names.empty())
	{
		//���в������
		map_dll_info_t::iterator it_dll = m_map_dll_info.begin();
		for (; it_dll != m_map_dll_info.end(); ++it_dll)
		{
			if (it_dll->second
				&& it_dll->second->is_enabled()
				&& it_dll->second->get_plugin())
			{
				plugin_ptr plugin = it_dll->second->get_plugin();
				const plugin_base::map_msg_func_t& map_msg_func = plugin->get_map_msg_func();
				plugin_base::map_msg_func_t::const_iterator it_func = map_msg_func.find(msg_id);
				if (it_func != map_msg_func.end())
				{
					(plugin->*(it_func->second.msg_function))(i_value, p_value);
				}
				
			}
		}
	}
	else
	{
		//��ָ���������
		for (size_t i = 0; i < plugin_names.size(); ++i)
		{
			plugin_ptr plugin = get_plugin(plugin_names.at(i));
			if (NULL != plugin)
			{
				const plugin_base::map_msg_func_t& map_msg_func = plugin->get_map_msg_func();
				plugin_base::map_msg_func_t::const_iterator it_func = map_msg_func.find(msg_id);
				if (it_func != map_msg_func.end())
				{
					(plugin->*(it_func->second.msg_function))(i_value, p_value);
				}
			}
		}
	}

	return true;
}

plugin_manager::plugin_manager():
m_ios(NULL)
, m_initialized(false)
, m_plugins_loaded(false)
{

}



plugin_manager::~plugin_manager(){

}

bool plugin_manager::add_plugin(plugin_dll_info_ptr plugin)
{
	assert(NULL != plugin && "can't insert null plugin");

	map_dll_info_t::iterator it_find = m_map_dll_info.find(plugin->get_plugin_name());
	if (it_find != m_map_dll_info.end())
	{
		return false;
	}
	else
	{
		m_map_dll_info.insert(make_pair(plugin->get_plugin_name(), plugin));

		return true;
	}
}
