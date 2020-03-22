#ifndef __MY_STRING_H__
	#define __MY_STRING_H__

#include "tmgr_types.h"

uint8_t * uint2str(uint8_t * mychars, 
				uint8_t char_len, 
				uint16_t mynum, uint8_t mytype);
			//void hex2str(uint8_t * mychars, \	//字符串指针，存放转换后的字符串。此数组长度必须大于位数。否则下标越界。
			//uint8_t char_len, \	//输出位数
			//uint16_t mynum, \	//需要转换的数字
			//uint8_t mytype);	//输出格式。0：空位补0，1：不补0，2：输出16进制字符

/*
** 函数功能：
*/
uint16_t str2uint(uint8_t * mychars);

uint8_t hex2char(uint8_t myhex);

uint8_t hex2bcd(uint8_t mhex);

//此函数返回字符串中“分隔符”的个数，作为其他函数的循环次数
uint16_t str_item(const uint8_t *s, uint8_t sep);



//此函数只保存一个项目,	成功返回0，暂时没有错误来源
//参数：源字符串，目的字符串，分隔符，要保存的项目编号。
uint8_t str_cut(const uint8_t *s,	 //源字符串
						uint8_t *t, //目的字符串
						uint8_t t_size, //目的字符串size
						uint8_t sep,	//分隔符
						uint8_t next );  //要保存的项目编号


/*
** 函数功能：提取字符串中某一行，返回提取后的指针。
*/
uint8_t * readline(const uint8_t * s,	//源字符串
									uint8_t * t,	//用于保存结果的字符串
									uint16_t line_num);	//需要的行号，0开始


/*
** 函数功能：合并字符串，返回合并后字符串的指针
*/
uint8_t * str_add(uint8_t *s_big,	//大字符串
						const uint8_t*s_small,	//小字符串
						uint16_t big_size);	//大字符串的大小

//计算字符串长度
uint16_t str_len(const uint8_t *s);

//计算字符串行数
uint16_t str_wc(const uint8_t *s);

//过滤字符串，将不能显示的字符替换为my_sep,返回替换次数
uint16_t str_filter(uint8_t *s, uint8_t my_sep);	

//输出字符串。int i 限定共输出多少字符串
void print1(uint16_t i, ...);

uint8_t str_head(uint8_t *s, uint8_t *fs);
int str_find(uint8_t *s, uint8_t *fs);
int str_find_str(uint8_t *s, uint8_t *fs);
int char_find(uint8_t *s, uint8_t fs);
uint8_t str2hex(uint8_t *p);
uint8_t gps_xor(uint8_t *p);

//查找最小值
uint16_t uint16_find_min(uint16_t *p, uint16_t psize);

//查找最大值
uint16_t uint16_find_max(uint16_t *p, uint16_t psize);

//平均值
uint16_t uint16_avg(uint16_t *p, uint16_t psize);

int32_t print_debug_printf(const char * fmt, ...);

#endif
