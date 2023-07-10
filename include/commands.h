#ifndef COMMANDS_H
#define COMMANDS_H

#include "Config.h"
#include "defines.hpp"
#include "ErrorHandler.hpp"
#include "command_structs.h"

#include <string>
#include <vector>
#include <deque>
#include <iostream>

START_CMD_NAMESPACE

START_ANONYMOUS_NAMESPACE

/*
*	All functions reside inside namespace cmd namespace anonymous so it cannot be accessed from outside
*/


void	oper(CmdInput& input) // OPER <name> <password> // OPER mike WzerT8zq 
{
	if (input.arguments.size() < 2)
	{
		error::error(index, error::Type::ERR_NEEDMOREPARAMS); 
		return ;
	}
	if (operators.findOper(arguments[1]))
	{
		if (operators.checkPass(arguments[1], arguments[2]) == false) //no es la contraseña correcta
		{
			errorHandler.error(index, error::Type::ERR_PASSWDMISMATCH);
			return ;
		}
		else 
		{
			std::string msg = ":" + serverName + " 381 " + data[index].getNickname() + " :You are now an IRC operator\r\n";
			data[index].setRole(CL_OPER);
			sendMsgUser(data[(pollfdIt)index].fd, msg);
		}
	}
	else 
		errorHandler.error(index, error::Type::ERR_NOOPERHOST); //no estas como oper en el host
}

void	pass(CmdInput& input)
{
	if (data[index].getAuthentificied())
		return ;
	if (arguments.size() < 2)
	{
		errorHandler.error(index, error::Type::ERR_PASSWDMISMATCH);
		errorHandler.fatalError(index, error::Type::ERR_BADPASSWORD);
		data.removeClient(index);
		removeClientChannels(index);
		return ;
	}
	if (serverInfo.password == arguments[1])
	{
		data[index].setAuthentificied(true);
		return ;
	}
	else
	{
		errorHandler.error(index, error::Type::ERR_PASSWDMISMATCH);
		errorHandler.fatalError(index, error::Type::ERR_BADPASSWORD);
		data.removeClient(index);
		removeClientChannels(index);
	}
}

void	kill(CmdInput& input)
{

	if (arguments.size() < 1)//ARGUMENT ERROR
	{
		errorHandler.error(index, error::Type::ERR_NEEDMOREPARAMS , "KILL");
		return;
	}
	if (data[index].getRole() != CL_OPER)
	{
		errorHandler.error(index, error::Type::ERR_NOPRIVILEGES);
		return;
	}
	std::string reason = "";
	if (arguments.size() > 2)
	{
		reason += joinStr(arguments, 2);
	}
	clientIt target_user = data.findNickname(arguments[1]);
	if (!target_user)
	{
		errorHandler.error(index, error::Type::ERR_NOSUCHNICK, arguments[1]);
		return;
	}error::Type::error::Type::

	std::string reason1 = "Killed (" + data[index].getNickname() + ")";

	std::string message_u = ':' + data[target_user].getUserMask() + " QUIT :Quit:" + data[index].getNickname() + "\r\n";
	//std::string message = data[index].getUserMask() + " has forced " + data[target_user].getNickname() + " to leave " + serverName + reason1 + "\r\n";
	std::string message_user = "ERROR :Closing Link: (~" + data[target_user].getUserMask() +  ") [Killed (" + data[index].getNickname() + " (" + reason + "))]\r\n";
	channels[0].broadcast(0, message_u);
	//sendMsgUser(data[(pollfdIt)target_user].fd, message_u);
	sendMsgUser(data[(pollfdIt)target_user].fd, message_user);



	close(data[(pollfdIt)target_user].fd);
//Closing link: (~eperaita@195.53.111.155) [Killed (DickCheney (meow))]
//Closing Link: (~eperaita!eperaita@10.12.7.2) [Killed (ullorent (Killed (ullorent)))] 
	
	data.backClient(target_user);
	removeClientChannels(target_user);

// this->quit(target_user, arguments);
/* QUIT

	std::string reason = "";
	if (arguments.size() == 2)
		reason = arguments[1];
	std::string message = ':' + data[index].getUserMask() + " QUIT :Quit:" + reason + "\r\n";
	channels[0].broadcast(0, message);
	data.backClient(index);
	removeClientChannels(index);
*/
	//std::string message = ":" + data[	index].getUserMask() + " 361 " + data[target_user].getNickname() + " :" + reason + "\r\n"; //361	RPL_KILLDONE	RFC1459
	//:dan-!d@localhost QUIT :Quit: Bye for now!
	//std::string message = ':' + data[index].getUserMask() + " QUIT :Quit:" + argument[1] + "\r\n";
}


void	nick(CmdInput& input)
{
	if (arguments.size() < 2 || arguments[1].empty())
	{
		errorHandler.error(index, error::Type::error::Type::ERR_NONICKNAMEGIVEN);
		//nueva string par que pueda meter uno nuevo ?
		return ;
	}
	if (arguments[1].size() == 0)
	{
		errorHandler.error(index, error::Type::error::Type::ERR_ERRONEUSNICKNAME);

		return;
	}
	for (std::string::iterator c = arguments[1].begin(); c != arguments[1].end(); c++)
	{
		if (*c == '#' || !std::isprint(*c))
		{
			errorHandler.error(index, error::Type::error::Type::ERR_ERRONEUSNICKNAME, arguments[1]); 
			return;
		}
	}
	//nickname repetido -> ERROR y cliente devielve de nuevo NICK + _ ?????
	if (data.findNickname(arguments[1])) 
	{
		errorHandler.error(index, error::Type::error::Type::ERR_NICKNAMEINUSE);
		return ;
	}
	else 
	{
		std::string message =  ":" + data[index].getUserMask() + " NICK :" + arguments[1] + "\r\n";
		data[index].setNickname(arguments[1]);
		channels[0].broadcast(0, message); 
		std::string mask = data[index].getUserMask();
	}
}

void	user(CmdInput& input)
{
	if (arguments.size() < 5 || arguments[1].empty()) //NO ARGS
	{
		errorHandler.error(index, error::Type::ERR_NEEDMOREPARAMS); 
		return ;
	}
	//The maximum length of <username> may be specified by the USERLEN RPL_ISUPPORT parameter.  MUST be silently truncated to the given length 
	// The minimum length of <username> is 1, ie. it MUST NOT be empty. 
	if (data.findUsername(arguments[1])) 
	{
		errorHandler.error(index, error::Type::ERR_ALREADYREGISTERED); 
		return ;
	}
	if (data.findNicknameBack(data[index].getNickname())) //si esta en back 
	{
		//std::cout << color::blue << "cliente recuperado : antes fd : " << color::reset << std::endl;
		data.transferIndex(index, data[index].getNickname());
		removeClientChannels(index);
	}
	else
	{
		//std::cout << color::blue << "nuevo cliente" << color::reset << std::endl;
		data[index].setUsername(arguments[1]); //lo añade a clients
	}
		
	if (data[index].getAuthentificied()) //ha autentificado, ha mandado PASS y es ok )
	{
		channels[0].addClient(index);

		/* SUCCESSFUL CONNECTION : info del server para el USUARIO */

		std::string welcome = "001 " + data[index].getNickname() + " :Welcome to the A O I R C server\n"; ///TIENE QUE IR AQUI??? , o FUERA EN EL BUCLE DE COMANDOS COMO METER???
		sendMsgUser(data[(pollfdIt)index].fd, welcome);
		


	/* INFO extra del server para el USUARIO 
	Upon successful completion of the registration process, the server MUST send, in this order, the 
	RPL_WELCOME (001), RPL_YOURHOST (002), RPL_CREATED (003), RPL_MYINFO (004), and at least one RPL_ISUPPORT (005) numeric to the client.
	*/

		std::string welcome_2 = "002 " + data[index].getNickname() + " : Your host is " + serverName + ", running version 1.0 " + "\r\n";//"<client> :Your host is <servername>, running version <version>"
		std::string welcome_3 = "003 " + data[index].getNickname() + " : This server was created 1" + "\r\n"; // "<client> :This server was created <datetime>"
		/*  "<client> <servername> <version> <available user modes>
			<available channel modes> [<channel modes with a parameter>]*/
		std::string welcome_4 = "004 " + data[index].getNickname() + " " + serverName + " 1.0 +i,+o\r\n";
		sendMsgUser(data[(pollfdIt)index].fd, welcome_2);
		sendMsgUser(data[(pollfdIt)index].fd, welcome_3);
		sendMsgUser(data[(pollfdIt)index].fd, welcome_4);

		std::vector<std::string> motd_arguments;
		motd(index, motd_arguments);

	/*llama a comando LUSERS 
	
	The server SHOULD then respond as though the client sent the LUSERS command and return the appropriate numerics. If the user has client modes set on them automatically upon joining the network, 
	the server SHOULD send the client the RPL_UMODEIS (221) reply or a MODE message with the client as target, preferably the former. The server MAY send other numerics and messages. 
	The server MUST then respond as though the client sent it the MOTD command, i.e. it must send either the successful Message of the Day numerics or the ERR_NOMOTD (422) numeric.
	
	*/

	}
	else
	{
		errorHandler.error(index, error::Type::ERR_PASSWDMISMATCH);
		errorHandler.fatalError(index, error::Type::ERR_BADPASSWORD);
		//remove client from server
		data.removeClient(index);
		removeClientChannels(index);
	}
}

void	privmsg(CmdInput& input)
{
	if (arguments.size() < 2 || arguments[1].empty()) //NO ARGS
	{
		//std::cout << color::red << "ERROR NORECIPIENT" << color::reset << std::endl;
		errorHandler.error(index, ERR_NORECIPIENT); 
		return ;
	}
	std::vector<std::string> targets = split(arguments[1], ',');
	std::set<std::string> uniqueNames;
	for (std::vector<std::string>::iterator target = targets.begin(); target != targets.end(); target++)
	{
		if (!uniqueNames.insert(*target).second) {
			// ERR_TOOMANYTARGETS (407) // <target> :Duplicate recipients. No message delivered
			errorHandler.error(index, error::Type::ERR_TOOMANYTARGETS, *target);
			continue ;
		}
		std::string message = ":" + data[index].getNickname() +  " " +  arguments[0] + " " + *target + " :" + arguments[2] + "\r\n";
		if ((*target)[0] == '#') 	//  channel membership prefix character (@, +, etc) 
		{
			
			uint32_t channel = findChannel(target->substr(1));
			if(channel == 0)
			{
				//std::cout << color::red << "ERROR NOCHANNEL 404" << color::reset << std::endl;
				errorHandler.error(index, error::Type::ERR_CANNOTSENDTOCHAN);//NO CHANNEL 404
				continue;
			}
			////CHECK BANNED??? MOD??? ->>>>>> ERR_CANNOTSENDTOCHAN (404)  
			channels[channel].broadcast(index, message); //PRIVMSG TO CHANNEL SUCCESS 
		}
		else 
		{
			clientIt user = data.findNickname(*target);
			if (user != 0)
			{
				if (data[user].getAwayStatus() == true) // RPL_AWAY (301)
				{
					sendMsgUser(data[(pollfdIt)index].fd, message);
					std::string away_msg = ":" + serverName + " 301 " + data[index].getNickname() + " " + data[user].getNickname() + " :" + data[user].getAwayMsg() + "\r\n";
					sendMsgUser(data[(pollfdIt)index].fd, away_msg);
				}
				else //PRIVMSG TO USER SUCCESS 
				{	
					if(arguments[2].empty()) //ERR_NOTEXTTOSEND (412) 
					{
					//	std::cout << color::red << "ERROR NOTEXT" << color::reset << std::endl;
						errorHandler.error(index, error::Type::ERR_NOTEXTTOSEND); 
					}
					sendMsgUser(data[(pollfdIt)user].fd, message);
				}
			
			}
			else 
			{
				//std::cout << color::red << "ERROR NO NICK" << color::reset << std::endl;
				errorHandler.error(index, error::Type::ERR_NOSUCHNICK);
			}
		}
	
	}
}

void	join(CmdInput& input)
{
	if (arguments.size() < 2)
	{
		errorHandler.error(index, ERR_NEEDMOREPARAMS, "JOIN");
		return ;
	}
	std::vector<std::string>channelNames = split(arguments[1], ',');

	//////JOIN 0 TE PIRAS SDE TODOS LOS CANALES 

	for (uint32_t i = 0;i < channelNames.size();i++)
	{
		 //ARGUMENT ERROR 
		if (arguments[1][0] != '#' || arguments[1].size() < 2 || !std::isprint(arguments[1][1]))
		{
			errorHandler.error(index, ERR_BADCHANMASK, arguments[1]);
			continue ;
		}
		uint32_t channel = findChannel(channelNames[i].substr(1));
		//NO EXISTE CHANNEL ->se crea y se une 
		if(!channel) 
		{
			if (data[index].getRole() != CL_OPER)
				data[index].setRole(CL_OP);
			channels.push_back(Channel(channelNames[i].substr(1), data[index].getUsername(), &data));
			channel = channels.size() - 1;
			channels[channel].addClient(index);

			std::string back = ':' + data[index].getUserMask() + " JOIN " + "#" + channels[channel].getName() + "\r\n";//bien
			std::string back_mode = ':' + serverName + " MODE #" + channels[channel].getName() + " " + " +nt\r\n";
			std::string back_list = ':' + serverName + " 353 " + data[index].getNickname() + " = #" + channels[channel].getName() + " :" + channels[channel].getUserList() + "\r\n";//@for the operator
			std::string back_list_end = ':' + serverName + " 366 " + data[index].getNickname() + " #" + channels[channel].getName() + " :End of /NAMES list.\r\n";

			sendMsgUser(data[(pollfdIt)index].fd, back);
			sendMsgUser(data[(pollfdIt)index].fd, back_mode);
			sendMsgUser(data[(pollfdIt)index].fd, back_list);
			sendMsgUser(data[(pollfdIt)index].fd, back_list_end);
		}
		else // EXISTE CHANNEL ->se une
		{
			channels[channel].addClient(index);

			std::string back = ':' + data[index].getUserMask() + " JOIN #" + channels[channel].getName() + "\r\n";
			std::string back_topic = ':' + serverName + " 332 " + data[index].getNickname() + " #" + channels[channel].getName() + " :" + channels[channel].getTopic() + "\r\n";
			//std::string back_channel  = ':' + serverName + " 333 " + data[index].getNickname() + " #" + channelNames[i] + "quien lo ha creado (mask) y cuando"  + "\r\n";
			std::string back_list  = ':' + serverName + " 353 " + data[index].getNickname() + " = #" + channels[channel].getName() + " :" + channels[channel].getUserList() + "\r\n";
			std::string back_list_end = ':' + serverName + " 366 " + data[index].getNickname()  + " #"+ channels[channel].getName() + " :End of /NAMES list." + "\r\n";

			sendMsgUser(data[(pollfdIt)index].fd, back);
			sendMsgUser(data[(pollfdIt)index].fd, back_topic);
			sendMsgUser(data[(pollfdIt)index].fd, back_list);
			sendMsgUser(data[(pollfdIt)index].fd, back_list_end);
		}
		std::string message =  ":" +  data[index].getUserMask() +" JOIN :#" + channels[channel].getName() + "\r\n";
		channels[channel].broadcast(index, message);
	}
}

void	list(CmdInput& input){

	////LISTA RESET EN LIME????
	// los demas parametros obviamos???
	(void)arguments; 
	std::string message = ":" + serverName + " 321: Channel Users Name\r\n";
	sendMsgUser(data[(pollfdIt)index].fd, message);
	for (uint32_t i = 1;i < channels.size() ; i++)
	{
		std::string back = ":" + serverName + " 322 " + data[index].getNickname() + " #" + channels[i].getName() + " " +  std::to_string(channels[i].getNumUser()) + " :" + channels[i].getTopic() + "\r\n";
		sendMsgUser(data[(pollfdIt)index].fd, back);
	}
}

void	part(CmdInput& input)
{
	if (arguments.size() < 2)//ARGUMENT ERROR
	{
		errorHandler.error(index, ERR_NEEDMOREPARAMS , "PART");
		return;
	}
	std::string reason;
	if (arguments.size() > 2)//REASON
		reason = joinStr(arguments, 2);

	std::vector<std::string> channelNames = split(arguments[1], ',');
	for (std::vector<std::string>::iterator channelName = channelNames.begin(); channelName != channelNames.end(); channelName++)
	{
		
		if(!((*channelName)[0] == '#' || (*channelName)[0] == '@')) //NO ES UN CHANNEL 
		{
			errorHandler.error(index, ERR_BADCHANMASK, *channelName);
			continue ;
		}
		uint32_t channel = findChannel(channelName->substr(1));
		if (channel == 0)//NO EXISTE CHANNEL 
		{
			errorHandler.error(index, ERR_NOSUCHCHANNEL, *channelName);
			continue ;
		}
		if(!channels[channel].findUser(index)) //NO ESTAS EN EL CHANNEL 
		{
			errorHandler.error(index, ERR_NOTONCHANNEL, *channelName);
			continue ;
		}
		std::string message;
		if (!reason.empty())
			message = ":" + data[index].getUserMask() + " PART #" + channels[channel].getName() + " :" + reason + "\r\n";
		else
			message = ":" + data[index].getUserMask() + " PART #" + channels[channel].getName() + "\r\n";
		channels[channel].broadcast(0, message);
		channels[channel].removeClient(index);
		if (channels[channel].getNumUser() == 0) //when all users leave a group channel, the channel is deleted
			deleteChannel(channel);
	}
}

void	topic(CmdInput& input) {

	if (arguments.size() < 2)//ARGUMENT ERROR
	{
		errorHandler.error(index, ERR_NEEDMOREPARAMS , "TOPIC");
		return;
	}
	if (!data[index].getRole()) //solo operators
	{
		errorHandler.error(index, ERR_CHANOPRIVSNEEDED, arguments[1].substr(1));//(481)   no channel operator 
		return;
	}
	uint32_t channel = findChannel(arguments[1].substr(1));
	if(!(arguments[1][0] == '#' || arguments[1][0] == '@')) //NO ES UN CHANNEL 
	{
		errorHandler.error(index, ERR_BADCHANMASK, arguments[1]);
		return ;
	}
	if (channel == 0)//NO EXISTE CHANNEL 
	{
		errorHandler.error(index, ERR_NOSUCHCHANNEL, arguments[1]);
		return ;
	}
	if(!channels[channel].findUser(index)) //NO ESTAS EN EL CHANNEL 
	{
		errorHandler.error(index, ERR_NOTONCHANNEL, arguments[1]);
		return ;
	}
	//si no eres operator
			// ERR_CHANOPRIVSNEEDED (482)
			// s-> :irc.example.com 482 dan #v5 :You're not channel operator

	if( arguments.size() >= 3) //SETTING topic
	{
		channels[channel].setTopic(joinStr(arguments, 2));
		std::string message = ":" + data[index].getUserMask() + " TOPIC #" + arguments[1].substr(1) + " :" + channels[channel].getTopic() + "\r\n"; //RPL_TOPIC (332)
		channels[channel].broadcast(0, message);
		channels[channel].setCreationDate(t_chrono::to_time_t(t_chrono::now()));
	}
	else	//VIEWING topic
	{
		if(channels[channel].getTopic().empty())
		{
			std::string no_topic  = ":" + serverName + " 331 " + data[index].getNickname() + " #" + channels[channel].getName() + " :No topic is set.\r\n"; //RPL_TOPIC (332)
			return ;
		}
		std::string message  = ":" + serverName + " 332 " + data[index].getNickname() + " #" + channels[channel].getName() + " :" + channels[channel].getTopic() + "\r\n"; //RPL_TOPIC (332)
		std::time_t date = channels[channel].getCreationDate();
		std::string message2 = ":" + serverName + " 333 " + data[index].getNickname() + " #" + channels[channel].getName() + " " +  channels[channel].getCreator() + " " +  std::ctime(&date) + "\r\n"; //RPL_TOPICWHOTIME (333) //cambiar la fecha a legible?
		channels[channel].broadcast(0, message);
		channels[channel].broadcast(0, message2);
	}
}


void	notice(CmdInput& input)
{
	if (arguments.size() < 2 || arguments[1].empty()) //NO ARGS
	{
		std::cout << color::red << "ERROR NORECIPIENT" << color::reset << std::endl;
		errorHandler.error(index, ERR_NORECIPIENT); 
		return ;
	}
	std::vector<std::string> targets = split(arguments[1], ',');
	std::set<std::string> uniqueNames;
	for (std::vector<std::string>::iterator target = targets.begin(); target != targets.end(); target++)
	{
		if (!uniqueNames.insert(*target).second) {
			errorHandler.error(index, ERR_TOOMANYTARGETS, *target);
			continue ;
		}
		std::string message = ":" + serverName + " NOTICE " + " :-" + data[index].getNickname() + "- " + arguments[2] + "\r\n";
		
		if ((*target)[0] == '#' /*&& usuarioEsOperador()*/) //a un CHANNEL ---- OJO!!!!! El NOTICE para los CHANNELS solo lo pueden usar los OPERADORES
		{
			uint32_t channel = findChannel(target->substr(1));
			if (channel == 0) {
				errorHandler.error(index, ERR_CANNOTSENDTOCHAN);
				continue;
			}
			channels[channel].broadcast(index, message);
		}
		else 
		{
			clientIt user = data.findNickname(*target);
			if (user != 0)
			{
				if (arguments[2].empty()) {
					errorHandler.error(index, ERR_NOTEXTTOSEND); 
				}
				sendMsgUser(data[(pollfdIt)user].fd, message);
			}
			else {
				errorHandler.error(index, ERR_NOSUCHNICK);
			}
		}
	}
}

	/*
	 casos en los que server hace quit a alguen "Ping timeout: 120 seconds", "Excess Flood", and "Too many connections from this IP" are examples of relevant reasons for closing or for a connection with a client to have been closed.
	*/

void	quit(CmdInput& input) //reply->  :dan-!d@localhost QUIT :Quit: Bye for now!
{	
	std::string reason = "";
	if (arguments.size() == 2)
		reason = arguments[1];
	std::string message = ':' + data[index].getUserMask() + " QUIT :Quit:" + reason + "\r\n";
	channels[0].broadcast(0, message);
	data.backClient(index);
	removeClientChannels(index);
	//std::cout << color::blue << "cliente a back" << color::reset << std::endl;
	
}

void	mode(CmdInput& input)
{
	(void)index;
	(void)arguments;
}

void	names(CmdInput& input)
{
	if (arguments.size() == 1)
	{
		for (std::deque<Channel>::const_iterator target = channels.begin() + 1; target != channels.end(); target++)
		{
			std::string message = ':' + serverName + " 353 " + data[index].getNickname() + " @ #" + target->getName() + " :" + target->getUserList() + "\r\n";
			std::string back_list_end = ':' + serverName + " 366 " + data[index].getNickname() + " #" + target->getName() + " :End of /NAMES list.\r\n";
			sendMsgUser(data[(pollfdIt)index].fd, message);
			sendMsgUser(data[(pollfdIt)index].fd, back_list_end);
		}
		return ;
	}
	std::vector<std::string> targets = split(arguments[1], ',');
	for (std::vector<std::string>::const_iterator target = targets.begin(); target != targets.end(); target++)
	{
		uint32_t channel = findChannel(target->substr(1));
		if (!channel)
		{
			errorHandler.error(index, ERR_NOSUCHCHANNEL, arguments[1]);
			continue;
		}
		std::string message = ':' + serverName + " 353 " + data[index].getNickname() + " = #" + channels[channel].getName() + " :" + channels[channel].getUserList() + "\r\n";
		std::string back_list_end = ':' + serverName + " 366 " + data[index].getNickname() + " #" + channels[channel].getName() + " :End of /NAMES list.\r\n";
		sendMsgUser(data[(pollfdIt)index].fd, message);
		sendMsgUser(data[(pollfdIt)index].fd, back_list_end);
	}
}

void	whois(CmdInput& input)
{
	(void)index;
	(void)arguments;
}

void	kick(CmdInput& input) // KICK <channel> <user> *( "," <user> ) [<comment>]
{
	if (arguments.size() < 3)
	{
		errorHandler.error(index, ERR_NEEDMOREPARAMS , "KICK");
		return;
	}
	if (!data[index].getRole())
	{
		errorHandler.error(index, ERR_CHANOPRIVSNEEDED, arguments[2].substr(1));//(481)   no channel operator 
		return;
	}
	////no operators are out!

	std::string reason;
	
	if (arguments.size() == 4) //REASON
	{
		reason = arguments[3];
	}
	else
	{
		reason = "The kick hammer has spoken!";   //no me gustaaaaaaa
	}
	uint32_t channel = findChannel(arguments[1].substr(1));
	if (channel == 0)
	{
		errorHandler.error(index, ERR_NOSUCHCHANNEL, arguments[1].substr(1));
		return ;
	}
	else if (data[index].getRole() == CL_OP && !channels[channel].findUser(index))
	{
		errorHandler.error(index, ERR_NOTONCHANNEL, channels[channel].getName()); //NO ESTAS EN ESE CHANNEL 
	}
	std::vector<std::string>targets = split(arguments[2], ',');
	for(std::vector<std::string>::const_iterator target = targets.begin();target != targets.end(); target++)
	{
		clientIt clientIdx = data.findNickname(*target);
		if (clientIdx == 0) 									//no existe target
			errorHandler.error(index, ERR_NOSUCHNICK);
		else if (channels[channel].findUser(clientIdx) == 0) 	//TARGET no esta en el canal 
			errorHandler.error(index, ERR_USERNOTINCHANNEL);
		else
		{
			std::string broadcast_message = ":" + data[index].getUserMask() + " KICK " + " #" + channels[channel].getName() + " " + data[clientIdx].getNickname() + " :" + reason + "\r\n";
			channels[channel].broadcast(0, broadcast_message);
			channels[channel].removeClient(clientIdx);
		}
	}
}

void	away(CmdInput& input)
{
	if (data[index].getAwayStatus() == true || arguments.size() < 2) {
		data[index].setAwayStatus(false);
		data[index].setAwayMsg("");
		std::string message  = ":" + data[index].getUserMask() + " 305 " + data[index].getNickname() + " :You are no longer marked as being away\r\n";
		sendMsgUser(data[(pollfdIt)index].fd, message);
		return ;
	}
	data[index].setAwayMsg(joinStr(arguments, 1));
	data[index].setAwayStatus(true);
	std::string message  = ":" + data[index].getUserMask() + " 306 " + data[index].getNickname() + " :You have been marked as being away\r\n";
	sendMsgUser(data[(pollfdIt)index].fd, message);
}


void	invite(CmdInput& input)
{
	if (arguments.size() < 2)//ARGUMENT ERROR
	{
		errorHandler.error(index, ERR_NEEDMOREPARAMS , "INVITE");
		return;
	}
	if (!data[index].getRole())
	{
		errorHandler.error(index, ERR_CHANOPRIVSNEEDED, arguments[2].substr(1));//(481)   no channel operator 
		return;
	}
	else if (arguments.size() == 3)
	{
		clientIt target_user = data.findNickname(arguments[1]);
		if (!target_user)
		{
			errorHandler.error(index, ERR_NOSUCHNICK, arguments[1]);
			return;
		}
		std::string channel_name = arguments[2].substr(1);
		uint32_t channel = findChannel(channel_name);
		if (channel == 0)
			errorHandler.error(index, ERR_NOSUCHCHANNEL, arguments[2]); // NO EXISTE CHANNEL
		else if (data[index].getRole() == CL_OP && !channels[channel].findUser(index))
		{
			errorHandler.error(index, ERR_NOTONCHANNEL, channels[channel].getName()); //NO ESTAS EN ESE CHANNEL 
		}
		else if (channels[channel].findUser(target_user)) //TARGET YA ESTA EN EL CHANNEL
		{
			errorHandler.error(index, ERR_USERONCHANNEL, arguments[1]);
		}
		else
		{
			std::string message = ":" + data[index].getUserMask() + " " + arguments[0] + " "+ data[target_user].getNickname() + " " + arguments[2] + "\r\n"; //reply->  :dan-!d@localhost INVITE Wiz #test
			sendMsgUser(data[(pollfdIt)target_user].fd, message);  ///SUCCESSSSS
		}
	}
	/*else if (arguments.size() == 2)
	{
		//list //RPL_INVITELIST (336) // RPL_ENDOFINVITELIST (337)
	}
	*/

}

void	motd(CmdInput& input) {
	(void)arguments;
	std::string message = ":" + data[index].getUserMask() + " 375 " + data[index].getNickname() + " :- " + SERVER_NAME + " Message of the day - \r\n";
	sendMsgUser(data[(pollfdIt)index].fd, message);

	std::vector<std::string> words = split(std::string(MOTD), '\n');
	for (std::vector<std::string>::iterator it = words.begin(); it != words.end(); it++) {
		std::string motd_message = ":" + data[index].getUserMask() + " 372 " + data[index].getNickname() + " : " + *it + "\r\n";
		sendMsgUser(data[(pollfdIt)index].fd, motd_message);
	}
	message = ":" + data[index].getUserMask() + " 376 " + data[index].getNickname() + " :End of /MOTD command.\r\n";
	sendMsgUser(data[(pollfdIt)index].fd, message);
}

void	ping(CmdInput& input)
{
	if (arguments.size() < 2)//ARGUMENT ERROR 
	{
		errorHandler.error(index, ERR_NEEDMOREPARAMS , "PING"); // ERR_NOORIGIN (409) IN THE OLD IRCS 
		return;
	}
	std::string mask = data[index].getUserMask();
	std::string message = ":" + mask + arguments[0] + " PONG " + joinStr(arguments, 2) + "\r\n";
	sendMsgUser(data[(pollfdIt)index].fd, message);
}

END_ANONYMOUS_NAMESPACE
END_CMD_NAMESPACE


#endif