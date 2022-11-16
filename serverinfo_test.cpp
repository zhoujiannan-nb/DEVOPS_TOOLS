//
// Created by flycat_miaomiaomiao on 2022/11/15.
// flycat_miaomiaomiao is the smartest in the world
//
#include "server_info.h"
#include <unistd.h>
#include <iostream>


int main(){
    server_info ss;
    struct CPUPACKED Cpupacked;
    struct CPUPACKED * cpupacked=&Cpupacked;
    while (1){

        ss.get_cpu_rate(cpupacked);
        std::cout<<ss.serverinfo.cpu_use<<std::endl;
        std::cout<<"\n-----------------------"<< std::endl;

        ss.get_memory_rate();
        std::cout<<ss.serverinfo.mem.memtotal<< std::endl;
        std::cout<<ss.serverinfo.mem.memfree<< std::endl;
        std::cout<<ss.serverinfo.mem.memavailable<< std::endl;
        std::cout<<ss.serverinfo.mem.memuserate<< std::endl;
        std::cout<<"-------------------------"<< std::endl;

        ss.get_disk_rate();
        printf(" total_size   = %0.2lf  GB\n",(double)ss.serverinfo.disk.total_size / (GB) );
        printf(" free_size   = %0.2lf  GB\n",(double)ss.serverinfo.disk.free_size / (GB) );
        printf(" used_size  = %0.2lf  GB\n",(double)ss.serverinfo.disk.used_size / (GB) );
        printf(" avail_size  = %0.2lf  GB\n",(double)ss.serverinfo.disk.avail_size / (GB) );

        std::cout<<"***************************"<< std::endl;
        sleep(1);
    }
    return 0;
}