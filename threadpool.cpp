#include <iostream>
#include <queue>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

typedef void (*TaskHandler)(int data);

class ThreadTask
{
  private:
    int _data;
    TaskHandler _handler;
  public:
  ThreadTask(){}
  ThreadTask(int data,TaskHandler handler):_data(data),_handler(handler){}
  void SetTask(int data, TaskHandler handler)
  {
    _data = data;
    _handler = handler;
  }
  void Run()
  {  
    _handler(_data);
  }
};

#define MAX_THR 5
class ThreadPool
{ 
  private: 
    int thr_num;//线程池中的线程最大数量
    std::queue<ThreadTask> _task_queue;//队列
    pthread_mutex_t _mutex;
    pthread_cond_t _cond_con;
    pthread_cond_t _cond_pro;
  private:
    static void *thr_start(void *arg)
    {
      ThreadPool *pool = (ThreadPool*)arg;
      while(1)
      {
        pool->ThreadLock();
        while(pool->TaskIsEmpty())
        {
          pool->ThreadWait();
        }
        ThreadTask tt;
        pool->QueuePop(tt);
        pool->ThreadUnlock();
        tt.Run();
      }
      return NULL;
    }
  public:
    ThreadPool(int max_thr = MAX_THR):thr_num(max_thr){}
    bool PoolInit()
    {
      pthread_mutex_init(&_mutex,NULL);
      pthread_cond_init(&_cond_con,NULL);
      pthread_cond_init(&_cond_pro,NULL);
      for(int i = 0;i < thr_num;i++)
      {
        pthread_t tid;
        int ret = pthread_create(&tid,NULL,thr_start,(void*)this);
        if(ret != 0)
        {
          std::cerr<<"create thread error\n";
          return false;
        }
        pthread_detach(tid);
      }
      return true;
    }
    bool TaskPush(ThreadTask &tt)
    {
      pthread_mutex_lock(&_mutex);
      _task_queue.push(tt);
      pthread_mutex_unlock(&_mutex);
      pthread_cond_signal(&_cond_con);
      return true;
    }
    bool QueuePop(ThreadTask &tt)
    {
      tt = _task_queue.front();
      _task_queue.pop();
      return true;
    }
    void ThreadWait()
    {
      pthread_cond_wait(&_cond_con,&_mutex);
    }
    void ThreadWakeUp()
    {
      pthread_cond_signal(&_cond_con);
    }
    bool TaskIsEmpty()
    {
      return _task_queue.empty();
    }
    void ThreadLock()
    {
      pthread_mutex_lock(&_mutex);
    }
    void ThreadUnlock()
    {
      pthread_mutex_unlock(&_mutex);
    }
};

void test(int data)
{
  srand(data);
  int sec = rand() % 5;
  printf("thread:%p--get data:%d,sleep %d sec\n",pthread_self(),data,sec);
  sleep(sec);
}

int main()
{
  ThreadPool pool;
  pool.PoolInit();
  for(int i = 0;i < 10;i++)
  {
    ThreadTask task(i,test);
    pool.TaskPush(task);
  } 
  while(1)
    sleep(1);
  return 0;
}
