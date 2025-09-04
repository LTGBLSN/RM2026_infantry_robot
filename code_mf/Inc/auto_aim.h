//
// Created by 21481 on 2025/8/30.
//

#ifndef BUBING_RM2025_AUTO_AIM_H
#define BUBING_RM2025_AUTO_AIM_H

#include <stdbool.h>
#include "main.h"

//�������λ��һģһ��
struct ReceivePacket
{
    uint8_t header;         // ��ͷ 0xA5
    uint8_t tracking : 1;   // ��1λ�� tracking
    uint8_t id : 3;         // ��������3λ�� id
    uint8_t armors_num : 3; // �ٽ�������3λ�� armors_num
    float x;                // Ŀ�� x ����
    float y;                // Ŀ�� y ����
    float z;                // Ŀ�� z ����
    float yaw;              // Ŀ����б�Ƕ�
    float vx;               // x �����ٶ�
    float vy;               // y �����ٶ�
    float vz;               // z �����ٶ�
    float v_yaw;            // Ŀ����ת���ٶ�
    float r1;               // װ�װ�뾶1
    float r2;               // װ�װ�뾶2
    float dz;               // ��һ��װ�װ�� z ��λ��
    uint16_t checksum;      // CRC16 У��� (С��)
} __attribute__((packed)); // �ؼ�����ֹ�������Ż����룬ȷ���ڴ沼��һ��

struct SentPacket
{
    uint8_t header ;// ��ͷ 0x5A
    uint8_t detect_color : 1  ; // 0: red, 1: blueҪʶ�����ɫĬ��1      �����ⲻ�԰�������ʱ��ʶ��ĺ�ɫ
    bool reset_tracker  : 1; //���ø�������Ĭ��1
    uint8_t reserved : 6;
    float roll; // // ��������ϵ����̨��ǰ�� roll
    float pitch; // ��������ϵ����̨��ǰ�� pitch
    float yaw; // ��������ϵ����̨��ǰ�� yaw
    float aim_x;//��ǰ��̨��׼��λ�����ڷ������ӻ� Marker
    float aim_y;//��ǰ��̨��׼��λ�����ڷ������ӻ� Marker
    float aim_z;//��ǰ��̨��׼��λ�����ڷ������ӻ� Marker
    uint16_t checksum ;
} __attribute__((packed));


extern struct ReceivePacket auto_aim_rx_packet;

extern struct SentPacket auto_aim_tx_packet;


void auto_aim_communication_data_parse(uint8_t rx_data);

uint16_t crc16_Get_CRC16_Check_Sum(const uint8_t *pchMessage, uint32_t dwLength, uint16_t wCRC) ;



#endif //BUBING_RM2025_AUTO_AIM_H
