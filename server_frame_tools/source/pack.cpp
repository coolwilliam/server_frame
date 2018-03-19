#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>

#include "pack.h"

/************************************
* 函数名：	is_little_endian
* 功  能：	检查大小端
* 返回值:	int 
			1 则是小端
			2 则是大端
************************************/
int is_little_endian()
{
	union w
	{
		int a;
		char b;
	} c;
	c.a = 1;
	return(c.b == 1);
}


uint64_t HTONLL(uint64_t var_8)
{
	uint64_t ret_64 = var_8;

#if __BYTE_ORDER == __LITTLE_ENDIAN
	union
	{
		uint64_t v_64;
		uint32_t v_32[2];
	} union_64, union_64_convert;

	union_64.v_64 = var_8;

	union_64_convert.v_32[0] = htonl(union_64.v_32[1]);
	union_64_convert.v_32[1] = htonl(union_64.v_32[0]);

	ret_64 = union_64_convert.v_64;
#endif

	return ret_64;
}


uint64_t NTOHLL(uint64_t var_8)
{
	uint64_t ret_64 = var_8;

#if __BYTE_ORDER == __LITTLE_ENDIAN
	union
	{
		uint64_t v_64;
		uint32_t     v_32[2];
	} union_64, union_64_convert;

	union_64.v_64 = var_8;

	union_64_convert.v_32[0] = htonl(union_64.v_32[1]);
	union_64_convert.v_32[1] = htonl(union_64.v_32[0]);

	ret_64 = union_64_convert.v_64;
#endif

	return ret_64;
}


int pack_byte_array(const char* IN var_array, const size_t IN length, char* IN_OUT buffer)
{
	if (NULL == var_array
		|| 0 == length
		|| NULL == buffer)
	{
		return eec_param_err;
	}

	memcpy(buffer, var_array, length);

	return length;
}

int pack1(const char* IN var_1,
	char* IN_OUT buffer)
{
	//判断当前传入参数的正确性
	if (NULL == var_1
		||NULL == buffer)
	{
		return eec_param_err;
	}

	memcpy(buffer, var_1, 1);

	return 1;
}

int pack2(const char* IN var_2,
	char* IN_OUT buffer)
{
	//判断当前传入参数的正确性
	if (NULL == var_2
		|| NULL == buffer)
	{
		return eec_param_err;
	}

	*(reinterpret_cast<uint16_t*>(buffer)) = htons(*(uint16_t*)(var_2));

/*
	char* tmp1 = NULL;
	char* tmp2 = NULL;

	if (!is_little_endian())
	{
		//如果是大端，则高地位转换
		tmp1 = (char*)(var_2 + 1);
		tmp2 = (char*)(var_2);
	}
	else
	{
		tmp1 = (char*)(var_2);
		tmp2 = (char*)(var_2 + 1);
	}

	int iRet = 0;
	if ((iRet = pack1(tmp1, buffer)) < 0)
	{
		return iRet;
	}

	if ((iRet = pack1(tmp2, buffer + 1)) < 0)
	{
		return iRet;
	}
*/

	return 2;
}

int pack4(const char* IN var_4, char* IN_OUT buffer)
{
	//判断当前传入参数的正确性
	if (NULL == var_4
		|| NULL == buffer)
	{
		return eec_param_err;
	}
	*(reinterpret_cast<uint32_t*>(buffer)) = htonl(*(uint32_t*)(var_4));

/*
	const char* tmp1 = var_4;
	const char* tmp2 = var_4 + 2;

	int iRet = 0;
	if ((iRet = pack2(tmp1, buffer)) < 0)
	{
		return iRet;
	}

	if ((iRet = pack2(tmp2, buffer + 2)) < 0)
	{
		return iRet;
	}
*/

	return 4;
}



int pack8(const char* IN var_8, char* IN_OUT buffer)
{
	//判断当前传入参数的正确性
	if (NULL == var_8
		|| NULL == buffer)
	{
		return eec_param_err;
	}

	*(reinterpret_cast<uint64_t*>(buffer)) = HTONLL(*(uint64_t*)(var_8));


/*
	const char* tmp1 = var_8;
	const char* tmp2 = var_8 + 4;

	int iRet = 0;
	if ((iRet = pack4(tmp1, buffer)) < 0)
	{
		return iRet;
	}

	if ((iRet = pack4(tmp2, buffer + 4)) < 0)
	{
		return iRet;
	}
*/

	return 8;
}



int unpack_byte_array(const char* IN buffer, const size_t IN length, char* IN_OUT var_array)
{
	if (NULL == var_array
		|| 0 == length
		|| NULL == buffer)
	{
		return eec_param_err;
	}

	memcpy(var_array, buffer, length);

	return length;
}

int unpack1(const char* IN buffer,
	char* IN_OUT var_1)
{
	//判断当前传入参数的正确性
	if (NULL == var_1
		|| NULL == buffer)
	{
		return eec_param_err;
	}

	memcpy(var_1, buffer, 1);

	return 1;
}

int unpack2(const char* IN buffer,
	char* IN_OUT var_2)
{
	//判断当前传入参数的正确性
	if (NULL == var_2
		|| NULL == buffer)
	{
		return eec_param_err;
	}

	*(reinterpret_cast<uint16_t*>(var_2)) = ntohs(*(uint16_t*)(buffer));

/*
	char* tmp1 = NULL;
	char* tmp2 = NULL;

	if (!is_little_endian())
	{
		//如果是大端，则高地位转换
		tmp1 = (char*)(buffer + 1);
		tmp2 = (char*)(buffer);
	}
	else
	{
		tmp1 = (char*)(buffer);
		tmp2 = (char*)(buffer + 1);
	}

	int iRet = 0;
	if ((iRet = unpack1(tmp1, var_2)) < 0)
	{
		return iRet;
	}

	if ((iRet = unpack1(tmp2, var_2 + 1)) < 0)
	{
		return iRet;
	}
*/

	return 2;
}

int unpack4(const char* IN buffer, char* IN_OUT var_4)
{
	//判断当前传入参数的正确性
	if (NULL == var_4
		|| NULL == buffer)
	{
		return eec_param_err;
	}

	*(reinterpret_cast<uint32_t*>(var_4)) = ntohl(*(uint32_t*)(buffer));

/*
	const char* tmp1 = buffer;
	const char* tmp2 = buffer + 2;

	int iRet = 0;
	if ((iRet = unpack2(tmp1, var_4)) < 0)
	{
		return iRet;
	}

	if ((iRet = unpack2(tmp2, var_4 + 2)) < 0)
	{
		return iRet;
	}
*/

	return 4;
}

int unpack8(const char* IN buffer, char* IN_OUT var_8)
{
	//判断当前传入参数的正确性
	if (NULL == var_8
		|| NULL == buffer)
	{
		return eec_param_err;
	}

	*(reinterpret_cast<uint64_t*>(var_8)) = NTOHLL(*(uint64_t*)(buffer));

/*
	const char* tmp1 = buffer;
	const char* tmp2 = buffer + 4;

	int iRet = 0;
	if ((iRet = unpack4(tmp1, var_8)) < 0)
	{
		return iRet;
	}

	if ((iRet = unpack4(tmp2, var_8 + 4)) < 0)
	{
		return iRet;
	}
*/

	return 8;
}
