//
// Created by flycat_miaomiaomiao on 2022/11/14.
// flycat_miaomiaomiao is the smartest in the world
//

#include "server_info.h"
#include <iostream>
#include <cstring>

server_info::server_info() {
}

server_info::~server_info() {
}

//cpu function
int server_info::get_cpu_rate(struct CPUPACKED *old_cpu)  {
    FILE *fd;       // 定义打开文件的指针
    char buff[256]; // 定义个数组，用来存放从文件中读取CPU的信息
    struct CPUPACKED new_cpu;
    //std::string cpu_use = "";
    serverinfo.cpu_use="";

    fd = fopen("/proc/stat", "r");

    if (fd != NULL)
    {
        // 读取第一行的信息，cpu整体信息
        fgets(buff, sizeof(buff), fd);
        if (strstr(buff, "cpu") != NULL) // 返回与"cpu"在buff中的地址，如果没有，返回空指针
        {
            // 从字符串格式化输出
            sscanf(buff, "%s %u %u %u %u %u %u %u", &new_cpu.name, &new_cpu.user, &new_cpu.nice, &new_cpu.system, &new_cpu.idle, &new_cpu.lowait, &new_cpu.irq, &new_cpu.softirq);
            // cpu的占用率 = （当前时刻的任务占用cpu总时间-前一时刻的任务占用cpu总时间）/ （当前时刻 - 前一时刻的总时间）
            serverinfo.cpu_use = std::to_string(getCpuUse(old_cpu,&new_cpu ) * 100) + "%";
            *old_cpu = new_cpu;
        }
    }
    //std::cout << cpu_use1 << std::endl; // 打印cpu的占用率
    return 0;
}

double server_info::getCpuUse(CPUPACKED *o, CPUPACKED *n){
    unsigned long od, nd;
    od = (unsigned long)(o->user + o->nice + o->system + o->idle + o->lowait + o->irq + o->softirq); //第一次(用户+优先级+系统+空闲)的时间再赋给od
    nd = (unsigned long)(n->user + n->nice + n->system + n->idle + n->lowait + n->irq + n->softirq); //第二次(用户+优先级+系统+空闲)的时间再赋给od
    double sum = nd - od;
    double idle = n->idle - o->idle;
    return (sum - idle) / sum;
}

//memory

int server_info::get_memory_rate() {
    FILE *fd;       // 定义打开文件的指针
    char buff[256]; // 定义个数组，用来存放从文件中读取CPU的信息
    //MEM mem;
    fd = fopen("/proc/meminfo", "r");

    if (fd != NULL)
    {
        //一行
        fgets(buff, sizeof(buff), fd);
        //二行
        fgets(buff+strlen(buff), sizeof(buff), fd);
        //三行
        fgets(buff+strlen(buff), sizeof(buff), fd);
        //std::cout<<buff<<std::endl;

        if (strstr(buff, "MemTotal") != NULL) // 返回与"MemTotal"在buff中的地址，如果没有，返回空指针
        {
            // 从字符串格式化输出
            sscanf(buff,"%s %d %s\n%s %d %s\n%s %d %s\n", &serverinfo.mem.name,&serverinfo.mem.memtotal,&serverinfo.mem.name,&serverinfo.mem.name,&serverinfo.mem.memfree,&serverinfo.mem.name,&serverinfo.mem.name,&serverinfo.mem.memavailable,&serverinfo.mem.name);
            serverinfo.mem.memuserate = 100*(1-(double(serverinfo.mem.memavailable)/double(serverinfo.mem.memtotal)));
            /*std::cout<<mem.memtotal<< std::endl;
            std::cout<<mem.memfree<< std::endl;
            std::cout<<mem.memavailable<< std::endl;
            std::cout<<mem.memuserate<< std::endl;
            std::cout<<"-------------------------"<< std::endl;*/

        }
    }
    return 0;
}

//disk

int server_info::get_disk_rate() {
    int state;
    /*
    fsblkcnt_t block_size = 0;
    fsblkcnt_t block_count = 0;
    fsblkcnt_t total_size;
    fsblkcnt_t free_size;
    fsblkcnt_t used_size;
    fsblkcnt_t avail_size;
     */
    /*读取根目录，就是整个系统的大小*/
    state = statvfs(PATH,&vfs);   /*设置路径，查看不同文件目录的大小*/
    if(state < 0){
        printf("read statvfs error!!!\n");
    }
    /*获取一个block的大小*/
    serverinfo.disk.block_size = vfs.f_bsize;
    /*获取总容量*/
    serverinfo. disk.total_size = vfs.f_blocks * serverinfo.disk.block_size;
    /*获取可用容量*/
    serverinfo.disk.free_size = vfs.f_bfree * serverinfo.disk.block_size;
    /*获取使用容量*/
    serverinfo.disk.used_size = (vfs.f_blocks - vfs.f_bavail) * serverinfo.disk.block_size;
    /*获取有效容量*/
    serverinfo.disk.avail_size = vfs.f_bavail * serverinfo.disk.block_size;
/*
    printf(" total_size   = %0.2lf  GB\n",(double)disk.total_size / (GB) );
    printf(" free_size   = %0.2lf  GB\n",(double)disk.free_size / (GB) );
    printf(" used_size  = %0.2lf  GB\n",(double)disk.used_size / (GB) );
    printf(" avail_size  = %0.2lf  GB\n",(double)disk.avail_size / (GB) );
*/
    return 0;




}