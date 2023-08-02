/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ullorent <ullorent@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/27 17:43:54 by ullorent          #+#    #+#             */
/*   Updated: 2023/08/02 17:58:28 by ullorent         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMANDS_H
#define COMMANDS_H

#include <string>
#include <vector>
#include <deque>
#include <iostream>
#include <set>

#include "defines.h"
#include "Utils.hpp"

#include "ErrorHandler.hpp"


/* CMD namespace  : 

	Util functions : removeClientChannels(), getFunctionMap() 																										//Public

	Command functions [...]																																			//Public

	Map of commandss : getFunctionMap() , getReplyMap()																												// Anonymous namespace

	Access function : callFunction() -> Will call any function that matches the key passed as a parameter. Receives a key and a CmdInput. Returns a CmdReturn. 		//Public

*/


START_CMD_NAMESPACE

/* --------------------------------			UTILS 		-------------------------- */

void removeClientChannels(sd::ServerData &serverData, sd::clientIt index);
bool checkChannelName(const std::string &channel);
//const CmdMap& getFunctionMap();

/* --------------------------------			COMMANDS		-------------------------- */

/* --------------------------------Server Queries and Commands-------------------------- */

eFlags	motd(CmdInput& input);
eFlags	mode(CmdInput& input);
eFlags	version(CmdInput& input);
eFlags	lusers(CmdInput& input);
eFlags	help(CmdInput& input);
//void time(CmdInput& input)  ;

/* --------------------------------Connection Messages---------------------------------- */

eFlags	ping(CmdInput& input);
eFlags	pass(CmdInput& input);
eFlags	nick(CmdInput& input);
eFlags	user(CmdInput& input);
eFlags	oper(CmdInput& input);
eFlags	quit(CmdInput& input);

/* --------------------------------Channel Operations----------------------------------- */

eFlags	join(CmdInput& input);
eFlags	part(CmdInput& input);
eFlags	topic(CmdInput& input);
eFlags	names(CmdInput& input);
eFlags	list(CmdInput& input);
eFlags	kick(CmdInput& input);
eFlags	invite(CmdInput& input);

/* -------------------------------- Sending Messages ------------------------------------- */

eFlags	privmsg(CmdInput& input);
eFlags	notice(CmdInput& input);

/* -------------------------------- User-Based Queries ----------------------------------- */

eFlags	whois(CmdInput& input);

/* -------------------------------- Operator Messages ------------------------------------ */

eFlags	kill(CmdInput& input);

/* -------------------------------- Optional Message ------------------------------------ */

eFlags	away(CmdInput& input);

/* ------------------------------- Capability negotiation ------------------------------- */

eFlags	cap(CmdInput& input); //(For now the server does not support capability negotiation )


START_ANONYMOUS_NAMESPACE //-> Access to functions reside inside the <namespace cmd anonymous> so that they cannot be accessed from outside

/* --------------------------------			MAP OF COMMANDS 		-------------------------- */

const CmdMap& getFunctionMap()
{
	static CmdMap cmdMap;
	if (cmdMap.empty())
	{
		cmdMap["PASS"]		= &pass;
		cmdMap["NICK"]		= &nick;
		cmdMap["USER"]		= &user;
		cmdMap["JOIN"]		= &join;
		cmdMap["PRIVMSG"]	= &privmsg;
		cmdMap["NOTICE"]	= &notice;
		cmdMap["QUIT"]		= &quit;
		cmdMap["MODE"]		= &mode;
		cmdMap["NAMES"]		= &names;
		cmdMap["WHOIS"]		= &whois;
		cmdMap["KICK"]		= &kick;
		cmdMap["AWAY"]		= &away;
		cmdMap["INVITE"]	= &invite;
		cmdMap["PING"]		= &ping;
		cmdMap["TOPIC"]		= &topic;
		cmdMap["LIST"]		= &list;
		cmdMap["PART"]		= &part;
		cmdMap["MOTD"]		= &motd;
		cmdMap["OPER"]		= &oper;
		cmdMap["KILL"]		= &kill;
		cmdMap["VERSION"]	= &version;
		cmdMap["LUSERS"]	= &lusers;
		//cmdMap["TIME"]		= &time;
		cmdMap["CAP"]		= &cap;
		cmdMap["HELP"]		=&help;
		
		/*
		
			cmdMap["CAP_REQ"]	= &Server::cap_req;
		 	cmdMap["CAP_LS"]	= &Server::cap_ls;
		 	cmdMap["CAP_END"]	= &Server::cap_end;
		 	cmdMap["CAP_ACK"]	= &Server::cap_ack;
		 	cmdMap["CAP_NAK"]	= &Server::cap_nak;

		*/
	}
	return cmdMap;
};

END_ANONYMOUS_NAMESPACE


/* --------------------------------			ACCESS TO FUNCTION 		-------------------------- */ 

inline eFlags callFunction(const std::string& key, CmdInput& input)
{
	CmdMap::const_iterator it = getFunctionMap().find(key);
	if (it != getFunctionMap().end()) {
		return it->second(input);
	}
	else
	{
		return eNoSuchFunction;
	}
}


END_CMD_NAMESPACE


#endif