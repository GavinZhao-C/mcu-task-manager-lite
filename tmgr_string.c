
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "tmgr_string.h"
#include "cpu/functions.h"

const uint8_t tab_hex[] = "0123456789ABCDEF";

//给str加 \0 结尾
void str_add0(uint8_t *str){
    while(*str){
        //if(*str<32 || *str>126)break;
        if(*str<32 || *str>126)break;
        str++;
    }
    *str = 0;
}

int str_cmp(uint16_t size, const uint8_t *str1, const uint8_t *str2){
    while(size--){
        if(*str1 != *str2)return -1;
        str1++;str2++;
    }
    return 1;
}

uint16_t str_item(const uint8_t *s, uint8_t sep){
    uint16_t cnt = 0;
    while(*s++){
        if(*s == sep)cnt++;
    }
    return cnt;
}

/*
此函数只保存一个项目,    成功返回0，若需要保存的项目编号大于项目总数则返回1;
参数：源字符串，目的字符串，分隔符，要保存的项目编号。

const uint8_t *s,     //源字符串
uint8_t *t, //目的字符串
uint8_t t_size, //目的字符串size
uint8_t sep,    //分隔符
uint8_t next ){  //要保存的项目编号
*/
uint8_t str_cut(const uint8_t *s, uint8_t *t, uint8_t t_size, uint8_t sep, uint8_t next ){
    uint16_t  num = 0, i = 0;
    uint8_t  my_ret = 0;
    if ((uint8_t)str_item(s,sep)<next){my_ret = 1;}
    while(*s){        
        if (num == next){    //若项目计数 等于 需要的项目数
            if(*s==sep || *s<32 || *s>126 || (i+1)==t_size){    //若当前项目为空 或 特殊字符
                *t = 0;
                break;    
            }else{    //若项目符合要求，保存当前字符，指针+1
                *t = *s;
                t++;
                i++;
            }
        }else{    //当前项目不是需要的项目
            if (*s == sep)    //若遇到分隔符，则说明已到下一个项目，项目计数+1
                num++;
        }
        s++;        
    }
    *t = 0;
    return my_ret;
}

//返回截取的字符串，过滤回车换行，item_id为编号，从0开始， sep 为分隔符， dist_str为目标位置
uint8_t * str_cut1(uint8_t item_id, uint8_t sep, uint8_t *src_str, uint8_t *dist_str){
    uint8_t i = 0, item=0;
    while(*src_str<=32 && *src_str > 0)src_str++;//过滤字符
    while(*src_str){
        if(*src_str == sep){
            item++;
            src_str++;
        }
        while(*src_str<=32 && *src_str > 0)src_str++;//过滤字符
        if(item == item_id){
            while(*src_str){
                dist_str[i]=*src_str;
                i++;src_str++;
                if(i>62 || *src_str == sep || *src_str<=32)break;
            }
        }
        if(i>0)break;
        while(*src_str != sep && *src_str != 0)src_str++;
    }
    dist_str[i]=0;
    return dist_str;
}

//返回截取的字符串，过滤回车换行，最大64字节，item_id为编号，从0开始， sep 为分隔符，
uint8_t * str_cut_64(uint8_t item_id, uint8_t sep, uint8_t *str){
    static uint8_t item_str[64];
    uint8_t i = 0, item=0;
    while(*str<=32 && *str > 0)str++;//过滤字符
    while(*str){
        if(*str == sep){
            item++;
            str++;
        }
        while(*str<=32 && *str > 0)str++;//过滤字符
        if(item == item_id){
            while(*str){
                item_str[i]=*str;
                i++;str++;
                if(i>62 || *str == sep || *str<=32)break;
            }
        }
        if(i>0)break;
        while(*str != sep && *str != 0)str++;
    }
    item_str[i]=0;
    return item_str;
}

//统计字符串长度
uint16_t str_len(const uint8_t *str){
    uint16_t i=0;
    while(*str++ && i < 65535)i++;
    return i;
}

// 返回 -1 未找到，返回其它表示找到的位置
int str_find_str1(uint8_t *src, uint8_t *flag){
    volatile uint8_t same=0;
    uint8_t * p_flg = flag;
    uint8_t * p_src = src;
    int index_src = 0;
    while(*p_src && *p_flg){
        if(*p_src == *p_flg){
            if(same == 0){
                same = 1;
                index_src = p_src - src;
            }
            p_flg++;
        }else{
            same = 0;
            p_flg = flag;
        }
        p_src++;
    }
    if(same == 1 && *p_flg == 0){
        return index_src;
    }else{
        return -1;
    }
}

//uint8_t chk_gps_xor(uint8_t *p){
//    uint8_t  sum = 0, sum_bak = 0, my_ret = 0;
//    uint8_t  temp[3];
//    str_cut(p,temp,3,'*',1); // 取出 * 号后面的两个校验字符
//    sum_bak = convert_str_hex_to_uint8(temp);
//    p++;
//    while(*p != '*'){
//        sum ^= *p;
//        p++; 
//    }
//    if (sum == sum_bak){
//        my_ret = 0;
//    }else{
//        my_ret = 1;
//    }
//    return my_ret;
//}

//此函数返回字符串中“分隔符”的个数，作为其他函数的循环次数
uint16_t str_find_sep(uint8_t *s, uint8_t sep){
    uint16_t num=0;
    while(*s){
        if (*s == sep)num++;
        s++;
    }
    return num;
}

/*
** 函数功能：提取字符串中某一行，返回提取后的指针。
        // uint8_t * s    //源字符串
        // uint8_t * t    //用于保存结果的字符串
        // uint16_t line_num    //需要的行号，0开始
*/
uint8_t * str_get_line(uint8_t * s,uint8_t * t,uint16_t line_num){

    uint16_t  cr_count=0;
    uint8_t *p;
    p = t;
    while(*s){
        if(cr_count == line_num){
            while(*s >31 && *s<127){
                *p = *s;
                p++;
                s++;
            }
            *p = '\0';
            break;
        }else{       //不是需要的行
            //找到需要的行，并定位到需要行的第一个可用字

            if      (*s == 10 && *(s+1) != 13){
                cr_count++;
            }else if(*s == 13 && *(s+1) != 10){
                cr_count++;
            }else if(*s == 10 && *(s+1) == 13){
                cr_count++;
                s++;
            }else if(*s == 13 && *(s+1) == 10){
                cr_count++;
                s++;
            }
            // 一堆判断是用于 0D 0A 或 0D 或 0A 或 0A 0D 的混乱处理
            // 0D,13,\r
            // 0A,10,\n
        }
        s++;    
    }
    return (t);
}


/*
** 函数功能：合并字符串，返回合并后字符串的指针
*/

uint8_t * str_add(uint8_t *s_big,    //大字符串
                  const uint8_t*s_small,    //小字符串
                  uint16_t big_size){    //大字符串的大小
    uint16_t  this_num = 0;
    uint8_t *p;
    p = s_big;
    while(*p){
        p++;
        this_num++;
    }
    while(*s_small){
        *p = *s_small;
        p++;
        s_small++;
        this_num++;
        if(*s_small == 0 || this_num >= big_size-1){
            *(p+big_size-1) = 0;
            break;
        }
    }
    *p = 0;
    return (s_big);
}


//统计字符串行数
uint16_t str_wc(const uint8_t *s){
    uint16_t my_len=0;
    while(*s){
        if(*s == '\n')my_len++;//此处需要改进，参考 readline
        s++;
    }
    return my_len;
}

/*
过滤不可见字符，将不可见字符替换为 my_sep
*/
uint16_t str_filter(uint8_t *s, uint8_t my_sep){
    uint16_t my_lines = 0;
    while(*s){
        if(*s < 32 || *s > 126){
            *s = my_sep;
            my_lines++;
        }
        s++;
    }
    return my_lines;
}


//判断 *fs 的所有字符是否等于 *s 的前端。返回 -1 表明前端不相等
int str_cmp_head(uint8_t *s, uint8_t *fs){
    int my_ret = -1;
    while(*fs && *s){
        if (*s != *fs){
            my_ret = -1;
            break;
        }else{
            my_ret = 0;
        }
        s++;
        fs++;
    }
    return my_ret;
}

//查找 fs 是否存在于 s 中。不存在返回-1，存在返回所在位置
int str_find_str(uint8_t *s, uint8_t *fs){
    int sfp = 0;
    uint8_t find_ok = 0;
    while(*s){
        if(*s == *fs){
            if(str_cmp_head(s, fs)>=0){
                find_ok = 1;
                break;
            }
        }
        sfp++;
        s++;
    }
    if(find_ok == 0)sfp = -1;
    return sfp;
}

//查找 fc 是否存在于 s 中。不存在返回-1，存在返回所在位置
int str_find_char(uint8_t *s, uint8_t fc){
    int  sfp1 = 0;
    uint8_t  find_ok1 = 0;
    while(*s){
        if(*s == fc){
           find_ok1 = 1;
           break;
        }
        sfp1++;
        s++;
    }
    if(find_ok1 == 0)sfp1 = -1;
    return sfp1;
}

//查找最小值
uint16_t uint16_find_min(uint16_t *p, uint16_t psize){
    uint16_t i=0, my_ret=0, fmin=*p;
    for(i=0; i<psize; i++){
        if (fmin > *p){
            fmin = *p;
            my_ret=i;
        }
        p++;
    }
    return my_ret;
}

//查找最大值
uint16_t uint16_find_max(uint16_t *p, uint16_t psize){
    uint16_t i=0, my_ret=0, fmax=*p;
    for(i=0; i<psize; i++){
        if (fmax < *p){
            fmax = *p;
            my_ret=i;
        }
        p++;
    }
    return my_ret;
}

//平均值
uint16_t uint16_avg(uint16_t *p, uint16_t psize){
    uint16_t i;
    uint32_t my_sum = 0;
    for(i=0; i<psize; i++){
        my_sum += *p;
        p++;
    }
    return (uint16_t)my_sum/psize;
}

static void put_c(uint8_t c){
    SendByte(c);
}
static void put_s(uint8_t *s){
    while(*s)put_c(*s++);
}

uint8_t byte_l(uint8_t n){
    return(n & 0x0F);
}

uint8_t byte_h(uint8_t n){
    return(n >> 4);
}

void put_1hex(uint8_t c){
    put_c(tab_hex[byte_h(c)]);
    put_c(tab_hex[byte_l(c)]);
    put_c(' ');
}

void put_2hex(uint16_t n){
    put_1hex((uint8_t)(n>>8));
    put_1hex((uint8_t)n);
}

void put_4hex(uint32_t n){
    uint8_t mystr[11]={'0','x',}, i;
    mystr[10] = 0;
    for(i=9; i>1; i--){
        mystr[i] = tab_hex[n & 0x0000000F];
        n = n>>4;
    }
    put_s(mystr);
}


//void prints(uint8_t *str, ...){
//    char *tmp_str;
//    va_list arg_ptr;
//    va_start( arg_ptr, str );
//    put_s(str);
//    while(1){
//        tmp_str = va_arg(arg_ptr, char *);
//        if(*tmp_str>0){
//            put_s((uint8_t *)tmp_str);
//        }else{
//            break;
//        }
//    }
//    va_end( arg_ptr );
//}

//int32_t print_eprintf(uint8_t *str, ...){
//   uint16_t str_size = 0;
//   uint16_t index = 0;
//   int32_t tmp_int32;
//   //uint8_t tmp_array[32];
//   uint8_t *tmp_uint8p;
//   uint8_t tmp_uint8;
//   uint16_t tmp_uint16;
//   
//   va_list arg_ptr;
//   va_start( arg_ptr, str );

//   str_size = str_len(str);
//   
//   for(index=0; index<str_size; index++){
//       
//       // normal str
//       if(str[index] != '%'){
//           put_c(str[index]);
//           continue;
//       }
//       
//       // format str
//       switch (str[index+1]){
//           case('d')://%d
//               tmp_int32 = va_arg(arg_ptr, int);
//               put_s(convert_uint32_to_str_dec(tmp_int32));
//               break;
//           case('l')://%ld
//               if(str[index+2] == 'd'){
//                   index++;
//                   tmp_int32 = va_arg(arg_ptr, long);
//                   put_s(convert_uint32_to_str_dec(tmp_int32));
//               }
//               break;
//           case('s')://%s
//               tmp_uint8p = (uint8_t *)va_arg(arg_ptr, char *);
//               put_s(tmp_uint8p);
//               break;
//           case('X'):
//               tmp_uint16 = (uint16_t)va_arg(arg_ptr, int);
//               put_2hex(tmp_uint16);
//               break;
//           case('c'):
//               tmp_uint8 = (uint8_t)va_arg(arg_ptr, char);
//               put_c(tmp_uint8);
//               break;
//           default:break;
//       }
//       index++;
//   }
//   va_end( arg_ptr );
//   return 0;
//}


int8_t str_is_num(uint8_t * data){
    while(*data){
        if(*data<'0' || *data>'9')return -1;
        data++;
    }
    return 1;
}

// 如果是 hex ，返回 hex 的字符数量，此函数还没有测试
// 如果不是 hex ，返回 -1
int str_is_hex(uint8_t * data){
    uint8_t * p_data = data;
    while(*p_data){
        if( (*p_data>='0' && *p_data<='9') ||
            (*p_data>='A' && *p_data<='F') ||
            (*p_data>='a' && *p_data<='f') ){
            return -1;
        }
        p_data++;
    }
    return (p_data-data);
}

int32_t print_debug_printf(const char * fmt, ...){
   /*
   http://www.deyisupport.com/question_answer/wireless_connectivity/bluetooth/f/103/t/69222.aspx?pi2132219853=2
   */
   uint32_t  ulLen;
   static uint8_t pBuf[256];
   va_list ap;

   //char *pBuf = (char*)osal_mem_alloc(PRINT_BUF_LEN);  // 开辟缓冲区
   va_start(ap, fmt);
   ulLen = vsprintf((char*)pBuf, fmt, ap);        // 用虚拟打印函数实现
   va_end(ap);

   //HalUARTWrite(HAL_UART_PORT_0, (uint8_t*)pBuf, ulLen); // 从串口0输出
   put_s((uint8_t *)pBuf);
   //osal_mem_free(pBuf);    // 释放内存空间
   return ulLen;
}


//int32_t print_debug_printf1(const uint8_t *str, ...){
//    uint16_t str_size = 0;
//    uint16_t index = 0;
//    int32_t tmp_int32;
//    uint8_t *tmp_uint8p;
//    uint8_t tmp_uint8;
//    uint16_t tmp_uint16;
//    
//    va_list arg_ptr;
//    va_start( arg_ptr, str );
//    str_size = str_len((uint8_t*)str);
//    
//    for(index=0; index<str_size; index++){
//        
//        // normal str
//        if(str[index] != '%'){
//            put_c(str[index]);
//            continue;
//        }
//        
//        // format str
//        index++;
//        switch (str[index]){
//            case('d')://%d
//                tmp_int32 = va_arg(arg_ptr, int);
//                put_s(convert_uint32_to_str_dec(tmp_int32));
//                break;
//            case('l')://%ld
//                if(str[index+1] == 'd'){
//                    index++;
//                    tmp_int32 = va_arg(arg_ptr, long);
//                    put_s(convert_uint32_to_str_dec(tmp_int32));
//                }
//                break;
//            case('s')://%s
//                tmp_uint8p = (uint8_t *)va_arg(arg_ptr, char *);
//                put_s(tmp_uint8p);
//                break;
//            case('x'):
//            case('X'):
//                tmp_uint16 = (uint16_t)va_arg(arg_ptr, int);
//                put_2hex(tmp_uint16);
//                break;
//            case('c'):
//                tmp_uint8 = (uint8_t)va_arg(arg_ptr, char);
//                put_c(tmp_uint8);
//                break;
////            case('o'):
////            case('u')://uint16_t
////            case('f'):
////            case('e')://指数
////            case('E'):
////            case('g')://
////            case('G'):
//            default:break;
//        }
//    }
//    va_end( arg_ptr );
//    return 0;
//}


// 计算 base 的 power 次方 , power > 0  ,  还没测试
uint32_t calc_pow(uint16_t base, uint8_t power){
    uint32_t tmp = base;
    uint8_t i=1;
    for(i=1; i<power; i++){
        tmp *= base;
    }
    return tmp;
}




