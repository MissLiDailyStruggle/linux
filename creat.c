//线程创建
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

void *thr_start(void *arg)
{
  printf("child thread\n");
  sleep(1);
  return NULL;
  //exit(0);
}
int main()
{
  pthread_t tid;
  char *ptr = "this is a thread";
  int ret = pthread_create(&tid,NULL,thr_start,(void*)ptr);
  if(ret != 0)
  {
    printf("pthread error\n");
    return -1;
  }
  char *retval;
  while(1)
  {
    printf("main thread\n");
    sleep(1);
  }
  return 0;
}
