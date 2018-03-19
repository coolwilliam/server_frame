#include "common_function.h"
#include <stdio.h>
#include <string.h>
#include <dlfcn.h>

#ifndef FILE_PATH_MAX
#define FILE_PATH_MAX 256
#endif

void common_function::get_module_fullpath(char* fullpath, void* local_addr /*= NULL*/)
{
	if (NULL == fullpath)
	{
		return;
	}

	Dl_info info;
	int rc;
	rc = dladdr(NULL == local_addr?(const void*)get_module_fullpath:local_addr, &info);
	if (!rc)
	{
		return;
	}
	else
	{
		snprintf(fullpath, FILE_PATH_MAX, info.dli_fname);
	}

	return;
}

std::string common_function::get_module_dir(void* local_addr /*= NULL*/)
{
	char fullpath[FILE_PATH_MAX] = { 0 };

	common_function::get_module_fullpath(fullpath, local_addr);

	if (strcmp(fullpath, "") == 0)
	{
		return std::string("");
	}
	//∑¥œÚ≤È’“ '/'
	std::string str_path = std::string(fullpath);
	std::string::size_type npos = str_path.rfind('/');
	if (npos != str_path.npos)
	{
		str_path = str_path.substr(0, npos + 1);
		return str_path;
	}
	else
	{
		return std::string("");
	}
}
