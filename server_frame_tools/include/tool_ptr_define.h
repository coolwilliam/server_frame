#ifndef tool_ptr_define_h__
#define tool_ptr_define_h__

#include <boost/shared_ptr.hpp>
//using namespace boost;

class data_buffer;
typedef boost::shared_ptr<data_buffer> data_buffer_ptr;

class simple_kv_config;
typedef boost::shared_ptr<simple_kv_config> simple_kv_config_ptr;

#endif  // tool_ptr_define_h__

