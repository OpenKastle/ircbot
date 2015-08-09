#include "EnhancedIRCBot.h"

EnhancedIRCBot::EnhancedIRCBot(const std::string& botNick,
                               const std::string& ownerNick,
                               const std::string& server,
                               const std::string& port,
                               const std::string& channel)
    : IRCBot(botNick, server, port, channel),
      m_ownerNick(ownerNick)
{
    // Add host addresses of operators here
    // m_operators.insert("<host address>");
}

void EnhancedIRCBot::OnPublicMessage(const IRCMessage& message)
{
}

void EnhancedIRCBot::OnPrivateMessage(const IRCMessage& message)
{
    // Forward the private message to the owner if connected to the same server
    PrivateSay("FW: " + message.GetNick() + ": " + message.GetParameters(), GetOwnerNick());

    // Respond to the user that we don't do anything with private messages
    PrivateSay("I don't respond to private messages yet.", message.GetNick());
}

void EnhancedIRCBot::OnJoin(const IRCMessage& message)
{
    // Greet and op the user if we recognize them as an operator
    if (IsOp(message.GetHost()))
    {
        Say("Sup, " + message.GetNick());
        Mode(message.GetNick(), "+o");
    }
}

void EnhancedIRCBot::OnPart(const IRCMessage& message)
{
    // Say goodbye to the user if we recognize them as an operator
    if (IsOp(message.GetHost()))
    {
        Say("Peace, " + message.GetNick());
    }
}

void EnhancedIRCBot::OnMode(const IRCMessage& message)
{
    // Respond to being opped
    if (message.GetParameters() == "+o " + GetBotNick())
    {
        Say("I'm the boss now!");
    }
}

bool EnhancedIRCBot::IsOp(const std::string& host)
{
    return m_operators.find(host) != end(m_operators);
}