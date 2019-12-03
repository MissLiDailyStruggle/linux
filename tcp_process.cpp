#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/wait.h>
#include "tcpsocket.hpp"

void sigcb(int signo)
{
  while(waitpid(-1,NULL,WNOHANG) > 0);
}

int main(int argc,char *argv[])
{
  if(argc != 3)
  {
    std::cout<<"./tcp_process ip port\n";
    return -1;
  }
  signal(SIGCHLD,sigcb);
  std::string ip = argv[1];
  uint16_t port = atoi(argv[2]);
  TcpSocket lst_sock;
  CHECK_RET(lst_sock.Socket());
  CHECK_RET(lst_sock.Bind(ip,port));
  CHECK_RET(lst_sock.Listen());
  while(1)
  {
    TcpSocket cli_sock;
    if(lst_sock.Accept(&cli_sock) == false)
    {
      continue;
    }
    std::cout<<"new connect"<<"\n";
    pid_t pid = fork();
    if(pid < 0)
    {
      cli_sock.Close();
      continue;
    }
    else if(pid == 0)
    {
      while(1)
      {
        std::string buf;
        cli_sock.Recv(buf);
        std::cout<<"client say:"<<buf<<"\n";
        std::cout<<"server say:";
        fflush(stdout);
        buf.clear();
        std::cin>>buf;
        cli_sock.Send(buf);
      }
      cli_sock.Close();
      exit(0);
    }
    cli_sock.Close();
  }
  lst_sock.Close();
  return 0;  
}
