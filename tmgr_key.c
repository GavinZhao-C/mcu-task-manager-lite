

#include "tmgr_types.h"
#include "tmgr_key.h"
#include "cpu/cpu_key.h"
#include "task.h"

key_callBackFunc_t pKeyCallBackFunc;
void key_set_callback(key_callBackFunc_t func){
	pKeyCallBackFunc = func;
}

// ͨ��ѭ����� ����������������״̬����ʱ���� IRQ
static void* key_irq_makeSureEnable(void*c){
	if(get_status_funcAfter(sys_key_process_irq) == STOP){
		cpu_key_irq_enable();
	}
	do_func_after(key_irq_makeSureEnable, 0, 600);
	return 0;
}

void* sys_key_process_irq(void*c){
	static uint16_t key_detected = 0;
	int pin = (int)c;
	if(cpu_read_key(pin) == 0){
		if(key_detected < 0xFFF0)key_detected++;
		// if key always active , then always scan
		do_func_after(sys_key_process_irq, (void*)pin, 20);
		do_func_after(key_irq_makeSureEnable, 0, 600);  // IRQ enable �ж�ʧ�������Ҫ����һ�������鲢��ʱ���� IRQ
	}else{
		pKeyCallBackFunc(pin, &key_detected);
		key_detected = 0;
		cpu_key_irq_enable();
	}
	return 0;
}



