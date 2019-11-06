//命名管道的基本操作
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>

int main()
{
  char *file = "./test.fifo";
  int ret = mkfifo(file,0664);
  if(ret < 0)
  {
    if(errno != EEXIST)
    {
      perror("mkfifo error:");
      return -1;
    }
  }
  int fd = open(file,O_WRONLY);
  if(fd < 0)
  {
    perror("open error:");
    return -1;
  }
  printf("open success\n");
  while(1)
  {
    char buf[1024] = {0};
    scanf("%s",buf);
    int ret = write(fd,buf,strlen(buf));
    if(ret == 0)
    {
      printf("管道没有人读了，所有读端被关闭\n");
      return 0;
    }
    else if(ret < 0)
    {
      perror("write error:");
      return -1;
    }
  }
  return 0;
}
