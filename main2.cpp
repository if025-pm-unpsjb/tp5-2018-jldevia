#include "FreeRTOS.h"
#include "mbed.h"
#include "task.h"

extern "C"{
  void vApplicationTickHook( void );
}

Serial pc(USBTX, USBRX);

struct TaskTR {

  char id [10];
  TickType_t c;
  TickType_t t;
  TickType_t d;
  int prioriy;
  TaskHandle_t hdl_task;

};

struct TaskTR systemTR [3] = {{"TR1", 2000, 4000, 4000, 4, NULL},{"TR2", 6000,5000,5000,3, NULL},{"TR3",1000,6000,6000,2, NULL}};

void eatCpu(TickType_t ticks)
{
  TickType_t cnt = 0;
  TickType_t prev = xTaskGetTickCount();
  while (true) {
    if (prev < xTaskGetTickCount()) {
      cnt++;
      prev = xTaskGetTickCount();
    }
    if (cnt > ticks) {
      break;

    }

  }
}

void taskTR(void* param){

  int ind = (int) param;
  int num_instancia = 0;
  TickType_t pxPreviousWakeTime = 0;

  while(true){
      TickType_t init = xTaskGetTickCount();

      eatCpu(systemTR[ind].c);

      TickType_t end = xTaskGetTickCount();

      pc.printf("Tarea %s [%d,%d,%d]\n\r", systemTR[ind].id, init, end, num_instancia);

      vTaskDelayUntil( &pxPreviousWakeTime, systemTR[ind].t);

      num_instancia++;

      vTaskSetThreadLocalStoragePointer( NULL, 0, ( void * ) (pxPreviousWakeTime + systemTR[ind].d) );

  }

}

void vApplicationTickHook( void ){
  TickType_t pxTickNow = xTaskGetTickCountFromISR();

  for (int i = 0;  i < 3; ++ i) {
    TickType_t next_venc = ( TickType_t ) pvTaskGetThreadLocalStoragePointer( systemTR[i].hdl_task, 0 );

    if ( next_venc < pxTickNow ) {
        //vTaskSuspendAll();
        pc.printf("Tarea %s vencida en %d, deberia vencer en %d. \n\r", systemTR[i].id, pxTickNow, next_venc);
    }
  }

}

int main()
{
   for (int i = 0; i < 3; i++ ){

       xTaskCreate(taskTR, systemTR[i].id , 256, (void *)i, systemTR[i].prioriy, &systemTR[i].hdl_task);

       vTaskSetThreadLocalStoragePointer( systemTR[i].hdl_task, 0, ( void * ) systemTR[i].d );
   }

    vTaskStartScheduler();

}


void vConfigureTimerForRunTimeStats( void )
{
const unsigned long TCR_COUNT_RESET = 2,
                    CTCR_CTM_TIMER = 0x00,
                    TCR_COUNT_ENABLE = 0x01;

    /* Power up and feed the timer with a clock. */
    LPC_SC->PCONP |= 0x02UL;
    LPC_SC->PCLKSEL0 = (LPC_SC->PCLKSEL0 & (~(0x3<<2))) | (0x01 << 2);

    /* Reset Timer 0 */
    LPC_TIM1->TCR = TCR_COUNT_RESET;

    /* Just count up. */
    LPC_TIM1->CTCR = CTCR_CTM_TIMER;

    /* Prescale to a frequency that is good enough to get a decent resolution,
    but not too fast so as to overflow all the time. */
    LPC_TIM1->PR =  ( configCPU_CLOCK_HZ / 10000UL ) - 1UL;

    /* Start the counter. */
    LPC_TIM1->TCR = TCR_COUNT_ENABLE;
}
