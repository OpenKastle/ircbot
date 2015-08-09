#pragma once

class ConnectionClient
{
public:
    virtual void ReceiveMessage(const std::string& message) = 0;
};