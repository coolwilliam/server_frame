#include "simple_kv_config.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <cctype>
#include <algorithm>
#include <functional>
using namespace std;

#include "common_macro.h"

const char* g_valid_typename[] =
{
	typeid(int).name(),
	typeid(unsigned int).name(),
	typeid(string).name(),
	typeid(unsigned short).name(),
	typeid(bool).name()
};

#ifndef COMMENT_CHAR
#define COMMENT_CHAR	"#"
#endif

//内联函数
inline string& ltrim(string &str) {
	string::iterator p = find_if(str.begin(), str.end(), not1(ptr_fun<int, int>(isspace)));
	str.erase(str.begin(), p);
	return str;
}

inline string& rtrim(string &str) {
	string::reverse_iterator p = find_if(str.rbegin(), str.rend(), not1(ptr_fun<int, int>(isspace)));
	str.erase(p.base(), str.end());
	return str;
}

inline string& trim(string &str) {
	ltrim(rtrim(str));
	return str;
}


simple_kv_config::simple_kv_config()
{
	init_valid_type_dic();
}


simple_kv_config::~simple_kv_config()
{
}

bool simple_kv_config::commit()
{
	return save_file();
}


simple_kv_config::map_kv_t simple_kv_config::get_all() const
{
	return m_map_kv;
}

bool simple_kv_config::match(const string& key, const string& data)
{
	string str;
	get(key, str);
	if (true == str.empty())
		return false;
	if (0 != key.compare(data))
		return false;
	else
		return true;
}

bool split(const std::string& str, const char* pattern, std::vector<std::string>& key_value)
{
	//如果是空的
	if (str.empty())
	{
		return false;
	}
	std::string::size_type pos;
	std::string::size_type start_pos = 0;
	std::string::size_type end_pos = str.size() - 1;
	std::string key;
	std::string value;

	if ((pos = str.find(COMMENT_CHAR)) != std::string::npos)
	{
		if (0 == pos)
		{
			//行的第一个就是注视行
			return false;
		}

		end_pos = pos - 1;
	}

	//预处理，删除注释部分
	std::string new_line = str.substr(start_pos, end_pos - start_pos + 1);

	if ((pos = new_line.find(pattern)) == std::string::npos)
	{
		//没有key value的赋值符号
		return false;
	}

	//key
	key = new_line.substr(0, pos);

	//value
	value = new_line.substr(pos + 1, end_pos - pos);

	//清除key的空白部分
	trim(key);
	if (key.empty())
	{
		//key为空
		return false;
	}

	//清除value的空白部分
	trim(value);

	key_value.push_back(key);
	key_value.push_back(value);

	return true;
}


bool simple_kv_config::load_file(const string& str_path)
{
	static size_t line_size = 4096;
	char* buf = new char[line_size];
	memset(buf, 0x00, line_size);
	string tmp_str;
	std::vector<std::string> key_value;

	// boost::unique_lock<boost::mutex> lock(m_kv_mutex);

	fstream ifs_kv(str_path.c_str());

	if (!ifs_kv.is_open()){
		printf("[%s][%d] %s \n", __FUNCTION__, __LINE__, str_path.c_str());
		if (NULL != buf)
		{
			delete[] buf;
			buf = NULL;
		}
		return false;
	}

	while (!ifs_kv.eof())
	{
		key_value.clear();
		memset(buf, 0, sizeof(buf));
		ifs_kv.getline(buf, 4096);

		if (ifs_kv.eof()){
			break;
		}
		if (buf[0] == 0 || buf[0] == '\n'){
			continue;
		}

		tmp_str = buf;

		bool b_parse = split(tmp_str, "=", key_value);
		if (false == b_parse || key_value.size() != 2){
			//printf("[%s][%d] tmp_str:[%s],size: %zu\n", __FUNCTION__, __LINE__, tmp_str.c_str(), key_value.size());
			continue;
		}

		map_kv_t::iterator inter;

		inter = m_map_kv.find(key_value[0]);
		if (inter == m_map_kv.end())
		{
			m_map_kv.insert(map_kv_t::value_type(key_value[0], key_value[1]));
		}
		else
		{
			inter->second = key_value[1];
		}
	}

	ifs_kv.close();

	m_path = str_path;

	if (NULL != buf)
	{
		delete[] buf;
		buf = NULL;
	}

	// m_kv_mutex.unlock();
	return true;
}



bool simple_kv_config::save_file()
{
	map_kv_t::iterator inter;
	// boost::unique_lock<boost::mutex> lock(m_kv_mutex);

	ofstream ofs_kv;

	ofs_kv.open(m_path.c_str(), ios::in | ios::out);
	if (!ofs_kv.is_open()){
		return false;
	}

	for (inter = m_map_kv.begin(); inter != m_map_kv.end(); ++inter)
	{
		ofs_kv << inter->first << "=" << inter->second << "\n";
	}

	ofs_kv.close();
	// m_kv_mutex.unlock();
	return true;
}


string simple_kv_config::get_string(const string& key)
{
	map_kv_t::iterator node;
	node = m_map_kv.find(key);
	if (node == m_map_kv.end())
		return string("");
	return node->second;
}

bool simple_kv_config::set_string(const string& key, const string& value)
{
	std::pair< map_kv_t::iterator, bool > ret;
	map_kv_t::iterator inter;
	inter = m_map_kv.find(key);
	if (inter == m_map_kv.end())
	{
		ret = m_map_kv.insert(map_kv_t::value_type(key, value));
		if (ret.second)
			return true;
		else
			return false;
	}
	else
	{
		inter->second = value;
		return true;
	}
}

void simple_kv_config::init_valid_type_dic()
{
	int icount = _countof(g_valid_typename);
	for (int i = 0; i < icount; ++i)
	{
		m_map_valid_type.insert(make_pair(g_valid_typename[i], g_valid_typename[i]));
	}
}

bool simple_kv_config::is_valid_type(const string& type_name)
{
	map_valid_type_t::iterator it_find = m_map_valid_type.find(type_name);
	if (it_find != m_map_valid_type.end())
	{
		return true;
	}
	else
	{
		return false;
	}
}
