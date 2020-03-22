

#include "tmgr_fifo.h"
#include "tmgr_string.h"

void fifo_init(single_byte_FIFO_t *myfifo, uint8_t * buff, uint16_t size){
	myfifo->p_buff = buff;
	myfifo->p_in = buff;
	myfifo->p_out = buff;
	myfifo->count = 0;
    myfifo->size = size;
}

void fifo_clear(single_byte_FIFO_t *myfifo){
	myfifo->p_in = myfifo->p_buff;
	myfifo->p_out = myfifo->p_buff;
	myfifo->count = 0;
}

uint16_t get_fifo_size(single_byte_FIFO_t *myfifo){
    return myfifo->size;
}

int8_t is_fifo_full(single_byte_FIFO_t *myfifo){
    if(myfifo->count == myfifo->size){
        return 1;
    }else{
        return -1;
    }
}

int8_t is_fifo_empty(single_byte_FIFO_t *myfifo){
    if(myfifo->count == 0){
        return 1;
    }else{
        return -1;
    }
}

uint8_t get_fifo_cnt(single_byte_FIFO_t *myfifo){
    return myfifo->count;
}

int8_t fifo_in(single_byte_FIFO_t *myfifo, uint8_t c){
    if(is_fifo_full(myfifo) == 1)return -1;
    *(myfifo->p_in) = c;
    myfifo->p_in++;
	if(myfifo->p_in == (myfifo->p_buff + myfifo->size)){
		myfifo->p_in = myfifo->p_buff; //地址到顶部回到底部
	}
	myfifo->count++;
    return 1;
}

//从fifo取一个byte,如不想等待则在调用前检测 count 是否 > 0。
uint8_t fifo_out(single_byte_FIFO_t * myfifo){
    uint8_t c;
    //while(myfifo->count == 0);   //空等待
    if(myfifo->count == 0)return 0;
    c = *(myfifo->p_out);
    myfifo->p_out++;
    myfifo->count--;
    if(myfifo->p_out == (myfifo->p_buff + myfifo->size)){
        myfifo->p_out = myfifo->p_buff; //地址到顶部回到底部
    }
    if (myfifo->p_out == myfifo->p_in){
        myfifo->count = 0;       //地址相等,接收缓冲区空，再取数前要检该标志
    }
    return c;
}

//取回指定大小的数据，返回实际取回数据大小
uint16_t fifo_out_nbr(single_byte_FIFO_t * myfifo, uint8_t * dat, uint16_t size){
    uint16_t i = 0;
	while( (myfifo->count > 0)  &&  (size > 0) ){
		dat[i] = fifo_out(myfifo);
		size--;
        i++;
	}
    return i;
}

//取回所有可用数据，返回数据大小
uint16_t fifo_out_all(single_byte_FIFO_t * myfifo, uint8_t * dat){
	uint16_t i=0;
	while(myfifo->count > 0){
		dat[i] = fifo_out(myfifo);
		i++;
	}
	return (i);
}

void test_fifo(void){
    uint8_t tmp_buff[32];
	uint8_t i=0;
    single_byte_FIFO_t tmp_fifo;
    //wait_tx();
    //debug_printf("this is %s.\r\n", __func__);
    //wait_tx();
    fifo_init(&tmp_fifo, tmp_buff, 32);
    for(i=0; i<255; i++){
        //debug_printf("fifo in : %d, ", i);
        fifo_in(&tmp_fifo, i);
        //debug_printf("fifo out : %d\r\n", fifo_out(&tmp_fifo));
        //wait_tx();
    }
}



