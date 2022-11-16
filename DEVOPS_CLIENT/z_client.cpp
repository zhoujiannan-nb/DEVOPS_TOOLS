//
// Created by flycat_miaomiaomiao on 2022/11/8.
// flycat_miaomiaomiao is the smartest in the world
//

#include "tcp_socket.h"
#include "server_info.h"
#include <unistd.h>

int main(){
    //create for net connect
    tcp_socket t_s;
    //create for read server info
    server_info ss;
    struct CPUPACKED Cpupacked;
    struct CPUPACKED * cpupacked=&Cpupacked;

    int c_s = t_s.c_to_host("127.0.0.1",10086);
    if (c_s==-1){
        perror("connect\n");
        return -1;
    }

    printf("connect success\n");
    for (int i = 0; i < 30; ++i) {
        ss.get_cpu_rate(cpupacked);
        ss.get_memory_rate();
        ss.get_disk_rate();
        std::string sendmsg="cpu use: "+ss.serverinfo.cpu_use+"\n"+
                "memtotal: "+std::to_string(ss.serverinfo.mem.memtotal)+"\n"+
                "memfree: "+std::to_string(ss.serverinfo.mem.memfree)+"\n"+
                "memavailable: "+std::to_string(ss.serverinfo.mem.memavailable)+"\n"+
                "memuserate: "+std::to_string(ss.serverinfo.mem.memuserate)+"\n"+
                "total_size: "+std::to_string((double)ss.serverinfo.disk.total_size / (GB))+"\n"+
                "free_size: "+std::to_string((double)ss.serverinfo.disk.free_size / (GB))+"\n"+
                "used_size: "+std::to_string((double)ss.serverinfo.disk.used_size / (GB))+"\n"+
                "avail_size: "+std::to_string((double)ss.serverinfo.disk.avail_size / (GB))+"\n"
                ;
        int s_s = t_s.send_msg(sendmsg);
        //int s_s = t_s.send_msg("bye");
        if (s_s==-1){
            perror("send msg is fail\n");
            return -1;
        }
        sleep(1);
    }
    sleep(1);
    int s_s = t_s.send_msg("bye");

    printf("send success bye bye~\n");
    return 0;
}