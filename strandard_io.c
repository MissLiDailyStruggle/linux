#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main()
{
  FILE *fp = NULL;
  fp = fopen("./text.txt","r+");
  if(fp == NULL)
  {
    perror("fopen error");
    return -1;
  }
  char *ptr = "好嗨哟\n";
 // char *ptr = "今天心情很美丽\n";
  int ret = fwrite(ptr,1,strlen(ptr),fp);
  if(ret != strlen(ptr))
  {
    printf("数据并没有完全写入\n");
    fclose(fp);
    return -1;
  }   
  fseek(fp,0,SEEK_SET);
  char buf[1024] = {0};
  ret = fread(buf,1023,1,fp);
  if(!feof(fp))
  {
    printf("已经读到文件末尾了");
    fclose(fp);
    return -1;
  }
  printf("buf:[%s]-%d\n",buf,ret);
  fclose(fp);
  return 0;
}
