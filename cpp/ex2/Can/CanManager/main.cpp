#include <iostream>
#include <winsock2.h>
#include <vector>
#include <thread>
#include <functional>

#include "CanNetwork.hpp"

#define PORT 65432

std::string inetAddr("192.168.56.1");
// Network(NetworkType type, std::string_view name, const int af, const int soketType, const int protocol, const u_short port,
//                 const std::string inetAddr);
CanNetwork<char> canNetwork(server,"TestServer",AF_INET,SOCK_STREAM,IPPROTO_TCP,PORT,inetAddr);
// Network<int> network(10);

int main()
{
    // MyClass<int> intObj(5);

    //  intObj.display();
    // TestClass<> testClass(100);
    //Network 
    return 0;
}