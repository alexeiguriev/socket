#ifndef CAN_HPP
#define CAN_HPP

#include "Network.hpp"

class CanNode
{
    public:
        Can(NetworkType type, const u_short port, const std::string inetAddr);

        ~Can();
        static void ClientSentMsg(const char *data, const int lenght);

    
    private:
        static bool _initialazed;
        static NetworkType _type;
        static Network *_network;

        static void newClientFunc(const SOCKET sock);
        static void processingDataFunc(const SOCKET sock, const char *data, const int lenght);
};

#endif /* CAN_HPP */

