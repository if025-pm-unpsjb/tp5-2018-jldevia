#include "FreeRTOS.h"
#include "mbed.h"
#include "task.h"
#include "semphr.h"

Serial pc(USBTX, USBRX);

struct TaskTR {

  char id [10];
  TickType_t c;
  TickType_t t;
  TickType_t d;
  int prioriy;
  TaskHandle_t hdl_task;

};

struct TaskTR systemTR [3] = {{"TR1", 2000, 4000, 4000, 4, NULL},{"TR2", 3000,6000,6000,3, NULL},{"TR3",5000,12000,12000,2, NULL}};

// Referencia del semaforo
SemaphoreHandle_t sem_mutex;

//Recurso compartido al cual van a tener acceso las tareas.
//Solo sirve para guardar el tick de ejecución actual de cada tarea
TickType_t resource_shared [3] = {0 ,0, 0};

void eatCpu(TickType_t ticks){
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

// Acceso al recurso compartido
void access_shared(int num_task){

  if ( xSemaphoreTake(sem_mutex, (TickType_t) 10 ) == pdTRUE ) {
      TickType_t tick = xTaskGetTickCount();
      pc.printf("Tarea %s: Accediendo a recurso compartido en el tick %d.\n\r", systemTR[num_task].id, tick);
      resource_shared[num_task] = tick;
      eatCpu(systemTR[num_task].c);

      //Se libera el semaforo
      xSemaphoreGive( sem_mutex );
  }

}

void taskTR(void* param){
  int ind = (int) param;
  TickType_t pxPreviousWakeTime = 0;

  while(true){
      access_shared(ind);
      vTaskDelayUntil( &pxPreviousWakeTime, systemTR[ind].t);
  }

}

int main ()
{
  vTraceEnable(TRC_INIT);
  sem_mutex = xSemaphoreCreateMutex();

  if (sem_mutex == NULL){
      pc.printf("Error al crear semaforo.");
      return -1;
  }

  for (int i = 0; i < 3; i++)
    {
      xTaskCreate (taskTR, systemTR[i].id, 256, (void *) i, systemTR[i].prioriy,
                   &systemTR[i].hdl_task);
    }
  vTraceEnable(TRC_START);
  vTaskStartScheduler ();
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



