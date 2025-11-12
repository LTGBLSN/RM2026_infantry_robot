//
// Created by 21481 on 2025/3/18.
//
#include "main.h"
#include "cmsis_os.h"
#include "can.h"
#include "dma.h"
#include "usart.h"
#include "gpio.h"

#include <stdio.h>
#include "board_LED.h"
#include "uart_printf.h"
#include "uart_sent.h"
#include "bsp_rc.h"
#include "remote_control.h"
#include "get_rc.h"
#include "bsp_can.h"
#include "CAN_receive.h"
#include "jy61p.h"
#include "auto_aim.h"


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart == &huart1)
    {
//        jy61p_Receive_Angle_Data(uart1_receive_data);//调用数据包处理函数,得到陀螺仪角度
//        jy61p_Receive_Speed_Data(uart1_receive_data);//调用数据包处理函数，得到陀螺仪角速度
        auto_aim_communication_data_parse(uart1_receive_data);//自瞄数据解析

        auto_aim_com_update_time = HAL_GetTick();


        HAL_UART_Receive_DMA(&huart1, &uart1_receive_data, 1);//继续进行中断接收



    }
}






