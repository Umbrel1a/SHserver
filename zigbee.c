#include"zigbee.h"
struct sockaddr_in addr;
int Link_s(int sockfd){
	pthread_t t1,t2;
	s_init();
	
	while(1){
		printf("wait for a client\n");
		struct sockaddr_in caddr;//定义结构体变量，保存连接成功的客户端信息
		socklen_t len = sizeof(addr);//定义初始化len
		printf("11\n");
		int connfd = accept(sockfd,(struct sockaddr*)&caddr,&len);
		if(-1==connfd){
			perror("accept");
			return -1;
		}
		printf("12\n");
		printf("serial-ip:%s port:%u\n",inet_ntoa(caddr.sin_addr),ntohs(caddr.sin_port));


		if(pthread_create(&t1,NULL,mes_recv,(void*)&connfd)<0){
			printf("create faild\n");
			continue;
		}
		if(pthread_create(&t2,NULL,mes_send,(void*)&connfd)<0){
		printf("create faild\n");
		continue;
		}
		pthread_join(t1,NULL);
		pthread_join(t2,NULL);
	}

}

void* mes_recv(void* arg){

	int connfd=*(int*)arg;
	
	while(1){
		char buf[36]={0};	

		int count=0;
		
		while(count<36){
			//usleep(300);
			int ret = read(fd,buf+count,36-count);

			if(ret<0){
				perror("read");
			}
			else if(ret==0){
				printf("read end\n");
			}
			printf("read-ret:%d\n",ret);
			count+=ret;
		}
	
		if((buf[0]&0xFF)!=0xBB){
			continue;
		}
		
		int i;
		int temperature = 0;//温度 45位
		int humidity = 0;//湿度 67位
		int light = 0;//光照强度 20 21 22 23位
		char result[128] = {0};
		temperature = buf[5]+buf[4]*16;
 		humidity = buf[7]+buf[6]*16;
 		//printf("%.2x %.2x %.2x %.2x\n",buf[23],buf[22], buf[21],buf[20]);
 		//printf("%d %d %d %d\n",(int)(buf[23]<<24),(int)(buf[22]<<16),(int)(buf[21]<<8),(int)(buf[20]));
 		light = (int)((buf[23]<<24) +(buf[22]<<16) + (buf[21]<<8) + buf[20]);
		
		sprintf(result,"temperature:%d,humidity:%d,light:%d\n",temperature,humidity,light);
		printf("\nresult:%s\n",result);
		int length = 0;
		for(i =0;i<128;i++){
			if(result[i]!='\0'){
				length++;
			}
		}
		signal(SIGPIPE,SIG_IGN);
		if(write(connfd,result,length)<0){
		printf("server disconnect");
		break;
		}
	    }
		
	}

void* mes_send(void* arg){
	
	int connfd=*(int*)arg;
	/*if(fd<0){
	printf("asd\n");
		perror("open");
		return;
	}*/

	while(1){
		unsigned char buf[36]={0xdd,0x07,0x24,0x00,0x00};
		char cmd[36]={0};
		int ret = read(connfd,cmd,sizeof(cmd));
		if(ret<=0){
			printf("disconnect\n");
			break;
		}
		printf("%s\n",cmd);
		if(strstr(cmd,"light_open")!=NULL){
			buf[4]=0x00;
		}
		else if(strstr(cmd,"light_close")!=NULL){
			buf[4]=0x01;
		}
		else if(strstr(cmd,"voice_open")!=NULL){
			buf[4]=0x02;
		}
		else if(strstr(cmd,"voice_close")!=NULL){
			buf[4]=0x03;
		}
		else if(strstr(cmd,"fan_open")!=NULL){
			buf[4]=0x04;
		}
		else if(strstr(cmd,"fan_close")!=NULL){
			buf[4]=0x08;
		}
		else if(strstr(cmd,"shu_open")!=NULL){
			buf[4]=0x09;
		}
		else if(strstr(cmd,"shu_close")!=NULL){
			buf[4]=0x0a;
		}
		int i;
		for(i=0;i<5;++i)
		{
			printf("%.2x ",buf[i]);
		}
		printf("\n");
		if(write(fd,buf,sizeof(buf))<0){
			break;
		}
	}
}

void s_init(){
	fd = open("/dev/ttyUSB0",O_RDWR | O_NOCTTY);
	if(fd==-1){
		perror("serial->open");
		return;
	}
	struct termios options,old;
	tcgetattr(fd,&old);
	
	memset(&options,0,sizeof(options));
	options.c_cflag = B115200 | CS8 | CLOCAL | CREAD;
	options.c_iflag = IGNPAR;
	options.c_oflag = 0;
	
	options.c_lflag = 0;
	
	options.c_cc[VTIME]=40;
	options.c_cc[VMIN]=0;

	tcflush(fd,TCIFLUSH);
	tcsetattr(fd,TCSANOW,&options);
	return;

}
