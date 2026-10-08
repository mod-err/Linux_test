#ifndef _THREADPOOL_H
#define _THREADPOOL_H

typedef struct Task Task;
typedef struct ThreadPool ThreadPool;

ThreadPool* threadPoolCreate(int min, int max, int queueCap);
int threadPoolDestroy(ThreadPool* pool);
void threadPoolAdd(ThreadPool* pool, void(*func)(void*), void* arg);
int threadPoolBusyNum(ThreadPool* pool);
int threadPoolAliveNum(ThreadPool* pool);
void* worker(void* arg);
void* manager(void* arg);
void threadExit(ThreadPool* pool);

#endif