
#ifndef __DATA_FIFO_H__
#define __DATA_FIFO_H__

/*********************************************************
* usage:
    uint8_t buff_a[10];
    uint8_t buff_b[20];
    uint8_t buff_c[30];
    single_byte_FIFO_t fifo_a, fifo_b, fifo_c;
    fifo_init( &fifo_a, buff_a, 10 );
    fifo_init( &fifo_b, buff_b, 20 );
    fifo_init( &fifo_c, buff_c, 30 );
    fifo_in( &fifo_a, 0x50 );
    fifo_in( &fifo_a, 0x60 );
    fifo_in( &fifo_a, 0x70 );
    
    if( is_fifo_empty(&fifo_a) == false ) fifo_out( &fifo_a ); //this will return 0x50
    if( is_fifo_empty(&fifo_a) == false ) fifo_out( &fifo_a ); //this will return 0x60
    if( is_fifo_empty(&fifo_a) == false ) fifo_out( &fifo_a ); //this will return 0x70
    
*********************************************************/


#include "tmgr_types.h"

typedef struct{
    uint8_t unit_size;  //每个fifo元素的大小
    uint16_t unit_nbr;  //fifo buffer count
    uint16_t count;     //缓冲区数量
	uint8_t *p_buff;    //buffer指针
	uint8_t *p_in;      //最后接收缓冲区位置
	uint8_t *p_out;     //最后取走的字节位置
}multi_byte_FIFO_t;


typedef struct{
    uint16_t size;      //fifo buffer count
    uint16_t count;     //缓冲区数量
	uint8_t *p_buff;    //buffer指针
	uint8_t *p_in;      //最后接收缓冲区位置
	uint8_t *p_out;     //最后取走的字节位置
}single_byte_FIFO_t;


void fifo_init(single_byte_FIFO_t *myfifo, uint8_t * buff, uint16_t size);

int8_t fifo_in(single_byte_FIFO_t *myfifo, uint8_t c);

//从fifo取一个byte,如不想等待则在调用前检测 nonempty 是否为1。
uint8_t fifo_out(single_byte_FIFO_t * myfifo);

//取回指定大小的数据，返回实际取回数据大小
uint16_t fifo_out_nbr(single_byte_FIFO_t * myfifo, uint8_t * dat, uint16_t size);

//取回所有可用数据，返回数据大小
uint16_t fifo_out_all(single_byte_FIFO_t * myfifo, uint8_t * dat);

int8_t is_fifo_full(single_byte_FIFO_t *myfifo);
int8_t is_fifo_empty(single_byte_FIFO_t *myfifo);
uint8_t get_fifo_cnt(single_byte_FIFO_t *myfifo);
uint16_t get_fifo_size(single_byte_FIFO_t *myfifo);
void fifo_clear(single_byte_FIFO_t *myfifo);

void test_fifo(void);

#endif  //#ifndef __DATA_FIFO_H__

