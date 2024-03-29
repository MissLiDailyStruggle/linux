//线程安全之生产者消费者模型（互斥锁+条件变量）
#include <iostream>
#include <queue>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#define  MAX_QUE 10

class BlockQueue
{
  private:
      std::queue<int> _queue;//动态增长的队列；
      int _capacity;
      pthread_cond_t _cond_productor;
      pthread_cond_t _cond_consumer;
      pthread_mutex_t mutex;
  public:
      BlockQueue(int que_num =  MAX_QUE): _capacity(que_num)
      {
        pthread_mutex_init(&mutex,NULL);
        pthread_cond_init(&_cond_productor,NULL);
        pthread_cond_init(&_cond_consumer,NULL);
      }
      ~BlockQueue()
      {
        pthread_mutex_destroy(&mutex);
        pthread_cond_destroy(&_cond_productor);
        pthread_cond_destroy(&_cond_consumer); 
      }
      bool QueuePush(int &data)
      {
        pthread_mutex_lock(&mutex);
        while(_queue.size() == _capacity)
        {
          pthread_cond_wait(&_cond_productor,&mutex);
        }
        _queue.push(data);
        pthread_mutex_unlock(&mutex);
        pthread_cond_signal(&_cond_consumer);
        return true;
      }    
      bool QueuePop(int &data)
      {
        pthread_mutex_lock(&mutex);
        while(_queue.empty())
        {
          pthread_cond_wait(&_cond_consumer,&mutex);
        }
        data = _queue.front();
        _queue.pop();
        pthread_mutex_unlock(&mutex);
        pthread_cond_signal(&_cond_productor);
        return true;
      }
};

void *thr_consumer(void *arg)
{
  BlockQueue *queue = (BlockQueue*)arg;
  while(1)
  {
    int data;
    queue->QueuePop(data);
    printf("get a data:%d\n",data);
  }
  return NULL;
 
}

void *thr_productor(void *arg)
{
  int i = 0;
  BlockQueue *queue = (BlockQueue*)arg;
  while(1)
  {
    queue->QueuePush(i);
    printf("put a data:%d\n",i++);
  }
  return NULL;
}

#define MAX_THREAD 4

int main()
{
  int i ,ret;
  pthread_t ctid[MAX_THREAD],ptid[MAX_THREAD];
  BlockQueue queue;
  for(i = 0;i < MAX_THREAD;i++)
  {
    ret = pthread_create(&ctid[i],NULL,thr_consumer,(void*)&queue);
    if(ret !=  0)
    {
      std::cerr<<"thread create error\n";
      return -1;
    }
  }
  for(i = 0;i < MAX_THREAD;i++)
  {
    ret = pthread_create(&ptid[i],NULL,thr_productor,(void*)&queue);
    if(ret !=  0)
    {
      std::cerr<<"thread create error\n";
      return -1;
    }
  }
  for(i = 0;i < MAX_THREAD;i++)
  {
    pthread_join(ctid[i],NULL);
    pthread_join(ptid[i],NULL);
  }
  return 0;
}
