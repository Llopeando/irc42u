#ifndef COMMANDS_H
#define COMMANDS_H

#include "Config.h"
#include "defines.hpp"
#include "ErrorHandler.hpp"
#include "command_structs.h"
#include "Client.hpp"
#include "Utils.hpp"

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
		invit
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
 	std::string message = ":" + input.serverData[input.index].getUserMask() + " 375 " + input.serverData[input.index].getNickname() + " :- " + input.serverData.getName() + " Message of the day - \r\n";
 	utils::sendMsgUser(input.serverData[(sd::pollfdIt)input.index].fd, message);
 	std::vector<std::string> words = utils::split(std::string(MOTD), '\n');
 	for (std::vector<std::string>::iterator it = words.begin(); it != words.end(); it++) {
 		std::string motd_message = ":" + input.serverData[input.index].getUserMask() + " 372 " + input.serverData[input.index].getNickname() + " : " + *it + "\r\n";
 		utils::sendMsgUser(input.serverData[(sd::pollfdIt)input.index].fd, motd_message);
 	}
 	message = ":" + input.serverData[input.index].getUserMask() + " 376 " + input.serverData[input.index].getNickname() + " :End of /MOTD command.\r\n";
 	utils::sendMsgUser(input.serverData[(sd::pollfdIt)input.index].fd, message);
 }

void	mode(CmdInput& input)  
{
	(void)input;
}

/* --------------------------------Connection Messages---------------------------------- */

void	ping(CmdInput& input)
{
	if (input.arguments.size() < 2)
	{
		error::error(input, error::ERR_NEEDMOREPARAMS , "PING");
		return;
	}
	std::string mask = input.serverData[input.index].getUserMask();
	std::string message = ":" + mask + input.arguments[0] + " PONG " + utils::joinStr(input.arguments, 2) + "\r\n";
	utils::sendMsgUser(input.serverData[(sd::pollfdIt)input.index].fd, message);
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
		std::string message =  ":" + input.serverData[input.index].getUserMask() + " NICK :" + input.arguments[1] + "\r\n";
		input.serverData[input.index].setNickname(input.arguments[1]);
		input.serverData[(sd::channIt)0].broadcast(0, message); 
		std::string mask = input.serverData[input.index].getUserMask();
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

		std::string welcome_1= "001 " + input.serverData[input.index].getNickname() + " :Welcome to the A O I R C server\n"; 
		std::string welcome_2 = "002 " + input.serverData[input.index].getNickname() + " : Your host is " + input.serverData.getName() + ", running version " + std::to_string(VERSION) + "\r\n"; //RPL_WELCOME (001)
		std::string welcome_3 = "003 " + input.serverData[input.index].getNickname() + " : This server was created " + input.serverData.getCreationDate() + "\r\n"; // RPL_YOURHOST (002)
		std::string welcome_4 = "004 " + input.serverData[input.index].getNickname() + " " + input.serverData.getName() + " " + std::to_string(VERSION) + "\r\n"; //RPL_MYINFO (004)
		std::string welcome_5 = "005 " + input.serverData[input.index].getNickname() + " CHANTYPES=# PREFIX=(o)@ MODES=4 CHANLIMIT=#:20 NICKLEN=16 USERLEN=10 HOSTLEN=63 TOPICLEN=390 KICKLEN=307 CHANNELLEN=32" + " :are supported by this server\r\n"; //RPL_ISUPPORT (005)

		utils::sendMsgUser(input.serverData[(sd::pollfdIt)input.index].fd, welcome_1);
		utils::sendMsgUser(input.serverData[(sd::pollfdIt)input.index].fd, welcome_2);
		utils::sendMsgUser(input.serverData[(sd::pollfdIt)input.index].fd, welcome_3);
		utils::sendMsgUser(input.serverData[(sd::pollfdIt)input.index].fd, welcome_4);
		utils::sendMsgUser(input.serverData[(sd::pollfdIt)input.index].fd, welcome_5);

	
		if (input.serverData[(sd::clientIt)input.index].getNickname().size() > 16) //	NICKLEN=16 
			input.serverData[(sd::clientIt)input.index].setNickname(input.serverData[(sd::clientIt)input.index].getNickname().substr(0, 16));
		if (input.serverData[(sd::clientIt)input.index].getUsername().size() > 10) // USERLEN=10
			input.serverData[(sd::clientIt)input.index].setNickname(input.serverData[(sd::clientIt)input.index].getUsername().substr(0, 10));

		/*	
		The server SHOULD then respond as though the client sent the LUSERS command and return the appropriate numerics. 
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
			std::string msg = ":" + input.serverData.getName() + " 381 " + input.serverData[input.index].getNickname() + " :You are now an IRC operator\r\n";
			input.serverData[input.index].setRole(CL_OPER);
			utils::sendMsgUser(input.serverData[(sd::pollfdIt)input.index].fd, msg);
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
	std::string message = ':' + input.serverData[input.index].getUserMask() + " QUIT :Quit:" + reason + "\r\n";
	input.serverData[(sd::channIt)0].broadcast(0, message);
	input.serverData.backClient(input.index);
	input.serverData.removeClientChannels(input.index);
	
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

			std::string back = ':' + input.serverData[input.index].getUserMask() + " JOIN " + "#" + input.serverData[channel].getName() + "\r\n";
			std::string back_mode = ':' + input.serverData.getName() + " MODE #" + input.serverData[channel].getName() + " " + " +nt\r\n";
			std::string back_list = ':' + input.serverData.getName() + " 353 " + input.serverData[input.index].getNickname() + " = #" + input.serverData[channel].getName() + " :" + input.serverData[channel].getUserList() + "\r\n";   //@for the operator??
			std::string back_list_end = ':' + input.serverData.getName() + " 366 " + input.serverData[input.index].getNickname() + " #" + input.serverData[channel].getName() + " :End of /NAMES list\r\n";

			utils::sendMsgUser(input.serverData[(sd::pollfdIt)input.index].fd, back);
			utils::sendMsgUser(input.serverData[(sd::pollfdIt)input.index].fd, back_mode);
			utils::sendMsgUser(input.serverData[(sd::pollfdIt)input.index].fd, back_list);
			utils::sendMsgUser(input.serverData[(sd::pollfdIt)input.index].fd, back_list_end);
		}
		else // EXISTE CHANNEL -> se une
		{
			input.serverData[channel].addClient(input.index);

			std::string back = ':' + input.serverData[input.index].getUserMask() + " JOIN #" + input.serverData[channel].getName() + "\r\n";
			std::string back_topic = ':' + input.serverData.getName() + " 332 " + input.serverData[input.index].getNickname() + " #" + input.serverData[channel].getName() + " :" + input.serverData[channel].getTopic() + "\r\n";
			std::string back_channel  = ':' + input.serverData.getName() + " 333 " + input.serverData[input.index].getNickname() + " #" + channelNames[i] + input.serverData[channel].getCreator() + " " + input.serverData[channel].getCreationDate() + "\r\n";
			std::string back_list  = ':' + input.serverData.getName() + " 353 " + input.serverData[input.index].getNickname() + " = #" + input.serverData[channel].getName() + " :" + input.serverData[channel].getUserList() + "\r\n";
			std::string back_list_end = ':' + input.serverData.getName() + " 366 " + input.serverData[input.index].getNickname()  + " #"+ input.serverData[channel].getName() + " :End of /NAMES list." + "\r\n";

			utils::sendMsgUser(input.serverData[(sd::pollfdIt)input.index].fd, back);
			utils::sendMsgUser(input.serverData[(sd::pollfdIt)input.index].fd, back_topic);
			utils::sendMsgUser(input.serverData[(sd::pollfdIt)input.index].fd, back_list);
			utils::sendMsgUser(input.serverData[(sd::pollfdIt)input.index].fd, back_list_end);
		}
		std::string message =  ":" +  input.serverData[input.index].getUserMask() +" JOIN :#" + input.serverData[channel].getName() + "\r\n";
		input.serverData[channel].broadcast(input.index, message);
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
		std::string message;
		if (!reason.empty())
			message = ":" + input.serverData[input.index].getUserMask() + " PART #" + input.serverData[channel].getName() + " :" + reason + "\r\n";
		else
			message = ":" + input.serverData[input.index].getUserMask() + " PART #" + input.serverData[channel].getName() + "\r\n";
		input.serverData[channel].broadcast(0, message);
		input.serverData[channel].removeClient(input.index);
		if (input.serverData[channel].getNumUser() == 0) 
			input.serverData.deleteChannel(channel);
	}
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
		std::string message = ":" + input.serverData[input.index].getUserMask() + " TOPIC #" + input.arguments[1].substr(1) + " :" + input.serverData[channel].getTopic() + "\r\n"; 
		input.serverData[channel].broadcast(0, message);
		input.serverData[channel].setCreationDate(utils::t_chrono::to_time_t(utils::t_chrono::now()));
	}
	else	//VIEWING topic
	{
		if(input.serverData[channel].getTopic().empty())
		{
			std::string no_topic  = ":" + input.serverData.getName() + " 331 " + input.serverData[input.index].getNickname() + " #" + input.serverData[channel].getName() + " :No topic is set.\r\n"; 
			return ;
		}
		std::string message  = ":" + input.serverData.getName() + " 332 " + input.serverData[input.index].getNickname() + " #" + input.serverData[channel].getName() + " :" + input.serverData[channel].getTopic() + "\r\n"; //RPL_TOPIC (332) 
		const char *date = input.serverData[channel].getCreationDate();
		std::string message2 = ":" + input.serverData.getName() + " 333 " + input.serverData[input.index].getNickname() + " #" + input.serverData[channel].getName() + " " +  input.serverData[channel].getCreator() + " " +  date + "\r\n"; //RPL_TOPICWHOTIME (333) //cambiar la fecha a legible?
		input.serverData[channel].broadcast(0, message);
		input.serverData[channel].broadcast(0, message2);
	}
}

void	names(CmdInput& input) 
{
	if (input.arguments.size() == 1)
	{
		for (uint32_t target = 1; target != input.serverData.getNumOfChannels(); target++)
		{
			std::string message = ':' + input.serverData.getName() + " 353 " + input.serverData[input.index].getNickname() + " @ #" + input.serverData[(sd::channIt)target].getName() + " :" + input.serverData[(sd::channIt)target].getUserList() + "\r\n";
			std::string back_list_end = ':' + input.serverData.getName() + " 366 " + input.serverData[input.index].getNickname() + " #" + input.serverData[(sd::channIt)target].getName() + " :End of /NAMES list.\r\n";
			utils::sendMsgUser(input.serverData[(sd::pollfdIt)input.index].fd, message);
			utils::sendMsgUser(input.serverData[(sd::pollfdIt)input.index].fd, back_list_end);
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
		std::string message = ':' + input.serverData.getName() + " 353 " + input.serverData[input.index].getNickname() + " = #" + input.serverData[channel].getName() + " :" + input.serverData[channel].getUserList() + "\r\n";
		std::string back_list_end = ':' + input.serverData.getName() + " 366 " + input.serverData[input.index].getNickname() + " #" + input.serverData[channel].getName() + " :End of /NAMES list.\r\n";
		utils::sendMsgUser(input.serverData[(sd::pollfdIt)input.index].fd, message);
		utils::sendMsgUser(input.serverData[(sd::pollfdIt)input.index].fd, back_list_end);
	}
}

void	list(CmdInput& input) 
{
	////RESET lista EN LIME????//

	std::string message = ":" + input.serverData.getName() + " 321 Channel:Users Name\r\n";
	utils::sendMsgUser(input.serverData[(sd::pollfdIt)input.index].fd, message);
	for (sd::channIt i = 1;i < input.serverData.getNumOfChannels() ; i++)
	{
		std::string back = ":" + input.serverData.getName() + " 322 " + input.serverData[input.index].getNickname() + " #" + input.serverData[i].getName() + " " +  std::to_string(input.serverData[i].getNumUser()) + " :" + input.serverData[i].getTopic() + "\r\n";
		utils::sendMsgUser(input.serverData[(sd::pollfdIt)input.index].fd, back);
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
			std::string message = ":" + input.serverData[input.index].getUserMask() + " " + input.arguments[0] + " "+ input.serverData[target_user].getNickname() + " " + input.arguments[2] + "\r\n";
			utils::sendMsgUser(input.serverData[(sd::pollfdIt)target_user].fd, message); 
		}
	}
	/*else if (arguments.size() == 2)
	{
		//list //RPL_INVITELIST (336) // RPL_ENDOFINVITELIST (337)
	}
	*/

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

	std::string reason;
	if (input.arguments.size() == 4)
	{
		reason = input.arguments[3];
	}
	else
	{
		reason = "The kick hammer has spoken!";
	}
	sd::channIt channel = input.serverData.findChannel(input.arguments[1].substr(1));
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
			std::string broadcast_message = ":" + input.serverData[input.index].getUserMask() + " KICK " + " #" + input.serverData[channel].getName() + " " + input.serverData[clientIdx].getNickname() + " :" + reason + "\r\n";
			input.serverData[channel].broadcast(0, broadcast_message);
			input.serverData[channel].removeClient(clientIdx);
		}
	}
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
		std::string message = ":" + input.serverData[input.index].getNickname() +  " " +  input.arguments[0] + " " + *target + " :" + input.arguments[2] + "\r\n";
		if ((*target)[0] == '#')
		{
			sd::channIt channel = input.serverData.findChannel(target->substr(1));
			if(channel == 0)
			{
				error::error(input, error::ERR_CANNOTSENDTOCHAN);  //NO CHANNEL 
				continue;
			}
			////CHECK BANNED??? MOD??? ->>>>>> ERR_CANNOTSENDTOCHAN (404)  
			input.serverData[channel].broadcast(input.index, message);
		}
		else 
		{
			sd::clientIt user = input.serverData.findNickname(*target);
			if (user != 0)
			{
				if (input.serverData[user].getAwayStatus() == true) 
				{
					utils::sendMsgUser(input.serverData[(sd::pollfdIt)input.index].fd, message);
					std::string away_msg = ":" + input.serverData.getName() + " 301 " + input.serverData[input.index].getNickname() + " " + input.serverData[user].getNickname() + " :" + input.serverData[user].getAwayMsg() + "\r\n"; // RPL_AWAY (301)
					utils::sendMsgUser(input.serverData[(sd::pollfdIt)input.index].fd, away_msg);
				}
				else 
				{	
					if(input.arguments[2].empty())  
					{
						error::error(input, error::ERR_NOTEXTTOSEND); //NO TEXT
					}
					utils::sendMsgUser(input.serverData[(sd::pollfdIt)user].fd, message);
				}
			
			}
			else 
			{
				//std::cout << color::red << "ERROR NO NICK" << color::reset << std::endl;
				error::error(input, error::ERR_NOSUCHNICK);
			}
		}
	
	}
}

void	notice(CmdInput& input) 
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
		std::string message = ":" + input.serverData.getName() + " NOTICE " + " :-" + input.serverData[input.index].getNickname() + "- " + input.arguments[2] + "\r\n";
		
		if ((*target)[0] == '#' /*&& usuarioEsOperador()*/) //a un CHANNEL ---- OJO!!!!! El NOTICE para los CHANNELS solo lo pueden usar los OPERADORES
		{
			sd::channIt channel = input.serverData.findChannel(target->substr(1));
			if (channel == 0) {
				error::error(input, error::ERR_CANNOTSENDTOCHAN);
				continue ;
			}
			input.serverData[channel].broadcast(input.index, message);
		}
		else 
		{
			sd::clientIt user = input.serverData.findNickname(*target);
			if (user != 0)
			{
				if (input.arguments[2].empty()) {
					error::error(input, error::ERR_NOTEXTTOSEND); 
				}
				utils::sendMsgUser(input.serverData[(sd::pollfdIt)user].fd, message);
			}
			else {
				error::error(input, error::ERR_NOSUCHNICK);
			}
		}
	}
}


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
	std::string reason = "";
	if (input.arguments.size() > 2)
	{
		reason += utils::joinStr(input.arguments, 2);
	}
	sd::clientIt target_user = input.serverData.findNickname(input.arguments[1]);
	if (!target_user)
	{
		error::error(input, error::ERR_NOSUCHNICK, input.arguments[1]);
		return;
	}

	std::string reason1 = "Killed (" + input.serverData[input.index].getNickname() + ")";

	std::string message_u = ':' + input.serverData[target_user].getUserMask() + " QUIT :Quit:" + input.serverData[input.index].getNickname() + "\r\n";
	//std::string message = data[index].getUserMask() + " has forced " + data[target_user].getNickname() + " to leave " + serverName + reason1 + "\r\n";
	std::string message_user = "ERROR :Closing Link: (~" + input.serverData[target_user].getUserMask() +  ") [Killed (" + input.serverData[input.index].getNickname() + " (" + reason + "))]\r\n";
	input.serverData[(sd::channIt)0].broadcast(0, message_u);
	//sendMsgUser(data[(pollfdIt)target_user].fd, message_u);
	utils::sendMsgUser(input.serverData[(sd::pollfdIt)target_user].fd, message_user);
	close(input.serverData[(sd::pollfdIt)target_user].fd);
	input.serverData.backClient(target_user);
	input.serverData.removeClientChannels(target_user);

// OTRA OPCION ----> this->quit(target_user, arguments);
//casos en los que server hace quit EL SERVER : "Ping timeout: 120 seconds", "Excess Flood", and "Too many connections from this IP" 

}
/* --------------------------------Optional Messages------------------------------------ */


void	away(CmdInput& input) 
{
	if (input.serverData[input.index].getAwayStatus() == true || input.arguments.size() < 2) {
		input.serverData[input.index].setAwayStatus(false);
		input.serverData[input.index].setAwayMsg("");
		std::string message  = ":" + input.serverData[input.index].getUserMask() + " 305 " + input.serverData[input.index].getNickname() + " :You are no longer marked as being away\r\n";
		utils::sendMsgUser(input.serverData[(sd::pollfdIt)input.index].fd, message);
		return ;
	}
	input.serverData[input.index].setAwayMsg(utils::joinStr(input.arguments, 1));
	input.serverData[input.index].setAwayStatus(true);
	std::string message  = ":" + input.serverData[input.index].getUserMask() + " 306 " + input.serverData[input.index].getNickname() + " :You have been marked as being away\r\n";
	utils::sendMsgUser(input.serverData[(sd::pollfdIt)input.index].fd, message);
}


END_ANONYMOUS_NAMESPACE
END_CMD_NAMESPACE


#endif