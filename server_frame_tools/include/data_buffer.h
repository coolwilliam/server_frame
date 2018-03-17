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
		init_data_length = 1024,		//��ʼ����
		append_data_length = 1024,		//������
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
	* ��������	reset_readpos
	* ��  �ܣ�	���ö�λ��
	* ��  ����
	* ����ֵ:	void
	************************************/
	void reset_readpos();

	/************************************
	* ��������	write_byte_array
	* ��  �ܣ�	д���ַ�����
	* ��  ����
	*			var_array
	*			length
	* ����ֵ:	void
	************************************/
	void write_byte_array(const char* var_array, unsigned long length);

	/************************************
	* ��������	read_byte_array
	* ��  �ܣ�	��ȡ�ַ�����
	* ��  ����
	*			var_array
	*			length
	* ����ֵ:	void
	************************************/
	void read_byte_array(char* var_array, unsigned long length);

	/************************************
	* ��������	get_data
	* ��  �ܣ�	��ȡ����
	* ��  ����
	* ����ֵ:	char*
	************************************/
	char* get_data();

	/************************************
	* ��������	get_data_length
	* ��  �ܣ�	��ȡ���ݳ���
	* ��  ����
	* ����ֵ:	unsigned long
	************************************/
	unsigned long get_data_length();

	/************************************
	* ��������	get_buffer_length
	* ��  �ܣ�	��ȡ���峤��
	* ��  ����
	* ����ֵ:	unsigned long
	************************************/
	unsigned long get_buffer_length();

	/************************************
	* ������:   	get_readpos
	* ��  ��:	��ȡ��ǰ��ȡλ��
	* ��  ��:
	* ����ֵ:   	unsigned long
	************************************/
	unsigned long get_readpos() const;
	/************************************
	* ������:   	set_readpos
	* ��  ��:	���õ�ǰ��ȡλ��
	* ��  ��:
	*			new_pos	�����õ�λ��
	* ����ֵ:   	void
	************************************/
	void set_readpos(unsigned long new_pos);

	/************************************
	* ������:   	clear_data
	* ��  ��:	�������
	* ��  ��: 	
	*			start_pos	��ʼλ��
	*			count		����ʼλ�ÿ�ʼ�ĸ���
	* ����ֵ:   	void
	************************************/
	void clear_data(unsigned long start_pos = 0, unsigned long count = 0);

private:
	//�����ڴ�
	char* m_buffer;

	//��ǰ���ݾ��峤��
	unsigned long m_current_datalength;

	//�����ܳ���
	unsigned long m_bufferlength;

	//��ǰ��ȡλ��
	unsigned long m_current_read_pos;

private:
	DISABLE_COPY(data_buffer)
};

template<typename T>
inline data_buffer& data_buffer::operator << (const T& var_in)
{
	unsigned long var_size = sizeof(var_in);

	//�жϼ�������������Ƿ�ᳬ����ǰ�����С����Ҫ����
	if (var_size + m_current_datalength > m_bufferlength)
	{
		unsigned long append_size = m_current_datalength + var_size - m_bufferlength;
		append_size = append_size > append_data_length ? append_size : append_data_length;

		m_buffer = (char*)realloc(m_buffer, (m_bufferlength + append_size)*sizeof(char));

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
	unsigned long var_size = sizeof(var_out);
	assert(var_size < m_current_datalength - m_current_read_pos);

	switch (var_size)
	{
		case 1:
			unpack1(m_buffer + m_current_read_pos, (char*)&var_out);
			break;
		case 2:
			unpack2(m_buffer + m_current_read_pos, (char*)&var_out);
			break;
		case 4:
			unpack4(m_buffer + m_current_read_pos, (char*)&var_out);
			break;
		case 8:
			unpack8(m_buffer + m_current_read_pos, (char*)&var_out);
			break;
		default:
			{
				unpack_byte_array(m_buffer + m_current_read_pos, var_size, (char*)&var_out);
			}
	}

	m_current_read_pos += var_size;

	return *this;
}

//���string�����⴦��
template<>
inline data_buffer& data_buffer::operator << <string>(const string& var_in)
{

	unsigned long var_size = var_in.size();

	//�жϼ�������������Ƿ�ᳬ����ǰ�����С����Ҫ����
	if (var_size + m_current_datalength > m_bufferlength)
	{
		unsigned long append_size = m_current_datalength + var_size - m_bufferlength;
		append_size = append_size > append_data_length ? append_size : append_data_length;

		m_buffer = (char*)realloc(m_buffer, (m_bufferlength + append_size)*sizeof(char));

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
	//���ԭ��������
	var_out.clear();

	if (m_current_read_pos == m_current_datalength)
	{
		return *this;
	}

	unsigned long var_size = strlen(m_buffer + m_current_read_pos);

	if (var_size != 0)
	{
		var_out.resize(var_size + 1);
		memcpy((void*)(var_out.c_str()), m_buffer + m_current_read_pos, var_size);
		var_out[var_size] = '\0';
	}

	// + 1 �������ַ���������'\0'
	m_current_read_pos += var_size + 1;

	return *this;
}

#endif
