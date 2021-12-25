/**********************************************************************************************************
 * @文件     iwdg.c
 * @说明     IWDG初始化
 * @版本  	 V1.0
 * @作者     戴军
 * @日期     2021.12
**********************************************************************************************************/

#include "main.h"
/*
 *        独立看门狗使用LSI作为时钟。
 *        MCU内部的LSI 的频率一般在 30~60KHZ 之间变化，根据温度和工作场合会有一定的漂移，我
 *        们一般取 40KHZ，所以独立看门狗的定时时间并一定非常精确，只适用于对时间精度
 *        要求比较低的场合。
 *
 * rlv:重装载寄存器的值，取值范围为：0-0XFFF
 * 函数调用举例：
 * IWDG_Config(IWDG_Prescaler_64 ,625);  // IWDG 1s 超时溢出 
 *                        （64/40）*625 = 1s
 */
void IWDG_Config(uint8_t prv ,uint16_t rlv)
{    
    
    // 设置预分频器值
		IWDG->KR=0X5555;
    IWDG->PR=prv;
    
    // 设置重装载寄存器值
    IWDG->KR=0X5555;
    IWDG->RLR=rlv;
    
    // 把重装载寄存器的值放到计数器中
    IWDG->KR=0XAAAA;
    
    // 使能 IWDG
    IWDG->KR=0XCCCC;  
}

// 喂狗
void IWDG_Feed(void)
{
    // 把重装载寄存器的值放到计数器中，喂狗，防止IWDG复位
    // 当计数器的值减到0的时候会产生系统复位
    IWDG->KR=0XAAAA;
}

