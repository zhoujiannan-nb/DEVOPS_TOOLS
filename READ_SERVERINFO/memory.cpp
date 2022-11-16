//
// Created by flycat_miaomiaomiao on 2022/11/13.
// flycat_miaomiaomiao is the smartest in the world
//

#include <cstring>
#include <iostream>
#include <stdio.h>
#include <unistd.h>

// 定义一个cpu occupy的结构体，用来存放CPU的信息
typedef struct MEMORYPACKED
{
    char name[20];         //没啥用，用来丢垃圾的
    unsigned int memtotal;   //总内存
    unsigned int memfree; //系统层面的空闲内存
    unsigned int memavailable;   //应用层面的可以内存
    double memuserate;   //应用层面的可以内存
}MEM;

// 该函数，利用上述公式。计算出两段时间的之间的CPU占用率
// 输入为：当前时刻和上一采样时刻cpu的信息

int main()
{
    //EMORYPACKED old_cpu_occupy;
    while (true)
    {
        FILE *fd;       // 定义打开文件的指针
        char buff[256]; // 定义个数组，用来存放从文件中读取CPU的信息
        MEM mem;

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
                sscanf(buff,"%s %d %s\n%s %d %s\n%s %d %s\n", &mem.name,&mem.memtotal,&mem.name,&mem.name,&mem.memfree,&mem.name,&mem.name,&mem.memavailable,&mem.name);
                mem.memuserate = 100*(1-(double(mem.memavailable)/double(mem.memtotal)));
                std::cout<<mem.memtotal<< std::endl;
                std::cout<<mem.memfree<< std::endl;
                std::cout<<mem.memavailable<< std::endl;
                std::cout<<mem.memuserate<< std::endl;
                std::cout<<"-------------------------"<< std::endl;

            }
        }
        sleep(1);                          // 延时1s；
    }
}

