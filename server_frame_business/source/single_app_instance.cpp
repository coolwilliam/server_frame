#include "single_app_instance.h"
#include <fcntl.h>
#include <stdlib.h>
#include <sstream>
#include <iostream>
#include <string.h>
#include <unistd.h>


single_app_instance::single_app_instance(const std::string& lock_file)
	:m_str_lock_file(lock_file)
{
}


single_app_instance::~single_app_instance()
{
}

bool single_app_instance::set_lock(int i_fd)
{

	//测试是否已经上锁
	struct flock lock;
	bzero(&lock, sizeof(lock));

	if (fcntl(i_fd, F_GETLK, &lock) < 0)
	{
		std::cerr << "Fail to fcntl F_GETLK to file: " << m_str_lock_file << " !" << std::endl;
		return false;
	}

	lock.l_type = F_WRLCK;
	lock.l_whence = SEEK_SET;

	if (fcntl(i_fd, F_SETLK, &lock) < 0)
	{
		std::cerr << "Fail to fcntl F_SETLK to file: " << m_str_lock_file << " !" << std::endl;
		return false;
	}

	// Clear file content.
	ftruncate(i_fd, 0);
	lseek(i_fd, 0, SEEK_SET);

	pid_t pid = getpid();

	std::ostringstream oss;
	oss << pid;
	write(i_fd, oss.str().c_str(), oss.str().size());

	return true;
}

bool single_app_instance::is_running()
{
	int i_fd = -1;

	i_fd = open(m_str_lock_file.c_str(), O_WRONLY | O_CREAT, 0666);
	if (i_fd < 0)
	{
		std::cerr<<"Fail to open mutex file: " << m_str_lock_file << " !" <<std::endl;
		return true;
	}

	if (false == set_lock(i_fd))
	{
		return true;
	}

	return false;
}
