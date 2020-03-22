


#ifndef __TASK_H__
#define __TASK_H__

#include "tmgr_types.h"

#define T_RUN 1
#define T_STOP 0

typedef void*(*allFunc_t)(void*);
typedef void(*taskFunc_t)(void);

typedef enum{
	STOP = 0,
	RUN_
}run_t;

typedef struct{
	run_t isrun;
	uint16_t wait_time;
	allFunc_t wait_func;
	void * func_arg;
}wait_func_t;

typedef struct{
	run_t isrun;
	taskFunc_t task_func;
}task_t;


void loop_task_in_main(void);

//
void loop_wait_in_main(void);

//
void loop_wait_in_irq(void);

// 在给定时间后执行函数
// 成功：返回数组下标
// 失败：返回 -1
int16_t do_func_after(allFunc_t func, void * arg, uint16_t ms_);

// 获取指定函数的运行状态： RUN_ or STOP
run_t get_status_funcAfter(allFunc_t func);

#endif //__TASK_H__

