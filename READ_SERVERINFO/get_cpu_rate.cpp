//
// Created by flycat_miaomiaomiao on 2022/11/11.
// flycat_miaomiaomiao is the smartest in the world
//
#include <cstring>
#include <iostream>
#include <stdio.h>
#include <unistd.h>

// 定义一个cpu occupy的结构体，用来存放CPU的信息
typedef struct CPUPACKED
{
    char name[20];       //定义一个char类型的数组名name有20个元素
    unsigned int user;   //定义一个无符号的int类型的user
    unsigned int nice;   //定义一个无符号的int类型的nice
    unsigned int system; //定义一个无符号的int类型的system
    unsigned int idle;   //定义一个无符号的int类型的idle
    unsigned int lowait;
    unsigned int irq;
    unsigned int softirq;
} CPU_OCCUPY;

// 该函数，利用上述公式。计算出两段时间的之间的CPU占用率
// 输入为：当前时刻和上一采样时刻cpu的信息
double getCpuUse(CPU_OCCUPY *o, CPU_OCCUPY *n)
{
    std::cout << "test" << std::endl;
    unsigned long od, nd;
    od = (unsigned long)(o->user + o->nice + o->system + o->idle + o->lowait + o->irq + o->softirq); //第一次(用户+优先级+系统+空闲)的时间再赋给od
    nd = (unsigned long)(n->user + n->nice + n->system + n->idle + n->lowait + n->irq + n->softirq); //第二次(用户+优先级+系统+空闲)的时间再赋给od
    double sum = nd - od;
    double idle = n->idle - o->idle;
    return (sum - idle) / sum;
}
int main()
{
    CPU_OCCUPY old_cpu_occupy;
    while (true)
    {
        FILE *fd;       // 定义打开文件的指针
        char buff[256]; // 定义个数组，用来存放从文件中读取CPU的信息
        CPU_OCCUPY cpu_occupy;
        std::string cpu_use = "";

        fd = fopen("/proc/stat", "r");

        if (fd != NULL)
        {
            // 读取第一行的信息，cpu整体信息
            fgets(buff, sizeof(buff), fd);
            if (strstr(buff, "cpu") != NULL) // 返回与"cpu"在buff中的地址，如果没有，返回空指针
            {
                // 从字符串格式化输出
                sscanf(buff, "%s %u %u %u %u %u %u %u", cpu_occupy.name, &cpu_occupy.user, &cpu_occupy.nice, &cpu_occupy.system, &cpu_occupy.idle, &cpu_occupy.lowait, &cpu_occupy.irq, &cpu_occupy.softirq);
                // cpu的占用率 = （当前时刻的任务占用cpu总时间-前一时刻的任务占用cpu总时间）/ （当前时刻 - 前一时刻的总时间）
                cpu_use = std::to_string(getCpuUse(&old_cpu_occupy, &cpu_occupy) * 100) + "%";
                old_cpu_occupy = cpu_occupy;
            }
        }
        std::cout << cpu_use << std::endl; // 打印cpu的占用率
        sleep(1);                          // 延时1s；
    }
}

