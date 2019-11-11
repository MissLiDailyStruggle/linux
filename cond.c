//通过吃面体会临界资源访问的时序合理性以及条件变量的基本使用
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

int is_have_noodle = 0;
pthread_cond_t cond_foodie;
pthread_cond_t cond_chef;
pthread_mutex_t mutex;

void *foodie(void *arge)
{
  while(1)
  {
    pthread_mutex_lock(&mutex);
    while(is_have_noodle == 0)
    {
      pthread_cond_wait(&cond_foodie,&mutex);
    }
    printf("eat noodles!\n");
    is_have_noodle--;
    pthread_mutex_unlock(&mutex);
    pthread_cond_signal(&cond_chef);
  }
  return NULL; 
}

void *chef(void *arg)
{
  while(1)
  {
    pthread_mutex_lock(&mutex);
    while(is_have_noodle == 1)
    {
      pthread_cond_wait(&cond_chef,&mutex);
    }
    printf("cook noodles\n");
    is_have_noodle++;
    pthread_mutex_unlock(&mutex);
    pthread_cond_signal(&cond_foodie);
  }
  return NULL;
}

#define MAX_FOODIE 4
int main()
{
  pthread_t foodie_tid,chef_tid;
  int ret = 0;
  pthread_cond_init(&cond_foodie,NULL);
  pthread_cond_init(&cond_chef,NULL);
  pthread_mutex_init(&mutex,NULL);
  int i = 0;
  for(i = 0;i < MAX_FOODIE;i++)
  {
    ret = pthread_create(&foodie_tid,NULL,foodie,NULL);
    if(ret != 0)
    {
      printf("thread creat error\n");
      return -1;
    }
  }
  ret = pthread_create(&chef_tid,NULL,chef,NULL);
  if(ret != 0)
  {
    printf("threa creat erroe\n");
    return -1;
  }
  pthread_join(foodie_tid,NULL);
  pthread_join(chef_tid,NULL);
  pthread_cond_destroy(&cond_foodie);
  pthread_cond_destroy(&cond_chef);
  pthread_mutex_destroy(&mutex);
  return 0;
}
