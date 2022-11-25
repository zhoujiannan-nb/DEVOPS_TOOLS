//
// Created by flycat_miaomiaomiao on 2022/11/7.
// flycat_miaomiaomiao is the smartest in the world
//
#include "tcp_socket.h"
#include "tcp_server.h"
#include<arpa/inet.h>
#include "./threads_tools/Thread_pools.h"
#include "./threads_tools/Thread_pools.cpp"


struct socket_info
{
    tcp_server* s;
    tcp_socket* tcp;
    struct sockaddr_in add_msg;
};

void working(void* arg)
{
    struct socket_info* pinfo = static_cast<struct socket_info*>(arg);
    // 连接建立成功, 打印客户端的IP和端口信息
    char ip[32];
    printf("  client IP: %s, port: %d\n",
           inet_ntop(AF_INET, &pinfo->add_msg.sin_addr.s_addr, ip, sizeof(ip)),
           ntohs(pinfo->add_msg.sin_port));

    // 5. 通信
    int count = 1;
    while (1)
    {
        printf("accept %d :\n",count++);
        std::string msg = pinfo->tcp->receive_msg();
        if (!msg.empty())
        {
             printf("%s\n",msg.data());
             if(msg=="bye"){
                 break;
             }
        }
        else{
            break;
        }
    }
    printf("收到结束信号，客户端下线\n");
    //delete pinfo->tcp;
    //delete pinfo;
    printf("线程执行完毕\n");
}



int main(){
    tcp_server server;
    server.setListen(10086);
    //开个线程池
    Thread_pools<socket_info> pools(30,50);
    while (1)
    {
        socket_info *info = new socket_info;
        tcp_socket* tcp = server.accept_c(&info->add_msg);
        if(tcp == nullptr){
            printf("this connect fail retry....\n");
            //continue;
        }
        //pthread_t tid;
        info->s = &server;
        info->tcp = tcp;
        pools.add_task(Task<socket_info>(working,info));
    }

    return 0;
}