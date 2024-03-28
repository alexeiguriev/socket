#include <iostream>
#include <winsock2.h>
#include <vector>
#include <thread>
#include <functional>

#define PORT 65432

typedef enum NetworkType
{
    server,
    client
}NetworkType;

typedef enum StatusType
{
    StatusNotInit,
    StatusInit
}StatusType;

typedef void (*NetworkDataProcessingFuncType)(const int id, const char *data);
typedef void (*NetworkNewClientConnectedFuncType)(const int id);
bool closeApp = false;

class Network
{
    public:

        Network(NetworkType type, std::string_view name, const int af,const int soketType,const int protocol, const u_long addr, const u_short port,
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
        
        ~Network()
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

        static void test()
        {

        }

        void StartLookingForClients()
        {
            if (_type == server)
            {
                std::thread acceptThread(std::bind(&Network::acceptClients, this));
                acceptThread.detach();
            }
        }

        void SetProcessingDataFunc(NetworkDataProcessingFuncType func)
        {
            if (NULL != func)
            {
                dataProcessingFunc = func;
            }
        }
        
        void SetNewClientFunc(NetworkNewClientConnectedFuncType func)
        {
            if (NULL != func)
            {
                newClientConnectedFunc = func;
            }
        }

        NetworkType type()
        {
            return _type;
        }

        const std::string_view name()
        {
            return _name;
        }


    private:
        WSADATA _wsaData;
        
        SOCKET _socket_fd{INVALID_SOCKET};
        struct sockaddr_in _address;
        std::vector<SOCKET> _clientSockets; // Vector to store connected client sockets
        std::vector<char*> _data;

        NetworkType _type;
        StatusType _status{StatusNotInit};
        std::string_view _name;
        NetworkDataProcessingFuncType dataProcessingFunc{NULL};
        NetworkNewClientConnectedFuncType newClientConnectedFunc{NULL};

        void acceptClients()
        {
            int addrlen = sizeof(_address);
            while(!closeApp)
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
                            newClientConnectedFunc(_clientSockets.size() - 1);
                        }
                        int id = _clientSockets.size() - 1;
                        std::thread recvThread(std::bind(&Network::receiveClientData, this,newSocket,id));
                        recvThread.detach();
                    }
                }
            }
        }
        
        void receiveClientData(SOCKET sock,int id)
        {
            while(!closeApp)
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
                        dataProcessingFunc(id,_data[id]);
                    }
                }
            }
        }
};

int main()
{

    std::string inetAddr("192.168.56.1");
    Network network(server,"TestServer",AF_INET,SOCK_STREAM,IPPROTO_TCP,INADDR_ANY,PORT,inetAddr);

    NetworkNewClientConnectedFuncType newClientFunc = [](const int id)
    {
        std::cout << "New client connected, ID: " << id << std::endl;
    };
    NetworkDataProcessingFuncType processingDataFunc = [](const int id, const char *data)
    {
        std::cout << "Client with ID: " << id << " sent data: " << data << std::endl;
    };
    
    network.SetProcessingDataFunc(processingDataFunc);
    network.SetNewClientFunc(newClientFunc);

    network.StartLookingForClients();

    std::cin.get();

    return 0;
}
