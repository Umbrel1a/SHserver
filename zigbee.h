#ifndef ZIGBEE_H
#define ZIGBEE_H


#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include<termios.h>
#include<signal.h>

void* mes_recv(void* arg);//从串口接受数据，再转发
void* mes_send(void* arg);//从QT接受指令，再发送
void s_init();//方便初始化串口

int fd;


#endif
