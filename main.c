#include"camThread.h"
#include"cam.h"
#include"camSer.h"

#include"zigbee.h"
void main()
{
	pthread_t camThr;
	pthread_create(&camThr,NULL,cam_start,NULL);
	
	int sockfd = server_init(9526);//设置端口,创建套接字
	Link_s(sockfd);//接受客户端连接
	while(1);
}
