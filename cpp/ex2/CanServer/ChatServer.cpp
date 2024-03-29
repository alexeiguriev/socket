#include <iostream>
#include <winsock2.h>
#include <vector>
#include <thread>
#include <functional>

#include "Chat.hpp"

#define PORT 65432
std::string inetAddr("192.168.56.1");

int main()
{
    Chat chat(server,PORT,inetAddr);

    std::cin.get();

    return 0;
}
