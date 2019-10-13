#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>


int main()
{
   while(1)
   {
      printf("[nbsun@localhost]# ");
      fflush(stdout); 
      char buf[1024] = {0};
      fgets(buf,1023,stdin);
      buf[strlen(buf) -1] = '\0';
      printf("cmd:[%s]\n",buf);

      int  argc = 0;
      char *argv[32] = {NULL};
      char *ptr = buf;
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

      int i = 0;
      for(i = 0; i < argc;i++)
      {
         printf("[%s]\n",argv[i]);
      }
      int pid = fork();
      if(pid == 0)
      {
        execvp(argv[0],argv);
        exit(0);
      }
      waitpid(-1,NULL,0);
    }
 }
