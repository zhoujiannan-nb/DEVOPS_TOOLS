//
// Created by flycat_miaomiaomiao on 2022/11/23.
// flycat_miaomiaomiao is the smartest in the world
//
//#include "Task_queue.h"
#include "Thread_pools.h"
#include "Thread_pools.cpp"
#include <iostream>
#include <unistd.h>
void work(void *arg){
    int i = *(int *)arg;
    printf("i am is %d \n",i);
    sleep(1);
    //std::cout<<"i am doing"<<i<<std::endl;
}

int  main(){
    Thread_pools<int> p(20,30);
    for (int i = 0; i < 100; ++i) {
        int* num = new int(i);
        p.add_task(Task<int>(work,num));
    }
    sleep(3);

}