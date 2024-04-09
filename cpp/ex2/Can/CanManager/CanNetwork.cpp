#include <thread>
#include <functional>

#include "Network.hpp"
#include "CanNetwork.hpp"

template <typename DataType>
CanNetwork<DataType>::CanNetwork(NetworkType type, std::string_view name, const int af,const int soketType,const int protocol, const u_short port,
        const std::string inetAddr) : Network<DataType>(type, name, af, soketType, protocol, port, inetAddr)
{
}

template <typename DataType>
CanNetwork<DataType>::~CanNetwork()
{
}

template <typename DataType>
DataType* CanNetwork<DataType>::DataToStrConverion(char *charStr) 
{
    return (DataType*)(charStr);
    // return static_cast<DataType*>(charStr);
}

template <typename DataType>
char* CanNetwork<DataType>::StrToDataConverion(DataType* data)
{
    return (char*)data;
}

template class CanNetwork<char>;
template class CanNetwork<uint8_t>;
