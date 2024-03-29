#include "Chat.hpp"
#include <string.h>

Chat::Chat(NetworkType type,const u_short port, const std::string inetAddr)
{
    _type = type;
    _network = new Network(type, "", AF_INET, SOCK_STREAM, IPPROTO_TCP, port, inetAddr);
    _network->SetProcessingDataFunc(processingDataFunc);
    if (_type == server)
    {
        _network->SetNewClientFunc(newClientFunc);
    }
    _network->StartProcessingData();
    _initialazed = true;
}
Chat::~Chat()
{
    if (NULL != _network)
    {
        delete _network;
    }
    _initialazed = false;
}

void Chat::ClientSentMsg(const char* data, const int lenght)
{
    _network->Send(data,lenght);
}

void Chat::newClientFunc(const SOCKET sock)
{
    std::cout << "New client connected, ID: " << sock << std::endl;
};

void Chat::processingDataFunc(const SOCKET sock, const char *data, const int lenght)
{
    if (_type == server)
    {
        std::vector<SOCKET> allClients = _network->GetAllConnectedClents();
        for (SOCKET client : allClients)
        {
            if (sock != client)
            {
                _network->Send(data,lenght,client);
            }
        }
    }
    if (_type == client)
    {
        char *lclData = new char[lenght + 1];
        lclData[lenght] = '\0';
        memcpy(lclData,data,lenght);
        
        std::cout << lclData << std::endl;

        delete[] lclData;
    }
};

bool Chat::_initialazed = false;
NetworkType Chat::_type;
Network *Chat::_network{NULL};