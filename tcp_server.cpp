//
// Created by flycat_miaomiaomiao on 2022/11/7.
// flycat_miaomiaomiao is the smartest in the world
//

#include "tcp_server.h"
#include "tcp_socket.h"
#include <sys/socket.h>
#include <sys/unistd.h>
#include<arpa/inet.h>
#include <netinet/in.h>
#include <cstring>


tcp_server::tcp_server()
{
    l_fd = socket(AF_INET, SOCK_STREAM, 0);
}

tcp_server::~tcp_server()
{
    close(l_fd);
    printf("a connect socket has been close\n");
}


//error return -1
int tcp_server::setListen(unsigned short port)
{
    struct sockaddr_in saddr;
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(port);
    saddr.sin_addr.s_addr = INADDR_ANY;  // 0 = 0.0.0.0
    int ret = bind(l_fd, (struct sockaddr*)&saddr, sizeof(saddr));
    if (ret == -1)
    {
        perror("bind");
        return -1;
    }
    //printf("bind success ...........................\n");

    ret = listen(l_fd, 128);
    if (ret == -1)
    {
        perror("listen");
        return -1;
    }
    printf("server is listening  at : %s:%d \n",inet_ntoa(saddr.sin_addr),port);

    return ret;
}

tcp_socket* tcp_server::accept_c(struct sockaddr_in *add_msg) {
    if (add_msg == NULL)
    {
        return nullptr;
    }

    socklen_t addrlen = sizeof(struct sockaddr_in);
    int cfd = accept(l_fd, (struct sockaddr*)add_msg, &addrlen);//接受得套接字用于通信
    if (cfd == -1)
    {
        perror("accept");
        return nullptr;
    }
    //printf("connect with client success..............\n");
    return new tcp_socket(cfd);

}
