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

#endif  // server_frame_ptr_define_h__
