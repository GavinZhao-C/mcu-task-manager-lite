

#ifndef __SYS_SLEEP_H__
#define __SYS_SLEEP_H__

#include <stdbool.h>

typedef enum  {
	BUSY_FAN,
	BUSY_CHG,
	BUSY_TIM,
	BUSY_MAX,
}dev_busy_id;

void config_sleep(void);

void* tmgr_enter_sleep(void* c);

void set_dev_busy(dev_busy_id dev_id, bool busy_flag);
bool get_dev_busy(dev_busy_id dev_id);

#endif


