#include <thread>
#include <functional>

#include "Network.hpp"

Network::Network(NetworkType type, std::string_view name, const int af,const int soketType,const int protocol, const u_long addr, const u_short port,
        const std::string inetAddr)
{
    _type = type;
    _name = name;

    if (WSAStartup(MAKEWORD(2, 2), &_wsaData) != 0)
    {
        std::cerr << "WSAStartup failed.\n";
        return;
    }

    // Creating socket file descriptor
    if ((_socket_fd = socket(af, soketType, protocol)) == INVALID_SOCKET)
    {
        std::cerr << "socket failed: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return;
    }

    _address.sin_family = af;
    _address.sin_port = htons(port);

    if (type == client)
    {
        _address.sin_addr.s_addr = inet_addr(inetAddr.c_str());
        if (connect(_socket_fd, (struct sockaddr*)&_address, sizeof(_address)) == SOCKET_ERROR)
        {
            std::cerr << "connection failed: " << WSAGetLastError() << std::endl;
            closesocket(_socket_fd);
            WSACleanup();
            return;
        }
    }
    else if (type == server)
    {
        _address.sin_addr.s_addr = addr;
        // Forcefully attaching socket to the port 65432
        if (bind(_socket_fd, (struct sockaddr *)&_address, sizeof(_address)) == SOCKET_ERROR)
        {
            std::cerr << "bind failed: " << WSAGetLastError() << std::endl;
            closesocket(_socket_fd);
            WSACleanup();
            return;
        }

        if (listen(_socket_fd, SOMAXCONN) == SOCKET_ERROR)
        {
            std::cerr << "listen failed: " << WSAGetLastError() << std::endl;
            closesocket(_socket_fd);
            WSACleanup();
            return;
        }
    }

    std::cout << "Network successful created! \n";

    _status = StatusInit;
}

Network::~Network()
{
    
    closesocket(_socket_fd);

    // Clear all vector data
    for (char* clientData : _data)
    {
        delete[] clientData; // Free the memory for each char* buffer
    }
    _data.clear();

    // Close all client sockets
    for (SOCKET sock : _clientSockets)
    {
        closesocket(sock); // Close each socket
    }
    _clientSockets.clear(); // Clear the vector (optional, as it's empty after closing sockets)


    closesocket(_socket_fd);
    WSACleanup();

}

void Network::StartProcessingData()
{
    if (_type == server)
    {
        std::thread acceptThread(std::bind(&Network::acceptClients, this));
        acceptThread.detach();
    }
    else if (_type == client)
    {
        _clientSockets.push_back(_socket_fd);
        _data.push_back(NULL);
        std::thread recvThread(std::bind(&Network::receiveClientData, this,_socket_fd));
        recvThread.detach();
    }
}

void Network::SetProcessingDataFunc(NetworkDataProcessingFuncType func)
{
    if (NULL != func)
    {
        dataProcessingFunc = func;
    }
}

void Network::SetNewClientFunc(NetworkNewClientConnectedFuncType func)
{
    if (NULL != func)
    {
        newClientConnectedFunc = func;
    }
}

int Network::Send(const char* data, const int lenght,const SOCKET sock)
{
    int id = getIdBySocket(sock);
    int status = (-1);

    status = send(_clientSockets[id], data, lenght, 0);
    if (status == SOCKET_ERROR)
    {
        std::cerr << "send failed: " << WSAGetLastError() << std::endl;
    }
    return status;
}

int Network::Send(const char* data, const int lenght)
{
    int status = (-1);
    if (client == _type)
    {
        status = Send(data,lenght,_socket_fd);
    }
    return status;
}

NetworkType Network::type()
{
    return _type;
}

const std::string_view Network::name()
{
    return _name;
}


void Network::acceptClients()
{
    int addrlen = sizeof(_address);
    while(true)
    {
        if ((_socket_fd != INVALID_SOCKET) && (_type == server))
        {
            SOCKET newSocket = accept(_socket_fd, (struct sockaddr *)&_address, &addrlen);
            if (newSocket != INVALID_SOCKET)
            {
                _clientSockets.push_back(newSocket);
                _data.push_back(NULL);
                if (NULL != newClientConnectedFunc)
                {
                    newClientConnectedFunc(newSocket);
                }                
                std::thread recvThread(std::bind(&Network::receiveClientData, this,newSocket));
                recvThread.detach();
            }
        }
    }
}

void Network::receiveClientData(const SOCKET sock)
{
    int id = getIdBySocket(sock);
    while(true)
    {        
        // Actual communication
        char buffer[1024] = {0};
        int valread = recv(sock, buffer, sizeof(buffer), 0);
        if (valread <= 0)
        {
            break;
        }
        
        if (valread > 0)
        {
            _data[id] = new char[valread];
            memcpy(_data[id],buffer,valread);
            if (NULL != dataProcessingFunc)
            {
                dataProcessingFunc(sock,_data[id],valread);
            }
        }
    }
    clientSocketRealese(sock);
}

void Network::clientSocketRealese(const SOCKET sock)
{
    int id = getIdBySocket(sock);

    if (id >= 0)
    {
        _clientSockets.erase(_clientSockets.begin() + id);
        _data.erase(_data.begin() + id);
        std::cout << "Client with ID: " << sock << " Disconected." << std::endl;
    }    
}

int Network::getIdBySocket(const SOCKET sock)
{
    auto it = std::find(_clientSockets.begin(), _clientSockets.end(), sock);
    int index;

    if (it != _clientSockets.end())
    {
        index = std::distance(_clientSockets.begin(), it);
    }
    else
    {
        index = -1;
    }

    return index;
}
