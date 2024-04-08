#ifndef CAN_HPP
#define CAN_HPP

#include "Network.hpp"

class CanManager
{
    public:
        CanManager(const u_short port, const std::string inetAddr);

        ~CanManager();

    
    private:
        static bool _initialazed;
        static Network *_network;

        static void newClientFunc(const SOCKET sock);
        static void processingDataFunc(const SOCKET sock, const char *data, const int lenght);
};

#endif /* CAN_HPP */

