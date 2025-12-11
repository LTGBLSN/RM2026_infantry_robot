#include <stdbool.h>
#include <string.h>
#include <math.h>
#include "main.h"
#include "auto_aim.h"
#include "refree_task.h"
#include "usart.h"


struct ReceivePacket auto_aim_rx_packet;//上位机发过来的
struct SentPacket auto_aim_tx_packet;//下位机要发出去的
static uint8_t auto_aim_rx_buffer[sizeof(struct ReceivePacket)];  // 自瞄数据接收缓冲区，大小由结构体决定
static uint8_t tx_buffer[sizeof(struct SentPacket)];    // 自瞄数据发送缓冲区，大小由结构体决定
static volatile uint8_t auto_aim_rx_state = 0;  // 接收状态标志位
static uint8_t auto_aim_rx_index = 0;  // 接收数组索引

// 数据解析函数，每次接收到一个字节时调用
void auto_aim_communication_data_parse(uint8_t rx_data)
{
    switch (auto_aim_rx_state)
    {
        case RX_STATE_WAIT_HEADER:
            // 等待包头 0xff
            if (rx_data == 0xFF)
            {
                auto_aim_rx_buffer[0] = rx_data;
                auto_aim_rx_index = 1;
                auto_aim_rx_state = RX_STATE_RECEIVING; // 进入接收数据状态
            }
            break;

        case RX_STATE_RECEIVING:
            // 将数据存入缓冲区
            auto_aim_rx_buffer[auto_aim_rx_index++] = rx_data;

            // 检查是否接收完一整包数据
            if (auto_aim_rx_index == sizeof(struct ReceivePacket))
            {
                // 数据已接收满，进行校验
                if (auto_aim_rx_buffer[sizeof(struct ReceivePacket) - 1] == 0x0D)
                {
                    // 帧尾校验成功！
                    // 将缓冲区数据安全地拷贝到数据包结构体中
                    // memcpy 是处理内存拷贝的标准方法，可以正确处理 float 等多字节数据
                    memcpy(&auto_aim_rx_packet, auto_aim_rx_buffer, sizeof(struct ReceivePacket));

                }
                else
                {
                    // 帧尾校验失败，这包数据是错误的
                    // 丢弃这包数据，回到等待包头状态
                    // 可以在这里添加错误计数等逻辑
                }

                // 无论成功与否，都重置状态，准备接收下一包
                auto_aim_rx_state = RX_STATE_WAIT_HEADER;
                auto_aim_rx_index = 0;
            }
            break;

        default:
            // 异常状态，重置状态机
            auto_aim_rx_state = RX_STATE_WAIT_HEADER;
            auto_aim_rx_index = 0;
            break;
    }
}

//发送
void sent_data_update()
{


    auto_aim_tx_packet.header = 0xff;//包头
    auto_aim_tx_packet.mod = RED;//敌方颜色
    auto_aim_tx_packet.roll = roll_angle_from_bmi088;
    auto_aim_tx_packet.pitch = -pitch_angle_from_bmi088;
    auto_aim_tx_packet.yaw = yaw_angle_from_bmi088;
    auto_aim_tx_packet.editor = 0x0d;//包尾



    memcpy(tx_buffer, &auto_aim_tx_packet, sizeof(auto_aim_tx_packet)); //拷贝到缓冲区

    HAL_UART_Transmit_IT(&huart1, tx_buffer, sizeof(auto_aim_tx_packet));

}
