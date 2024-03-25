#include <iostream>
#include <winsock2.h>

#define PORT 65432

int main() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        std::cerr << "WSAStartup failed.\n";
        return 1;
    }

    SOCKET server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET)
    {
        std::cerr << "socket failed: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return 1;
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Forcefully attaching socket to the port 65432
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) == SOCKET_ERROR)
    {
        std::cerr << "bind failed: " << WSAGetLastError() << std::endl;
        closesocket(server_fd);
        WSACleanup();
        return 1;
    }

    if (listen(server_fd, SOMAXCONN) == SOCKET_ERROR)
    {
        std::cerr << "listen failed: " << WSAGetLastError() << std::endl;
        closesocket(server_fd);
        WSACleanup();
        return 1;
    }

    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, &addrlen)) == INVALID_SOCKET)
    {
        std::cerr << "accept failed: " << WSAGetLastError() << std::endl;
        closesocket(server_fd);
        WSACleanup();
        return 1;
    }

    // Actual communication
    char buffer[1024] = {0};
    int valread = recv(new_socket, buffer, sizeof(buffer), 0);
    if (valread == SOCKET_ERROR)
    {
        std::cerr << "recv failed: " << WSAGetLastError() << std::endl;
        closesocket(new_socket);
        closesocket(server_fd);
        WSACleanup();
        return 1;
    }

    std::cout << "Received: " << buffer << std::endl;

    const char* hello = "Hello from server";
    if (send(new_socket, hello, strlen(hello), 0) == SOCKET_ERROR)
    {
        std::cerr << "send failed: " << WSAGetLastError() << std::endl;
        closesocket(new_socket);
        closesocket(server_fd);
        WSACleanup();
        return 1;
    }

    std::cout << "Hello message sent\n";

    closesocket(new_socket);
    closesocket(server_fd);
    WSACleanup();
    return 0;
}
