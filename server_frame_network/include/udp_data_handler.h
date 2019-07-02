#ifndef udp_data_handler_h__
#define udp_data_handler_h__
#include "network_ptr_define.h"

class udp_data_handler
{
public:
	virtual ~udp_data_handler(){}

	virtual bool on_received_data(udp_session_ptr session, const std::string& str_data) = 0;

	virtual int on_session_error(udp_session_ptr session, int err_code, const std::string& err_msg){ return 0; }
};
#endif // udp_data_handler_h__
