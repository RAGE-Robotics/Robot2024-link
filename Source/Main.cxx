// Authors: Caleb Heydon <probablyme@caleb.engineer>
// License: CC0-1.0

#include "Link.hxx"

#include <vector>
#include <string>

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

    std::vector<std::string> vecAutoModes = {"Default"};
    Link link(vecAutoModes);

    while (true)
        ;

#ifdef _WIN32
    WSACleanup();
#endif

    return 0;
}
