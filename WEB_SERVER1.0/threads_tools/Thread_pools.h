//
// Created by flycat_miaomiaomiao on 2022/11/23.
// flycat_miaomiaomiao is the smartest in the world
//

#ifndef THREAD_POOLS_THREAD_POOLS_H
#define THREAD_POOLS_THREAD_POOLS_H
#include "Task_queue.h"
#include "Task_queue.cpp"
template <typename T>
class Thread_pools {
public:
    Thread_pools(int min,int max);//创建线程池并初始化
    ~Thread_pools();//析构销毁线程池
    void add_task(Task<T> task);//向任务队列加任务
    int get_busy_num();//获取线程忙得个数
    int get_live_num();//获取活着得线程数

private:
    static void* worker(void* arg);//工作线程,静态方法
    static void* manager(void* arg);//管理线程，静态方法，有自己的地址
    void* thread_exit();//线程退出

private:
    Task_queue<T>* task_q;//任务队列,直接使用任务队列对象
    pthread_t manager_id;//管理者线程
    pthread_t *thread_id;//工作的线程
    int min_num;//最小线程数
    int max_num;//最大线程数
    int busy_num;//忙着的线程
    int live_num;//活着的线程
    int exit_num;//要销毁的线程
    pthread_mutex_t mutex_pool;//线程池的锁
    pthread_cond_t not_empty;//条件变量，任务队列是否为空

    static const int NUMBER = 2;//管理者线程每次更新的数

    int shutdown;//是不是要销毁线程池，要的1，不要0
};


#endif //THREAD_POOLS_THREAD_POOLS_H
