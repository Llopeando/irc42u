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
*	All functions  reside inside the <namespace cmd namespace anonymous> so that they cannot be accessed from outside
*/

	/*	----------------------      COMMAND INDEX    --------------------------

		/-----/Server Queries and Commands//

		motd
		mode
		version
		lusers

		/-----/Connection messages//

		ping
		cap

			cap_req
			cap_ls
			cap_end
			cap_ack
			cap_nak

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
		who

		/-----/Operator Messages//

		kill

		/----/Optional Messages//

		away
	
	--------------------------------------------------------------------------------
*/


/* --------------------------------Server Queries and Commands-------------------------- */

void	motd(CmdInput& input)  
{
	(void)input;
	utils::sendMsgUser(input.serverData[(sd::pollfdIt)input.index].fd, reply(eRPL_MOTD, input));
}

void	mode(CmdInput& input)  
{
	(void)input;
}

void	version(CmdInput& input)  {
	utils::sendMsgUser(input.serverData[(sd::pollfdIt)input.index].fd, reply(eRPL_VERSION, input));
	utils::sendMsgUser(input.serverData[(sd::pollfdIt)input.index].fd, reply(eRPL_ISUPPORT, input));

}

void	lusers(CmdInput& input){
	utils::sendMsgUser(input.serverData[(sd::pollfdIt)input.index].fd, reply(eRPL_LUSERCLIENT, input));
	utils::sendMsgUser(input.serverData[(sd::pollfdIt)input.index].fd, reply(eRPL_LUSERME, input));
}

/* --------------------------------Connection Messages---------------------------------- */

void	ping(CmdInput& input)
{
	if (input.arguments.size() < 2)
	{
		error::error(input, error::ERR_NEEDMOREPARAMS , "PING");
		return;
	}
	utils::sendMsgUser(input.serverData[(sd::pollfdIt)input.index].fd, reply(eRPL_PONG, input));
}

//void	cap(CmdInput& input)
//{
//	void (Server::*cap_func)(sd::clientIt index, std::vector<std::string>& arguments) = commands.cap_funcmap[input.arguments[0]];
//	if (cap_func != nullptr)
//	{
//		(this->*cap_func)(index, input.arguments);
//		return;
//	}
//}

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

void	pass(CmdInput& input)
{
	if (input.serverData[input.index].getAuthentificied())
		return ;
	if (input.arguments.size() < 2)
	{
		error::fatalError(input, error::ERR_BADPASSWORD);
		input.serverData.removeClient(input.index);
		input.serverData.removeClientChannels(input.index);
		return ;
	}
	if (input.serverData.getPassword() == input.arguments[1])
	{
		input.serverData[input.index].setAuthentificied(true);
		return ;
	}
	else
	{
		error::error(input, error::ERR_PASSWDMISMATCH);
		input.serverData.removeClient(input.index);
		input.serverData.removeClientChannels(input.index);
		return ;
	}
}

void	nick(CmdInput& input)
{
	if (input.arguments.size() < 2 || input.arguments[1].empty())
	{
		error::error(input, error::ERR_NONICKNAMEGIVEN);
		return ;
	}
	if (input.arguments[1].size() == 0)
	{
		error::error(input, error::ERR_ERRONEUSNICKNAME);
		return;
	}
	for (std::string::iterator c = input.arguments[1].begin(); c != input.arguments[1].end(); c++)
	{
		if (*c == '#' || !std::isprint(*c))
		{
			error::error(input, error::ERR_ERRONEUSNICKNAME, input.arguments[1]); 
			return;
		}
	}
	if (input.serverData.findNickname(input.arguments[1])) 
	{
		error::error(input, error::ERR_NICKNAMEINUSE);
		return ;
	}
	else 
	{
		input.serverData[input.index].setNickname(input.arguments[1]);
		input.serverData[(sd::channIt)0].broadcast(0, reply(eRPL_NICK, input));
	}
}

void	user(CmdInput& input)
{
	if (input.arguments.size() < 5 || input.arguments[1].empty()) 
	{
		error::error(input, error::ERR_NEEDMOREPARAMS); //NO ARGS
		return ;
	}
	if (input.serverData.findUsername(input.arguments[1])) 
	{
		error::error(input, error::ERR_ALREADYREGISTERED); 
		return ;
	}
	if (input.serverData.findNicknameBack(input.serverData[input.index].getNickname())) //ESTA EN BACK 
	{
		input.serverData.transferIndex(input.index, input.serverData[input.index].getNickname());
		input.serverData.removeClientChannels(input.index);
	}
	else
	{
		input.serverData[input.index].setUsername(input.arguments[1]);
	}
		
	if (input.serverData[input.index].getAuthentificied())
	{
		input.serverData[(sd::channIt)0].addClient(input.index); 

		/* SUCCESSFUL CONNECTION : info del server para el USUARIO */   

		utils::sendMsgUser(input.serverData[(sd::pollfdIt)input.index].fd, reply(eRPL_WELCOME, input));
		utils::sendMsgUser(input.serverData[(sd::pollfdIt)input.index].fd, reply(eRPL_YOURHOST, input));
		utils::sendMsgUser(input.serverData[(sd::pollfdIt)input.index].fd, reply(eRPL_CREATED, input));
		utils::sendMsgUser(input.serverData[(sd::pollfdIt)input.index].fd, reply(eRPL_MYINFO, input));
		utils::sendMsgUser(input.serverData[(sd::pollfdIt)input.index].fd, reply(eRPL_ISUPPORT, input));

	
		if (input.serverData[(sd::clientIt)input.index].getNickname().size() > 16) //	NICKLEN=16 
			input.serverData[(sd::clientIt)input.index].setNickname(input.serverData[(sd::clientIt)input.index].getNickname().substr(0, 16));
		if (input.serverData[(sd::clientIt)input.index].getUsername().size() > 10) // USERLEN=10
			input.serverData[(sd::clientIt)input.index].setNickname(input.serverData[(sd::clientIt)input.index].getUsername().substr(0, 10));

		/*	
		Send RPL_UMODEIS (221) reply or a MODE message with the client as target, preferably the former. The server MAY send other numerics and messages. 
		*/
		motd(input);
	}
	else
	{
		error::error(input, error::ERR_PASSWDMISMATCH);
		error::fatalError(input, error::ERR_BADPASSWORD);
		input.serverData.removeClient(input.index);
		input.serverData.removeClientChannels(input.index);
	}
}

void	oper(CmdInput& input) 
{
	if (input.arguments.size() < 2)
	{
		error::error(input, error::ERR_NEEDMOREPARAMS); 
		return ;
	}
	if (input.serverData.findOper(input.arguments[1]))
	{
		if (input.serverData.checkOperPass(input.arguments[1], input.arguments[2]) == false)
		{
			error::error(input, error::ERR_PASSWDMISMATCH);
			return ;
		}
		else 
		{
	
			input.serverData[input.index].setRole(CL_OPER);
			utils::sendMsgUser(input.serverData[(sd::pollfdIt)input.index].fd, reply(eRPL_YOUREOPER, input));
		}
	}
	else 
		error::error(input, error::ERR_NOOPERHOST);
}


void	quit(CmdInput& input) 
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

	input.serverData[(sd::channIt)0].broadcast(0, reply(eRPL_QUIT, input));
	input.serverData.backClient(input.index);
	input.serverData.removeClientChannels(input.index);

	input.var = nullptr;	
}


/* --------------------------------Channel Operations----------------------------------- */

void	join(CmdInput& input) 
{
	if (input.arguments.size() < 2)
	{
		error::error(input, error::ERR_NEEDMOREPARAMS, "JOIN");
		return ;
	}
	std::vector<std::string>channelNames = utils::split(input.arguments[1], ',');
	if (input.arguments[1] == "#0")
	{
		input.serverData.removeClientChannels(input.index);
		return; 
	}
	for (uint32_t i = 0;i < channelNames.size();i++)
	{
		if (input.arguments[1][0] != '#' || input.arguments[1].size() < 2 || !std::isprint(input.arguments[1][1]))
		{
			error::error(input, error::ERR_BADCHANMASK, input.arguments[1]);
			continue ;
		}
		sd::channIt channel = input.serverData.findChannel(channelNames[i].substr(1));
		if(!channel) //NO EXISTE CHANNEL -> se crea y se une 
		{
			if (input.serverData[input.index].getRole() != CL_OPER)
				input.serverData[input.index].setRole(CL_OP);
			input.serverData.addChannel(channelNames[i].substr(1), input.serverData[input.index].getUsername(), input.serverData);
			channel = input.serverData.getNumOfChannels() - 1;
			input.serverData[channel].addClient(input.index);

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
}

void	part(CmdInput& input) 
{
	if (input.arguments.size() < 2)
	{
		error::error(input, error::ERR_NEEDMOREPARAMS , "PART"); //ARGUMENT ERROR
		return;
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
		if (input.serverData[channel].getNumUser() == 0) 
			input.serverData.deleteChannel(channel);
	}
	input.var = nullptr;
}

void	topic(CmdInput& input) 
{

	if (input.arguments.size() < 2)
	{
		error::error(input, error::ERR_NEEDMOREPARAMS , "TOPIC"); //ARGUMENT ERROR
		return;
	}
	if (!input.serverData[input.index].getRole()) 
	{
		error::error(input, error::ERR_CHANOPRIVSNEEDED, input.arguments[1].substr(1));// NO CHANNEL OPER 
		return;
	}
	sd::channIt channel = input.serverData.findChannel(input.arguments[1].substr(1));
	if(!(input.arguments[1][0] == '#' || input.arguments[1][0] == '@')) 
	{
		error::error(input, error::ERR_BADCHANMASK, input.arguments[1]); //INCORRECT FORMAT 
		return ;
	}
	if (channel == 0)
	{
		error::error(input, error::ERR_NOSUCHCHANNEL, input.arguments[1]); //NO EXISTE CHANNEL 
		return ;
	}
	if(!input.serverData[channel].findUser(input.index)) 
	{
		error::error(input, error::ERR_NOTONCHANNEL, input.arguments[1]); //NO ESTAS EN EL CHANNEL 
		return ;
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
			return ;
		}
		input.serverData[channel].broadcast(0, reply(eRPL_TOPIC, input)); //sendmesssageuser ???
		input.serverData[channel].broadcast(0, reply(eRPL_TOPICWHOTIME, input)); //sendmesssageuser ???
	}
}

void	names(CmdInput& input) 
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
		return ;
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
}

void	list(CmdInput& input) 
{
	////RESET lista EN LIME????//

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
}

void	invite(CmdInput& input) 
{
	if (input.arguments.size() < 2)
	{
		error::error(input, error::ERR_NEEDMOREPARAMS , "INVITE"); //ARGUMENT ERROR
		return;
	}
	if (!input.serverData[input.index].getRole())
	{
		error::error(input, error::ERR_CHANOPRIVSNEEDED, input.arguments[2].substr(1));//NO OPERATOR  
		return;
	}
	else if (input.arguments.size() == 3)
	{
		sd::clientIt target_user = input.serverData.findNickname(input.arguments[1]);
		if (!target_user)
		{
			error::error(input, error::ERR_NOSUCHNICK, input.arguments[1]);
			return;
		}
		std::string channel_name = input.arguments[2].substr(1);
		sd::channIt channel = input.serverData.findChannel(channel_name);
		if (channel == 0)
			error::error(input, error::ERR_NOSUCHCHANNEL, input.arguments[2]); // NO EXISTE CHANNEL
		else if (input.serverData[input.index].getRole() == CL_OP && !input.serverData[channel].findUser(input.index))
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

}

void	kick(CmdInput& input) 
{
	if (input.arguments.size() < 3)
	{
		error::error(input, error::ERR_NEEDMOREPARAMS , "KICK"); // ARGUMENT ERROR 
		return;
	}
	if (!input.serverData[input.index].getRole())
	{
		error::error(input, error::ERR_CHANOPRIVSNEEDED, input.arguments[2].substr(1)); //NO OPER 
		return;
	}

	sd::channIt channel = input.serverData.findChannel(input.arguments[1].substr(1));

	CmdInputVar var1;
	var1.data = &channel;
	var1.pnext = nullptr;
	input.var = &var1;

	if (channel == 0)
	{
		error::error(input, error::ERR_NOSUCHCHANNEL, input.arguments[1].substr(1)); //INCORRECT FORMAT 
		return ;
	}
	else if (input.serverData[input.index].getRole() == CL_OP && !input.serverData[channel].findUser(input.index))
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
}

/* --------------------------------Sending Messages------------------------------------- */


void	privmsg(CmdInput& input) 
{
	if (input.arguments.size() < 2 || input.arguments[1].empty()) 
	{
		error::error(input, error::ERR_NORECIPIENT); //NO ARGS
		return ;
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
			////CHECK BANNED??? MOD??? ->>>>>> ERR_CANNOTSENDTOCHAN (404)  
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
					if(input.arguments[2].empty())  
					{
						error::error(input, error::ERR_NOTEXTTOSEND); //NO TEXT
					}
					utils::sendMsgUser(input.serverData[(sd::pollfdIt)user].fd, reply(eRPL_PRIVMSG, input));
				}
			
			}
			else
			{
				//std::cout << color::red << "ERROR NO NICK" << color::reset << std::endl;
				error::error(input, error::ERR_NOSUCHNICK);
			}
		}
		input.var = nullptr;
	
	}
}

//void	notice(CmdInput& input) 
//{
//	if (input.arguments.size() < 2 || input.arguments[1].empty()) 
//	{
//
//		error::error(input, error::ERR_NORECIPIENT); //NO ARGS
//		return ;
//	}
//	std::vector<std::string> targets = utils::split(input.arguments[1], ',');
//	std::set<std::string> uniqueNames;
//	for (std::vector<std::string>::iterator target = targets.begin(); target != targets.end(); target++)
//	{
//		if (!uniqueNames.insert(*target).second) {
//			error::error(input, error::ERR_TOOMANYTARGETS, *target);
//			continue ;
//		}
//		std::string message = ":" + input.serverData.getName() + " NOTICE " + " :-" + input.serverData[input.index].getNickname() + "- " + input.arguments[2] + "\r\n";
//		
//		if ((*target)[0] == '#' /*&& usuarioEsOperador()*/) //a un CHANNEL ---- OJO!!!!! El NOTICE para los CHANNELS solo lo pueden usar los OPERADORES
//		{
//			sd::channIt channel = input.serverData.findChannel(target->substr(1));
//			if (channel == 0) {
//				error::error(input, error::ERR_CANNOTSENDTOCHAN);
//				continue ;
//			}
//			input.serverData[channel].broadcast(input.index, message);
//		}
//		else 
//		{
//			sd::clientIt user = input.serverData.findNickname(*target);
//			if (user != 0)
//			{
//				if (input.arguments[2].empty()) {
//					error::error(input, error::ERR_NOTEXTTOSEND); 
//				}
//				utils::sendMsgUser(input.serverData[(sd::pollfdIt)user].fd, message);
//			}
//			else {
//				error::error(input, error::ERR_NOSUCHNICK);
//			}
//		}
//	}
//}


/* --------------------------------User-Based Queries----------------------------------- */

void	whois(CmdInput& input) 
{
	(void)input;
}

/* --------------------------------Operator Messages------------------------------------ */


void	kill(CmdInput& input) 
{

	if (input.arguments.size() < 1)
	{
		error::error(input, error::ERR_NEEDMOREPARAMS , "KILL"); //ARGUMENT ERROR
		return;
	}
	if (input.serverData[input.index].getRole() != CL_OPER)
	{
		error::error(input, error::ERR_NOPRIVILEGES);
		return;
	}
	sd::clientIt target_user = input.serverData.findNickname(input.arguments[1]);
	if (!target_user)
	{
		error::error(input, error::ERR_NOSUCHNICK, input.arguments[1]);
		return;
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

//casos en los que server hace quit EL SERVER : "Ping timeout: 120 seconds", "Excess Flood", and "Too many connections from this IP" 

}
/* --------------------------------Optional Messages------------------------------------ */


void	away(CmdInput& input) 
{
	if (input.serverData[input.index].getAwayStatus() == true || input.arguments.size() < 2) {
		input.serverData[input.index].setAwayStatus(false);
		input.serverData[input.index].setAwayMsg("");
		utils::sendMsgUser(input.serverData[(sd::pollfdIt)input.index].fd, reply(eRPL_UNAWAY, input));
		return ;
	}
	input.serverData[input.index].setAwayMsg(utils::joinStr(input.arguments, 1));
	input.serverData[input.index].setAwayStatus(true);
	utils::sendMsgUser(input.serverData[(sd::pollfdIt)input.index].fd, reply(eRPL_NOWAWAY, input));
}

END_ANONYMOUS_NAMESPACE
END_CMD_NAMESPACE


#endif