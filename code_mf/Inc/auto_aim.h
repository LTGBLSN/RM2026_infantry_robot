//
// Created by 21481 on 2025/8/30.
//

#ifndef BUBING_RM2025_AUTO_AIM_H
#define BUBING_RM2025_AUTO_AIM_H

#include <stdbool.h>
#include "main.h"

//必须和上位机一模一样
struct ReceivePacket
{
    uint8_t header;         // 包头 0xA5
    uint8_t tracking : 1;   // 低1位是 tracking
    uint8_t id : 3;         // 接下来的3位是 id
    uint8_t armors_num : 3; // 再接下来的3位是 armors_num
    float x;                // 目标 x 坐标
    float y;                // 目标 y 坐标
    float z;                // 目标 z 坐标
    float yaw;              // 目标倾斜角度
    float vx;               // x 方向速度
    float vy;               // y 方向速度
    float vz;               // z 方向速度
    float v_yaw;            // 目标旋转角速度
    float r1;               // 装甲板半径1
    float r2;               // 装甲板半径2
    float dz;               // 上一块装甲板的 z 轴位置
    uint16_t checksum;      // CRC16 校验和 (小端)
} __attribute__((packed)); // 关键！防止编译器优化对齐，确保内存布局一致

struct SentPacket
{
    uint8_t header ;// 包头 0x5A
    uint8_t detect_color : 1  ; // 0: red, 1: blue要识别的颜色默认1      ？？这不对啊启动的时候识别的红色
    bool reset_tracker  : 1; //重置跟踪器，默认1
    uint8_t reserved : 6;
    float roll; // // 世界坐标系下云台当前的 roll
    float pitch; // 世界坐标系下云台当前的 pitch
    float yaw; // 世界坐标系下云台当前的 yaw
    float aim_x;//当前云台瞄准的位置用于发布可视化 Marker
    float aim_y;//当前云台瞄准的位置用于发布可视化 Marker
    float aim_z;//当前云台瞄准的位置用于发布可视化 Marker
    uint16_t checksum ;
} __attribute__((packed));


extern struct ReceivePacket auto_aim_rx_packet;

extern struct SentPacket auto_aim_tx_packet;


void auto_aim_communication_data_parse(uint8_t rx_data);

uint16_t crc16_Get_CRC16_Check_Sum(const uint8_t *pchMessage, uint32_t dwLength, uint16_t wCRC) ;



#endif //BUBING_RM2025_AUTO_AIM_H
