#include "threadPool.h"
#include <stdlib.h>     //malloc()
#include <string.h>     //memset()
#include <stdio.h>      //printf()
#include <pthread.h>    //pthread
#include <unistd.h>     //sleep()

#define NUMBER 2 //一次最多创建线程数量

//任务队列：主线程收到客户端数据后不自己处理，把任务打包为Task，放入任务队列
struct Task
{
    void (*function)(void* arg);
    void* arg;
};

//线程池结构体
struct ThreadPool
{
    Task* taskQueue;   //环形任务队列
    int queueCapacity; //队列容量
    int queueSize;     //当前任务个数
    int queueFront;    //队头：存数据
    int queueRear;     //队尾：取数据

    pthread_t managerID;   //管理者线程ID
    pthread_t* threadIDs;  //接收pthread_create传出的 工作的线程ID，指向pthread_t类型的动态数组
    int minNum;  //线程池 最小线程数
    int maxNum;  //线程池 最大线程数
    int liveNum; //线程池 存活线程数
    int busyNum; //线程池 忙线程数
    int exitNum; //线程池 需要销毁的线程数

    pthread_mutex_t mutexPool;  //互斥锁：整个线程池的互斥锁
    pthread_mutex_t mutexBusy;  //互斥锁：锁busyNum变量。提高并发性能，避免所有线程都在争抢同一把锁
    pthread_cond_t notFull;   //条件变量：任务队列是否满
    pthread_cond_t notEmpty;  //条件变量：任务队列是否空

    int shutdown; //是否要销毁线程池，销毁为1，不销毁为0
};  

/*
函数作用：创建线程池
参数含义：min最小线程数。线程池刚创建时，立刻启动的线程数量。
         max：最大线程数。线程池扩容的上限。
         queueCap：任务队列的容量。最多能存放多少个待处理的任务。
*/
ThreadPool* threadPoolCreate(int min, int max, int queueCap)
{
    //1.初始化线程池结构体（堆区申请内存）
    ThreadPool* pool = (ThreadPool*)malloc(sizeof(ThreadPool));
    do{
        if(pool == NULL)
        {
            printf("malloc threadPool fail...\n");
            break;
        }

        //2.分配线程ID数组并且清零（堆区申请内存）
        pool->threadIDs = (pthread_t*)malloc(sizeof(pthread_t) * max);
        if(pool->threadIDs == NULL)
        {
            printf("malloc threadIDs fail...\n");
            break;
        }
        memset(pool->threadIDs, 0, sizeof(pthread_t) * max); //把线程ID这个动态数组清零
        //初始化结构体计数变量
        pool->minNum = min;
        pool->maxNum = max;
        pool->liveNum = min; //初始状态下存活线程和最小线程数相等
        pool->busyNum = 0;
        pool->exitNum = 0;
        //初始化互斥锁和条件变量
        int ret1 = pthread_mutex_init(&pool->mutexPool, NULL);
        int ret2 = pthread_mutex_init(&pool->mutexBusy, NULL);
        int ret3 = pthread_cond_init(&pool->notFull, NULL);
        int ret4 = pthread_cond_init(&pool->notEmpty, NULL);
        if(ret1 != 0 || ret2 != 0 || ret3 != 0 || ret4 != 0)
        {
            printf("mutex or condition init fail...\n");
            break;
        }
        //初始化任务队列
        pool->taskQueue = (Task*)malloc(sizeof(Task) * queueCap);
        pool->queueCapacity = queueCap;
        pool->queueSize = 0;
        pool->queueFront = 0;
        pool->queueRear = 0;
        //初始化是否销毁线程池标志位
        pool->shutdown = 0;
        //3.创建N个任务线程
        pthread_create(&pool->managerID, NULL, manager, pool);
        for(int i = 0; i < min; i++)
        {
            pthread_create(&pool->threadIDs[i], NULL, worker, pool);
        }
        //创建成功直接返回
        return pool;
    }while(0);
    
    //释放在内存创建的资源
    if(pool != NULL && pool->threadIDs != NULL)
    {
        free(pool->threadIDs);
    }
    if(pool != NULL && pool->taskQueue != NULL)
    {
        free(pool->taskQueue);
    }
    if(pool != NULL)
    {
        free(pool);
    }
    return NULL;
}

/*
函数作用：销毁线程池
参数含义：pool待销毁的线程池
*/
int threadPoolDestroy(ThreadPool* pool)
{
    //如果pool为空直接返回
    if(pool == NULL)
    {
        return -1;
    }
    //销毁线程池标志位置1
    pool->shutdown = 1;
    //阻塞回收管理者线程
    pthread_join(pool->managerID, NULL);
    //等待所有工作线程退出
    for(int i = 0; i < pool->maxNum; i++)
    {
        if (pool->threadIDs[i] != 0) {
            pthread_join(pool->threadIDs[i], NULL);
        }
    }
    //释放在内存创建的资源
    if(pool->threadIDs != NULL)
    {
        free(pool->threadIDs);
    }
    if(pool->taskQueue != NULL)
    {
        free(pool->taskQueue);
    }

    //销毁同步原语
    pthread_mutex_destroy(&pool->mutexPool);
    pthread_mutex_destroy(&pool->mutexBusy);
    pthread_cond_destroy(&pool->notEmpty);
    pthread_cond_destroy(&pool->notFull);

    //释放结构体
    free(pool);
  
    return 0;
}

/*
函数作用：生产者往队列中添加任务
参数含义：pool线程池
         func任务的回调函数
         arg任务的参数
*/
void threadPoolAdd(ThreadPool* pool, void(*func)(void*), void* arg)
{
    //1.锁住整个线程池
    pthread_mutex_lock(&pool->mutexPool);
    //2.若任务队列满，则等待有空位置
    while(pool->queueSize == pool->queueCapacity && pool->shutdown == 0)
    {
        pthread_cond_wait(&pool->notFull, &pool->mutexPool);
    }
    //3.等待期间线程池被销毁，则不再添加任务，直接解锁返回
    if(pool->shutdown == 1)
    {
        pthread_mutex_unlock(&pool->mutexPool);
        return;
    }
    //4.向环形队列添加任务
    pool->taskQueue[pool->queueRear].function = func;
    pool->taskQueue[pool->queueRear].arg = arg;
    pool->queueRear = (pool->queueRear + 1) % pool->queueCapacity; //环形队列尾指针移动
    pool->queueSize++;
    //5. 唤醒一个工作线程
    pthread_cond_signal(&pool->notEmpty);
    //6.解锁
    pthread_mutex_unlock(&pool->mutexPool);  
}

/*
函数作用：查询忙线程数量
参数含义：pool线程池
*/
int threadPoolBusyNum(ThreadPool* pool)
{
    if (pool == NULL)
    {
        return -1;
    }
    pthread_mutex_lock(&pool->mutexBusy);   //加锁
    int abusyNum = pool->busyNum;            //读取忙线程数
    pthread_mutex_unlock(&pool->mutexBusy); //解锁
    return abusyNum;                         //返回副本
}

/*
函数作用：查询存活线程数量
参数含义：pool线程池
*/
int threadPoolAliveNum(ThreadPool* pool)
{
    if (pool == NULL)
    {
        return -1;
    }
    pthread_mutex_lock(&pool->mutexPool);   //加锁
    int aliveNum = pool->liveNum;           //读取存活线程数
    pthread_mutex_unlock(&pool->mutexPool); //解锁
    return aliveNum;                        //返回副本
}

//子线程执行函数: 消费者
void* worker(void* arg)
{
    ThreadPool* pool = (ThreadPool*)arg; //void*类型改为ThreadPool*:内存中数据没变，只是读取方式变了
    while(1)
    {
        pthread_mutex_lock(&pool->mutexPool);
        //当前任务队列是否为空
        while(pool->queueSize == 0 && pool->shutdown == 0)
        {
            //1.等待任务: 阻塞工作线程，等待生产者生产出任务
            pthread_cond_wait(&pool->notEmpty, &pool->mutexPool);
            //2.线程池缩容：判断是否要销毁线程，exitNum需要销毁线程的数量
            if(pool->exitNum > 0)
            {
                pool->exitNum--; //每次销毁1个线程
                //如果存活的线程数量大于最小线程数量
                if(pool->liveNum > pool->minNum)
                {
                    pool->liveNum--;
                    pthread_mutex_unlock(&pool->mutexPool);
                    //销毁线程
                    threadExit(pool);
                }
            }
        }
        //3.判断线程池是否被关闭
        if(pool->shutdown == 1)
        {
            pthread_mutex_unlock(&pool->mutexPool);
            threadExit(pool);
        }
        //4.从环形队列中取出任务
        Task task; //初始化一个任务结构体
        task.function = pool->taskQueue[pool->queueFront].function; //从队列中取出执行任务的回调函数
        task.arg = pool->taskQueue[pool->queueFront].arg; //从队列中取出回调函数的参数
        pool->queueFront = (pool->queueFront + 1) % pool->queueCapacity; //对头指针后移
        pool->queueSize--; 
        pthread_cond_signal(&pool->notFull);
        //5.解锁
        pthread_mutex_unlock(&pool->mutexPool);
        //6.执行任务
        printf("thread %ld start working...\n", pthread_self()); //开始执行

        pthread_mutex_lock(&pool->mutexBusy);
        pool->busyNum++;
        pthread_mutex_unlock(&pool->mutexBusy);
        
        task.function(task.arg);

        free(task.arg);
        task.arg = NULL;
        printf("thread %ld end working...\n", pthread_self()); //执行完毕
        
        pthread_mutex_lock(&pool->mutexBusy);
        pool->busyNum--;
        pthread_mutex_unlock(&pool->mutexBusy);
    }
    return NULL;
}

/*
子线程执行函数: 生产者
函数作用：线程不处理具体任务，只负责监控线程池状态，动态调整线程数量
*/
void* manager(void* arg)
{
    ThreadPool* pool = (ThreadPool*)arg;
    //不销毁线程池
    while(pool->shutdown == 0)
    {
        //1.定期监控: 每3s检查一次线程池状态
        sleep(3); 
        //取出线程池中 任务的数量
        pthread_mutex_lock(&pool->mutexPool);
        int queSize = pool->queueSize; //任务数量 
        pthread_mutex_unlock(&pool->mutexPool);
        //取出 忙线程的数量 和 存活线程的数量
        int abusyNum = threadPoolBusyNum(pool);
        int aliveNum = threadPoolAliveNum(pool);
        //2.扩容机制: 任务数 > 存活线程数(活干不完)，且存活线程数 < 最大线程数
        if(queSize > aliveNum && aliveNum < pool->maxNum)
        {
            pthread_mutex_lock(&pool->mutexPool);
            //创建线程计数
            int counter = 0;
            //一次创建线程的数量 < 规定数量NUMBER，且存活线程数 < 最大线程数
            for(int i = 0; i < pool->maxNum && counter < NUMBER &&  pool->liveNum < pool->maxNum; i++)
            {
                //遍历threadIDs数组，找到空位，值为0
                if(pool->threadIDs[i] == 0)
                {
                    pthread_create(&pool->threadIDs[i], NULL, worker, pool);
                    counter++;
                    pool->liveNum++;
                }
            }
            pthread_mutex_unlock(&pool->mutexPool);
        }
        //3.缩容机制: 忙线程数×2 < 存活线程数(空闲线程太多)，且存活线程数 > 最小线程数
        if (abusyNum * 2 < aliveNum && aliveNum > pool->minNum)
        {
            pthread_mutex_lock(&pool->mutexPool);
            pool->exitNum = NUMBER;
            pthread_mutex_unlock(&pool->mutexPool);
            for (int i = 0; i < NUMBER; i++)
            {
                pthread_cond_signal(&pool->notEmpty);
            }
        }
    }
    return NULL;
}

/*
函数作用：退出线程
*/
void threadExit(ThreadPool* pool)
{
    pthread_t tid = pthread_self();
    for (int i = 0; i < pool->maxNum; ++i)
    {
        if (pool->threadIDs[i] == tid)
        {
            pool->threadIDs[i] = 0;
            printf("threadExit() called, %ld exiting...\n", tid);
            break;
        }
    }
    pthread_exit(NULL);
}

/*----------------------------测试代码-----------------------------*/

/*
任务函数: 每个任务打印自己的线程 ID 和参数，然后 sleep(1) 模拟耗时操作
*/
void taskFunc(void* arg)
{
    int* tmp = (int*)arg;
    printf("thread %ld is working, number = %d\n", pthread_self(), *tmp);
    sleep(1);  // 模拟耗时 1 秒的业务
}

int main()
{
    //创建线程池: 初始化3个线程，最大扩容10个，任务队列容量100
    ThreadPool* pool = threadPoolCreate(3, 10, 100);
    for (int i = 0; i < 100; i++)
    {
        int* num = (int*)malloc(sizeof(int));
        *num = i + 100; //每个任务分配一块堆内存
        threadPoolAdd(pool, taskFunc, num); //只在队列满时才阻塞。队列容量100，所以这100个任务瞬间全部入队
    }
    sleep(30); //主线程进入休眠，把 CPU 让给工作线程和管理者线程

    threadPoolDestroy(pool);
    return 0;
}