//通过匿名管道实现管道符
//ps -ef | grep ssh
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main()
{
  int pipefd[2] = {0};
  int ret = pipe(pipefd);
  if(ret < 0)
  {
    perror("pipe error");
    return -1;
  }
  int pid1 = fork();
  if(pid1 == 0)
  {
    //进程1实现ps
    close(pipefd[0]);
    dup2(pipefd[1],1);
    execlp("ps","ps","-ef",NULL);
    exit(0);
  }
  int pid2 = fork();
  if(pid2 == 0)
  {
    //进程2实现grep
    close(pipefd[1]);
    dup2(pipefd[0],0);
    execlp("grep","grep","ssh",NULL);
    exit(0);
  } 
  close(pipefd[0]);
  close(pipefd[1]);
  waitpid(pid1,NULL,0);
  waitpid(pid2,NULL,0);
  return 0; 
}

