#ifndef server_frame_ptr_define_h__
#define server_frame_ptr_define_h__

#include <boost/shared_ptr.hpp>
using namespace boost;

class data_handler;
typedef shared_ptr<data_handler> data_handler_ptr;

class common_session;
typedef shared_ptr<common_session> common_session_ptr;

class common_server;
typedef shared_ptr<common_server> common_server_ptr;

#endif // server_frame_ptr_define_h__
