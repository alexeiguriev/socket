#include <iostream>
#include <winsock2.h>
#include <vector>
#include <thread>
#include <functional>

#include "Network.hpp"

#define PORT 65432

std::string inetAddr("192.168.56.1");
Network network(client,"TestServer",AF_INET,SOCK_STREAM,IPPROTO_TCP,INADDR_ANY,PORT,inetAddr);

void PrepareAnswer(const SOCKET sock, const char *data,const int lenght)
{
    network.Send(data,lenght,sock);
}

int main()
{
    NetworkDataProcessingFuncType processingDataFunc = [](const SOCKET sock, const char *data, const int lenght)
    {
        std::cout << "New data: " << data << std::endl;
        //PrepareAnswer(sock, data, lenght);
    };
    
    network.SetProcessingDataFunc(processingDataFunc);

    network.StartProcessingData();

    while(true)
    {
        std::cout << "Input a new message: ";
        std::string msg;
        std::cin >> msg;
        std::cout << std::endl;

        network.Send(msg.c_str(), msg.length());
    }

    std::cin.get();

    return 0;
}
