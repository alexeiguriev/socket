#include "CanManager.hpp"
#include <string.h>

CanManager::CanManager(const u_short port, const std::string inetAddr)
{
    _network = new Network(server, "", AF_INET, SOCK_STREAM, IPPROTO_TCP, port, inetAddr);
    _network->SetProcessingDataFunc(processingDataFunc);
    if (_type == server)
    {
        _network->SetNewClientFunc(newClientFunc);
    }
    _network->StartProcessingData();
    _initialazed = true;
}

CanManager::~CanManager()
{
    if (NULL != _network)
    {
        delete _network;
    }
    _initialazed = false;
}

void CanManager::newClientFunc(const SOCKET sock)
{
    std::cout << "New client connected, ID: " << sock << std::endl;
};

void CanManager::processingDataFunc(const SOCKET sock, const char *data, const int lenght)
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

bool Can::_initialazed = false;
Network *Can::_network{NULL};