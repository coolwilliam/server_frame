#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "data_buffer.h"


data_buffer::data_buffer()
{
	m_buffer				= (char*)malloc(init_data_length*sizeof(char));
	memset(m_buffer, 0x00, init_data_length);

	m_bufferlength			= init_data_length;
	m_current_datalength	= 0;
	m_current_read_pos		= 0;
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

void data_buffer::write_byte_array(const char* var_array, unsigned long length)
{

	unsigned long var_size = length;

	//�жϼ�������������Ƿ�ᳬ����ǰ�����С����Ҫ����
	if (var_size + m_current_datalength > m_bufferlength)
	{
		unsigned long append_size = m_current_datalength + var_size - m_bufferlength;
		append_size = append_size > append_data_length ? append_size : append_data_length;

		m_buffer = (char*)realloc(m_buffer, (m_bufferlength + append_size)*sizeof(char));

		m_bufferlength += append_size;
	}


	pack_byte_array(var_array, length, m_buffer+m_current_datalength);

	m_current_datalength += var_size;

}

void data_buffer::read_byte_array(char* var_array, unsigned long length)
{
	unsigned long var_size = length;

	//�жϼ�������������Ƿ�ᳬ����ǰ���ݴ�С
	assert(var_size + m_current_read_pos < m_current_datalength);

	unpack_byte_array(m_buffer + m_current_read_pos, length, var_array);

	m_current_read_pos += var_size;
}

char* data_buffer::get_data()
{
	return m_buffer;
}

unsigned long data_buffer::get_data_length()
{
	return m_current_datalength;
}

unsigned long data_buffer::get_buffer_length()
{
	return m_bufferlength;
}

unsigned long data_buffer::get_readpos() const
{
	return m_current_read_pos;
}

void data_buffer::set_readpos(unsigned long new_pos)
{
	if (new_pos <= m_current_datalength)
	{
		m_current_read_pos = new_pos;
	}
}

void data_buffer::clear_data(unsigned long start_pos /*= 0*/, unsigned long count /*= 0*/)
{
	//�����������Ϊ0 ��ɾ��ȫ������
	if (start_pos == 0
		&& count == 0)
	{
		free(m_buffer);
		m_buffer = NULL;

		//����ǰ��ȡλ������
		reset_readpos();

		//����ǰ�����ݳ��Ⱥͻ��峤����0
		m_current_datalength = 0;
		m_bufferlength = 0;

		return;
	}

	//�ж��Ƿ񳬳���Χ
	if (start_pos >= m_current_datalength
		|| start_pos + count > m_current_datalength)
	{
		assert(false && "can't erase buffer out of range!");
	}

	//�������������ǰ�ƶ�
	memmove(m_buffer + start_pos, m_buffer + start_pos + count, m_current_datalength - count - start_pos);
	//���ƶ�ĩβλ�Ʒ�Χ��0
	memset(m_buffer + m_current_datalength - count, 0x00, count);

	//�������ݳ���
	m_current_datalength -= count;

	//����ǰ��ȡλ������
	reset_readpos();
}
