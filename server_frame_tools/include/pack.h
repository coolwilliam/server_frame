/********************************************************************
	created:	2016/05/11
	created:	11:5:2016   17:04
	file base:	pack
	file ext:	h
	author:		wuhaitao
	
*********************************************************************/

#ifndef pack_h__
#define pack_h__

#ifndef IN
#define IN
#endif

#ifndef OUT
#define OUT
#endif

#ifndef IN_OUT
#define IN_OUT
#endif

enum em_err_code
{
	eec_param_err = -1,		//参数错误
};

/*================================= pack ===================================*/

/************************************
* 函数名：	pack_byte_array
* 功  能：	打包数组
* 参  数：
*			var_array
*			length
*			buffer
* 返回值:	int
************************************/
int pack_byte_array(const char* IN var_array, 
	const unsigned long IN length, 
	char* IN_OUT buffer);
/************************************
* 函数名：	pack1
* 功  能：	打包1字节类型
* 参  数：
*			var_1
*			buffer
* 返回值:	int
************************************/
int pack1(const char* IN var_1, 
	char* IN_OUT buffer);
/************************************
* 函数名：	pack2
* 功  能：	打包2字节类型
* 参  数：
*			var_2
*			buffer
* 返回值:	int
************************************/
int pack2(const char* IN var_2, 
	char* IN_OUT buffer);
/************************************
* 函数名：	pack4
* 功  能：	打包4字节类型
* 参  数：
*			var_4
*			buffer
* 返回值:	int
************************************/
int pack4(const char* IN var_4, 
	char* IN_OUT buffer);
/************************************
* 函数名：	pack8
* 功  能：	打包8字节类型
* 参  数：
*			var_8
*			buffer
* 返回值:	int
************************************/
int pack8(const char* IN var_8, 
	char* IN_OUT buffer);

/*================================= unpack ===================================*/

/************************************
* 函数名：	unpack_byte_array
* 功  能：	解包数组
* 参  数：
*			buffer
*			length
*			var_array
* 返回值:	int
************************************/
int unpack_byte_array(const char* IN buffer, 
	const unsigned long IN length, 
	char* IN_OUT var_array);


/************************************
* 函数名：	unpack1
* 功  能：	解包1字节类型
* 参  数：
*			buffer
*			var_1
* 返回值:	int
************************************/
int unpack1(const char* IN buffer, char* IN_OUT var_1);
/************************************
* 函数名：	unpack2
* 功  能：	解包2字节类型
* 参  数：
*			buffer
*			var_2
* 返回值:	int
************************************/
int unpack2(const char* IN buffer, char* IN_OUT var_2);
/************************************
* 函数名：	unpack4
* 功  能：	解包4字节类型
* 参  数：
*			buffer
*			var_4
* 返回值:	int
************************************/
int unpack4(const char* IN buffer, char* IN_OUT var_4);
/************************************
* 函数名：	unpack8
* 功  能：	解包8字节类型
* 参  数：
*			buffer
*			var_8
* 返回值:	int
************************************/
int unpack8(const char* IN buffer, char* IN_OUT var_8);

#endif // pack_h__


