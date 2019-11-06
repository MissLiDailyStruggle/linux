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
  int fd = open(file,O_RDONLY);
  if(fd < 0)
  {
    perror("open error:");
    return -1;
  }
  printf("open success\n");
  while(1)
  {
    char buf[1024] = {0};
    int ret = read(fd,buf,1023);
    if(ret == 0)
    {
      printf("管道没有人写了，所有写端被关闭\n");
      return 0;
    }
    else if(ret < 0)
    {
      perror("read error:");
      return -1;
    }
    printf("buf:[%s]\n",buf);
  }
  return 0;
}
