#ifndef business_ptr_define_h__
#define business_ptr_define_h__

#include <boost/shared_ptr.hpp>
//using namespace boost;

class task_obj;
typedef boost::shared_ptr<task_obj> task_obj_ptr;

class thread_obj;
typedef boost::shared_ptr<thread_obj> thread_obj_ptr;

class session_connection;
typedef boost::shared_ptr<session_connection> session_connection_ptr;

class session_connection_manager;
typedef boost::shared_ptr<session_connection_manager> session_connection_manager_ptr;

class thread_pool;
typedef thread_pool* thread_pool_ptr;

#endif  // business_ptr_define_h__
