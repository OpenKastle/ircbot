#pragma once

#include "Connection.h"
#include "ConnectionClient.h"
#include "IRCMessage.h"

#include <iostream>
#include <vector>
#include <map>
#include <functional>
#include <memory>

class IRCMessage;

class IRCBot : public ConnectionClient
{
public:
    IRCBot(const std::string& botNick,
           const std::string& server,
           const std::string& port,
           const std::string& channel);

    virtual ~IRCBot() = 0;

    void Start();
    void Stop();
    void ReceiveMessage(const std::string& message);

    // Message handlers to be overriden by derived IRC bots
    virtual void OnPublicMessage(const IRCMessage& message) {}
    virtual void OnPrivateMessage(const IRCMessage& message) {}
    virtual void OnJoin(const IRCMessage& message) {}
    virtual void OnPart(const IRCMessage& message) {}
    virtual void OnMode(const IRCMessage& message) {}

protected:
    // Commands
    void Say(const std::string& message);
    void PrivateSay(const std::string& message, const std::string& user);
    void Kick(const std::string& nick, const std::string& reason);
    void Mode(const std::string& nick, const std::string& mode);

    const std::string& GetBotNick()     { return m_botNick; }
    const std::string& GetServer()      { return m_server; }
    const std::string& GetPort()        { return m_port; }
    const std::string& GetChannel()     { return m_channel; }

private:
    void Send(const std::string& message);
    void HandleMessage(const IRCMessage& message);
    void OnMessage(const IRCMessage& message);

    std::string m_botNick;
    std::string m_server;
    std::string m_port;
    std::string m_channel;

    std::unique_ptr<Connection> m_pConnection;

    unsigned int m_messagesReceived = 0;

    using IRCMessageHandler = std::function<void(const IRCMessage&)>;
    std::map<IRCMessageType, IRCMessageHandler> m_messageHandlers;
};