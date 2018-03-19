///////////////////////////////////////////////////////////
//  plugin_factory_base.h
//  Implementation of the Class plugin_factory_base
//  Created on:      17-ʮ��-2016 10:28:38
//  Original author: wuht
///////////////////////////////////////////////////////////

#if !defined(EA_13731866_32C6_41eb_B045_970F0E5BA47D__INCLUDED_)
#define EA_13731866_32C6_41eb_B045_970F0E5BA47D__INCLUDED_

#include <stddef.h>
#include "plugin_module_ptr_define.h"
#include "business_exports.h"
#include "common_macro.h"

class plugin_manager;

/**
 * �����������
 */
class SERVER_FRAME_BUSINESS_API plugin_factory_base
{
	friend class plugin_manager;
public:
	/**
	 * �������
	 */
	virtual plugin_ptr create_plugin(plugin_manager_base_ptr plugin_mgr) = 0;

protected:
	plugin_factory_base(){}
	virtual ~plugin_factory_base(){}

private:
	DISABLE_COPY(plugin_factory_base)
};

//������������
typedef plugin_factory_base_ptr (*create_plugin_factory_func_t)(plugin_manager_base_ptr);

//����������
#define CREATE_PLUGIN_FACTORY_FUNC_NAME	"create_plugin_factory"

#endif // !defined(EA_13731866_32C6_41eb_B045_970F0E5BA47D__INCLUDED_)
