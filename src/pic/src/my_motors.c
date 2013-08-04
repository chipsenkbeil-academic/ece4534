#include "my_motors.h"
#include "my_uart.h"



void setMotorSpeeds(char m1, char m2)
{
    unsigned char motor_commands[MSGLEN];
    if(m1>127 || m1 == 0)
        m1=64;
    if(m2>127 || m2 == 0)
        m2=64;
    motor_commands[0] = m1;
    motor_commands[1] = m2+128;
    uart_send(2, motor_commands);
}
