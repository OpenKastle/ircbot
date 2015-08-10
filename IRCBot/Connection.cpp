#include "Connection.h"
#include "ConnectionClient.h"

#include <assert.h>
#include <cstdio>
#include <iostream>
#include <vector>

#include <WS2tcpip.h>

#ifdef max
#undef max
#endif

Connection::Connection(ConnectionClient* pConnectionClient)
    : m_pConnectionClient(pConnectionClient)
{
    WSADATA wsaData;

    // Initialize Winsock
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0)
    {
        assert(false); // Winsock failed to initialize
        exit(1);
    }
}

Connection::~Connection()
{
    // Close the socket and terminate Winsock
    closesocket(m_socket);
    WSACleanup();
}

bool Connection::Connect(const std::string& server,
                         const std::string& port)
{
    // Initialize address info variables
    ADDRINFO* addressInfo = nullptr;
    ADDRINFO hints;

    // Zero out the memory for the hint address info
    memset(&hints, 0, sizeof(hints));

    // IP4 vs. IP6 is unspecified
    hints.ai_family = AF_UNSPEC;

    // Use a stream socket
    hints.ai_socktype = SOCK_STREAM;

    // Use the TCP protocol
    hints.ai_protocol = IPPROTO_TCP;

    int connectionResult = std::numeric_limits<int>::max();
    int connectionAttempt = 0;
    while (connectionResult != 0)
    {
        // Resolve the server address and port
        int result = GetAddrInfo(server.c_str(), port.c_str(), &hints, &addressInfo);
        if (result != 0)
        {
            assert(false); // Failed to resolve server address and port
            return false;
        }

        // Attempt to create a socket from the resolved address
        m_socket = socket(addressInfo->ai_family,
                          addressInfo->ai_socktype,
                          addressInfo->ai_protocol);

        if (m_socket == INVALID_SOCKET)
        {
            assert(false); // Failed to create a valid socket
            return false;
        }

        std::cout << "Connection attempt " << connectionAttempt << "...\n";

        // Attempt to make a connection to the resolved address
        connectionResult = connect(m_socket, addressInfo->ai_addr, static_cast<int>(addressInfo->ai_addrlen));
        if (connectionResult != 0)
        {
            // Failed to make a connection
            std::cout << "Connection failed with error code " << WSAGetLastError() << "\n";
            ++connectionAttempt;
        }
        else
        {
            std::cout << "Now connected to " << server << ":" << port << "\n";
        }
    }

    // Clean up the address info resources
    FreeAddrInfo(addressInfo);

    // Check to make sure the connected socket is valid
    if (m_socket == INVALID_SOCKET)
    {
        assert(false); // Failed to connect with a valid socket
        return false;
    }

    return true;
}

void Connection::StartListening()
{
    const int bufMax = 1024;
    char recvBuf[bufMax];

    int result = std::numeric_limits<int>::max();
    while (true)
    {
        // recv is blocking
        result = recv(m_socket, recvBuf, bufMax, 0);
        if (result > 0)
        {
            // Send the message to the connection client to be handled
            m_pConnectionClient->ReceiveMessage(std::string(recvBuf));
        }
        else if (result == 0)
        {
            // Connect is closed
            return;
        }
        else
        {
            assert(false); // recv failed
            return;
        }

        // Clear the buffer
        memset(recvBuf, 0, bufMax);
    }
}

void Connection::Send(const std::string& message)
{
    std::vector<char> writable(begin(message), end(message));
    char* c = &writable[0];

    send(m_socket, c, writable.size(), 0);
}
