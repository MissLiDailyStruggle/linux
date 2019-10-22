#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

int main()
{
   while(1)
   {
      //打印minishell的界面
      printf("[nbsun@localhost]# ");
      fflush(stdout);

      //获取输入的操作指令并打印出来
      char buf[1024] = {0};
      fgets(buf,1023,stdin);
      buf[strlen(buf) -1] = '\0';
      printf("cmd:[%s]\n",buf);
      
      //解析重定向符<< 和 <
      int redirect_flag = 0;
      char *file = NULL;
      char *ptr = buf;
      while(*ptr)
      {
          if(*ptr == '>')
          {
            redirect_flag++;
            *ptr = '\0';
            ptr++;
            if(*ptr == '>')
            {
              redirect_flag++;
              *ptr = '\0';
              ptr++;
            }
          while(isspace(*ptr) && *ptr != '\0')
               ptr++;
          file = ptr;
          while(!isspace(*ptr) && *ptr != '\0')
                ptr++; 
          *ptr = '\0';
          }
           ptr++;
      }

      //解析普通命令
      int  argc = 0;
      char *argv[32] = {NULL};
      ptr = buf;
      while(*ptr)
      {
          if(!isspace(*ptr))
             argv[argc++] = ptr;
          while(!isspace(*ptr) && *ptr != '\0')
             ptr++;
          *ptr = '\0';
          ptr++;
      }
      argv[argc] = NULL;

      int pid = fork();
      if(pid == 0)
      {
         if(redirect_flag == 1)
         {
           int fd = open(file,O_RDWR|O_CREAT|O_TRUNC,0664);
           dup2(fd,1);
         }  
         else if(redirect_flag == 2)
         {
           int fd =  open(file,O_RDWR|O_CREAT|O_APPEND,0664);
           dup2(fd,1);
         } 

        execvp(argv[0],argv);
        exit(0);
      }
      waitpid(-1,NULL,0);
    }

}
