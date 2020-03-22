
#ifndef __SYS_KEY_H__
#define __SYS_KEY_H__

#include "task.h"

typedef void(*key_callBackFunc_t)(uint16_t pin, uint16_t * timeout);

void key_set_callback(key_callBackFunc_t func);

void* sys_key_process_irq(void*c);

#endif  // __SYS_KEY_H__
