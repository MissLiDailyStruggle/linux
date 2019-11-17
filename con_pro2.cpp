//信号量实现生产者消费者模型
#include <iostream>
#include <vector>
#include <string>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define  MAX_QUE 10

class RingQueue
{
  private:
      std::vector<int> _queue;//使用数组实现环形队列
      int _capacity;
      int _step_read;
      int _step_write;
      sem_t _sem_lock;
      sem_t _sem_con;
      sem_t _sem_pro;
  public:
      RingQueue(int max_que  =  MAX_QUE): _capacity(max_que),_queue(max_que),
          _step_read(0),_step_write(0)
      {
        sem_init(&_sem_lock,0,1);
        sem_init(&_sem_con,0,0);
        sem_init(&_sem_pro,0,max_que);
      }
      ~RingQueue()
      {
        sem_destroy(&_sem_lock);
        sem_destroy(&_sem_pro);
        sem_destroy(&_sem_con); 
      }
      bool QueuePush(int &data)
      {
        sem_wait(&_sem_pro);
        sem_wait(&_sem_lock);
        _queue[_step_write] = data;
        _step_write = (_step_write + 1) % _capacity;
        sem_post(&_sem_lock);
        sem_post(&_sem_con);
        return true;
      }    
      bool QueuePop(int &data)
      {
        sem_wait(&_sem_con);
        sem_wait(&_sem_lock);
        data = _queue[_step_read];
        _step_read = (_step_read + 1) % _capacity;
        sem_post(&_sem_lock);
        sem_post(&_sem_pro);
        return true;
      }
};

void *consumer(void *arg)
{
  RingQueue *queue = (RingQueue*)arg;
  while(1)
  {
    int data;
    queue->QueuePop(data);
    printf("consumer get a data:%d\n",data);
  }
  return NULL;
 
}

void *productor(void *arg)
{
  int i = 0;
  RingQueue *queue = (RingQueue*)arg;
  while(1)
  {
    queue->QueuePush(i);
    printf("productor put a data:%d\n",i++);
  }
  return NULL;
}

#define MAX_THREAD 4

int main()
{
  int i ,ret;
  pthread_t ctid[MAX_THREAD],ptid[MAX_THREAD];
  RingQueue queue;
  for(i = 0;i < MAX_THREAD;i++)
  {
    ret = pthread_create(&ctid[i],NULL,consumer,(void*)&queue);
    if(ret !=  0)
    {
      std::cerr<<"thread create error\n";
      return -1;
    }
  }
  for(i = 0;i < MAX_THREAD;i++)
  {
    ret = pthread_create(&ptid[i],NULL,productor,(void*)&queue);
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
