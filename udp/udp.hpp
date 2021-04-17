#pragma once

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <iostream>
#include <string>

class UdpSvr
{
    private:
        int sockfd_;

    public:
        UdpSvr()
        {
            sockfd_ = -1;
        }
        ~UdpSvr()
        {

        }

        //创建套接字
        int CreatSocket()
        {
            sockfd_ = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
            if(sockfd_ < 0)
            {
                perror("socket");
                return -1;
            }
            return 0;
        }

        int Bind(std::string ip, uint16_t port)
        {
            struct sockaddr_in addr;
            addr.sin_family = AF_INET;
            addr.sin_port = htons(port);
            addr.sin_addr.s_addr = inet_addr(ip.c_str());
            int ret = bind(sockfd_, (struct sockaddr*)&addr, sizeof(addr));
            if(ret < 0)
            {
                perror("bind");
                return -1;
            }

            return 0;
        }

        int Sendto(std::string data, struct sockaddr_in* dest_addr)
        {
            ssize_t send_size = sendto(sockfd_, data.c_str(), data.size(), 0 ,(struct sockaddr*)dest_addr, sizeof(struct sockaddr_in));
            if(send_size < 0)
            {
                perror("sendto");
                return -1;
            }
            return send_size;
        }

        int Recvfrom(std::string* data, struct sockaddr_in* peer_addr)
        {
            char buf[1024] = {0};
            socklen_t peer_addr_len = sizeof(struct sockaddr_in);
            ssize_t recv_size = recvfrom(sockfd_, buf, sizeof(buf) - 1, 0, (struct sockaddr*)peer_addr, &peer_addr_len);
            if(recv_size < 0)
            {
                perror("recvfrom");
                return -1;
            }

            data->assign(buf, strlen(buf));
            return recv_size;
        }

        void Close()
        {
            close(sockfd_);
        }
};
