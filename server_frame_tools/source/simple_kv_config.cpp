#include "simple_kv_config.h"
#include <vector>
using namespace std;

#include "common_macro.h"
#include <iostream>
#include <fstream>

const char* g_valid_typename[] =
{
	typeid(int).name(),
	typeid(unsigned int).name(),
	typeid(string).name(),
	typeid(unsigned short).name()
};

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

std::vector<std::string> split(const std::string& str, const char* pattern)
{
	std::string::size_type pos;
	std::vector<std::string> result;
	std::string str_tmp = str;

	str_tmp += pattern;//在末尾临时加上分隔符

	std::string::size_type size = str_tmp.size();

	for (std::string::size_type i = 0; i < size; i++)
	{
		pos = str_tmp.find(pattern, i);
		if (pos < size)
		{

			std::string s = str_tmp.substr(i, pos - i);
			result.push_back(s);
			i = pos + strlen(pattern) - 1;
		}
	}
	return result;
}


bool simple_kv_config::load_file(const string& str_path)
{

	char buf[4096] = { 0 };
	string tmp_str;
	std::vector<std::string> arr;

//	boost::unique_lock<boost::mutex> lock(m_kv_mutex);


	fstream ifs_kv(str_path.c_str());

	if (!ifs_kv.is_open()){
		printf("[%s][%d] %s \n", __FUNCTION__, __LINE__, m_path.c_str());
		return false;
	}

	while (!ifs_kv.eof())
	{
		memset(buf, 0, sizeof(buf));
		ifs_kv.getline(buf, 4096);

		if (ifs_kv.eof()){
			break;
		}
		if (buf[0] == 0 || buf[0] == '\n'){
			break;
		}

		tmp_str = buf;

		arr = split(tmp_str, "=");
		if (arr.size() != 2){
			printf("[%s][%d] tmp_str:[%s],size: %zu\n", __FUNCTION__, __LINE__, tmp_str.c_str(), arr.size());
			continue;
		}

		map_kv_t::iterator inter;

		inter = m_map_kv.find(arr[0]);
		if (inter == m_map_kv.end())
		{
			m_map_kv.insert(map_kv_t::value_type(arr[0], arr[1]));
		}
		else
		{
			inter->second = arr[1];
		}
	}

	ifs_kv.close();

	m_path = str_path;

	//	m_kv_mutex.unlock();
	return true;
}



bool simple_kv_config::save_file()
{
	map_kv_t::iterator inter;
	//boost::unique_lock<boost::mutex> lock(m_kv_mutex);

	ofstream ofs_kv;

	ofs_kv.open(m_path.c_str(), ios::in | ios::out);
	if (!ofs_kv.is_open()){
		return false;
	}

	for (inter = m_map_kv.begin(); inter != m_map_kv.end(); inter++)
	{
		ofs_kv << inter->first << "=" << inter->second << "\n";
	}

	ofs_kv.close();
	//	m_kv_mutex.unlock();
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
