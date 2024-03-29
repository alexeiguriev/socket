#ifndef CHAT_HPP
#define CHAT_HPP

#include "Network.hpp"

class Chat
{
    public:
        Chat(NetworkType type, const u_short port, const std::string inetAddr);

        ~Chat();
        static void ClientSentMsg(const char *data, const int lenght);

    
    private:
        static bool _initialazed;
        static NetworkType _type;
        static Network *_network;

        static void newClientFunc(const SOCKET sock);
        static void processingDataFunc(const SOCKET sock, const char *data, const int lenght);
};

#endif /* CHAT_HPP */

