#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string> 
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h> 
#include <arpa/inet.h>

class UdpSocket
{
  private:
    int _sockfd;
  private:
    void MakeAddr(struct sockaddr_in &addr,const std::string &ip,const uint16_t &port)
    {
      addr.sin_family = AF_INET;
      addr.sin_port = htons(port);
      addr.sin_addr.s_addr = inet_addr(ip.c_str());
    }
  public:
    UdpSocket():_sockfd(-1){} 
    ~UdpSocket(){}
    bool Socket()
    {
      _sockfd = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
      if(_sockfd < 0)
      {
        perror("socket error");
        return false;
      }
      return true;
    }
    bool Bind(std::string &ip,uint16_t port)
    {
      struct sockaddr_in addr;
      MakeAddr(addr,ip,port);
      socklen_t len = sizeof(addr);
      int ret = bind(_sockfd,(struct sockaddr*)&addr,len);
      if(ret < 0)
      {
        perror("bind error");
        return false;
      }

      return true;
    }
    bool Recv(std::string &buf,struct sockaddr_in *saddr)
    {
      char tmp[1500] = {0};
      socklen_t len = sizeof(struct sockaddr_in);
      int ret = recvfrom(_sockfd,tmp,1500,0,(struct sockaddr*)saddr,&len);
      if(ret < 0)
      {
        perror("recvfrom error");
        return false;
      }
      buf.assign(tmp,ret);   
      return true;
    } 
    bool Send(std::string &buf,struct sockaddr_in *daddr)
    {
      socklen_t len = sizeof(struct sockaddr_in);
      int ret = sendto(_sockfd,buf.c_str(),buf.size(),0,(struct sockaddr*)daddr,len);
      if(ret < 0)
      {
        perror("sendto error");
        return false;
      }
      return true;
    }
    bool Close()
    {
      close(_sockfd);
      _sockfd = -1;
    }
};
