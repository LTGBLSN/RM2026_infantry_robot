# RMvision下位机接收



## 下位机->上位机



### 结构体

```c
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
```

### 发送函数：

```c
uint8_t tx_buffer[sizeof(auto_aim_tx_packet)];
struct SentPacket auto_aim_tx_packet;//下位机要发出去的


void sent_data_update()
{


    auto_aim_tx_packet.header = 0xff;//包头
    auto_aim_tx_packet.detect_color = RED;//要识别的颜色待验证
    auto_aim_tx_packet.reset_tracker = reset_tracker;//重置跟踪器
    auto_aim_tx_packet.reserved = 0; // 保留位，不知道干啥的
    auto_aim_tx_packet.roll = roll_radian_from_bmi088;
    auto_aim_tx_packet.pitch = pitch_radian_from_bmi088;
    auto_aim_tx_packet.yaw = yaw_radian_from_bmi088;
    auto_aim_tx_packet.aim_x = aim_x;
    auto_aim_tx_packet.aim_y = aim_y;
    auto_aim_tx_packet.aim_z = aim_z;


    memcpy(tx_buffer, &auto_aim_tx_packet, sizeof(auto_aim_tx_packet)); //拷贝到缓冲区

    uint16_t calculated_crc = crc16_Get_CRC16_Check_Sum(tx_buffer,sizeof(auto_aim_tx_packet) - 2,0xFFFF);

    tx_buffer[sizeof(auto_aim_tx_packet) - 2] = calculated_crc & 0xFF;       // CRC低字节
    tx_buffer[sizeof(auto_aim_tx_packet) - 1] = (calculated_crc >> 8) & 0xFF; // CRC高字节

    HAL_UART_Transmit_IT(&huart1, tx_buffer, sizeof(auto_aim_tx_packet)); // 100ms超时

}
```



### crc校验

```c
// CRC16 查表法使用的表
const uint16_t W_CRC_TABLE[256] = {
        0x0000, 0x1189, 0x2312, 0x329b, 0x4624, 0x57ad, 0x6536, 0x74bf, 0x8c48, 0x9dc1, 0xaf5a, 0xbed3,
        0xca6c, 0xdbe5, 0xe97e, 0xf8f7, 0x1081, 0x0108, 0x3393, 0x221a, 0x56a5, 0x472c, 0x75b7, 0x643e,
        0x9cc9, 0x8d40, 0xbfdb, 0xae52, 0xdaed, 0xcb64, 0xf9ff, 0xe876, 0x2102, 0x308b, 0x0210, 0x1399,
        0x6726, 0x76af, 0x4434, 0x55bd, 0xad4a, 0xbcc3, 0x8e58, 0x9fd1, 0xeb6e, 0xfae7, 0xc87c, 0xd9f5,
        0x3183, 0x200a, 0x1291, 0x0318, 0x77a7, 0x662e, 0x54b5, 0x453c, 0xbdcb, 0xac42, 0x9ed9, 0x8f50,
        0xfbef, 0xea66, 0xd8fd, 0xc974, 0x4204, 0x538d, 0x6116, 0x709f, 0x0420, 0x15a9, 0x2732, 0x36bb,
        0xce4c, 0xdfc5, 0xed5e, 0xfcd7, 0x8868, 0x99e1, 0xab7a, 0xbaf3, 0x5285, 0x430c, 0x7197, 0x601e,
        0x14a1, 0x0528, 0x37b3, 0x263a, 0xdecd, 0xcf44, 0xfddf, 0xec56, 0x98e9, 0x8960, 0xbbfb, 0xaa72,
        0x6306, 0x728f, 0x4014, 0x519d, 0x2522, 0x34ab, 0x0630, 0x17b9, 0xef4e, 0xfec7, 0xcc5c, 0xddd5,
        0xa96a, 0xb8e3, 0x8a78, 0x9bf1, 0x7387, 0x620e, 0x5095, 0x411c, 0x35a3, 0x242a, 0x16b1, 0x0738,
        0xffcf, 0xee46, 0xdcdd, 0xcd54, 0xb9eb, 0xa862, 0x9af9, 0x8b70, 0x8408, 0x9581, 0xa71a, 0xb693,
        0xc22c, 0xd3a5, 0xe13e, 0xf0b7, 0x0840, 0x19c9, 0x2b52, 0x3adb, 0x4e64, 0x5fed, 0x6d76, 0x7cff,
        0x9489, 0x8500, 0xb79b, 0xa612, 0xd2ad, 0xc324, 0xf1bf, 0xe036, 0x18c1, 0x0948, 0x3bd3, 0x2a5a,
        0x5ee5, 0x4f6c, 0x7df7, 0x6c7e, 0xa50a, 0xb483, 0x8618, 0x9791, 0xe32e, 0xf2a7, 0xc03c, 0xd1b5,
        0x2942, 0x38cb, 0x0a50, 0x1bd9, 0x6f66, 0x7eef, 0x4c74, 0x5dfd, 0xb58b, 0xa402, 0x9699, 0x8710,
        0xf3af, 0xe226, 0xd0bd, 0xc134, 0x39c3, 0x284a, 0x1ad1, 0x0b58, 0x7fe7, 0x6e6e, 0x5cf5, 0x4d7c,
        0xc60c, 0xd785, 0xe51e, 0xf497, 0x8028, 0x91a1, 0xa33a, 0xb2b3, 0x4a44, 0x5bcd, 0x6956, 0x78df,
        0x0c60, 0x1de9, 0x2f72, 0x3efb, 0xd68d, 0xc704, 0xf59f, 0xe416, 0x90a9, 0x8120, 0xb3bb, 0xa232,
        0x5ac5, 0x4b4c, 0x79d7, 0x685e, 0x1ce1, 0x0d68, 0x3ff3, 0x2e7a, 0xe70e, 0xf687, 0xc41c, 0xd595,
        0xa12a, 0xb0a3, 0x8238, 0x93b1, 0x6b46, 0x7acf, 0x4854, 0x59dd, 0x2d62, 0x3ceb, 0x0e70, 0x1ff9,
        0xf78f, 0xe606, 0xd49d, 0xc514, 0xb1ab, 0xa022, 0x92b9, 0x8330, 0x7bc7, 0x6a4e, 0x58d5, 0x495c,
        0x3de3, 0x2c6a, 0x1ef1, 0x0f78
};

/**
  * @brief  CRC16 Caculation function (移植自上位机)
  * @param  pchMessage: 数据缓冲区指针
  * @param  dwLength: 数据长度
  * @param  wCRC: CRC16 初始值 (默认 0xFFFF)
  * @retval 计算出的CRC16校验和
  */
uint16_t crc16_Get_CRC16_Check_Sum(const uint8_t *pchMessage, uint32_t dwLength, uint16_t wCRC)
{
    uint8_t ch_data;

    if (pchMessage == NULL) return 0xFFFF;
    while (dwLength--) {
        ch_data = *pchMessage++;
        wCRC = ((uint16_t)(wCRC) >> 8) ^ W_CRC_TABLE[((uint16_t)(wCRC) ^ (uint16_t)(ch_data)) & 0x00ff];
    }

    return wCRC;
}
```





## 上位机->下位机

### 结构体

```c
//必须和上位机一模一样
struct ReceivePacket
{
    uint8_t header;         // 包头 0xA5
    uint8_t tracking : 1;   // 低1位是 tracking 代表当前是否锁定目标
    uint8_t id : 3;         // 接下来的3位是 id 0-outpost 6-guard 7-base
    uint8_t armors_num : 3; // 再接下来的3位是 armors_num 2-balance 3-outpost 4-normal
    float x;                // 目标 x 坐标(世界坐标系)
    float y;                // 目标 y 坐标(世界坐标系)
    float z;                // 目标 z 坐标(世界坐标系)
    float yaw;              // 目标倾斜角度(世界坐标系)
    float vx;               // x 方向速度(世界坐标系)
    float vy;               // y 方向速度(世界坐标系)
    float vz;               // z 方向速度(世界坐标系)
    float v_yaw;            // 目标旋转角速度
    float r1;               // 装甲板半径1目标其中一组装甲板相对中心的半径
    float r2;               // 装甲板半径2目标另一组装甲板相对中心的半径
    float dz;               // tracking 中的装甲板的上一块装甲板的 z 轴位置
    uint16_t checksum;      // CRC16 校验和 (小端)
} __attribute__((packed)); // 关键！防止编译器优化对齐，确保内存布局一致
```

### 接收函数

```c
struct ReceivePacket auto_aim_rx_packet;//上位机发过来的
static uint8_t auto_aim_rx_buffer[48];  // 自瞄数据接收缓冲区
static volatile uint8_t auto_aim_rx_state = 0;  // 接收状态标志位
static uint8_t auto_aim_rx_index = 0;  // 接收数组索引

//数据解析
void auto_aim_communication_data_parse(uint8_t rx_data)
{
    uint16_t calculated_checksum = 0;//计算CRC中间变量

    if (auto_aim_rx_state == 0)  // 等待包头
    {
        if (rx_data == 0xA5)  // 收到包头
        {
            auto_aim_rx_index = 0 ;
            auto_aim_rx_buffer[auto_aim_rx_index] = rx_data;
            auto_aim_rx_state = 1;
            auto_aim_rx_index = 1;
        }
    }
    else if(auto_aim_rx_state == 1)//包头检验过，存储数据
    {

        auto_aim_rx_buffer[auto_aim_rx_index++] = rx_data;//一直存到包满

        if(auto_aim_rx_index == 48)
        {
            //CRC校验，计算的是除最后校验数据之外的,不能传入整个包
            calculated_checksum = crc16_Get_CRC16_Check_Sum(auto_aim_rx_buffer, 46, 0xFFFF);

            //提取末尾一并发送过来的校验和，小端格式
            uint16_t received_checksum = (auto_aim_rx_buffer[46] | (auto_aim_rx_buffer[47] << 8));

            //判断计算的和发送来的是否相同，不相同说明包中间有错数据
            if (calculated_checksum == received_checksum)
            {
                //和发送方结构体一模一样，且确保内存布局一致（结构体后面有取消编译器优化对齐）
                memcpy(&auto_aim_rx_packet, auto_aim_rx_buffer, sizeof(auto_aim_rx_packet));

                //数据接收完成
            }
             else {
                 // 此处为校验失败，一般情况不会进入，但可以作为检查通讯链路是否稳定，添加日志等
             }

            //重置状态
            auto_aim_rx_state = 0; //等待包头
            auto_aim_rx_index = 0; //重置索引
        }
    }

    //自瞄解算
    //2025.9.22暂时仅取中心点，仅解算yaw
    infantry_auto_aim_target.yaw_angle = (0.0f + (float)atan2f(auto_aim_rx_packet.y, auto_aim_rx_packet.x) * (float )( 180.0f / M_PI ) );
//    auto_aim_rx_packet.x





}
```