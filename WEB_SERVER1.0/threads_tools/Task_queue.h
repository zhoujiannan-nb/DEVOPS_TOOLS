//
// Created by flycat_miaomiaomiao on 2022/11/22.
// flycat_miaomiaomiao is the smartest in the world
//

#ifndef THREAD_POOLS_TASK_QUEUE_H
#define THREAD_POOLS_TASK_QUEUE_H
#include <queue>//直接使用c++里面的stl容器，队列
#include <pthread.h>

//(*function)(void* arg); 取个别名
using callback = void (*)(void* arg);//c++11 新特性

//任务结构体
template<typename T>
struct Task{
    Task(){
        function = nullptr;
        arg = nullptr;
    }
    Task(callback f,void* arg){
        this->function = f;
        this->arg = (T*)arg;
    }
    callback function;
    T* arg;
};

//任务队列类
template<typename T>
class Task_queue {
public:
    Task_queue();
    ~Task_queue();
    //add 任务
    void Add_task(Task<T> &task);
    void Add_task(callback f,void* arg);
    //取出任务
    Task<T> Take_task();
    //获取当前任务队列的个数，定义成内联函数，因为比较简单
    inline size_t Task_number(){
        return m_task.size();//直接读取stl queue队列自带的大小；
    }


private:
    pthread_mutex_t m_mutex;
    std::queue<Task<T>> m_task;

};


#endif //THREAD_POOLS_TASK_QUEUE_H
