//封装tcpsocket类，通过实例化的tcpsocket对象，来实现数据的通信（向外提供简单的接口）
#ifndef __M_TCP_H__
#define __M_TCP_H__
#include <iostream>
#include <string>
#include <stdio.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define CHECK_RET(q) if((q) == false){return -1;}

#define MAX_LISTEN 10

class TcpSocket
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
    TcpSocket():_sockfd(-1){}
    //1、创建套接字
    bool Socket()
    {
      _sockfd = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
      if(_sockfd < 0)
      {
        perror("socket error:");
        return false;
      }
      return true;
    }
    //2、绑定地址信息
    bool Bind(const std::string &ip,const uint16_t &port)
    {
      struct sockaddr_in addr;
      MakeAddr(addr,ip,port);
      socklen_t len = sizeof(addr);
      int ret = bind(_sockfd,(struct sockaddr*)&addr,len);
      if(ret < 0)
      {
        perror("bind error:");
        return false;
      }
      return true;
    }
    //3、开始监听（服务端）
    bool Listen(int backlog = MAX_LISTEN)
    {
      int ret = listen(_sockfd,backlog);
      if(ret < 0)
      {
        perror("listen error:");
        return false;
      }
      return true;
    }
    //3、发起连接请求（客户端）
    bool Connect(const std::string &serip,const uint16_t &serport)
    {
      struct sockaddr_in addr;
      MakeAddr(addr,serip,serport);
      socklen_t len = sizeof(addr);
      int ret = connect(_sockfd,(struct sockaddr*)&addr,len);
      if(ret < 0)
      {
        perror("connect error:");
        return false;
      }
      return true;
    }
    //4、获取新连接（服务端）
    bool Accept(TcpSocket *sock,std::string *ip,uint16_t *port)
    {
      struct sockaddr_in addr;
      socklen_t len = sizeof(addr);
      int newfd = accept(_sockfd,(struct sockaddr*)&addr,&len);
      if(newfd < 0)
      {
        perror("accept error:");
        return false;
      }
      sock->_sockfd = newfd;
      if(ip != NULL)
      {
        *ip = inet_ntoa(addr.sin_addr);
      }
      if(port !=NULL)
      {
        *port = ntohs(addr.sin_port);
      }
      return true;
    }
    //5、接收数据
    bool Recv(std::string &buf)
    {
      char tmp[4096] = {0};
      int rlen = recv(_sockfd,tmp,4096,0);
      if(rlen < 0)
      {
        perror("recv error:");
        return false;
      }
      else if(rlen == 0)
      {
        std::cerr<<"perr shutdown\n";
        return false;
      }
      buf.assign(tmp,rlen);
      return true;
    }
    //6、发送数据
    bool Send(std::string &buf)
    {
      int ret = send(_sockfd,&buf[0],buf.size(),0);
      if(ret < 0)
      {
        perror("send error:");
        return false;
      }
      return true;
    }
    //7、关闭套接字
    bool Close()
    {
      close(_sockfd);
      return true;
    }
 
};
#endif
