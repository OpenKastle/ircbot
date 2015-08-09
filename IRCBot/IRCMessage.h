#pragma once

#include <string>

enum class IRCMessageType
{
    eUnrecognized,
    eJoin,
    ePart,
    eMessage,
    eMode
};

class IRCMessage
{
public:
    IRCMessage(const std::string& messageText);
    ~IRCMessage() = default;

    IRCMessageType GetType() const              { return m_type; }
    const std::string& GetNick() const          { return m_nick; }
    const std::string& GetUser() const          { return m_user; }
    const std::string& GetHost() const          { return m_host; }
    const std::string& GetCommand() const       { return m_command; }
    const std::string& GetTarget() const        { return m_target; }
    const std::string& GetParameters() const    { return m_parameters; }

private:
    IRCMessageType m_type = IRCMessageType::eUnrecognized;
    std::string m_nick;
    std::string m_user;
    std::string m_host;
    std::string m_command;
    std::string m_target;
    std::string m_parameters;
};