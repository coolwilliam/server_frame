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
	* 函数名:   	load_file
	* 功  能:	加载对应配置文件
	* 参  数:
	* 返回值:   	bool
	************************************/
	bool load_file(const string& str_path);

	/************************************
	* 函数名:   	get
	* 功  能:	获取对应key值的对应信息
	* 参  数: 	
	*			key		键
	*			value	值
	* 返回值:   	bool
	************************************/
	template<typename T>
	bool get(const string& key, T& value);

	/************************************
	* 函数名:   	set
	* 功  能:	设置key值信息
	* 参  数: 	
	*			key
	*			value
	* 返回值:   	bool
	************************************/

	template<typename T>
	bool set(const string& key, const T& value);

	/************************************
	* 函数名:   	match
	* 功  能:	对比对应key值中的信息
	* 参  数: 	
	*			key
	*			data
	* 返回值:   	bool
	************************************/
	bool match(const string& key, const string& data);

	/************************************
	* 函数名:   	commit
	* 功  能:	提交数据，修改过后，需要进行此操作，才能保存
	* 参  数:
	* 返回值:   	bool
	************************************/
	bool commit();

private:

	/************************************
	* 函数名:   	save_file
	* 功  能:	保存配置信息到配置文件
	* 参  数: 	
	* 返回值:   	bool
	************************************/
	bool save_file();

	/************************************
	* 函数名:   	get_string
	* 功  能:	获取key对应的value string类型值
	* 参  数: 	
	*			key
	* 返回值:   	string
	************************************/
	string  get_string(const string& key);

	/************************************
	* 函数名:   	set_string
	* 功  能:	设置key value
	* 参  数: 	
	*			key
	*			value
	* 返回值:   	bool
	************************************/
	bool	set_string(const string& key, const string& value);

	/************************************
	* 函数名:   	init_valid_type_dic
	* 功  能:	初始化有效类型名称字典
	* 参  数: 	
	* 返回值:   	void
	************************************/
	void init_valid_type_dic();

	/************************************
	* 函数名:   	is_valid_type
	* 功  能:	检查是否是有效支持的类型
	* 参  数: 	
	*			type_name
	* 返回值:   	bool
	************************************/
	bool is_valid_type(const string& type_name);

private:

	typedef map<string, string> map_kv_t, map_valid_type_t;

	//配置字典
	map_kv_t m_map_kv;

	//配置路径
	string m_path;

	//锁
//	boost::mutex m_kv_mutex;

	//有效的类型名称字典
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
