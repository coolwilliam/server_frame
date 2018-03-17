///////////////////////////////////////////////////////////
//  common_session.cpp
//  Implementation of the Class common_session
//  Created on:      09-八月-2016 11:35:01
//  Original author: wuht
///////////////////////////////////////////////////////////

#include "common_session.h"
#include <boost/bind.hpp>
#include <boost/asio/placeholders.hpp>
#include <boost/make_shared.hpp>
#include "data_buffer.h"
#include "data_handler.h"


common_session::common_session(io_service & ios)
	:m_socket(ios),
	m_receive_cache_size(receive_cache_size_default)
{

}


common_session::~common_session(){
	//close();
}

void common_session::close(){

	if (m_socket.is_open())
	{
		m_socket.close();

		data_handler_ptr p_data_handler = get_data_handler();
		if (NULL != p_data_handler)
		{
			//通知监听者 此会话已关闭
			p_data_handler->on_session_close(shared_from_this(), get_remote_ip());
		}

	}
}


void common_session::data_write(){

	//判断链表是否为空
	if (true == m_list_send.empty())
	{
		return;
	}

	data_buffer_ptr data_ptr = m_list_send.front();

	m_socket.async_write_some(
		boost::asio::buffer((void*)(data_ptr->get_data() + data_ptr->get_readpos()), data_ptr->get_data_length() - data_ptr->get_readpos()),
		boost::bind(&common_session::handle_write,
		shared_from_this(),
		boost::asio::placeholders::error,
		boost::asio::placeholders::bytes_transferred)
		);


}


string common_session::get_remote_ip()
{

	return m_str_remote_ip;
}


tcp::socket& common_session::get_socket()
{
	return m_socket;
}


void common_session::handle_read(system::error_code error, size_t size, boost::shared_ptr<string> p_cache){


	if (boost::system::errc::success == error)
	{
		//如果接受大小为 0，则有错误，跳过并继续接收
		if (0 == size)
		{
			start_receive();

			return;
		}

		//将收到的数据传递给监听者
		data_handler_ptr p_handler = get_data_handler();
		if (NULL != p_handler)
		{
			string str_data;
			str_data.resize(size);
			memcpy((void*)(str_data.data()), p_cache->data(), size);

			if (false == p_handler->on_received_data(shared_from_this(), str_data))
			{
				close();

				return;
			}
		}

		start_receive();
	}
	else
	{
		//中途发生错误
		
		printf("%s:%s--->session:%p, list_count:%zu, error:%s\n", __FILE__, __FUNCTION__, this, m_list_send.size(), error.message().c_str());

		close();

		return;
	}

	return;
}


void common_session::handle_write(const system::error_code& error, size_t size){

	if (boost::system::errc::success == error)
	{
		data_buffer_ptr data_ptr = m_list_send.front();

		size_t left_size = data_ptr->get_data_length() - data_ptr->get_readpos();

		if (left_size == size
			&& 0 != size)
		{
			//如果发送的大小等于数据缓存大小，则从队列中删除
			m_list_send.pop_front();
		}
		else if (left_size > size)
		{
			//如果发送的大小小于数据缓存大小，则偏移下一次发送的位置
			data_ptr->set_readpos(data_ptr->get_readpos() + size);
		}
		
		printf("%s:%s--->sesson:%p, list_count:%zu,send size:%zu, buff:%s\n", __FILE__, __FUNCTION__, this, m_list_send.size(), size, (char*)(data_ptr->get_data()));

		data_write();
	} 
	else
	{
		m_list_send.pop_front();
		
		printf("%s:%s--->session:%p, error:%s\n", __FILE__, __FUNCTION__, this, error.message().c_str());

		close();
	}

	return;
}


int common_session::send_msg(const string & str_data){

	data_buffer_ptr buff = data_buffer_ptr(new data_buffer);
	buff->write_byte_array(str_data.data(), str_data.size());

	bool b_write = m_list_send.empty();
	
	//将数据推到待发送链表中
	m_list_send.push_back(buff);
	
	if (b_write)
	{
		data_write();
	}
	
	return 0;
}


void common_session::set_data_handler(data_handler_ptr new_val){

	assert(NULL == get_data_handler() 
		&& "can't set data_hanler more than once!");
	m_p_dataHandler = new_val;
}


void common_session::set_remote_ip(const string& new_val){

	m_str_remote_ip = new_val;
}


void common_session::start(){

	set_remote_ip(get_socket().remote_endpoint().address().to_string());
	start_receive();
}


bool common_session::start_receive(){


	boost::shared_ptr<string> p_data_recv(new string);

	p_data_recv->resize(get_recv_cache_size());

	m_socket.async_read_some(
		boost::asio::buffer((char*)(p_data_recv->data()), p_data_recv->size()),
		boost::bind(&common_session::handle_read, 
		this, 
		boost::asio::placeholders::error, 
		boost::asio::placeholders::bytes_transferred, 
		p_data_recv)
		);

	return false;
}

void common_session::set_recv_cache_size(size_t val)
{
	assert(0 != val && "can't set cache size to 0!");

	m_receive_cache_size = val;
}

data_handler_ptr common_session::get_data_handler() const
{
	return m_p_dataHandler;
}
