#ifndef DATA_BUFFER_H
#define DATA_BUFFER_H

#include "pack.h"
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <string>
using namespace std;

#include "tool_exports.h"
#include "common_macro.h"

class SERVER_FRAME_TOOL_API data_buffer
{
	enum
	{
		init_data_length = 1024,		//初始长度
		append_data_length = 1024,		//增长量
	};
public:
	data_buffer();
	~data_buffer();

public:
	template<typename T>

	inline data_buffer& operator >> (T& var_out);

	template<typename T>

	inline data_buffer& operator << (const T& var_in);

	/************************************
	* 函数名：	reset_readpos
	* 功  能：	重置读位置
	* 参  数：
	* 返回值:	void
	************************************/
	void reset_readpos();

	/************************************
	* 函数名：	write_byte_array
	* 功  能：	写入字符数组
	* 参  数：
	*			var_array
	*			length
	* 返回值:	void
	************************************/
	void write_byte_array(const char* var_array, size_t length);

	/************************************
	* 函数名：	read_byte_array
	* 功  能：	读取字符数组
	* 参  数：
	*			var_array
	*			length
	* 返回值:	void
	************************************/
	void read_byte_array(char* var_array, size_t length);

	/************************************
	* 函数名：	get_data
	* 功  能：	获取数据
	* 参  数：
	* 返回值:	char*
	************************************/
	char* get_data();

	/************************************
	* 函数名：	get_data_length
	* 功  能：	获取数据长度
	* 参  数：
	* 返回值:	unsigned long
	************************************/
	size_t get_data_length();

	/************************************
	* 函数名：	get_buffer_length
	* 功  能：	获取缓冲长度
	* 参  数：
	* 返回值:	unsigned long
	************************************/
	size_t get_buffer_length();

	/************************************
	* 函数名:   	get_readpos
	* 功  能:	获取当前读取位置
	* 参  数:
	* 返回值:   	unsigned long
	************************************/
	size_t get_readpos() const;
	/************************************
	* 函数名:   	set_readpos
	* 功  能:	设置当前读取位置
	* 参  数:
	*			new_pos	新设置的位置
	* 返回值:   	void
	************************************/
	void set_readpos(size_t new_pos);

	/************************************
	* 函数名:   	clear_data
	* 功  能:	清除数据
	* 参  数: 	
	*			start_pos	起始位置
	*			count		从起始位置开始的个数
	* 返回值:   	void
	************************************/
	void clear_data(size_t start_pos = 0, size_t count = 0);

private:
	//缓冲内存
	char* m_buffer;

	//当前数据具体长度
	size_t m_current_datalength;

	//缓冲总长度
	size_t m_bufferlength;

	//当前读取位置
	size_t m_current_read_pos;

private:
	DISABLE_COPY(data_buffer)
};

template<typename T>
inline data_buffer& data_buffer::operator << (const T& var_in)
{
	size_t var_size = sizeof(var_in);

	//判断即将打包的数据是否会超出当前缓冲大小，需要扩充
	if (var_size + m_current_datalength > m_bufferlength)
	{
		size_t append_size = m_current_datalength + var_size - m_bufferlength;
		append_size = append_size > append_data_length ? append_size : append_data_length;

		m_buffer = reinterpret_cast<char*>(realloc(m_buffer, (m_bufferlength + append_size)*sizeof(char)));

		m_bufferlength += append_size;
	}

	switch (var_size)
	{
		case 1:
			pack1((const char*)&var_in, m_buffer + m_current_datalength);
			break;
		case 2:
			pack2((const char*)&var_in, m_buffer + m_current_datalength);
			break;
		case 4:
			pack4((const char*)&var_in, m_buffer + m_current_datalength);
			break;
		case 8:
			pack8((const char*)&var_in, m_buffer + m_current_datalength);
			break;
		default:
			{
				pack_byte_array((const char*)&var_in, var_size, m_buffer + m_current_datalength);
			}
	}

	m_current_datalength += var_size;

	return *this;
}



template<typename T>
inline data_buffer& data_buffer::operator >> (T& var_out)
{
	size_t var_size = sizeof(var_out);
	assert(var_size <= m_current_datalength - m_current_read_pos);

	switch (var_size)
	{
		case 1:
			unpack1(m_buffer + m_current_read_pos, reinterpret_cast<char*>(&var_out));
			break;
		case 2:
			unpack2(m_buffer + m_current_read_pos, reinterpret_cast<char*>(&var_out));
			break;
		case 4:
			unpack4(m_buffer + m_current_read_pos, reinterpret_cast<char*>(&var_out));
			break;
		case 8:
			unpack8(m_buffer + m_current_read_pos, reinterpret_cast<char*>(&var_out));
			break;
		default:
			{
				unpack_byte_array(m_buffer + m_current_read_pos, var_size, reinterpret_cast<char*>(&var_out));
			}
	}

	m_current_read_pos += var_size;

	return *this;
}

// 针对string的特殊处理
template<>
inline data_buffer& data_buffer::operator << <string>(const string& var_in)
{
	size_t var_size = var_in.size();

	//判断即将打包的数据是否会超出当前缓冲大小，需要扩充
	if (var_size + m_current_datalength > m_bufferlength)
	{
		size_t append_size = m_current_datalength + var_size - m_bufferlength;
		append_size = append_size > append_data_length ? append_size : append_data_length;

		m_buffer = reinterpret_cast<char*>(realloc(m_buffer, (m_bufferlength + append_size)*sizeof(char)));

		m_bufferlength += append_size;
	}

	write_byte_array(var_in.data(), var_size);
	
	if (var_in.at(var_size - 1) != '\0')
	{
		this->operator<<('\0');
	}

	return *this;
}

template<>
inline data_buffer& data_buffer::operator >> <string>(string& var_out)
{
	//清除原本的数据
	var_out.clear();

	if (m_current_read_pos == m_current_datalength)
	{
		return *this;
	}

	size_t var_size = strnlen(m_buffer + m_current_read_pos, m_current_datalength - m_current_read_pos);

	if (var_size != 0)
	{
		var_out.resize(var_size + 1);
		memcpy((void*)(var_out.c_str()), m_buffer + m_current_read_pos, var_size);
		var_out[var_size] = '\0';
	}

	// + 1 是跳过字符串结束符'\0'
	m_current_read_pos += var_size + 1;

	return *this;
}

#endif
