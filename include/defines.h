#ifndef DEFINES_H
#define DEFINES_H

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include "Utils.hpp"

#define DEBUG_FILE "logging/log.txt"

#define DEBUG true

#define SERVER_FAILURE -1

#define CL_STATE_OK 0
#define CL_STATE_INPUT 1
#define CL_STATE_LS 2
#define CL_STATE_REQ 3

#define CL_STATE_ACTIVE 1
#define CL_STATE_INACTIVE 0

#define CL_USER 0
#define CL_OPER 2

#define RCVBUFSIZE 1024

namespace color {
	const std::string reset		= "\033[0m";
	const std::string black		= "\033[30m";
	const std::string red		= "\033[31m";
	const std::string green		= "\033[32m";
	const std::string yellow	= "\033[33m";
	const std::string blue		= "\033[34m";
	const std::string magenta	= "\033[35m";
	const std::string cyan		= "\033[36m";
	const std::string white		= "\033[37m";

	const std::string boldblack		=  "\033[1m\033[30m";
	const std::string boldred		=  "\033[1m\033[31m";
	const std::string boldgreen		=  "\033[1m\033[32m";
	const std::string boldyellow	=  "\033[1m\033[33m";
	const std::string boldblue		=  "\033[1m\033[34m";
	const std::string boldmagenta	=  "\033[1m\033[35m";
	const std::string boldcyan		=  "\033[1m\033[36m";
	const std::string boldwhite		=  "\033[1m\033[37m";

	const std::string boldgreenback =  "\033[1m\033[42m";
}

#define START_CMD_NAMESPACE namespace cmd {
#define END_CMD_NAMESPACE }

#define START_ERROR_NAMESPACE namespace error{
#define END_ERROR_NAMESPACE }

#define START_SERVER_DATA_NAMESPACE namespace sd{
#define END_SERVER_DATA_NAMESPACE }

#define START_ANONYMOUS_NAMESPACE namespace {
#define END_ANONYMOUS_NAMESPACE }

enum eFlags{
	eSuccess = 0,
	eError = 1,
	eNoSuchFunction = 2,
	eExited = 8,
	eBack = 16,
	eRemoveChannel = 32,
	eRemoveClientChannel = 64,
	eReordered = 124,
};

#define INFO "\n " \
        "                                                       Welcome to A O I R C server. \n" \
        "These are COMMANDS you may find in a IRC server as described in RFC2119 protocol, marked with a * ONLY those that A O I R C server allows.   \n" \
        "                                         \n" \
        "Connection Messages                      \n" \
        "    * PASS     <password>                \n" \
        "    * NICK     <nickname>                \n" \
        "    * USER     <username> 0 * <realname> \n" \
        "    * PING     <token>                   \n" \
        "    * OPER     <name> <password>         \n" \
        "    * QUIT     [<reason>]                \n" \
        "                                         \n" \
        "Channel Operations                                                                      \n" \
        "    * JOIN      <channel>{,<channel>}                                                   \n" \
        "    * PART      <channel>{,<channel>} [<reason>]                                        \n" \
        "    * TOPIC     <channel> [<topic>]                                                     \n" \
        "    * NAMES     <channel>{,<channel>}                                                   \n" \
        "    * LIST      [<channel>{,<channel>}]                                                 \n" \
        "    * INVITE    <nickname> <channel>                           //only channel operators \n" \
        "    * KICK      <channel> <user>{,<user>} [<comment>]                                   \n" \
        "                                                                                        \n" \
        "Server Queries and Commands                                                             \n" \
        "    * MOTD                                                                              \n" \
        "    * VERSION                                                                           \n" \
        "    * LUSERS                                                                            \n" \
        "    * MODE                                                     //No available by server \n" \
        "                                                      \n" \
        "Sending Messages                                      \n" \
        "    * PRIVMSG   <target>{,<target>} <text to be sent> \n" \
        "    * NOTICE    <target>{,<target>} <text to be sent> \n" \
        "                                                      \n" \
        "User-Based Queries                                    \n" \
        "    * WHOIS    [<target>] <nick>                      \n" \
        "    * HELP                                            \n" \
        "                                                      \n" \
        "Operator Messages                                     \n" \
        "    * KILL    <nickname> <comment>                    \n" \
        "                                                      \n" \
        "Optional Messages                                     \n" \
        "    * AWAY     [<text>]                               \n"

#define MOTD "\n " \
			"         _____                           _______                           _____                            _____                            _____          \n" \
			"         /\\    \\                         /::\\    \\                         /\\    \\                          /\\    \\                          /\\    \\         \n" \
			"        /::\\    \\                       /::::\\    \\                       /::\\    \\                        /::\\    \\                        /::\\    \\        \n" \
			"       /::::\\    \\                     /::::::\\    \\                      \\:::\\    \\                      /::::\\    \\                      /::::\\    \\       \n" \
			"      /::::::\\    \\                   /::::::::\\    \\                      \\:::\\    \\                    /::::::\\    \\                    /::::::\\    \\      \n" \
			"     /:::/\\:::\\    \\                 /:::/~~\\:::\\    \\                      \\:::\\    \\                  /:::/\\:::\\    \\                  /:::/\\:::\\    \\     \n" \
			"    /:::/__\\:::\\    \\               /:::/    \\:::\\    \\                      \\:::\\    \\                /:::/__\\:::\\    \\                /:::/  \\:::\\    \\    \n" \
			"   /::::\\   \\:::\\    \\             /:::/    / \\:::\\    \\                     /::::\\    \\              /::::\\   \\:::\\    \\              /:::/    \\:::\\    \\   \n" \
			"  /::::::\\   \\:::\\    \\           /:::/____/   \\:::\\____\\           ____    /::::::\\    \\            /::::::\\   \\:::\\    \\            /:::/    / \\:::\\    \\  \n" \
			" /:::/\\:::\\   \\:::\\    \\         |:::|    |     |:::|    |         /\\   \\  /:::/\\:::\\    \\          /:::/\\:::\\   \\:::\\____\\          /:::/    /   \\:::\\    \\ \n" \
			"/:::/  \\:::\\   \\:::\\____\\        |:::|____|     |:::|    |        /::\\   \\/:::/  \\:::\\____\\        /:::/  \\:::\\   \\:::|    |        /:::/____/     \\:::\\____\\\n" \
			"\\::/    \\:::\\  /:::/    /         \\:::\\    \\   /:::/    /         \\:::\\  /:::/    \\::/    /        \\::/   |::::\\  /:::|____|        \\:::\\    \\      \\::/    /\n" \
			" \\/____/ \\:::\\/:::/    /           \\:::\\    \\ /:::/    /           \\:::\\/:::/    / \\/____/          \\/____|:::::\\/:::/    /          \\:::\\    \\      \\/____/ \n" \
			"          \\::::::/    /             \\:::\\    /:::/    /             \\::::::/    /                         |:::::::::/    /            \\:::\\    \\             \n" \
			"           \\::::/    /               \\:::\\__/:::/    /               \\::::/____/                          |::|\\::::/    /              \\:::\\    \\            \n" \
			"           /:::/    /                 \\::::::::/    /                 \\:::\\    \\                          |::| \\::/____/                \\:::\\    \\           \n" \
			"          /:::/    /                   \\::::::/    /                   \\:::\\    \\                         |::|  ~|                       \\:::\\    \\          \n" \
			"         /:::/    /                     \\::::/    /                     \\:::\\____\\                        |::|   |                        \\:::\\____\\         \n" \
			"        /:::/    /                       \\::/    /                       \\::/    /                         \\:|   |                         \\::/    /         \n" \
			"        \\::/____/                         \\/____/                         \\/____/                           \\|___|                          \\/____/          \n"

#endif