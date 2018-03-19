///////////////////////////////////////////////////////////
//  plugin_base.h
//  Implementation of the Class plugin_base
//  Created on:      17-ʮ��-2016 10:28:38
//  Original author: wuht
///////////////////////////////////////////////////////////

#if !defined(EA_AC25137A_B4E4_4961_8E93_58C76FFD81A8__INCLUDED_)
#define EA_AC25137A_B4E4_4961_8E93_58C76FFD81A8__INCLUDED_

#include <map>
#include <string>
#include <assert.h>

#include "plugin_module_macros.h"
#include "plugin_module_ptr_define.h"
#include "plugin_manager_base.h"
#include "business_exports.h"

class plugin_manager;

/**
 * ����ӿ���
 */
class SERVER_FRAME_BUSINESS_API plugin_base
{

public:
	/**
	 * �������
	 */
	std::string get_plugin_name() const
	{
		return m_str_plugin_name;
	}
	/**
	 * ��ʼ��
	 */
	virtual bool init()
	{
		return true;
	}
	/**
	 * post��Ϣ
	 */
	bool post_message(const std::string& msg_id, int i_value = 0, void* p_value = NULL)
	{
		assert(NULL != m_plugin_mgr && "plugin manager is null");
		std::string str_name = get_plugin_name();
		assert(true != str_name.empty() && "plugin name is empty");
		
		plugin_manager_base::vect_plugin_names_t vect_names;
		vect_names.push_back(str_name);
		return m_plugin_mgr->post_message(msg_id, i_value, p_value, vect_names);
	}
	/**
	 * send��Ϣ
	 */
	bool send_message(const std::string& msg_id, int i_value = 0, void* p_value = NULL)
	{
		assert(NULL != m_plugin_mgr && "plugin manager is null");
		std::string str_name = get_plugin_name();
		assert(true != str_name.empty() && "plugin name is empty");

		plugin_manager_base::vect_plugin_names_t vect_names;
		vect_names.push_back(str_name);
		return m_plugin_mgr->send_message(msg_id, i_value, p_value, vect_names);
	}
	/**
	 * �������
	 */
	void set_plugin_name(const std::string& newVal)
	{
		m_str_plugin_name = newVal;
	}

protected:

	friend class plugin_manager;

	typedef void (plugin_base::*on_msg_function_t)(int, void*);

	/**
	* ��Ϣ��Ӧ��������Ϣ
	*/
	struct plugin_msg_func_info_t
	{
		/**
		* ��Ϣ����
		*/
		std::string* msg_ptr;

		/**
		* ��Ϣ��Ӧ����
		*/
		plugin_base::on_msg_function_t msg_function;
	};


	plugin_base() :m_plugin_mgr(NULL)
	{
		m_map_msg_func.clear();
		m_str_plugin_name.clear();
	}
	virtual ~plugin_base(){}


	/**
	 * ���������
	 */
	plugin_manager_base_ptr get_plugin_mgr()
	{
		return m_plugin_mgr;
	}
	/**
	 * �����Ϣӳ�亯����Ϣ
	 */
	void insert_msg_func_info(const plugin_msg_func_info_t& msg_func_info)
	{
		assert(NULL != msg_func_info.msg_ptr
			&& false == msg_func_info.msg_ptr->empty()
			&& NULL != msg_func_info.msg_function
			&& "invalid information");

		map_msg_func_t::iterator it_find = m_map_msg_func.find(*(msg_func_info.msg_ptr));
		if (it_find != m_map_msg_func.end())
		{
			assert(false && "multiple registration");
		}
		else
		{
			m_map_msg_func.insert(make_pair(*(msg_func_info.msg_ptr), msg_func_info));
		}
	}
	/**
	 * ͳһ����Ϣ��Ӧ����
	 */
	virtual void on_message(const std::string& msg_id, int i_value = 0, void* p_value = NULL){}

	/**
	 * ע����Ϣӳ�亯��
	 */
	DECLARE_MSG_MAP_BASE()

private:
	typedef std::map<std::string, plugin_msg_func_info_t> map_msg_func_t;

	/**
	* ��Ϣӳ���
	*/
	const plugin_base::map_msg_func_t& get_map_msg_func() const
	{
		return m_map_msg_func;
	}

	/**
	* ���������
	*/
	void set_plugin_mgr(plugin_manager_base_ptr newVal)
	{
		m_plugin_mgr = newVal;
	}

private:
	/**
	 * ��Ϣӳ���
	 */
	plugin_base::map_msg_func_t m_map_msg_func;
	/**
	 * ���������
	 */
	plugin_manager_base_ptr m_plugin_mgr;
	/**
	 * �������
	 */
	std::string m_str_plugin_name;

private:
	DISABLE_COPY(plugin_base)
};
#endif // !defined(EA_AC25137A_B4E4_4961_8E93_58C76FFD81A8__INCLUDED_)
