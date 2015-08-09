#pragma once

#include "IRCBot.h"

#include <set>

// This is an example of a concrete IRC bot which derives from the
// abstract base IRC bot and adds some additional functionality
class EnhancedIRCBot : public IRCBot
{
public:
    EnhancedIRCBot(const std::string& botNick,
                   const std::string& ownerNick,
                   const std::string& server,
                   const std::string& port,
                   const std::string& channel);

    virtual ~EnhancedIRCBot() = default;

protected:
    virtual void OnPublicMessage(const IRCMessage& message);
    virtual void OnPrivateMessage(const IRCMessage& message);
    virtual void OnJoin(const IRCMessage& message);
    virtual void OnPart(const IRCMessage& message);
    virtual void OnMode(const IRCMessage& message);

    const std::string& GetOwnerNick() const { return m_ownerNick; }

private:
    bool IsOp(const std::string& host);

    std::string m_ownerNick;
    std::set<std::string> m_operators;
};