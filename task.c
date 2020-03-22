
#include "task.h"
//#include "functions.h"

#define WAIT_FUNC_CNT 32

#define TASK_CNT ARRAY_SIZE(task_tables)

void * func_empty(void * c){
	return 0;
}

void task_empty(){
	;
}

task_t task_tables[]={
	{STOP, task_empty}
};

wait_func_t wait_func_tab[WAIT_FUNC_CNT]={
	{STOP, 0, func_empty, 0}
};

void loop_task_in_main(void){
	uint16_t i = 0;
	//wait_func_t *pfunc = wait_func_tab;
	for(i=0; i<TASK_CNT; i++){
		// check running only
		if(task_tables[i].isrun == STOP)continue;
		task_tables[i].task_func();
	}
}

//
void loop_wait_in_main(void){
	uint16_t i = 0;
	//wait_func_t *pfunc = wait_func_tab;
	for(i=0; i<WAIT_FUNC_CNT; i++){
		if(wait_func_tab[i].wait_func == func_empty)break;
		if(wait_func_tab[i].isrun == STOP)continue;
		
		if(wait_func_tab[i].wait_time == 0){
			wait_func_tab[i].isrun = STOP;
			wait_func_tab[i].wait_func(wait_func_tab[i].func_arg);
		}
	}
}

//
void loop_wait_in_irq(void){
	uint16_t i = 0;
	//wait_func_t *pfunc = wait_func_tab;
	for(i=0; i<WAIT_FUNC_CNT; i++){
		// check running only
		if(wait_func_tab[i].isrun == STOP)continue;
		if(wait_func_tab[i].wait_time > 0)wait_func_tab[i].wait_time--;;
	}
}

uint16_t get_waitFuncTab_real_cnt(void){
	uint16_t i;
	for(i=0; i<WAIT_FUNC_CNT; i++){
		if(wait_func_tab[i].wait_func == func_empty)break;
	}
	return i;
}

uint16_t find_1st_stopFunc(){
	uint16_t i;
	for(i=0; i<WAIT_FUNC_CNT; i++){
		if(wait_func_tab[i].isrun == STOP)break;
	}
	return i;
}

uint16_t find_last_runFunc(){
	uint16_t i;
	uint16_t run = 0;
	for(i=0; i<WAIT_FUNC_CNT; i++){
		if(wait_func_tab[i].wait_func == func_empty){
			break;
		}
		if(wait_func_tab[i].isrun == RUN_){
			run = i;
		}
	}
	return run;
}

static void set_wait_tab_empty(uint16_t index){
	wait_func_tab[index].isrun = STOP;
	wait_func_tab[index].wait_time = 0;
	wait_func_tab[index].wait_func = func_empty;
	wait_func_tab[index].func_arg = 0;
}

run_t get_status_funcAfter(allFunc_t func){
	uint16_t i;
	run_t run = STOP;
	for(i=0; i<WAIT_FUNC_CNT; i++){
		if(wait_func_tab[i].wait_func == func_empty){
			break;
		}
		if(wait_func_tab[i].wait_func == func){
			run = wait_func_tab[i].isrun;
			break;
		}
	}
	return run;
}

static int16_t get_wait_index(allFunc_t func){
	int16_t i = 0;
	uint8_t find = 0;
	for(i=0; i<WAIT_FUNC_CNT; i++){
		if(wait_func_tab[i].wait_func == func_empty){
			break;
		}
		if(wait_func_tab[i].wait_func == func){
			find = 1;
			break;
		}
	}
	if(find)return i;
	return -1;
}

// 在给定时间后执行函数
// 成功：返回数组下标
// 失败：返回 -1
// 设置时间为 0， 直接 stop 现有函数
int16_t do_func_after(allFunc_t func, void * arg, uint16_t ms_){
	uint16_t waitFuncFreeIndex = 0;
	uint16_t last_runFuncIndex = 0;
	int16_t same_index;
	
	// 查找是否有空余的 STOP 位置， 并判断是否满员
	waitFuncFreeIndex = find_1st_stopFunc();
	if(waitFuncFreeIndex >= (WAIT_FUNC_CNT - 2)){
		return -1;
	}
	
	// 查找是否存在相同的 func
	same_index = get_wait_index(func);
	
	// 设置时间为 0， 直接 stop 现有函数
	if((ms_ == 0) && (same_index > -1)){
		wait_func_tab[same_index].isrun = STOP;
		wait_func_tab[same_index].wait_time = ms_;
		return 0;
	}
	
	// 查找最后一个标记为 RUN_ 的 func
	last_runFuncIndex = find_last_runFunc();
	
	// 如果有相同的 func ，则不占用新的位置
	if(same_index > -1)waitFuncFreeIndex = same_index;
	
	wait_func_tab[waitFuncFreeIndex].isrun = RUN_;
	wait_func_tab[waitFuncFreeIndex].wait_time = ms_;
	wait_func_tab[waitFuncFreeIndex].wait_func = func;
	wait_func_tab[waitFuncFreeIndex].func_arg = arg;
	
	// 设置 结尾 empty
	if(last_runFuncIndex < WAIT_FUNC_CNT){
		set_wait_tab_empty(last_runFuncIndex + 2); // last 占用一个， 刚添加的也占用一个，所以置空这里 +2
	}
	return waitFuncFreeIndex;
}

//中断函数
//void SysTick_Handler(void){
//    //sys_tick_cnt++;
//}

//ISR(TIM1_OVF_vect){
//	TCNT1H = 0xFC;
//	TCNT1L = 0x1A;
//	sys_tick_cnt++;
//}
