#include <stdlib.h>
#include <stdio.h>
#include "tcpsocket.hpp"

void *thr_start(void *arg)
{
  TcpSocket *cli_sock = (TcpSocket*)arg;
  while(1)
  {
    std::string buf;
    bool ret = cli_sock->Recv(buf);
    if(ret == false)
    {
      cli_sock->Close();
      continue;
    }
    std::cout<<"client say:"<<buf<<"\n";
    std::cout<<"server say:";
    fflush(stdout);
    buf.clear();
    std::cin>>buf;
    ret = cli_sock->Send(buf);
    if(ret == false)
    {
      cli_sock->Close();
      continue;
    }
  }
  delete cli_sock;
  cli_sock->Close();
  return NULL;
}

int main(int argc,char *argv[])
{
  if(argc != 3)
  {
    std::cout<<"./tcp_srv ip port\n";
    return -1;
  }
  std::string ip = argv[1];
  uint16_t port = atoi(argv[2]);
  TcpSocket lst_sock;
  CHECK_RET(lst_sock.Socket());
  CHECK_RET(lst_sock.Bind(ip,port));
  CHECK_RET(lst_sock.Listen());
  while(1)
  {
    TcpSocket *cli_sock = new TcpSocket();
    std::string cli_ip;
    uint16_t cli_port;
    if(lst_sock.Accept(cli_sock,&cli_ip,&cli_port) == false)
    {
      continue;
    }
    std::cout<<"new connect:"<<cli_ip<<":"<<cli_port<<"\n";
    pthread_t tid;
    pthread_create(&tid,NULL,thr_start,(void*)cli_sock);
    pthread_detach(tid);
  }
  lst_sock.Close();
  return 0;
}
