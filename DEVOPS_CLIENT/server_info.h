//
// Created by flycat_miaomiaomiao on 2022/11/14.
// flycat_miaomiaomiao is the smartest in the world
//

#ifndef LINUX_MSG_SERVER_INFO_H
#include <sys/statvfs.h>
#include <iostream>
#define LINUX_MSG_SERVER_INFO_H
#define SYSTEM_BITS  64
#define PATH "/"
#define KB 1024
#define MB 1024*1024
#define GB  1024*1024*1024

//cpu info
struct CPUPACKED
{
    char name[20];       //定义一个char类型的数组名name有20个元素
    unsigned int user;   //定义一个无符号的int类型的user
    unsigned int nice;   //定义一个无符号的int类型的nice
    unsigned int system; //定义一个无符号的int类型的system
    unsigned int idle;   //定义一个无符号的int类型的idle
    unsigned int lowait;
    unsigned int irq;
    unsigned int softirq;
};
//memory info
typedef struct MEMORYPACKED
{
    char name[20];         //没啥用，用来丢垃圾的
    unsigned int memtotal;   //总内存
    unsigned int memfree; //系统层面的空闲内存
    unsigned int memavailable;   //应用层面的可以内存
    double memuserate;   //应用层面的可以内存
}MEM;

typedef struct DISKINFO
{
    fsblkcnt_t block_size = 0;
    fsblkcnt_t block_count = 0;
    fsblkcnt_t total_size;
    fsblkcnt_t free_size;
    fsblkcnt_t used_size;
    fsblkcnt_t avail_size;
}DISK;


struct SERVERINFO{
    std::string cpu_use;
    struct MEMORYPACKED mem;
    struct DISKINFO disk ;
};

class server_info {
    public:
        struct statvfs vfs;
        /*
        std::string cpu_use;
        struct MEMORYPACKED mem;
        struct DISKINFO disk;
        */
        struct SERVERINFO serverinfo;


    public:
        server_info();
        ~server_info();
        int get_cpu_rate(struct CPUPACKED *new_cpu);
        int get_memory_rate();
        int get_disk_rate();

    private:
        double getCpuUse(CPUPACKED *o, CPUPACKED *n);


};


#endif //LINUX_MSG_SERVER_INFO_H
