#include "plugin_base.h"

class plugin_log:public plugin_base
{
public:
	plugin_log(){}
	~plugin_log(){}
	bool init();
protected:
	DECLARE_MSG_MAP()
private:
	void on_log_msg(int ivalue, void* pvalue);
};
