#ifndef simple_kv_config_h__
#define simple_kv_config_h__

/*system headers*/
#include <assert.h>
#include <string.h>
#include <map>
#include <string>
#include <typeinfo>
#include <sstream>
using namespace std;

/*local headers*/
#include "common_macro.h"
#include "tool_exports.h"
#include "tool_ptr_define.h"

/*boost headers*/
#include "boost/unordered_map.hpp"

class SERVER_FRAME_TOOL_API simple_kv_config
{
public:
	simple_kv_config();
	~simple_kv_config();

	typedef map<string, string> map_kv_t, map_valid_type_t;

	/************************************
	* ������:   	load_file
	* ��  ��:	���ض�Ӧ�����ļ�
	* ��  ��:
	* ����ֵ:   	bool
	************************************/
	bool load_file(const string& str_path);

	/************************************
	* ������:   	get
	* ��  ��:	��ȡ��Ӧkeyֵ�Ķ�Ӧ��Ϣ
	* ��  ��: 	
	*			key		��
	*			value	ֵ
	* ����ֵ:   	bool
	************************************/
	template<typename T>
	bool get(const string& key, T& value);

	/************************************
	* ������:   	set
	* ��  ��:	����keyֵ��Ϣ
	* ��  ��: 	
	*			key
	*			value
	* ����ֵ:   	bool
	************************************/

	template<typename T>
	bool set(const string& key, const T& value);

	/************************************
	* ������:   	match
	* ��  ��:	�Աȶ�Ӧkeyֵ�е���Ϣ
	* ��  ��: 	
	*			key
	*			data
	* ����ֵ:   	bool
	************************************/
	bool match(const string& key, const string& data);

	/************************************
	* ������:   	commit
	* ��  ��:	�ύ���ݣ��޸Ĺ�����Ҫ���д˲��������ܱ���
	* ��  ��:
	* ����ֵ:   	bool
	************************************/
	bool commit();

	/************************************
	* ������:   	get_all
	* ��  ��:	��ȡ����������
	* ��  ��: 	
	* ����ֵ:   	simple_kv_config::map_kv_t
	************************************/
	map_kv_t get_all() const;

	/************************************
	* ������:   set_child
	* ��  ��:	�����Ӷ���
	* ��  ��: 	
	*			key
	*			child
	* ����ֵ:   bool
	************************************/
	bool set_child(const std::string& key, simple_kv_config_ptr child);

	/************************************
	* ������:   get_child
	* ��  ��:	��ȡ�Ӷ���
	* ��  ��: 	
	*			key
	*			child_out
	* ����ֵ:   bool
	************************************/
	bool get_child(const std::string& key, simple_kv_config_ptr& child_out);

	/************************************
	* ������:   has_children
	* ��  ��:	�Ƿ����Ӷ���
	* ��  ��: 	
	* ����ֵ:   bool
	************************************/
	bool has_children();

private:
	/************************************
	* ������:   	save_file
	* ��  ��:	����������Ϣ�������ļ�
	* ��  ��: 	
	* ����ֵ:   	bool
	************************************/
	bool save_file();

	/************************************
	* ������:   	get_string
	* ��  ��:	��ȡkey��Ӧ��value string����ֵ
	* ��  ��: 	
	*			key
	* ����ֵ:   	string
	************************************/
	string  get_string(const string& key);

	/************************************
	* ������:   	set_string
	* ��  ��:	����key value
	* ��  ��: 	
	*			key
	*			value
	* ����ֵ:   	bool
	************************************/
	bool	set_string(const string& key, const string& value);

	/************************************
	* ������:   	init_valid_type_dic
	* ��  ��:	��ʼ����Ч���������ֵ�
	* ��  ��: 	
	* ����ֵ:   	void
	************************************/
	void init_valid_type_dic();

	/************************************
	* ������:   	is_valid_type
	* ��  ��:	����Ƿ�����Ч֧�ֵ�����
	* ��  ��: 	
	*			type_name
	* ����ֵ:   	bool
	************************************/
	bool is_valid_type(const string& type_name);

private:
	typedef boost::unordered_map<std::string, simple_kv_config_ptr> map_children_t;

	//�����ֵ�
	map_kv_t m_map_kv;

	//����·��
	string m_path;

	//��
	// boost::mutex m_kv_mutex;

	//��Ч�����������ֵ�
	map_valid_type_t m_map_valid_type;

	// �Ӷ����ֵ�
	map_children_t m_map_children;

private:
	DISABLE_COPY(simple_kv_config)
};

template<typename T>
bool simple_kv_config::get(const string& key, T& value)
{
	stringstream ss;
	string str_value = get_string(key);

	if (true == str_value.empty())
	{
		return false;
	}

	const char* str_typename = typeid(T).name();

	assert(is_valid_type(string(str_typename)) && "is not valid config type");

	ss.write(str_value.c_str(), str_value.size());

	ss >> value;

	return true;
}

template<typename T>
bool simple_kv_config::set(const string& key, const T& value)
{
	const char* str_typename = typeid(T).name();

	assert(is_valid_type(string(str_typename)) && "is not valid config type");

	stringstream ss;

	ss << value;

	return set_string(key, ss.str());
}

#endif  // simple_kv_config_h__
