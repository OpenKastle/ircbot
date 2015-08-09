#include "EnhancedIRCBot.h"

int main()
{
    EnhancedIRCBot bot("<bot nickname>",
                       "<bot owner nickname>",
                       "<server address>",
                       "<server port>",
                       "<channel>");
    bot.Start();

    return 0;
}