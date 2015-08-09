#include "IRCBot.h"

#include <assert.h>

IRCBot::IRCBot(const std::string& botNick,
               const std::string& server,
               const std::string& port,
               const std::string& channel)
    : m_botNick(botNick),
      m_server(server),
      m_port(port),
      m_channel(channel)
{
    using namespace std::placeholders;

    m_messageHandlers[IRCMessageType::eMessage] = std::bind(&IRCBot::OnMessage, this, _1);
    m_messageHandlers[IRCMessageType::eJoin] = std::bind(&IRCBot::OnJoin, this, _1);
    m_messageHandlers[IRCMessageType::ePart] = std::bind(&IRCBot::OnPart, this, _1);
    m_messageHandlers[IRCMessageType::eMode] = std::bind(&IRCBot::OnMode, this, _1);
}

IRCBot::~IRCBot()
{
}

void IRCBot::Start()
{
    m_pConnection = std::make_unique<Connection>(this);
    if (!m_pConnection->Connect(m_server, m_port))
    {
        assert(false); // Failed to start a connection
        exit(1);
    }

    m_pConnection->StartListening();
}

void IRCBot::Send(const std::string& message)
{
    if (message.rfind('\n') != message.size() - 1)
        std::cout << "\n";

    m_pConnection->Send(message);
}

void IRCBot::Say(const std::string& message)
{
    std::string outMessage = "PRIVMSG " + m_channel + " :" + message + "\r\n";

    m_pConnection->Send(outMessage);
}

void IRCBot::PrivateSay(const std::string& message, const std::string& user)
{
    std::string outMessage = "PRIVMSG " + user + " :" + message + "\r\n";

    m_pConnection->Send(outMessage);
}

void IRCBot::Kick(const std::string& nick, const std::string& reason)
{
    Send("KICK " + GetChannel() + " " + nick + " :" + reason + "\r\n");
}

void IRCBot::Mode(const std::string& nick, const std::string& mode)
{
    Send("MODE " + GetChannel() + " " + mode + " " + nick + "\r\n");
}

void IRCBot::ReceiveMessage(const std::string& message)
{
    ++m_messagesReceived;

    if (m_messagesReceived < 3)
    {
        // Do nothing
    }

    // After three receives, we send our nick name and user name
    else if (m_messagesReceived == 3)
    {
        Send("NICK " + m_botNick + "\r\n");
        Send("USER guest 0 * :" + m_botNick + "\r\n");
    }

    // After 4 receives, we can join a channel
    else if (m_messagesReceived == 4)
    {
        Send("JOIN " + m_channel + "\r\n");
    }

    // PONG to PINGs to avoid getting disconnected
    else if (message.find("PING") != std::string::npos)
    {
        Send("PONG ");
    }

    // Otherwise just send this message to the derived bot for handling
    else
    {

        HandleMessage(IRCMessage(message));
    }
}

void IRCBot::HandleMessage(const IRCMessage& message)
{
    auto find = m_messageHandlers.find(message.GetType());
    if (find != end(m_messageHandlers))
    {
        find->second(message);
    }
}

void IRCBot::OnMessage(const IRCMessage& message)
{
    // We can't differentiate public messages from private messages based on the keyword
    // alone so we can't use the keyword as a key to the message handler.  So we just
    // handle those messages manually here.
    if (message.GetType() == IRCMessageType::eMessage)
    {
        if (message.GetTarget() == GetChannel())
        {
            OnPublicMessage(message);
        }
        else if (message.GetTarget() == GetBotNick())
        {
            OnPrivateMessage(message);
        }
    }
}