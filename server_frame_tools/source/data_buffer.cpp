#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "data_buffer.h"


data_buffer::data_buffer()
{
	m_bufferlength			= init_data_length;
	m_current_datalength	= 0;
	m_current_read_pos		= 0;

	m_buffer = reinterpret_cast<char*>(malloc(init_data_length*sizeof(char)));
	if (NULL == m_buffer)
	{
		assert(false && "malloc data failed!");
	}
	memset(m_buffer, 0x00, init_data_length);
}


data_buffer::~data_buffer()
{
	if (NULL != m_buffer)
	{
		free(m_buffer);
		m_buffer = NULL;
	}

	m_bufferlength			= 0;
	m_current_datalength	= 0;
	m_current_read_pos		= 0;
}

void data_buffer::reset_readpos()
{
	m_current_read_pos = 0;
}

void data_buffer::write_byte_array(const char* var_array, size_t length)
{
	size_t var_size = length;

	//判断即将打包的数据是否会超出当前缓冲大小，需要扩充
	if (var_size + m_current_datalength > m_bufferlength)
	{
		size_t append_size = m_current_datalength + var_size - m_bufferlength;
		append_size = append_size > append_data_length ? append_size : append_data_length;

		m_buffer = reinterpret_cast<char*>(realloc(m_buffer, (m_bufferlength + append_size)*sizeof(char)));

		m_bufferlength += append_size;
	}


	pack_byte_array(var_array, length, m_buffer+m_current_datalength);

	m_current_datalength += var_size;
}

void data_buffer::read_byte_array(char* var_array, size_t length)
{
	size_t var_size = length;

	//判断即将打包的数据是否会超出当前数据大小
	assert(var_size + m_current_read_pos <= m_current_datalength);

	unpack_byte_array(m_buffer + m_current_read_pos, length, var_array);

	m_current_read_pos += var_size;
}

char* data_buffer::get_data()
{
	return m_buffer;
}

size_t data_buffer::get_data_length()
{
	return m_current_datalength;
}

size_t data_buffer::get_buffer_length()
{
	return m_bufferlength;
}

size_t data_buffer::get_readpos() const
{
	return m_current_read_pos;
}

void data_buffer::set_readpos(size_t new_pos)
{
	if (new_pos <= m_current_datalength)
	{
		m_current_read_pos = new_pos;
	}
}

void data_buffer::clear_data(size_t start_pos /*= 0*/, size_t count /*= 0*/)
{
	// 如果两个参数为0 则删除全部数据
	if (start_pos == 0
		&& count == 0)
	{
		free(m_buffer);
		m_buffer = NULL;

		// 将当前读取位置重置
		reset_readpos();

		// 将当前的数据长度和缓冲长度置0
		m_current_datalength = 0;
		m_bufferlength = 0;

		return;
	}

	// 判断是否超出范围
	if (start_pos >= m_current_datalength
		|| start_pos + count > m_current_datalength)
	{
		assert(false && "can't erase buffer out of range!");
	}

	// 将后面的数据往前移动
	memmove(m_buffer + start_pos, m_buffer + start_pos + count, m_current_datalength - count - start_pos);
	// 将移动末尾位移范围置0
	memset(m_buffer + m_current_datalength - count, 0x00, count);

	// 更新数据长度
	m_current_datalength -= count;

	//如果清除的长度大于叠加长度，则重新分配内存，减少常驻内存
	if (count > append_data_length * 5)
	{
		m_buffer = reinterpret_cast<char*>(realloc(m_buffer, (m_bufferlength - count) * sizeof(char)));
		m_bufferlength -= count;
	}

	// 将当前读取位置重置
	reset_readpos();
}
