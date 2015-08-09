#pragma once

#include <string>
#include <WinSock2.h>

class ConnectionClient;

class Connection
{
public:
    Connection(ConnectionClient* pConnectionClient);
    ~Connection();

    bool Connect(const std::string& server,
                 const std::string& port);

    void StartListening();
    void Send(const std::string& message);

private:
    SOCKET m_socket = INVALID_SOCKET;
    ConnectionClient* m_pConnectionClient = nullptr;
};