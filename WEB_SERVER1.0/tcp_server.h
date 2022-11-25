//
// Created by flycat_miaomiaomiao on 2022/11/7.
// flycat_miaomiaomiao is the smartest in the world
//

#ifndef WEB_SERVER_TCP_SERVER_H
#define WEB_SERVER_TCP_SERVER_H
#include "tcp_socket.h"


class tcp_server {
    public:
        tcp_server();
        ~tcp_server();
        int setListen(unsigned short port);
        tcp_socket* accept_c(struct sockaddr_in* add_msg = nullptr);

    private:
        int l_fd;	// 监听的套接字
};


#endif //WEB_SERVER_TCP_SERVER_H
