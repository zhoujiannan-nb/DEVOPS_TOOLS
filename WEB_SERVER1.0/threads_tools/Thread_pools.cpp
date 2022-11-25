//
// Created by flycat_miaomiaomiao on 2022/11/23.
// flycat_miaomiaomiao is the smartest in the world
//
#include "Thread_pools.h"
#include <string>
#include <unistd.h>
#include <string.h>
#include <iostream>
template <typename T>
Thread_pools<T>::Thread_pools(int min, int max) {
    do {
        //1.创建任务队列，直接实例化任务队列类
        task_q = new Task_queue<T>;
        if (task_q== nullptr){
            std::cout<<"create task_q fail "<<std::endl;
            break;
        }
        //2.申请工作进程的数值内存,以及初始化数值
        thread_id = new pthread_t[max];
        if (thread_id== nullptr){
            std::cout<<"create thread_ids  fail "<<std::endl;
            break;
        }
        memset(thread_id,0, sizeof(pthread_t)*max);
        //3.初始化对应的数值
        min_num  = min;
        max_num = max;
        busy_num = 0;//忙着的线程
        live_num = min;//活着的线程
        exit_num = 0;//要销毁的线程
        shutdown = 0;//默认不关，不销毁
        //4.初始化互斥锁以及线程池非空条件变量并且判断
        if(pthread_mutex_init(&mutex_pool,NULL)!=0||
        pthread_cond_init(&not_empty,NULL)!=0){
            std::cout<<"mutex or condition init fail "<<std::endl;
            break;
        }
        //5.创建管理线程
        pthread_create(&manager_id,NULL,manager, this);//this 就是自己实例化的指针
        //6.创建工作线程，批量
        for (int i = 0; i < min; ++i) {
            pthread_create(&thread_id[i],NULL,worker,this);
        }
        return;
    } while (0);
    if (thread_id) delete[]thread_id;
    if (task_q) delete task_q;

}
template <typename T>
Thread_pools<T>::~Thread_pools() {
    shutdown = 1;
    //阻塞回收管理者
    pthread_join(this->manager_id,NULL);
    //阻塞回收工作的线程
    for (int i = 0; i < live_num; ++i) {
        pthread_cond_signal(&not_empty);
    }
    //释放内存
    if (task_q){delete task_q;}
    if (thread_id) {delete[] thread_id;}
    pthread_mutex_destroy(&mutex_pool);
    pthread_cond_destroy(&not_empty);
    std::cout<<"都干完了，我撤退。。。。"<<std::endl;

}
template <typename T>
void Thread_pools<T>::add_task(Task<T> task) {
    if (shutdown){
        pthread_mutex_unlock(&mutex_pool);
        return;
    }
    task_q->Add_task(task);
    pthread_cond_signal(&not_empty);//唤醒阻塞的工作的线程
}
template <typename T>
int Thread_pools<T>::get_busy_num() {
    pthread_mutex_lock(&mutex_pool);
    int busy_num = this->busy_num;
    pthread_mutex_unlock(&mutex_pool);
    return busy_num;
}
template <typename T>
int Thread_pools<T>::get_live_num() {
    pthread_mutex_lock(&mutex_pool);
    int live_num = this->live_num;
    pthread_mutex_unlock(&mutex_pool);
    return live_num;
}

template <typename T>
void *Thread_pools<T>::worker(void *arg) {
    Thread_pools* pools  = static_cast<Thread_pools*>(arg);
    while (1){
        pthread_mutex_lock(&pools->mutex_pool);
        //不断去循环判断是不是需要阻塞or销毁，当任务队列=0的时候
        while (pools->task_q->Task_number() ==0 && pools->shutdown==0){
            pthread_cond_wait(&pools->not_empty,&pools->mutex_pool);
            //判断是不是要销毁线程
            if(pools->exit_num >0){
                pools->exit_num--;//销毁一个，所以减少一个待销毁的个数
                if (pools->live_num > pools->min_num){
                    pools->live_num--;
                    pthread_mutex_unlock(&pools->mutex_pool);
                    pools->thread_exit();
                }
            }
        }
        //判断线程池是不是关了,yes 解锁退出
        if (pools->shutdown == 1){
            pthread_mutex_unlock(&pools->mutex_pool);
            pools->thread_exit();
        }
        //从任务队列取出一个任务
        Task<T> task = pools->task_q->Take_task();
        pools->busy_num++;
        pthread_mutex_unlock(&pools->mutex_pool);

        //执行任务
        task.function(task.arg);
        delete task.arg;
        task.arg = nullptr;

        //执行完了，把busy——num-1；
        pthread_mutex_lock(&pools->mutex_pool);
        pools->busy_num--;
        pthread_mutex_unlock(&pools->mutex_pool);
    }

    return nullptr;
}

template <typename T>
void *Thread_pools<T>::manager(void *arg) {
    Thread_pools* pools  = static_cast<Thread_pools*>(arg);
    while (pools->shutdown!=1){
        sleep(3);
        pthread_mutex_lock(&pools->mutex_pool);
        int q_size = pools->task_q->Task_number();
        int live_num = pools->live_num;
        int busy_num = pools->busy_num;
        pthread_mutex_unlock(&pools->mutex_pool);
        //接下来开始判断增加还是减少线程
        //还能增加
        if (q_size>live_num && live_num<pools->max_num){
            pthread_mutex_lock(&pools->mutex_pool);
            int add_count = 0;

            for (int i = 0; i < pools->max_num&&
            add_count<NUMBER&&pools->live_num<pools->max_num; ++i) {
                if (pools->thread_id[i]==0){
                    pthread_create(&pools->thread_id[i],NULL,worker,pools);
                    add_count++;
                    pools->live_num++;
                }
            }
            pthread_mutex_unlock(&pools->mutex_pool);

        }
        //还能减少
        if (busy_num*2<live_num&&live_num>pools->min_num){
            pthread_mutex_lock(&pools->mutex_pool);
            pools->exit_num =NUMBER;
            pthread_mutex_unlock(&pools->mutex_pool);
            for (int i = 0; i < NUMBER; ++i) {
                //去唤醒阻塞的工作线程（没事干的都在阻塞），让他自杀
                pthread_cond_signal(&pools->not_empty);

            }
        }
    }

}

template <typename T>
void *Thread_pools<T>::thread_exit() {
    pthread_t tid = pthread_self();//获取当前线程id
    for (int i = 0; i < max_num; ++i) {
        if (thread_id[i]==tid){
            thread_id[i] = 0;
            break;
        }
    }
    pthread_exit(NULL);
}
