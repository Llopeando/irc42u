/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ullorent <ullorent@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/27 17:43:54 by ullorent          #+#    #+#             */
/*   Updated: 2023/07/27 18:58:57 by ullorent         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMANDS_H
#define COMMANDS_H

#include "Config.h"
#include "defines.hpp"
#include "ErrorHandler.hpp"
#include "cmd_structs.h"
#include "Client.hpp"
#include "Utils.hpp"
#include "cmd_reply.h"

#include <string>
#include <vector>
#include <deque>
#include <iostream>

START_CMD_NAMESPACE

START_ANONYMOUS_NAMESPACE

/*
*	// All functions reside inside the <namespace cmd namespace anonymous> so that they cannot be accessed from outside
*
*	Private: 
*		//all the command() functions
*/


	/*	----------------------      COMMAND INDEX    --------------------------

		/-----/Server Queries and Commands//

		motd
		mode
		version
		lusers
		time

		/-----/Connection messages//

		ping
		pass
		nick
		user
		oper
		quit

		/----/Channel Operations//

		join
		part
		topic
		names
		list
		invite
		kick


		/-----/Sending Messages//

		privmsg
		notice

		/-----/User-Based Queries//

		whois

		/-----/Operator Messages//

		kill

		/----/Optional Messages//

		away

		/----/Capabilities Negotiation (For now the server does not support capability negotiation) ///

		cap

			cap_req
			cap_ls
			cap_end
			cap_ack
			cap_nak

	
-------------------------------------------------------------------------------
*/


/* --------------------------------Server Queries and Commands-------------------------- */

eFlags	motd(CmdInput& input)  
{
	utils::sendMsgUser(input.serverData[(sd::pollfdIt)input.index].fd, reply(eRPL_MOTD, input));
	return eSuccess;
}

eFlags	mode(CmdInput& input)  //only available to modify user mode OPERATOR
{
	if (input.arguments.size() < 2) {
		error::error(input, error::ERR_NEEDMOREPARAMS, "MODE");
		return eError;
	}
	if (input.arguments[1] == "+o")
	{
		if(input.serverData.findOper(input.serverData[(sd::clientIt)input.index].getUsername()))
		{
			error::error(input, error::ERR_NOPRIVILEGES);
		}
		input.serverData[input.index].setRole(CL_OPER);
	}
	else if (input.arguments[1] == "-o")
		input.serverData[input.index].setRole(CL_USER);
	else
		return eError;
	return eSuccess;
}

eFlags	version(CmdInput& input)  {
	utils::sendMsgUser(input.serverData[(sd::pollfdIt)input.index].fd, reply(eRPL_VERSION, input));
	utils::sendMsgUser(input.serverData[(sd::pollfdIt)input.index].fd, reply(eRPL_ISUPPORT, input));
	return eSuccess;
}

eFlags	lusers(CmdInput& input){
	utils::sendMsgUser(input.serverData[(sd::pollfdIt)input.index].fd, reply(eRPL_LUSERCLIENT, input));
	utils::sendMsgUser(input.serverData[(sd::pollfdIt)input.index].fd, reply(eRPL_LUSERME, input));
	return eSuccess;
}

eFlags help(CmdInput& input){

	utils::sendMsgUser(input.serverData[(sd::pollfdIt)input.index].fd, reply(eRPL_HELPSTART, input));
	utils::sendMsgUser(input.serverData[(sd::pollfdIt)input.index].fd, reply(eRPL_HELPTXT, input));
 return(eSuccess);
}

//void time(CmdInput& input)  
//{
//	utils::sendMsgUser(input.serverData[(sd::pollfdIt)input.index].fd, reply(eRPL_TIME, input));
//}

/* --------------------------------Connection Messages---------------------------------- */

eFlags	ping(CmdInput& input)
{
	if (input.arguments.size() < 2)
	{
		error::error(input, error::ERR_NEEDMOREPARAMS , "PING");
		return eError;
	}
	utils::sendMsgUser(input.serverData[(sd::pollfdIt)input.index].fd, reply(eRPL_PONG, input));
	return eSuccess;
}


eFlags	pass(CmdInput& input)
{
	if (input.serverData[input.index].getAuthentificied() == sd::eAuthentified)
		return eSuccess;
	if (input.arguments.size() < 2)
	{
		error::fatalError(input, error::ERR_PASSWDMISMATCH);
		input.serverData.removeClient(input.index);
		input.serverData.removeClientChannels(input.index);
		return eExited;
	}
	if (input.serverData.getPassword() == input.arguments[1])
	{
		input.serverData[input.index].setAuthentificied(sd::ePass);
		return eSuccess;
	}
	else
	{
		error::fatalError(input, error::ERR_PASSWDMISMATCH);
		input.serverData.removeClient(input.index);
		input.serverData.removeClientChannels(input.index);
		return eExited;
	}
	return eSuccess;
}

eFlags	nick(CmdInput& input)
{
	if (input.arguments.size() < 2 || input.arguments[1].empty())
	{
		error::error(input, error::ERR_NONICKNAMEGIVEN);
		return eError;
	}
	if (input.arguments[1].size() == 0)
	{
		error::error(input, error::ERR_ERRONEUSNICKNAME);
		return eError;
	}
	for (std::string::iterator c = input.arguments[1].begin(); c != input.arguments[1].end(); c++)
	{
		if (*c == '#' || !std::isprint(*c))
		{
			error::error(input, error::ERR_ERRONEUSNICKNAME, input.arguments[1]); 
			return eError;
		}
	}
	if (input.serverData.findNickname(input.arguments[1])) 
	{
		error::error(input, error::ERR_NICKNAMEINUSE);
		return eError;
	}
	else 
	{
		input.serverData[input.index].setNickname(input.arguments[1]);
		if (!input.serverData[input.index].getUsername().empty())
			input.serverData[(sd::channIt)0].broadcast(0, reply(eRPL_NICK, input));
		input.serverData[input.index].setAuthentificied(sd::eNick);
	}
	return eSuccess;
}

eFlags	user(CmdInput& input)
{
	if (input.serverData[input.index].getAuthentificied() == sd::eAuthentified)
		return eError;
	if (input.arguments.size() < 5 || input.arguments[1].empty()) 
	{
		error::error(input, error::ERR_NEEDMOREPARAMS); //NO ARGS
		return eError;
	}
	if (input.serverData.findUsername(input.arguments[1])) 
	{
		error::error(input, error::ERR_ALREADYREGISTERED); 
		return eError;
	}
	if (input.serverData.findNicknameBack(input.serverData[input.index].getNickname())) //ESTA EN BACK 
	{
		input.serverData.transferIndex(input.index, input.serverData[input.index].getNickname());
		input.serverData.removeClientChannels(input.index);
	}
	else
	{
		input.serverData[input.index].setUsername(input.arguments[1]);
		input.serverData[input.index].setAuthentificied(sd::eUser);
	}
	if (input.serverData[input.index].getAuthentificied() == sd::eAuthentified)
	{
		input.serverData[(sd::channIt)0].addClient(input.index); 
		/* SUCCESSFUL CONNECTION : info del server para el USUARIO */   

		utils::sendMsgUser(input.serverData[(sd::pollfdIt)input.index].fd, reply(eRPL_WELCOME, input));
		utils::sendMsgUser(input.serverData[(sd::pollfdIt)input.index].fd, reply(eRPL_YOURHOST, input));
		utils::sendMsgUser(input.serverData[(sd::pollfdIt)input.index].fd, reply(eRPL_CREATED, input));
		utils::sendMsgUser(input.serverData[(sd::pollfdIt)input.index].fd, reply(eRPL_MYINFO, input));
		utils::sendMsgUser(input.serverData[(sd::pollfdIt)input.index].fd, reply(eRPL_ISUPPORT, input));

	
		if (input.serverData[(sd::clientIt)input.index].getNickname().size() > input.serverData.getConfig().nicklen) 
			input.serverData[(sd::clientIt)input.index].setNickname(input.serverData[(sd::clientIt)input.index].getNickname().substr(0, 16));
		if (input.serverData[(sd::clientIt)input.index].getUsername().size() > input.serverData.getConfig().userlen) 
			input.serverData[(sd::clientIt)input.index].setNickname(input.serverData[(sd::clientIt)input.index].getUsername().substr(0, 10));

		motd(input);
	}
	else
	{
		error::error(input, error::ERR_PASSWDMISMATCH);
		error::fatalError(input, error::ERR_BADPASSWORD);
		input.serverData.removeClient(input.index);
		input.serverData.removeClientChannels(input.index);
		return eExited;
	}
	return eSuccess;
}

eFlags	oper(CmdInput& input) 
{
	if (input.arguments.size() < 2)
	{
		error::error(input, error::ERR_NEEDMOREPARAMS); 
		return eError;
	}
	if (input.serverData.findOper(input.arguments[1]))
	{
		if (input.serverData.checkOperPass(input.arguments[1], input.arguments[2]) == false)
		{
			error::error(input, error::ERR_PASSWDMISMATCH);
			return eError;
		}
		else 
		{
	
			input.serverData[input.index].setRole(CL_OPER);
			utils::sendMsgUser(input.serverData[(sd::pollfdIt)input.index].fd, reply(eRPL_YOUREOPER, input));
		}
	}
	else 
	{
		error::error(input, error::ERR_NOOPERHOST);
		return eError;
	}
	return eSuccess;
}


eFlags	quit(CmdInput& input) 
{	
	std::string reason = "";
	if (input.arguments.size() == 2)
		reason = input.arguments[1];

	CmdInputVar var;
	var.data = &reason;
	var.pnext = nullptr;
	input.var = &var;

	CmdInputVar var2;
	var2.data = &input.index;
	var2.pnext = nullptr;
	var.pnext = &var2;
	
	input.serverData.removeClientChannels(input.index);
	input.serverData[(sd::channIt)0].broadcast(0, reply(eRPL_QUIT, input));
	close(input.serverData[(sd::pollfdIt)input.index].fd);
	input.serverData.backClient(input.index);
	input.serverData.removeClientChannels(input.index);
	input.var = nullptr;
	return eExited;
}


/* --------------------------------Channel Operations----------------------------------- */

eFlags	join(CmdInput& input) 
{
	if (input.arguments.size() < 2)
	{
		error::error(input, error::ERR_NEEDMOREPARAMS, "JOIN");
		return eError;
	}
	std::vector<std::string>channelNames = utils::split(input.arguments[1], ',');
	if (input.arguments[1] == "#0")
	{
		input.serverData.removeClientChannels(input.index);
		return eSuccess;
	}
	for (uint32_t i = 0;i < channelNames.size();i++)
	{
		if (input.arguments[1][0] != '#' || input.arguments[1].size() < 2 || !std::isprint(input.arguments[1][1]))
		{
			error::error(input, error::ERR_BADCHANMASK, input.arguments[1]);
			return eError;
		}
		sd::channIt channel = input.serverData.findChannel(channelNames[i].substr(1));
		if(!channel) //NO EXISTE CHANNEL -> se crea y se une 
		{
			//se covierte en op de ese canal

			input.serverData.addChannel(channelNames[i].substr(1), input.serverData[input.index].getUsername(), input.serverData);
			channel = input.serverData.getNumOfChannels() - 1;
			input.serverData[channel].addClient(input.index);
			
			input.serverData[input.index].addChannelToOps(channel);

			CmdInputVar var;
			var.data = &channel;
			var.pnext = nullptr;
			input.var = &var;

			
			utils::sendMsgUser(input.serverData[(sd::pollfdIt)input.index].fd, reply(eRPL_JOIN, input));
			utils::sendMsgUser(input.serverData[(sd::pollfdIt)input.index].fd, reply(eRPL_JOINMODE, input));
			utils::sendMsgUser(input.serverData[(sd::pollfdIt)input.index].fd, reply(eRPL_NAMREPLY, input));
			utils::sendMsgUser(input.serverData[(sd::pollfdIt)input.index].fd, reply(eRPL_ENDOFNAMES, input));

			input.var = nullptr;
		}
		else // EXISTE CHANNEL -> se une
		{
			//std::cout << color::blue << input.serverData.getConfig().maxuserschan << color::reset << "\n";
			if (input.serverData[channel].getNumUser() == input.serverData.getConfig().maxuserschan)
			{
				error::error(input, error::ERR_CHANNELISFULL);
				return eError;
			}
			input.serverData[channel].addClient(input.index);

			CmdInputVar var1;
			var1.data = &channel;
			var1.pnext = nullptr;
			input.var = &var1;

			utils::sendMsgUser(input.serverData[(sd::pollfdIt)input.index].fd, reply(eRPL_JOIN, input));
			utils::sendMsgUser(input.serverData[(sd::pollfdIt)input.index].fd, reply(eRPL_TOPIC, input));
			utils::sendMsgUser(input.serverData[(sd::pollfdIt)input.index].fd, reply(eRPL_TOPICWHOTIME, input));
			utils::sendMsgUser(input.serverData[(sd::pollfdIt)input.index].fd, reply(eRPL_NAMREPLY, input));
			utils::sendMsgUser(input.serverData[(sd::pollfdIt)input.index].fd, reply(eRPL_ENDOFNAMES, input));

			input.var = nullptr;
		}
		CmdInputVar var;
		var.data = &channel;
		var.pnext = nullptr;
		input.var = &var;
		input.serverData[channel].broadcast(input.index, reply(eRPL_JOIN, input));
		input.var = nullptr;
	}
	return eSuccess;
}

eFlags	part(CmdInput& input) 
{
	if (input.arguments.size() < 2)
	{
		error::error(input, error::ERR_NEEDMOREPARAMS , "PART"); //ARGUMENT ERROR
		return eError;
	}
	std::string reason;
	if (input.arguments.size() > 2)
		reason = utils::joinStr(input.arguments, 2);
	CmdInputVar var1;
	var1.data = &reason;
	var1.pnext = nullptr;
	input.var = &var1;

	std::vector<std::string> channelNames = utils::split(input.arguments[1], ',');
	for (std::vector<std::string>::iterator channelName = channelNames.begin(); channelName != channelNames.end(); channelName++)
	{
		if(!((*channelName)[0] == '#' || (*channelName)[0] == '@')) 
		{
			error::error(input, error::ERR_BADCHANMASK, *channelName); //FORMAT INCORRECT  
			continue ;
		}
		sd::channIt channel = input.serverData.findChannel(channelName->substr(1));
		if (channel == 0)
		{
			error::error(input, error::ERR_NOSUCHCHANNEL, *channelName); //NO CHANNEL
			continue ;
		}
		if(!input.serverData[channel].findUser(input.index)) 
		{
			error::error(input, error::ERR_NOTONCHANNEL, *channelName); //NO ESTAS EN EL CHANNEL 
			continue ;
		}

		CmdInputVar var2;
		var2.data = &channel;
		var2.pnext = nullptr;
		var1.pnext = &var2;


		input.serverData[channel].broadcast(0, reply(eRPL_PART, input));
		
		var1.pnext = nullptr;

		input.serverData[channel].removeClient(input.index);
	}
	input.var = nullptr;
	return eSuccess;
}

eFlags	topic(CmdInput& input) 
{

	if (input.arguments.size() < 2)
	{
		error::error(input, error::ERR_NEEDMOREPARAMS , "TOPIC"); //ARGUMENT ERROR
		return eError;
	}
	sd::channIt channel = input.serverData.findChannel(input.arguments[1].substr(1));
	if(!(input.arguments[1][0] == '#' || input.arguments[1][0] == '@')) 
	{
		error::error(input, error::ERR_BADCHANMASK, input.arguments[1]); //INCORRECT FORMAT 
		return eError;
	}
	if (channel == 0)
	{
		error::error(input, error::ERR_NOSUCHCHANNEL, input.arguments[1]); //NO EXISTE CHANNEL 
		return eError;
	}
	if (!input.serverData[input.index].findChannelInOps(channel)) 
	{
		error::error(input, error::ERR_CHANOPRIVSNEEDED, input.arguments[1].substr(1));// NO CHANNEL OPER 
		return eError;
	}
	if(!input.serverData[channel].findUser(input.index)) 
	{
		error::error(input, error::ERR_NOTONCHANNEL, input.arguments[1]); //NO ESTAS EN EL CHANNEL 
		return eError;
	}
	if( input.arguments.size() >= 3) //SETTING TOPIC 
	{
		input.serverData[channel].setTopic(utils::joinStr(input.arguments, 2));
		
		CmdInputVar var;
		var.pnext = nullptr;
		var.data = &channel;
		input.var = &var;

		input.serverData[channel].broadcast(0, reply(eRPL_TOPIC, input));
		input.serverData[channel].setCreationDate(utils::t_chrono::to_time_t(utils::t_chrono::now()));

		input.var = nullptr;
	}
	else	//VIEWING topic
	{
		if(input.serverData[channel].getTopic().empty())
		{
			utils::sendMsgUser(input.serverData[(sd::pollfdIt)input.index].fd, reply(eRPL_NOTOPIC, input));
			return eSuccess;
		}
		input.serverData[channel].broadcast(0, reply(eRPL_TOPIC, input)); //sendmesssageuser ???
		input.serverData[channel].broadcast(0, reply(eRPL_TOPICWHOTIME, input)); //sendmesssageuser ???
	}
	return eSuccess;
}

eFlags	names(CmdInput& input) 
{
	if (input.arguments.size() == 1)
	{
		for (uint32_t target = 1; target != input.serverData.getNumOfChannels(); target++)
		{
			CmdInputVar var;
			var.pnext = nullptr;
			var.data = &target;
			input.var = &var;

			utils::sendMsgUser(input.serverData[(sd::pollfdIt)input.index].fd, reply(eRPL_NAMREPLY, input));
			utils::sendMsgUser(input.serverData[(sd::pollfdIt)input.index].fd, reply(eRPL_ENDOFNAMES, input));

			input.var = nullptr;
		}
		return eSuccess;
	}
	std::vector<std::string> targets = utils::split(input.arguments[1], ',');
	for (std::vector<std::string>::const_iterator target = targets.begin(); target != targets.end(); target++)
	{
		sd::channIt channel = input.serverData.findChannel(target->substr(1));
		if (!channel)
		{
			error::error(input, error::ERR_NOSUCHCHANNEL, input.arguments[1]);
			continue;
		}

		CmdInputVar var;
		var.pnext = nullptr;
		var.data = &channel;
		input.var = &var;

		utils::sendMsgUser(input.serverData[(sd::pollfdIt)input.index].fd, reply(eRPL_NAMREPLY, input));
		utils::sendMsgUser(input.serverData[(sd::pollfdIt)input.index].fd, reply(eRPL_ENDOFNAMES, input));

		input.var = nullptr;
	}
	return eSuccess;
}

eFlags	list(CmdInput& input) 
{
	

	utils::sendMsgUser(input.serverData[(sd::pollfdIt)input.index].fd, reply(eRPL_LISTSTART, input));
	for (sd::channIt channel = 1;channel < input.serverData.getNumOfChannels() ; channel++)
	{
		CmdInputVar var;
		var.pnext = nullptr;
		var.data = &channel;
		input.var = &var;

		utils::sendMsgUser(input.serverData[(sd::pollfdIt)input.index].fd, reply(eRPL_LIST, input));
		input.var = nullptr;
	}
	utils::sendMsgUser(input.serverData[(sd::pollfdIt)input.index].fd, reply(eRPL_LISTEND, input));
	return eSuccess;
}

eFlags	invite(CmdInput& input) 
{
	if (input.arguments.size() < 2)
	{
		error::error(input, error::ERR_NEEDMOREPARAMS , "INVITE"); //ARGUMENT ERROR
		return eError;
	}
	sd::channIt channel = input.serverData.findChannel(input.arguments[1].substr(1));
	if (!input.serverData[input.index].findChannelInOps(channel)) 
	{
		error::error(input, error::ERR_CHANOPRIVSNEEDED, input.arguments[1].substr(1));// NO CHANNEL OPER 
		return eError;
	}

	else if (input.arguments.size() == 3)
	{
		sd::clientIt target_user = input.serverData.findNickname(input.arguments[1]);
		if (!target_user)
		{
			error::error(input, error::ERR_NOSUCHNICK, input.arguments[1]);
			return eError;
		}
		sd::channIt channel = input.serverData.findChannel(input.arguments[2].substr(1));
		if (channel == 0)
			error::error(input, error::ERR_NOSUCHCHANNEL, input.arguments[2]); // NO EXISTE CHANNEL

		else if (!input.serverData[input.index].findChannelInOps(channel)) 
		{
			error::error(input, error::ERR_CHANOPRIVSNEEDED, input.arguments[1].substr(1));// NO CHANNEL OPER 
			return eError;
		}
		else if (!input.serverData[channel].findUser(input.index))
		{
			error::error(input, error::ERR_NOTONCHANNEL, input.serverData[channel].getName()); //NO ESTAS EN ESE CHANNEL 
		}
		else if (input.serverData[channel].findUser(target_user)) 
		{
			error::error(input, error::ERR_USERONCHANNEL, input.arguments[1]); //TARGET YA ESTA EN EL CHANNEL
		}
		else
		{
			CmdInputVar var;
			var.data = &target_user;
			var.pnext = nullptr;
			input.var = &var;
			utils::sendMsgUser(input.serverData[(sd::pollfdIt)target_user].fd, reply(eRPL_INVITING, input)); 
			input.var = nullptr;
		}
	}
	return eSuccess;
}

eFlags	kick(CmdInput& input) 
{
	if (input.arguments.size() < 3)
	{
		error::error(input, error::ERR_NEEDMOREPARAMS , "KICK"); // ARGUMENT ERROR 
		return eError;
	}
	sd::channIt channel = input.serverData.findChannel(input.arguments[1].substr(1));
	if (!input.serverData[input.index].findChannelInOps(channel)) 
	{
		error::error(input, error::ERR_CHANOPRIVSNEEDED, input.arguments[1].substr(1));// NO CHANNEL OPER 
		return eError;
	}

	CmdInputVar var1;
	var1.data = &channel;
	var1.pnext = nullptr;
	input.var = &var1;

	if (channel == 0)
	{
		error::error(input, error::ERR_NOSUCHCHANNEL, input.arguments[1].substr(1)); //INCORRECT FORMAT 
		return eError;
	}
	if (!input.serverData[input.index].findChannelInOps(channel)) 
	{
		error::error(input, error::ERR_CHANOPRIVSNEEDED, input.arguments[1].substr(1));// NO CHANNEL OPER 
		return eError;
	}
	else if (!input.serverData[channel].findUser(input.index))
	{
		error::error(input, error::ERR_NOTONCHANNEL, input.serverData[channel].getName()); //NO ESTAS EN ESE CHANNEL 
	}
	std::vector<std::string>targets = utils::split(input.arguments[2], ',');
	for(std::vector<std::string>::const_iterator target = targets.begin();target != targets.end(); target++)
	{
		sd::clientIt clientIdx = input.serverData.findNickname(*target);
		if (clientIdx == 0) 									
			error::error(input, error::ERR_NOSUCHNICK); //NO TARGET 
		else if (input.serverData[channel].findUser(clientIdx) == 0) 	
			error::error(input, error::ERR_USERNOTINCHANNEL); //TARGET NO ESTA EN CANAL 
		else
		{
			CmdInputVar var2;
			var2.data = &clientIdx;
			var2.pnext = nullptr;
			var1.pnext = &var2;
			
			input.serverData[channel].broadcast(0, reply(eRPL_KICK, input));
			input.serverData[channel].removeClient(clientIdx);

			var1.pnext = nullptr;
		}
	}
	input.var = nullptr;
	return eSuccess;
}

/* --------------------------------Sending Messages------------------------------------- */


eFlags	privmsg(CmdInput& input) 
{
	if (input.arguments.size() < 2 || input.arguments[1].empty()) 
	{
		error::error(input, error::ERR_NORECIPIENT); //NO ARGS
		return eError;
	}
	std::vector<std::string> targets = utils::split(input.arguments[1], ',');
	std::set<std::string> uniqueNames;
	for (std::vector<std::string>::iterator target = targets.begin(); target != targets.end(); target++)
	{

		if (!uniqueNames.insert(*target).second) {
			error::error(input, error::ERR_TOOMANYTARGETS, *target);
			continue ;
		}

		CmdInputVar var1;
		var1.data = &(*target);
		var1.pnext = nullptr;
		input.var = &var1;

		if ((*target)[0] == '#') //ES UN CANAL
		{
			sd::channIt channel = input.serverData.findChannel(target->substr(1));
			if(channel == 0)
			{
				error::error(input, error::ERR_CANNOTSENDTOCHAN);  //NO CHANNEL 
				continue;
			}
			input.serverData[channel].broadcast(input.index, reply(eRPL_PRIVMSG, input));
		}
		else  //ES UN USER 
		{
			sd::clientIt user = input.serverData.findNickname(*target);
			if (user != 0)
			{
				if (input.serverData[user].getAwayStatus() == true) 
				{
					utils::sendMsgUser(input.serverData[(sd::pollfdIt)input.index].fd, reply(eRPL_PRIVMSG, input));
					CmdInputVar var2;
					var2.data = &user;
					var2.pnext = nullptr;
					input.var = &var2;
					utils::sendMsgUser(input.serverData[(sd::pollfdIt)input.index].fd, reply(eRPL_AWAY, input));
					input.var = nullptr;
				}
				else 
				{	
					if(input.arguments.size() < 3 || input.arguments[2].empty())  
					{
						error::error(input, error::ERR_NOTEXTTOSEND); //NO TEXT
					}
					utils::sendMsgUser(input.serverData[(sd::pollfdIt)user].fd, reply(eRPL_PRIVMSG, input));
				}
			
			}
			else
			{
				error::error(input, error::ERR_NOSUCHNICK);
			}
		}
		input.var = nullptr;
	}
	return eSuccess;
}

eFlags	notice(CmdInput& input) 
{
	if (input.arguments.size() < 2 || input.arguments[1].empty()) 
		return eError;
	std::vector<std::string> targets = utils::split(input.arguments[1], ',');
	std::set<std::string> uniqueNames;
	for (std::vector<std::string>::iterator target = targets.begin(); target != targets.end(); target++)
	{
		if (!uniqueNames.insert(*target).second)
			continue ;

		CmdInputVar var1;
		var1.data = &(*target);
		var1.pnext = nullptr;
		input.var = &var1;
		
		if ((*target)[0] == '#') //ES UN CANAL - solo OPs 
		{
			sd::channIt channel = input.serverData.findChannel(target->substr(1));
			if (!input.serverData[input.index].findChannelInOps(channel)) 
			{
				error::error(input, error::ERR_CHANOPRIVSNEEDED, input.arguments[1].substr(1));// NO CHANNEL OPER 
				return eError;
			}
			input.serverData[channel].broadcast(input.index, reply(eRPL_PRIVMSG, input));
		}
		else  //ES UN USER 
		{
			sd::clientIt user = input.serverData.findNickname(*target);
			if (user != 0)
			{
				if (input.serverData[user].getAwayStatus() == true) 
					continue;
				else 
				{	
					if(!input.arguments[2].empty())  
						utils::sendMsgUser(input.serverData[(sd::pollfdIt)user].fd, reply(eRPL_PRIVMSG, input));
				}
			
			}
		}
		input.var = nullptr;
	}
	return eSuccess;
}


/* --------------------------------User-Based Queries----------------------------------- */

eFlags	whois(CmdInput& input)
{
	// Arguments checker
	if (input.arguments.size() < 2) {
		error::error(input, error::ERR_NEEDMOREPARAMS , "WHOIS"); // ARGUMENTS ERROR
		return eError;
	}
	else if (input.arguments[1].empty()) {
		error::error(input, error::ERR_NONICKNAMEGIVEN , "WHOIS");
		return eError;
	}
	sd::clientIt target_user = input.serverData.findNickname(input.arguments[1]);
	if (!target_user) {
		error::error(input, error::ERR_NOSUCHNICK, input.arguments[1]);
		return eError;
	}
	CmdInputVar var;
	var.data = &target_user;
	var.pnext = nullptr;
	input.var = &var;
	utils::sendMsgUser(input.serverData[(sd::pollfdIt)input.index].fd, reply(eRPL_WHOIS, input));
	input.var = nullptr;
	return eSuccess;
}

/* --------------------------------Operator Messages------------------------------------ */


eFlags	kill(CmdInput& input) 
{

	if (input.arguments.size() < 1)
	{
		error::error(input, error::ERR_NEEDMOREPARAMS , "KILL"); //ARGUMENT ERROR
		return eError;
	}
	if (input.serverData[input.index].getRole() != CL_OPER)
	{
		error::error(input, error::ERR_NOPRIVILEGES);
		return eError;
	}
	sd::clientIt target_user = input.serverData.findNickname(input.arguments[1]);
	if (!target_user)
	{
		error::error(input, error::ERR_NOSUCHNICK, input.arguments[1]);
		return eError;
	}

	std::string reason = "";
	if (input.arguments.size() > 2)
	{
		reason += utils::joinStr(input.arguments, 2);
	}
	
	CmdInputVar var;
	var.data = &target_user;
	var.pnext = nullptr;
	input.var = &var;

	CmdInputVar var2;
	var2.data = &reason;
	var2.pnext = nullptr;
	var.pnext = &var2;
	
	utils::sendMsgUser(input.serverData[(sd::pollfdIt)target_user].fd, reply(eRPL_KILL, input));


	std::string message = input.serverData[input.index].getUserMask() + " has forced " + input.serverData[target_user].getNickname() + " to leave " + input.serverData.getName();
	var.data = &message;
	var2.data = &target_user;

	input.serverData[(sd::channIt)0].broadcast(0,reply(eRPL_QUIT, input));

	input.var = nullptr;
	var.pnext = nullptr;

	close(input.serverData[(sd::pollfdIt)target_user].fd);
	input.serverData.backClient(target_user);
	input.serverData.removeClientChannels(target_user);
	return eSuccess;

//casos en los que server hace quit EL SERVER : "Ping timeout: 120 seconds", "Excess Flood", and "Too many connections from this IP" 

}
/* --------------------------------Optional Messages------------------------------------ */


eFlags	away(CmdInput& input) 
{
	if (input.serverData[input.index].getAwayStatus() == true || input.arguments.size() < 2) {
		input.serverData[input.index].setAwayStatus(false);
		input.serverData[input.index].setAwayMsg("");
		utils::sendMsgUser(input.serverData[(sd::pollfdIt)input.index].fd, reply(eRPL_UNAWAY, input));
		return eSuccess;
	}
	input.serverData[input.index].setAwayMsg(utils::joinStr(input.arguments, 1));
	input.serverData[input.index].setAwayStatus(true);
	utils::sendMsgUser(input.serverData[(sd::pollfdIt)input.index].fd, reply(eRPL_NOWAWAY, input));
	return eSuccess;
}

/* --------------------- CAPABILITIES NEGOTIATION (For now the server does not support capability negotiation ) ----------------------- */


eFlags	cap(CmdInput& input)
{
	(void)input;
	return eSuccess;
	////
	//void (Server::*cap_func)(sd::clientIt index, std::vector<std::string>& arguments) = commands.cap_funcmap[input.arguments[0]];
	////
	//if (cap_func != nullptr)
	////
	//{
	////
	//	(this->*cap_func)(index, input.arguments);
	//
	//	return;
	//}
}

//	// CAP FUNCTIONS
//
//	void	Server::cap_req(clientIt index, std::vector<std::string> &arguments)
//	{
//		std::string ack = "CAP * ACK";
//		std::string nack = "CAP * NACK";
//	
//		//en vez de esto podemos usar una funcion generica  Server::cap_available() que devuelva las capabilities disponibles, osea las que coinciden en un vector 
//	
//		for (uint32_t i = 3; i < arguments.size(); i++)
//		{
//			bool found = false;
//			//for (uint32_t j = 0; j < COMMANDS;j++)
//			//{
//			//0	std::cout << "	[" << j << "]"<<commands.cmd[j] << "\n" << color::reset;
//				if (arguments[i] == "multi-prefix")
//				{
//					ack +=  " multi-prefix";
//					found = true;
//				}
//				else
//					nack += " " + arguments[i];
//			/*}
//			if (!found)
//			{
//			}*/
//		}
//		sendMsgUser(data[(pollfdIt)index].fd, ack);
//		std::cout << "SENDED CAP [" << ack << "]\n";
//		std::cout << "SENDED NACK [" << nack << "]\n";
//		sendMsgUser(data[(pollfdIt)index].fd, nack);
//		//sendMssgUser(data[(pollfdIt)index].fd, "CAP END")
//	}
//	
//	void	Server::cap_ls(clientIt index, std::vector<std::string> &arguments)
//	{
//		(void)arguments;
//		//std::cout << "CAP LS REACHED\n"; 	//mostrar las capabilities que ofrecemos, 	
//		std::string message = "CAP * LS :multi-prefix sasl ";//sasl account-notify extended-join away-notify chghost userhost-in-names cap-notify server-time message-tags invite-notify batch echo-message account-tag";
//		sendMsgUser(data[(pollfdIt)index].fd, message);
//	}
//	
//	void	Server::cap_end(clientIt index, std::vector<std::string> &arguments)
//	{
//		(void)arguments;	
//		std::cout << "CAP END!!!! \n";
//		std::string message = "001 " + data[index].getNickname() + " :Welcome to the A O I R C server\n"; ///PARA MI NO TIENE QUE IR AQUI , FUERA EN EL BUCLE DE COMANDOS COMO METER???
//		sendMsgUser(data[(pollfdIt)index].fd, message);
//	
//	}
//	
//	void	Server::cap_ack(clientIt index, std::vector<std::string> &arguments)
//	{
//		(void)arguments;
//		std::string message = "CAP * ACK : multi-prefix sasl account-notify extended-join away-notify chghost userhost-in-names cap-notify server-time message-tags invite-notify batch echo-message account-tag";
//		sendMsgUser(data[(pollfdIt)index].fd, message);
//		//sendMsgUser(index, "CAP * END");
//	}
//		
//		
//	void	Server::cap_nak(clientIt index, std::vector<std::string> &arguments)
//	{
//	
//		(void)index;
//		(void)arguments;
//	}
//	
//	
//	/*void	Server::cap_available(std::vector<std::string> &arguments)
//	{
//			//va a devolver las capabilities disponibles, osea las que coinciden 
//	
//		std::vector<std::string> availables;
//	
//	
//	}*/


END_ANONYMOUS_NAMESPACE
END_CMD_NAMESPACE


#endif