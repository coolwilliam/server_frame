#ifndef simple_kv_config_h__
#define simple_kv_config_h__

//#include <boost/shared_ptr.hpp>
//#include <boost/thread/mutex.hpp>
#include <map>
#include <string>
#include <typeinfo>
#include <sstream>
#include <assert.h>
#include <string.h>
using namespace std;

#include "common_macro.h"
#include "tool_exports.h"

class SERVER_FRAME_TOOL_API simple_kv_config
{
public:
	simple_kv_config();
	~simple_kv_config();

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

	typedef map<string, string> map_kv_t, map_valid_type_t;

	//�����ֵ�
	map_kv_t m_map_kv;

	//����·��
	string m_path;

	//��
//	boost::mutex m_kv_mutex;

	//��Ч�����������ֵ�
	map_valid_type_t m_map_valid_type;

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

#endif // simple_kv_config_h__
