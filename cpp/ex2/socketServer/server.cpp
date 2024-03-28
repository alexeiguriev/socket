#include <iostream>
#include <winsock2.h>
#include <vector>
#include <thread>
#include <functional>

#include "Network.hpp"

#define PORT 65432

std::string inetAddr("192.168.56.1");
Network network(server,"TestServer",AF_INET,SOCK_STREAM,IPPROTO_TCP,INADDR_ANY,PORT,inetAddr);

void PrepareAnswer(const SOCKET sock, const char *data,const int lenght)
{
    network.Send(data,lenght,sock);
}

int main()
{
    NetworkNewClientConnectedFuncType newClientFunc = [](const SOCKET sock)
    {
        std::cout << "New client connected, ID: " << sock << std::endl;
    };
    NetworkDataProcessingFuncType processingDataFunc = [](const SOCKET sock, const char *data, const int lenght)
    {
        std::cout << "Client with ID: " << sock << " sent data: " << data << std::endl;
        PrepareAnswer(sock, data, lenght);
    };
    
    network.SetProcessingDataFunc(processingDataFunc);
    network.SetNewClientFunc(newClientFunc);

    network.StartProcessingData();

    std::cin.get();

    return 0;
}
