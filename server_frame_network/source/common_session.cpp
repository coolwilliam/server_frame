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
	:m_socket(ios)
	, m_receive_cache_size(receive_cache_size_default)
	, m_comm_mode(ECM_ASYNC)
	, m_started(false)
	, m_strand(ios)
{
}

common_session::~common_session(){
	// close();
}

void common_session::close(){
	if (m_socket.is_open())
	{
		data_handler_ptr p_data_handler = get_data_handler();

		system::error_code ec;
		m_socket.shutdown(boost::asio::socket_base::shutdown_both, ec);
		m_socket.close(ec);
		if (ec)
		{
			//在关闭时出错，接收错误消息
			if (NULL != p_data_handler)
			{
				p_data_handler->on_session_error(shared_from_this(), ec.value(), ec.message());
			}
		}

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
		boost::asio::buffer(reinterpret_cast<void*>(data_ptr->get_data() + data_ptr->get_readpos()), data_ptr->get_data_length() - data_ptr->get_readpos()),
		m_strand.wrap(boost::bind(&common_session::handle_write,
		shared_from_this(),
		boost::asio::placeholders::error,
		boost::asio::placeholders::bytes_transferred)));
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

//		printf("%s:%s--->session:%p, list_count:%zu, error:%s\n", __FILE__, __FUNCTION__, this, m_list_send.size(), error.message().c_str());

		data_handler_ptr p_handler = get_data_handler();
		if (NULL != p_handler)
		{
			p_handler->on_session_error(shared_from_this(), error.value(), error.message());
		}

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

//		printf("%s:%s--->sesson:%p, list_count:%zu,send size:%zu, buff:%s\n", __FILE__, __FUNCTION__, this, m_list_send.size(), size, data_ptr->get_data());

		data_write();
	}
	else
	{
		m_list_send.pop_front();

//		printf("%s:%s--->session:%p, error:%s\n", __FILE__, __FUNCTION__, this, error.message().c_str());

		data_handler_ptr p_handler = get_data_handler();
		if (NULL != p_handler)
		{
			p_handler->on_session_error(shared_from_this(), error.value(), error.message());
		}

		close();
	}

	return;
}


int common_session::send_msg(const string & str_data){
	assert(get_comm_mode() == ECM_ASYNC && "Can't call this function in sync mode");

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
	set_started();
	
	boost::system::error_code err;
	set_remote_ip(get_socket().remote_endpoint(err).address().to_string());

	
	if (get_comm_mode() == ECM_ASYNC)
	{
		//异步模式
		start_receive();
	}
	else
	{
		//同步模式
	}
}


bool common_session::start_receive(){
	boost::shared_ptr<string> p_data_recv(new string);

	p_data_recv->resize(get_recv_cache_size());

	m_socket.async_read_some(
		boost::asio::buffer((char*)(p_data_recv->data()), p_data_recv->size()),
		m_strand.wrap(boost::bind(&common_session::handle_read,
		shared_from_this(),
		boost::asio::placeholders::error,
		boost::asio::placeholders::bytes_transferred,
		p_data_recv)));

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

enum_communicate_mode common_session::get_comm_mode() const
{
	return m_comm_mode;
}

void common_session::set_comm_mode(enum_communicate_mode newVal){
	assert(!is_started() && "Can't change mode since session has been started!");
	m_comm_mode = newVal;
}

bool common_session::is_started() const
{
	return m_started;
}

void common_session::set_started(bool newVal){
	m_started = newVal;
}

size_t common_session::receive(string& str_data_recv, int& err_code, string& err_msg){
	assert(get_comm_mode() == ECM_SYNC && "Can't call this function in async mode");

	str_data_recv.clear();
	string received_buff;
	system::error_code error;

	received_buff.resize(get_recv_cache_size());

	size_t recv_size = m_socket.read_some(boost::asio::buffer((char*)(received_buff.data()), received_buff.size()), error);
	if (boost::system::errc::success != error)
	{
		err_code = error.value();
		err_msg = error.message();
		return 0;
	}

	if (recv_size > 0)
	{
		str_data_recv.assign(received_buff.data(), recv_size);
	}

	return recv_size;
}

size_t common_session::send(const string& str_data_send, int& err_code, string& err_msg)
{
	assert(get_comm_mode() == ECM_SYNC && "Can't call this function in async mode");
	
	system::error_code error;
	size_t actual_send_size = str_data_send.size();

	data_buffer buff;
	buff.write_byte_array(str_data_send.data(), str_data_send.size());

	size_t left_size = buff.get_data_length() - buff.get_readpos();

	while (left_size)
	{
		size_t send_size = m_socket.write_some(boost::asio::buffer(reinterpret_cast<void*>(buff.get_data() + buff.get_readpos()), buff.get_data_length() - buff.get_readpos()), error);
		if (boost::system::errc::success != error)
		{
			err_code = error.value();
			err_msg = error.message();
			actual_send_size = 0;

			break;
		}

		left_size = buff.get_data_length() - buff.get_readpos();

		if (left_size == send_size
			&& 0 != send_size)
		{
			//如果发送的大小等于数据缓存大小，发送完毕
			break;
		}
		else if (left_size > send_size)
		{
			//如果发送的大小小于数据缓存大小，则偏移下一次发送的位置
			buff.set_readpos(buff.get_readpos() + send_size);
		}
	}

	return actual_send_size;
}

