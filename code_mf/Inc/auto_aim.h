//
// Created by 21481 on 2025/8/30.
//

#ifndef BUBING_RM2025_AUTO_AIM_H
#define BUBING_RM2025_AUTO_AIM_H

#include <stdbool.h>
#include "main.h"


#define OUTPOST_ARMOR 0 //ǰ��վ
#define HERO_ARMOR 1 //Ӣ��
#define ENGINEER_ARMOR 2 //����
#define INFANTRY_3_ARMOR 3 //����3
#define INFANTRY_4_ARMOR 4 //����4
#define INFANTRY_5_ARMOR 5 //����5
#define GUARD_ARMOR 6 //�ڱ�
#define BASE_ARMOR 7 //����

#define OUTPOST_ARMOR_QUANTITY 3//ǰ��վ3��װ�װ�
#define INFANTRY_ARMOR_QUANTITY 4//��ͨ��4��װ�װ�

struct auto_aim_calculation_gimbal_target
{
    float yaw_angle ;
    float pitch_angle ;
    float if_shoot ;
    float vx ;
    float vy ;
    float vround ;
};


struct armor_posture//װ�װ�λ��
{
    float x ;
    float y ;
    float z ;
    float yaw ;
};



//�������λ��һģһ��
struct ReceivePacket
{
    uint8_t header;         // ��ͷ 0xA5
    uint8_t tracking : 1;   // ��1λ�� tracking ����ǰ�Ƿ�����Ŀ��
    uint8_t id : 3;         // ��������3λ�� id 0-outpost 6-guard 7-base
    uint8_t armors_num : 3; // �ٽ�������3λ�� armors_num 2-balance 3-outpost 4-normal
    float x;                // Ŀ�� x ����(��������ϵ)
    float y;                // Ŀ�� y ����(��������ϵ)
    float z;                // Ŀ�� z ����(��������ϵ)
    float yaw;              // Ŀ����б�Ƕ�(��������ϵ)
    float vx;               // x �����ٶ�(��������ϵ)
    float vy;               // y �����ٶ�(��������ϵ)
    float vz;               // z �����ٶ�(��������ϵ)
    float v_yaw;            // Ŀ����ת���ٶ�
    float r1;               // װ�װ�뾶1Ŀ������һ��װ�װ�������ĵİ뾶
    float r2;               // װ�װ�뾶2Ŀ����һ��װ�װ�������ĵİ뾶
    float dz;               // tracking �е�װ�װ����һ��װ�װ�� z ��λ��
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

void sent_data_update();

uint16_t crc16_Get_CRC16_Check_Sum(const uint8_t *pchMessage, uint32_t dwLength, uint16_t wCRC) ;



#endif //BUBING_RM2025_AUTO_AIM_H
