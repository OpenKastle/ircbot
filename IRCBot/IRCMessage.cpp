#include "IRCMessage.h"

#include <regex>
#include <assert.h>

IRCMessage::IRCMessage(const std::string& messageText)
{
    std::string message = messageText.substr(0, messageText.size() - 2);
    std::regex regex("^:([^!]+)!([^\\s]+)@([^\\s]+)\\s([^\\s]+)\\s:?([^\\s]+)\\s?:?(.*)$");

    if (!std::regex_match(message, regex))
    {
        m_type = IRCMessageType::eUnrecognized;
    }
    else
    {
        std::smatch match;
        std::regex_search(message, match, regex);

        if (1 < match.size())
        {
            m_nick = match[1];
        }

        if (2 < match.size())
        {
            m_user = match[2];
        }

        if (3 < match.size())
        {
            m_host = match[3];
        }

        if (4 < match.size())
        {
            m_command = match[4];
        }

        if (5 < match.size())
        {
            m_target = match[5];
        }

        if (6 < match.size())
        {
            m_parameters = match[6];
        }

        if (m_command == "JOIN")
        {
            m_type = IRCMessageType::eJoin;
        }
        else if (m_command == "PART")
        {
            m_type = IRCMessageType::ePart;
        }
        else if (m_command == "PRIVMSG")
        {
            m_type = IRCMessageType::eMessage;
        }
        else if (m_command == "MODE")
        {
            m_type = IRCMessageType::eMode;
        }
        else
        {
            m_type = IRCMessageType::eUnrecognized;
        }
    }
}