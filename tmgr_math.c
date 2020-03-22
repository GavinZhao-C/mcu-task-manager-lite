


#include "task-manager/tmgr_math.h"

uint16_t find_num_min(uint16_t *dat, uint16_t dat_len){
    uint16_t num_min = 0xFFFF;
    while(dat_len--){
        if(*dat < num_min)num_min = *dat;
        dat++;
    }
    return num_min;
}

typedef struct{
    uint16_t num_limit;
    uint16_t index_limit;
}limit_u16_t;

typedef struct{
    limit_u16_t min;
    limit_u16_t max;
}min_max_u16_t;


float num_avg_without_min_max(uint16_t *dat, uint16_t dat_len){
    min_max_u16_t min_max_tmp;
    uint16_t loop_cnt = 0;
    uint32_t dat_sum = 0;
    while(loop_cnt < dat_len){
        if((loop_cnt != min_max_tmp.max.index_limit) && 
           (loop_cnt != min_max_tmp.min.index_limit)){
            dat_sum += *dat;
        }
        loop_cnt++; dat++;
    }
    
    return ((float)dat_sum/(dat_len-2));
}
