//
// Created by flycat_miaomiaomiao on 2022/11/7.
// flycat_miaomiaomiao is the smartest in the world
//

#ifndef WEB_SERVER_TCP_SOCKET_H
#include <iostream>
#define WEB_SERVER_TCP_SOCKET_H


class tcp_socket {
    public:

        tcp_socket();
        tcp_socket(int socket);
        ~tcp_socket();
        int c_to_host(std::string ip,unsigned short port);
        int send_msg(std::string msg);
        std::string receive_msg();

    private:

        int readn(char* buf, int size);// int readn(int fd, char* buf, int size);
        int writen(const char* msg, int size);// int writen(int fd, const char* msg, int size);

    private:
        int wfd;//we socket id ~cfd
};


#endif //WEB_SERVER_TCP_SOCKET_H
