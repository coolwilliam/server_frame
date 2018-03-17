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
	eec_param_err = -1,		//��������
};

/*================================= pack ===================================*/

/************************************
* ��������	pack_byte_array
* ��  �ܣ�	�������
* ��  ����
*			var_array
*			length
*			buffer
* ����ֵ:	int
************************************/
int pack_byte_array(const char* IN var_array, 
	const unsigned long IN length, 
	char* IN_OUT buffer);
/************************************
* ��������	pack1
* ��  �ܣ�	���1�ֽ�����
* ��  ����
*			var_1
*			buffer
* ����ֵ:	int
************************************/
int pack1(const char* IN var_1, 
	char* IN_OUT buffer);
/************************************
* ��������	pack2
* ��  �ܣ�	���2�ֽ�����
* ��  ����
*			var_2
*			buffer
* ����ֵ:	int
************************************/
int pack2(const char* IN var_2, 
	char* IN_OUT buffer);
/************************************
* ��������	pack4
* ��  �ܣ�	���4�ֽ�����
* ��  ����
*			var_4
*			buffer
* ����ֵ:	int
************************************/
int pack4(const char* IN var_4, 
	char* IN_OUT buffer);
/************************************
* ��������	pack8
* ��  �ܣ�	���8�ֽ�����
* ��  ����
*			var_8
*			buffer
* ����ֵ:	int
************************************/
int pack8(const char* IN var_8, 
	char* IN_OUT buffer);

/*================================= unpack ===================================*/

/************************************
* ��������	unpack_byte_array
* ��  �ܣ�	�������
* ��  ����
*			buffer
*			length
*			var_array
* ����ֵ:	int
************************************/
int unpack_byte_array(const char* IN buffer, 
	const unsigned long IN length, 
	char* IN_OUT var_array);


/************************************
* ��������	unpack1
* ��  �ܣ�	���1�ֽ�����
* ��  ����
*			buffer
*			var_1
* ����ֵ:	int
************************************/
int unpack1(const char* IN buffer, char* IN_OUT var_1);
/************************************
* ��������	unpack2
* ��  �ܣ�	���2�ֽ�����
* ��  ����
*			buffer
*			var_2
* ����ֵ:	int
************************************/
int unpack2(const char* IN buffer, char* IN_OUT var_2);
/************************************
* ��������	unpack4
* ��  �ܣ�	���4�ֽ�����
* ��  ����
*			buffer
*			var_4
* ����ֵ:	int
************************************/
int unpack4(const char* IN buffer, char* IN_OUT var_4);
/************************************
* ��������	unpack8
* ��  �ܣ�	���8�ֽ�����
* ��  ����
*			buffer
*			var_8
* ����ֵ:	int
************************************/
int unpack8(const char* IN buffer, char* IN_OUT var_8);

#endif // pack_h__


