#ifndef CAN_NETWORK_HPP
#define CAN_NETWORK_HPP

#include "Network.hpp"

template<typename DataType>
class CanNetwork : public Network<DataType>
{
    public:
        CanNetwork(NetworkType type, std::string_view name, const int af, const int soketType, const int protocol, const u_short port,
                   const std::string inetAddr);

        ~CanNetwork();

    protected:
        virtual DataType* DataToStrConverion(char *charStr) override;
        virtual char* StrToDataConverion(DataType* data) override;
};

#endif /* CAN_NETWORK_HPP */

