///////////////////////////////////////////////////////////
//  plugin_dll_info.cpp
//  Implementation of the Class plugin_dll_info
//  Created on:      17-Ê®ÔÂ-2016 10:28:38
//  Original author: wuht
///////////////////////////////////////////////////////////

#include "plugin_dll_info.h"


plugin_dll_info::plugin_dll_info():
m_dll_handler(NULL)
, m_enabled(true)
, m_plugin(NULL)
, m_plugin_factory(NULL)
{
	m_plugin_name.clear();
}

plugin_dll_info::~plugin_dll_info()
{

}

plugin_dll_info::dll_handler_t plugin_dll_info::get_dll_handler(){

	return m_dll_handler;
}


plugin_ptr plugin_dll_info::get_plugin(){

	return m_plugin;
}


plugin_factory_base_ptr plugin_dll_info::get_plugin_factory()
{

	return m_plugin_factory;
}


std::string plugin_dll_info::get_plugin_name(){

	return m_plugin_name;
}


bool plugin_dll_info::is_enabled()
{

	return m_enabled;
}


void plugin_dll_info::set_dll_handler(plugin_dll_info::dll_handler_t newVal){

	m_dll_handler = newVal;
}


void plugin_dll_info::set_enabled(bool newVal){

	m_enabled = newVal;
}


void plugin_dll_info::set_plugin(plugin_ptr newVal){

	m_plugin = newVal;
}


void plugin_dll_info::set_plugin_factory(plugin_factory_base_ptr newVal){

	m_plugin_factory = newVal;
}


void plugin_dll_info::set_plugin_name(const std::string& newVal){

	m_plugin_name = newVal;
}