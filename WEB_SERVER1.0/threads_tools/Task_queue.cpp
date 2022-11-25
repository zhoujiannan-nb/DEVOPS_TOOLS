//
// Created by flycat_miaomiaomiao on 2022/11/22.
// flycat_miaomiaomiao is the smartest in the world
//

#include "Task_queue.h"
#include <iostream>

template<typename T>
Task_queue<T>::Task_queue() {
    pthread_mutex_init(&m_mutex,NULL);//初始化互斥锁
}

template<typename T>
Task_queue<T>::~Task_queue() {
    pthread_mutex_destroy(&m_mutex);//销毁锁
}
template<typename T>
void Task_queue<T>::Add_task(Task<T>& task) {
    pthread_mutex_lock(&m_mutex);
    m_task.push(task);

    pthread_mutex_unlock(&m_mutex);
}
template<typename T>
//重载函数，将函数和参数分开
void Task_queue<T>::Add_task(callback f, void *arg) {
    pthread_mutex_lock(&m_mutex);
    Task<T> task;
    task.function = f;
    task.arg = arg;
    m_task.push(task);
    pthread_mutex_unlock(&m_mutex);
}

template<typename T>
Task<T> Task_queue<T>::Take_task() {
    Task<T> t;
    pthread_mutex_lock(&m_mutex);
    if (m_task.size() > 0){
        t = m_task.front();//先取出来
        m_task.pop();//弹出第一个
    }
    pthread_mutex_unlock(&m_mutex);
    return t;
}

