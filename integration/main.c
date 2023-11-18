#include <math.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"

#include "../FreeRTOS-Kernel/include/task.h"


//Our modules
#include "../motor/motor.c"

#define TEST_TASK_PRIORITY				( tskIDLE_PRIORITY + 1UL )

int main(){
    TaskHandle_t task;

    xTaskCreate(forward, "MoveCarForward", configMinimal_STACK_SIZE, NULL, TEST_TASK_PRIORITY);

    while(1){
        vTaskStartScheduler();
    }

    return 0;
}