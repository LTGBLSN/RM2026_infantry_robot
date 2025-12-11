//
// Created by 21481 on 2025/8/30.
//

#ifndef BUBING_RM2025_AUTO_AIM_H
#define BUBING_RM2025_AUTO_AIM_H

#include <stdbool.h>
#include "main.h"


// 定义状态机状态
#define RX_STATE_WAIT_HEADER 0
#define RX_STATE_RECEIVING   1
#define RX_STATE_READY       2 // 表示一包数据已成功接收并解析

// 必须和上位机协议一模一样
struct ReceivePacket
{
    uint8_t header;         // 包头 0xff
    uint8_t fite_advance;   // 开火建议 (0x01是，0x00不是)
    float pitch;            // 目标pitch角度 (单位:角度)
    float yaw;              // 目标yaw角度 (单位:角度)
    float distance;         // 目标距离 (单位:m)
    uint8_t reserved;       // 保留/无用字节
    uint8_t tail;           // 包尾 0x0d
} __attribute__((packed)); // 关键！防止编译器优化对齐，确保内存布局一致

struct SentPacket
{
    uint8_t header ;// 包头 0xff
    uint8_t mod ; // 敌方颜色0x01 BLUE   |  0x00 RED
    float roll; // // 世界坐标系下云台当前的 roll
    float pitch; // 世界坐标系下云台当前的 pitch
    float yaw; // 世界坐标系下云台当前的 yaw
    uint8_t none ;//无用
    uint8_t editor ;//包尾 0x0d
} __attribute__((packed));


extern struct ReceivePacket auto_aim_rx_packet;
extern struct SentPacket auto_aim_tx_packet;



void auto_aim_communication_data_parse(uint8_t rx_data);

void sent_data_update();

uint16_t crc16_Get_CRC16_Check_Sum(const uint8_t *pchMessage, uint32_t dwLength, uint16_t wCRC) ;



#endif //BUBING_RM2025_AUTO_AIM_H
