#include "can_app.h"


/* 车速消息发送 */
  void SendVehicleSpeed(uint8_t speed_kph)
  {
    CAN_Message_t msg;
    msg.id = 0x100;           // 车速消息ID
    msg.format = 0;           // 标准帧
    msg.type = 0;             // 数据帧
    msg.len = 2;              // 2字节数据
    // 车速数据 (0.1 km/h分辨率)
    msg.data[0] = (speed_kph *10) >> 8; 
    msg.data[1] = (speed_kph *10) & 0xFF;  
    if (CAN_Transmit(&msg) == HAL_OK) 
    {
    printf("车速消息发送成功: %d km/h\r\n", speed_kph);
    }
  }


/* 发动机转速消息发送 */
void SendEngineRPM(uint16_t rpm)
{
  CAN_Message_t msg;
  msg.id = 0x200;
  msg.format = 0;
  msg.type = 0;
  msg.len = 2;
  msg.data[0] = rpm >> 8;
  msg.data[1] = rpm & 0xFF;
  CAN_Transmit(&msg);
}

/* 多参数消息发送示例 */
void SendVehicleStatus(void)
{
  CAN_Message_t msg;
  msg.id = 0x300;
  msg.format = 0;
  msg.type = 0;
  msg.len = 8;
  // 字节0-1: 车速 (0.01 km/h)
  uint16_t speed = 8550;  // 85.50 km/h
  msg.data[0] = speed >> 8;
  msg.data[1] = speed & 0xFF;
  // 字节2-3: 发动机转速 (rpm)
  uint16_t rpm = 2500;
  msg.data[2] = rpm >> 8;
  msg.data[3] = rpm & 0xFF;
  // 字节4: 油门踏板位置 (0-100%)
  msg.data[4] = 45;
  // 字节5: 刹车踏板位置 (0-100%)
  msg.data[5] = 0;
  // 字节6: 状态位
  //   bit0: 发动机状态 (0=关闭, 1=运行)
  //   bit1: 车门状态 (0=关闭, 1=打开)
  //   bit2: 灯光状态
  msg.data[6] = 0x01;  // 发动机运行
  // 字节7: 齿轮位置
  //   0=P, 1=R, 2=N, 3=D
  msg.data[7] = 3;  // D档
  CAN_Transmit(&msg);
}


/* 消息接收处理 */
void ProcessCANMessages(void)
{
  CAN_Message_t msg;
  while (CAN_Receive(&msg))
  {
    printf("Process ID = 0x%03X\r\n", msg.id);
    switch (msg.id) 
    {
        case 0x100:  // 车速消息
        {
        uint16_t speed = (msg.data[0] << 8) | msg.data[1];
        printf("收到车速: %d.%d km/h\r\n", speed/10, speed%10);
        printf("yes100\n");
        break;
        }
        case 0x200:  // 转速消息
        {
        uint16_t rpm = (msg.data[0] << 8) | msg.data[1];
        printf("收到转速: %d rpm\r\n", rpm);
        break;
        }
        case 0x300:  // 综合状态
        {
        uint16_t speed = (msg.data[0] << 8) | msg.data[1];
        uint16_t rpm = (msg.data[2] << 8) | msg.data[3];
        uint8_t throttle = msg.data[4];
        uint8_t brake = msg.data[5];
        uint8_t status = msg.data[6];
        uint8_t gear = msg.data[7];
        printf("车辆状态:\r\n");
        printf("  车速: %d.%02d km/h\r\n", speed/100, speed%100);
        printf("  转速: %d rpm\r\n", rpm);
        printf("  油门: %d%%\r\n", throttle);
        printf("  刹车: %d%%\r\n", brake);
        printf("  发动机: %s\r\n", (status & 0x01) ? "运行" : "停止");
        if (gear <= 3)
        {
          printf("  档位: %c\r\n", "PRND"[gear]);
        }
        break;
        }
        default:
        printf("未知消息ID: 0x%X\r\n", msg.id);
        break;
    }
  }
} 








