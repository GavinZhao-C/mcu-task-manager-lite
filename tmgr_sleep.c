
#include "tmgr_sleep.h"
#include "tmgr_types.h"
#include "cpu/functions.h"
#include "app_display.h"

uint8_t g_dev_busy = 0;

uint8_t sleeping = 0;

void config_sleep(void){
//	_SLEEP_CONTROL_REG |= 0x04;  //Power-down
//	_SLEEP_CONTROL_REG |= _SLEEP_ENABLE_MASK;  // MCUCR |= (1<<SE);
}

void LPW_GPIO_Config(void)  // 休眠 GPIO 输入模式省电
{
	GPIO_InitSettingType z;
	GPIO->PADIR.Word = 0xFFFFFFFF;	
	GPIO->PAINEB.Word = 0xFFFFFFFF;		// 模拟口模式

	GPIO->PBDIR.Word = 0xFFFF;
	GPIO->PBINEB.Word = 0xFFFF;			// 模拟口模式

	z.Dir = GPIO_Direction_Input;
	z.PUE = GPIO_PUE_Input_Enable;  
	GPIO_Init(GPIO_Pin_B9, &z);   
}

void* tmgr_enter_sleep(void* c){
	sleeping = 1;
	disable_display();
	//LPW_GPIO_Config();
	cpu_Sleep();
	return 0;
	
	
	//if(g_dev_busy > 0)return 0;
//	set_led_on(0);
//	set_int0_lowlevel();
//	sleep_cpu();
}

void set_dev_busy(dev_busy_id dev_id, bool busy_flag){
	if(dev_id >= BUSY_MAX){
		g_dev_busy = 0;
		return;
	}
	if(busy_flag == true){
		g_dev_busy |= (1<<dev_id);
	}else{
		g_dev_busy &= ~(1<<dev_id);
	}
}

bool get_dev_busy(dev_busy_id dev_id){
	if(dev_id >= BUSY_MAX)return false;
	if((g_dev_busy & (1<<dev_id)) > 0){
		return true;
	}else{
		return false;
	}
}
