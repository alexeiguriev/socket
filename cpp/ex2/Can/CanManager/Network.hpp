#ifndef NETWORK_HPP
#define NETWORK_HPP

#include <iostream>
#include <winsock2.h>
#include <string>
#include <vector>

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

typedef void (*NetworkDataProcessingFuncType)(const SOCKET sock, const char *data, const int lenght);
typedef void (*NetworkNewClientConnectedFuncType)(const SOCKET sock);

// template <typename DataType = char*>
template <typename DataType>
class Network
{
    public:
        Network(NetworkType type, std::string_view name, const int af, const int soketType, const int protocol, const u_short port,
                const std::string inetAddr);

        ~Network();

        void StartProcessingData();

        void SetProcessingDataFunc(NetworkDataProcessingFuncType func);

        void SetNewClientFunc(NetworkNewClientConnectedFuncType func);

        int Send(const char* data, const int lenght,const SOCKET sock);
        /* used for clients only */
        int Send(const char* data, const int lenght);

        std::vector<SOCKET> GetAllConnectedClents();

        NetworkType type();

        const std::string_view name();

    private:
        WSADATA _wsaData;
        
        SOCKET _socket_fd{INVALID_SOCKET};
        struct sockaddr_in _address;
        std::vector<SOCKET> _clientSockets; // Vector to store connected client sockets
        std::vector<char*> _data;
        std::vector<int> _dataSize;

        NetworkType _type;
        StatusType _status{StatusNotInit};
        std::string_view _name;
        NetworkDataProcessingFuncType dataProcessingFunc{NULL};
        NetworkNewClientConnectedFuncType newClientConnectedFunc{NULL};

        void acceptClients();
        void receiveClientData(const SOCKET sock);
        void clientSocketRealese(const SOCKET sock);
        int getIdBySocket(const SOCKET sock);

        protected:
        virtual DataType* DataToStrConverion(char *charStr);
        virtual char* StrToDataConverion(DataType* data);
};

#endif /* NETWORK_HPP */

