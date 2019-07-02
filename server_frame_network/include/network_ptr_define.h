#ifndef server_frame_ptr_define_h__
#define server_frame_ptr_define_h__

#include <boost/shared_ptr.hpp>
//using namespace boost;

class data_handler;
typedef boost::shared_ptr<data_handler> data_handler_ptr;

class common_session;
typedef boost::shared_ptr<common_session> common_session_ptr;

class common_server;
typedef boost::shared_ptr<common_server> common_server_ptr;

class common_client;
typedef boost::shared_ptr<common_client> common_client_ptr;

class io_service_pool;
typedef boost::shared_ptr<io_service_pool> io_service_pool_ptr;

class local_stream_session;
typedef boost::shared_ptr<local_stream_session> local_stream_session_ptr;

class local_stream_server;
typedef boost::shared_ptr<local_stream_server> local_stream_server_ptr;

class local_stream_client;
typedef boost::shared_ptr<local_stream_client> local_stream_client_ptr;

class local_stream_data_handler;
typedef boost::shared_ptr<local_stream_data_handler> local_stream_data_handler_ptr;

class udp_data_handler;
typedef boost::shared_ptr<udp_data_handler> udp_data_handler_ptr;

class udp_session;
typedef boost::shared_ptr<udp_session> udp_session_ptr;

class udp_server;
typedef boost::shared_ptr<udp_server> udp_server_ptr;

class udp_client;
typedef boost::shared_ptr<udp_client> udp_client_ptr;

typedef std::string udp_buffer;
typedef boost::shared_ptr<udp_buffer> udp_buffer_ptr;

#endif  // server_frame_ptr_define_h__
