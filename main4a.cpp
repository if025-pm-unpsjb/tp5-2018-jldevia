/*
 * main4a.cpp : Corresponde al inciso 5 del Trabajo Práctico N° 5.
 *
 *  Created on: 27 nov. 2018
 *      Author: jldevia
 */
#include "FreeRTOS.h"
#include "mbed.h"
#include "task.h"
#include "queue.h"
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

struct TaskTR systemTR [3] = {{"TR1", 2000, 4000, 4000, 4, NULL},{"TR2", 3000,4000,4000,4, NULL},{"TR3",5000,4000,4000,4, NULL}};

//Handler de colas
QueueHandle_t queueMsgTR2, queueMsgTR3;

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


void taskTR(void* param){
  int ind = (int) param;
  TickType_t pxPreviousWakeTime = 0;

  int msg;

  while(true){
      switch (ind){
        case 0:
          pc.printf("Ejecutando TR1\n\r");

          eatCpu(systemTR[ind].c);
          if ( xQueueSend( queueMsgTR2, ( void * ) &msg, ( TickType_t ) 10 ) != pdTRUE ){
              pc.printf("Error TR1: No se pudo escribir en la cola de mensajes.\n\r");
          }else{
              pc.printf("TR1: mensaje enviado.\n\r");
          }
          break;
        case 1:
          pc.printf("Ejecutando TR2\n\r");
          if ( xQueueReceive(queueMsgTR2, &msg, ( TickType_t ) 50 ) ) {
             pc.printf("TR2: mensaje recibido.\n\r");
             if ( xQueueSend( queueMsgTR3, ( void * ) &msg, ( TickType_t ) 10 ) != pdTRUE ) {
                 pc.printf("Error TR2: No se pudo escribir en la cola de mensajes.\n\r");
             }else{
                 pc.printf("TR2: mensaje enviado.\n\r");
             }
          }
          break;
        case 2:
          pc.printf("Ejecutando TR3\n\r");
          if ( xQueueReceive(queueMsgTR3, &msg, ( TickType_t ) 50 ) ) {
              pc.printf("TR3: mensaje recibido.\n\r");
          }
     }

      vTaskDelayUntil( &pxPreviousWakeTime, systemTR[ind].t);
  }

}


int main (){
  queueMsgTR2 = xQueueCreate( 5, sizeof(int) );
  queueMsgTR3 = xQueueCreate( 5, sizeof(int) );

  // Si no se pudo crear las colas se termina el programa
  if ( queueMsgTR2 == NULL || queueMsgTR3 == NULL ){
      exit(1);
  }

  for (int i = 0; i < 3; i++){
      xTaskCreate (taskTR, systemTR[i].id, 256, (void *) i, systemTR[i].prioriy,
                     &systemTR[i].hdl_task);
  }

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



