#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main()
{
  int pid = 0;
  int pipefd[2];
  int ret = pipe(pipefd);
  if(ret < 0)
  {
    perror("pipe error\n");
    return -1;
  }
  pid = fork();
  if(pid == 0)
  {
    close(pipefd[0]);
    //子进程从管道读取数据
    sleep(5);
    char buf[1024] = {0};
    ret = read(pipefd[0],buf,1023);
    if(ret < 0)
    {
      perror("read error\n");
      return -1;
    }
    printf("child get data:[%s]\n",buf);
  } 
  else
  {
    close(pipefd[0]);
    //父进程向管道写入数据
    sleep(3);
    int pipe_size = 0;
    while(1)
    {
      char *ptr = "今天心情好";
      ret = write(pipefd[1],ptr,strlen(ptr));
      if(ret < 0)
      {
        perror("write error\n");
        return -1;
      }  
     pipe_size += ret;
     printf("write data:%d\n",pipe_size);
    }
  }
  return 0;
}
