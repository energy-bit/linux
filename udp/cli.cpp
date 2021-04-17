#include "../udp.hpp"

#define CHECK_RET(p) if(p < 0){return -1;}

int main(int argc, char* argv[])
{
    // ./cli [ip] [port]
    if(argc != 3)
    {
        printf("using ./cli [ip] [port]\n");
        return -1;
    }

    UdpSvr us;
    CHECK_RET(us.CreatSocket());

    std::string ip = argv[1]; 
    uint16_t port = atoi(argv[2]);

    struct sockaddr_in dest_addr;
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(port);
    dest_addr.sin_addr.s_addr = inet_addr(ip.c_str());

    while(1)
    {
        std::string data;
        std::cout << "please enter msg to svr:";
        fflush(stdout);
        std::cin >> data;

        int ret = us.Sendto(data, &dest_addr);
        if(ret < 0)
        {
            continue;
        }
        
        data.clear();

        struct sockaddr_in peer_addr;
        ret = us.Recvfrom(&data, &peer_addr);
        if(ret < 0)
        {
            continue;
        }

        std::cout << "svr say: " << data << std::endl;
    }

    us.Close();
    
    return 0;
}
