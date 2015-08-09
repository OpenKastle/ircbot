Overview
---
This is a very simple IRC bot which connects to an IRC server via a socket,
reads and interprets messages from the IRC server, and sends the messages
in a digestible format to a handler which may then choose to perform
additional processing.

Structure
---
IRCBot contains a Connection which is responsible for abstracting the
connection details involved with sending messages to a server.  The
Connection class refers back to the IRCBot via its ConnectionClient
interface which allows the Connection object to send messages to
the IRCBot.  The IRCBot handles IRC-specific details such as performing
server handshake operations, PONGing to PINGS, etc.

For common messages like public messages, private messages, joins, parts,
etc. the IRCBot interprets the message, creating an IRCMessage object, and
sends the interpreted message to the appropriate handler.

The IRCBot is abstract, it is the intention that concrete bots derive from
it and override the message handlers of interest.

Threading
---
This bot is currently single-threaded.  It blocks at the connection level
on the call to recv() which listens for new messages from the socket.  It
listens indefinitely and does not currently have a way to kick out of the
listening loop.  There are no thread-safety mechanisms which make any of
this code thread-safe if it were to be used in a multi-threaded way.

Known issues
---
In testing I've found that the bot will send a message in response to an
incoming server message but for some reason the message never makes it to
the server.  I've verified that the send() call succeeds in these cases,
so I can't figure out why this happens.  I've noticed that it tends to happen
after a long period of bot inactivity, I have a suspicion that this may
have something to do with it.