#include "common_function.h"
#include <stdio.h>
#include <string.h>
#include <dlfcn.h>
#include <malloc.h>
#include <sched.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/sysinfo.h>

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

static void copy_argv_envir(int argc, char* argv[]){
	extern char **environ;
	int i, size = 0;
	char* last = argv[argc - 1] + strlen(argv[argc - 1]) + 1;
	memset(argv[0], 0, last - argv[0] + 1);
	for (i = 0; environ[i]; i++){
		//fprintf(stderr,"%s\n",environ[i]);
		size += strlen(environ[i]) + 1;
	}
	char* p = (char*)malloc(size);
	char* q = p;
	for (i = 0; environ[i]; i++){
		strcpy(q, environ[i]);
		environ[i] = q;
		q += strlen(environ[i]) + 1;
	}
}

/*rename process*/
static void rename_proc_title(int argc, char* argv[], const char* title){
	copy_argv_envir(argc, argv);
	strcpy(argv[0], title);
}


void common_function::set_proc_title(int argc, char* argv[], const std::string& title)
{
	rename_proc_title(argc, argv, title.c_str());
}

int common_function::set_proc_on_cpu(const vect_cpu_id_t& bind_cpu_id_array)
{
	if (bind_cpu_id_array.empty())
	{
		return -1;
	}

	cpu_set_t mask;
	CPU_ZERO(&mask);
	for (vect_cpu_id_t::const_iterator it = bind_cpu_id_array.begin(); it != bind_cpu_id_array.end(); ++it)
	{
		CPU_SET(*it, &mask);
	}

	return sched_setaffinity(0, sizeof(cpu_set_t), &mask);
}

int common_function::get_configured_cpu_count()
{
	return sysconf(_SC_NPROCESSORS_CONF);
}

int common_function::get_enabled_cpu_count()
{
	return sysconf(_SC_NPROCESSORS_ONLN);
}
