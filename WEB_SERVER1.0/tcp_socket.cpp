//
// Created by flycat_miaomiaomiao on 2022/11/7.
// flycat_miaomiaomiao is the smartest in the world
//

#include "tcp_socket.h"
#include <sys/socket.h>
#include <sys/unistd.h>
#include<arpa/inet.h>
#include <netinet/in.h>
#include <cstring>
//make and delete
tcp_socket::tcp_socket() {
    wfd = socket(AF_INET,SOCK_STREAM,0);
}
tcp_socket::tcp_socket(int socket) {
    wfd = socket;
}
tcp_socket::~tcp_socket() {
    if (wfd>0){
        close(wfd);
        printf("connect socket has been closed\n");
    }
}

//back -1 means error
int tcp_socket::c_to_host(std::string ip, unsigned short port) {
    struct sockaddr_in add_msg;
    add_msg.sin_family = AF_INET;
    add_msg.sin_port = htons(port);
    inet_pton(AF_INET,ip.data(),&add_msg.sin_addr.s_addr);//copy ip.data() to s_addr as  internet methods
    int res = connect(wfd,(struct sockaddr*)&add_msg, sizeof(add_msg));
    if (res == -1){
        perror("connect");
        return -1;
    }
    printf("connect success\n");
    return res;
}



//send msg
int tcp_socket::send_msg(std::string msg) {
    // 申请内存空间: 数据长度 + 包头4字节(存储数据长度)
    char* data = new char[msg.size() + 4];
    int bigLen = htonl(msg.size());
    memcpy(data, &bigLen, 4);
    memcpy(data + 4, msg.data(), msg.size());
    // 发送数据
    int ret = writen(data, msg.size() + 4);
    delete[]data;
    return ret;
}
int tcp_socket::writen(const char* msg, int size)
{
    int left = size;
    int nwrite = 0;
    const char* p = msg;

    while (left > 0)
    {
        if ((nwrite = write(wfd, msg, left)) > 0)
        {
            p += nwrite;
            left -= nwrite;
        }
        else if (nwrite == -1)
        {
            return -1;
        }
    }
    return size;
}


//receive_msg~
std::string tcp_socket::receive_msg()
{
    // 接收数据
    // 1. 读数据头
    int len = 0;
    int read_head = readn((char*)&len, 4);
    len = ntohl(len);
    //printf(" data.size is : %d\n",len);
    // 根据读出的长度分配内存
    char* buf = new char[len + 1];
    int ret = readn(buf, len);
    if (ret != len)
    {
        return std::string();
    }
    buf[len] = '\0';
    std::string retStr(buf);
    delete[]buf;

    return retStr;
}
int tcp_socket::readn(char* buf, int size)
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


