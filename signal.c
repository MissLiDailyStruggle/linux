//自定义信号处理的基本操作
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

struct sigaction oldact;

void sigcb(int signum)
{  
  printf("recv a signal:%d\n",signum);
  sigaction(signum,&oldact,NULL);
}

int main()
{
  //signal(2,sigcb);
  //signal(3,sigcb);
  //int sigaction(int signum,struct sigaction *act,struct sigaction *oldact);
  //signum:信号值
  //act:signum当前修改的新的动作
  //oldact：用于获取signum信号原有的动作(便于以后还原回去)
  struct sigaction newact;
  struct sigaction oldact;
  newact.sa_handler = sigcb;//设置自定义回调函数
  newact.sa_flags = 0;//默认使用sa_handler回调函数
  sigemptyset(&newact.sa_mask);//清空临时要阻塞的信号集合
  sigaction(2,&newact,&oldact);
  while(1)
  {
    printf("------------------\n");
    sleep(1);
  }
  return 0;
}
