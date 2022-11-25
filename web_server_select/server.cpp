//
// Created by flycat_miaomiaomiao on 2022/11/7.
// flycat_miaomiaomiao is the smartest in the world
//
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>

int readn(char* buf, int size,int wfd)
{
    int nread = 0;
    int left = size;
    char* p = buf;
    while (left > 0)
    {
        if ((nread = read(wfd, p, left)) > 0)
        {
            //printf("id is %d \n",nread);
            //printf("socket is %d \n",wfd);
            p += nread;
            left -= nread;
        }
        else if (nread == 0|nread == -1)//0 mean read the net files ending,-1 means read socket has been closed!
        {
            return -1;
        }
    }
    return size;
}


int main()
{
    // 1. 创建监听的fd
    int lfd = socket(AF_INET, SOCK_STREAM, 0);
    // 2. 绑定
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(10086);
    addr.sin_addr.s_addr = INADDR_ANY;
    bind(lfd, (struct sockaddr*)&addr, sizeof(addr));
    // 3. 设置监听
    listen(lfd, 128);
    // 将监听的fd的状态检测委托给内核检测
    int maxfd = lfd;
    // 初始化检测的读集合
    fd_set rdset;
    fd_set rdtemp;
    // 清零
    FD_ZERO(&rdset);
    // 将监听的lfd设置到检测的读集合中
    FD_SET(lfd, &rdset);
    // 通过select委托内核检测读集合中的文件描述符状态, 检测read缓冲区有没有数据
    // 如果有数据, select解除阻塞返回
    // 应该让内核持续检测
    while(1)
    {
        // 默认阻塞
        // rdset 中是委托内核检测的所有的文件描述符
        rdtemp = rdset;
        int num = select(maxfd+1, &rdtemp, NULL, NULL, NULL);
        // rdset中的数据被内核改写了, 只保留了发生变化的文件描述的标志位上的1, 没变化的改为0
        // 只要rdset中的fd对应的标志位为1 -> 缓冲区有数据了
        // 判断
        // 有没有新连接
        if(FD_ISSET(lfd, &rdtemp))
        {
            // 接受连接请求, 这个调用不阻塞
            struct sockaddr_in cliaddr;
            int cliLen = sizeof(cliaddr);
            int cfd = accept(lfd, (struct sockaddr*)&cliaddr, &cliLen);

            // 得到了有效的文件描述符
            // 通信的文件描述符添加到读集合
            // 在下一轮select检测的时候, 就能得到缓冲区的状态
            FD_SET(cfd, &rdset);
            // 重置最大的文件描述符
            maxfd = cfd > maxfd ? cfd : maxfd;
        }

        // 没有新连接, 通信
        for(int i=0; i<maxfd+1; ++i)
        {
            // 判断从监听的文件描述符之后到maxfd这个范围内的文件描述符是否读缓冲区有数据
            if(i != lfd && FD_ISSET(i, &rdtemp))
            {
                int len = 0;
                int read_head = readn((char*)&len, 4,i);
                if (read_head ==-1){
                    FD_CLR(i,&rdset);
                }
                len = ntohl(len);
                //printf(" data.size is : %d\n",len);
                // 根据读出的长度分配内存
                char* buf = new char[len + 1];
                int ret = readn(buf, len,i);
                if (ret ==-1){
                    FD_CLR(i,&rdset);
                }
                std::cout<<buf<<std::endl;
                delete[]buf;
            }
        }
    }

    return 0;
}

