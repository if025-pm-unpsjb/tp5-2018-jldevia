#include "FreeRTOS.h"
#include "mbed.h"
#include "task.h"
#include <string>

Serial pc(USBTX, USBRX);

struct TaskTR {

  char id [10];
  TickType_t c;
  TickType_t t;
  TickType_t d;
  int prioriy;

};

struct TaskTR systemTR [3] = {{"TR1", 200, 4000, 4000, 4},{"TR2", 100,5000,5000,3},{"TR3",100,6000,6000,2}};
//TaskTR systemTR[3];


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

  }

}

int main()
{
   for (int i = 0; i < 3; i++ ){

       xTaskCreate(taskTR, systemTR[i].id , 256, (void *)i, systemTR[i].prioriy, NULL);
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


