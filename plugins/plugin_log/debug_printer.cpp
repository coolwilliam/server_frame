#include "debug_printer.h"
#include <string>
#include <stdio.h>
#include <stdarg.h>

#define PRINT_DEBUG  1

void __print_msg(const char* fmt, va_list args)
{
#if (PRINT_DEBUG == 1)
	vprintf(fmt, args);
	printf("\n");
#endif
}

void debug_printer::print_msg(const char* fmt, ...)
{
	va_list argp;
	va_start(argp, fmt);

	std::string str_fmt;
	str_fmt = std::string("\033[40;32m") + std::string(fmt) + std::string("\033[0m");
	__print_msg(str_fmt.c_str(), argp);

	va_end(argp);
}

void debug_printer::print_err(const char* fmt, ...)
{
	std::string str_fmt;
	str_fmt = std::string("\033[40;31mError: ") + std::string(fmt) + std::string("\033[0m");

	va_list argp;
	va_start(argp, fmt);

	__print_msg(str_fmt.c_str(), argp);

	va_end(argp);

}
