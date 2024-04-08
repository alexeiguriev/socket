#include <iostream>
#include <winsock2.h>
#include <vector>
#include <thread>
#include <functional>

#include "Chat.hpp"

#define PORT 65432
//std::string inetAddr("192.168.56.1");
std::string inetAddr("127.0.0.1");
int main()
{
    // Chat(server,PORT,inetAddr);

    // Chat::Init(server,PORT,inetAddr);
    Chat chat(client,PORT,inetAddr);

    while(true)
    {
        std::string msg;
        std::getline(std::cin, msg);
        std::cout << std::endl;

        chat.ClientSentMsg(msg.c_str(), msg.length());
    }

    std::cin.get();

    return 0;
}
