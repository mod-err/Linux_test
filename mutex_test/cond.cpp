#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

//公共区域：链表节点结构体
struct Node
{
    Node(int data = 0)
    : next_(nullptr)
    , data_(data)
    {}
    Node* next_;
    int data_;
};

//初始化一个头节点
Node* head_;
//定义互斥锁
pthread_mutex_t lock;
//定义条件变量
pthread_cond_t cond;

void* consumer(void* arg)
{
    while(1)
    {
        pthread_mutex_lock(&lock);
        //头节点指针为空说明没有节点
        if(head_ == nullptr)
        {
            pthread_cond_wait(&cond, &lock); //阻塞等待条件满足，解锁
        }
        Node* node = head_; //记录头节点
        head_ = head_->next_; //消费一个节点

        pthread_mutex_unlock(&lock);

        printf("Consume %lu --- %d\n", pthread_self(), node->data_);
        delete node;

        sleep(rand()%5);
    }
}

void* producer(void* arg)
{
    while(1)
    {
        int data = rand()%1000+1;
        //生产一个新节点
        Node* node = new Node(data);
        printf("Produce %lu --- %d\n", pthread_self(), node->data_);
        
        //先获取锁
        pthread_mutex_lock(&lock);
        //链接链表
        node->next_ = head_;
        head_ = node;

        pthread_mutex_unlock(&lock);
        //把等待在该条件变量的一个线程唤醒
        pthread_cond_signal(&cond);
        
        sleep(rand()%5);
    }
}

int main()
{
    //初始化互斥锁
    pthread_mutex_init(&lock, NULL);

    //创建生产者和消费者线程
    pthread_t pid, cid;
    srand(time(NULL));

    pthread_create(&pid, NULL, producer, NULL);
    pthread_create(&cid, NULL, consumer, NULL);

    pthread_join(pid, NULL);
    pthread_join(cid, NULL);

    return 0;
}