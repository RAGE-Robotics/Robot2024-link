// Authors: Caleb Heydon <probablyme@caleb.engineer>
// License: CC0-1.0

#include "Link.hxx"

#ifdef _WIN32
#include <WinSock2.h>
#include <stdexcept>
#endif

int main()
{
#ifdef _WIN32
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
        throw std::runtime_error("failed to initialize winsock");
#endif

    Link link;

    while (true)
        ;

#ifdef _WIN32
    WSACleanup();
#endif

    return 0;
}
