#include "Start_Task.h"

#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "tim.h"

uint32_t CPU_high_water;

#define START_TASK_PRIO 1  //任务优先级
#define START_STK_SIZE 512 //任务堆栈
static TaskHandle_t StartTask_Handler; //任务句柄

#define CPU_TASK_PRIO 20  //任务优先级
#define CPU_STK_SIZE 512 //任务堆栈
static TaskHandle_t CPUTask_Handler; //任务句柄

void start_task(void *pvParameters)
{
  taskENTER_CRITICAL();
	
	xTaskCreate((TaskFunction_t)CPU_task,          //任务函数
                (const char *)"CPU_task",          //任务名称
                (uint16_t)CPU_STK_SIZE,            //任务堆栈大小
                (void *)NULL,                        //传递给任务函数的参数
                (UBaseType_t)CPU_TASK_PRIO,        //任务优先级
                (TaskHandle_t *)&CPUTask_Handler); //任务句柄
	
	vTaskDelete(StartTask_Handler); //删除开始任务
  taskEXIT_CRITICAL();            //退出临界区
}



void CPU_task(void *pvParameters)
{
	
	uint8_t CPU_RunInfo1[200]; //保存任务运行时间信息 分别是：任务名 任务状态 优先级 剩余栈 任务序号
  uint8_t CPU_RunInfo2[200]; //保存任务运行时间信息 分别是：任务名 运行计数  使用率
	
   while (1) {
    memset(CPU_RunInfo1,0,200); //信息缓冲区清零
 
    vTaskList((char *)&CPU_RunInfo1); //获取任务运行时间信息
    
    memset(CPU_RunInfo2,0,200); //信息缓冲区清零
 
    vTaskGetRunTimeStats((char *)&CPU_RunInfo2);
    vTaskDelay(1000); /* 延时 500 个 tick */
		 
#if INCLUDE_uxTaskGetStackHighWaterMark
        CPU_high_water = uxTaskGetStackHighWaterMark(NULL);
#endif
    }
}
void startTast(void)
{
    xTaskCreate((TaskFunction_t)start_task,          //任务函数
                (const char *)"start_task",          //任务名称
                (uint16_t)START_STK_SIZE,            //任务堆栈大小
                (void *)NULL,                        //传递给任务函数的参数
                (UBaseType_t)START_TASK_PRIO,        //任务优先级
                (TaskHandle_t *)&StartTask_Handler); //任务句柄
}

