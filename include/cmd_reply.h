/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_reply.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ullorent <ullorent@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/27 17:44:03 by ullorent          #+#    #+#             */
/*   Updated: 2023/08/03 20:15:43 by ullorent         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "defines.h"
#include "ServerData.hpp"

#include <map>


/* CMD namespace  : 

	ereply() -> Enums of the replies that will be the key to call reply()																										//Public

	struct CmdInput and struct CmdInputVar -> the package that will be sent to the reply. CmdInputVar is an aux void * variable to allocate data								//Public

	typedef eFlags , typedef std::string (*pRplFunction)(CmdInput& input), , typedef CmdMap, typedef RplMap;																	//Anonymous

	reply() -> Call function to access the map 																																	//Public

	Al the rpl_command() that return the string definitions																														//Anonymous	

	const RplMap& getReplyMap() -> map to find the key of the command																											//Anonymous
*/


START_CMD_NAMESPACE

enum eReply{
	eRPL_WELCOME	= 001,
	eRPL_YOURHOST	= 002,
	eRPL_CREATED	= 003,
	eRPL_MYINFO		= 004,
	eRPL_ISUPPORT	= 005,

	eRPL_PONG		=   6,
	eRPL_NICK		=   7,
	eRPL_QUIT		=   8,
	eRPL_JOIN		=   9,
	eRPL_JOINMODE	=  10,
	eRPL_PART		=  11,
	eRPL_KICK		=  12,
	eRPL_PRIVMSG	=  13,
	eRPL_KILL		=  14,
	eRPL_LUSERCLIENT = 251,
	eRPL_LUSERME	= 255,
	eRPL_AWAY		= 301,
	eRPL_UNAWAY		= 305,
	eRPL_NOWAWAY	= 306,
	eRPL_LISTSTART	= 321,
	eRPL_LIST		= 322,
	eRPL_LISTEND	= 323,
	eRPL_NOTOPIC	= 331,
	eRPL_TOPIC		= 332,
	eRPL_TOPICWHOTIME = 333,
	eRPL_INVITING	= 341,
	eRPL_VERSION	= 351,
	eRPL_NAMREPLY	= 353,
	eRPL_ENDOFNAMES	= 366,
	eRPL_WHOIS		= 318,
	eRPL_MOTD		= 375,
	eRPL_YOUREOPER	= 385,
	eRPL_TIME		= 391,
	eRPL_HELPSTART	= 704,
	eRPL_HELPTXT	= 705
};

struct CmdInputVar{
	void *data;
	CmdInputVar *pnext;
};

struct CmdInput{
	CmdInput(std::vector<std::string>& arguments, sd::ServerData &serverData,sd::clientIt index)
		: arguments(arguments), serverData(serverData), index(index){
	}

	std::vector<std::string>& arguments;
	sd::ServerData &serverData;
	sd::clientIt index;
	CmdInputVar	*var;
};


START_ANONYMOUS_NAMESPACE

typedef eFlags (*pCmdFunction)(CmdInput& input);
typedef std::string (*pRplFunction)(CmdInput& input);
typedef std::map<std::string, pCmdFunction> CmdMap;
typedef std::map<eReply, pRplFunction> RplMap;

const RplMap& getReplyMap();

END_ANONYMOUS_NAMESPACE

inline std::string reply(eReply key, CmdInput &input)
{
	RplMap::const_iterator it = getReplyMap().find(key);
	if (it != getReplyMap().end())
		return it->second(input);
	else
	{
		std::cout << color::red << "ERROR, failed to find reply\n" << color::reset;
		return "";
	}
}

START_ANONYMOUS_NAMESPACE

inline std::string	rpl_welcome(CmdInput& input){
	return ("001 " + input.serverData[input.index].getNickname() + " :Welcome to the A O I R C server\r\n"); 
}

inline std::string	rpl_yourhost(CmdInput& input){
	return ("002 " + input.serverData[input.index].getNickname() + " : Your host is " + input.serverData.getName() + ", running version " + input.serverData.getConfig().version + "\r\n"); 
}

inline std::string	rpl_created(CmdInput& input){
	return ("003 " + input.serverData[input.index].getNickname() + " : This server was created " + input.serverData.getCreationDate() + "\r\n");
}

inline std::string	rpl_myinfo(CmdInput& input){
	return ("004 " + input.serverData[input.index].getNickname() + " " + input.serverData.getName() + " " + input.serverData.getConfig().version + "\r\n");
}
inline std::string	rpl_isupport(CmdInput& input){
	return ("005 " + input.serverData[input.index].getNickname() + " CHANTYPES=" + input.serverData.getConfig().chantypes
																+ " PREFIX=" + input.serverData.getConfig().prefix
																+ " MODES=" + std::to_string(input.serverData.getConfig().modes)
																+ " CHANLIMIT=" + std::to_string(input.serverData.getConfig().chanlimit)
																+ " NICKLEN=" + std::to_string(input.serverData.getConfig().nicklen)
																+ " USERLEN=" + std::to_string(input.serverData.getConfig().userlen)
																+ " HOSTLEN=" + std::to_string(input.serverData.getConfig().hostlen)
																+ " TOPICLEN=" + std::to_string(input.serverData.getConfig().topiclen)
																+ " KICKLEN=" + std::to_string(input.serverData.getConfig().kicklen)
																+ " CHANNELLEN=" + std::to_string(input.serverData.getConfig().channellen) + " :are supported by this server\r\n");
}

inline std::string	rpl_motd(CmdInput& input) 
{
	std::string message = ":" + input.serverData[input.index].getUserMask() + " 375 " + input.serverData[input.index].getNickname() + " :- " + input.serverData.getName() + " Message of the day - \r\n";
	utils::sendMsgUser(input.serverData[(sd::pollfdIt)input.index].fd, message);
	std::vector<std::string> words = utils::split(std::string(MOTD), '\n');
	for (std::vector<std::string>::iterator it = words.begin(); it != words.end(); it++) 
	{
		std::string motd_message = ":" + input.serverData[input.index].getUserMask() + " 372 " + input.serverData[input.index].getNickname() + " : " + *it + "\r\n";
		utils::sendMsgUser(input.serverData[(sd::pollfdIt)input.index].fd, motd_message);
	}
	std::string end = ":" + input.serverData[input.index].getUserMask() + " 376 " + input.serverData[input.index].getNickname() + " :End of /MOTD command.\r\n";
	return(end);
}

inline std::string rpl_version(CmdInput& input)
{
	return (":" + input.serverData[input.index].getUserMask() + " 351 " + input.serverData[input.index].getNickname() + " " + input.serverData.getConfig().version + " " + input.serverData.getName() + ":" + input.serverData.getConfig().versionComments + "\r\n");
}

inline std::string rpl_luserclient(CmdInput& input)
{
	return ("251 " + input.serverData[input.index].getNickname() + " :There are " + std::to_string(input.serverData.getNumOfClients()-1) + " users on 1 servers\r\n"); // we only have 1 server 
}

inline std::string rpl_luserme(CmdInput& input)
{
	return ("255 " + input.serverData[input.index].getNickname() + " : A O I R C : I have " + std::to_string(input.serverData.getNumOfClients()-1) + " clients and 0 servers\r\n"); //we will never have any server connected 
}

inline std::string rpl_pong(CmdInput& input)
{
	std::string mask = input.serverData[input.index].getUserMask();
	return(":" + mask + " PONG " + input.serverData.getName() + " :" + utils::joinStr(input.arguments, 1) + "\r\n");
}

inline std::string rpl_nick(CmdInput& input)
{
	return (":" + input.serverData[input.index].getUsername() + " NICK :" + input.arguments[1] + "\r\n");
}

inline std::string rpl_youreoper(CmdInput& input)
{
	return (":" + input.serverData.getName() + " 381 " + input.serverData[input.index].getNickname() + " :You are now an IRC operator\r\n");
}

inline std::string rpl_quit(CmdInput& input)
{
	std::string *reason = static_cast<std::string *>(input.var->data);
	std::string *mask = static_cast<std::string *>(input.var->pnext->data);
	return (':' + *mask + " QUIT :Quit: " + *reason + "\r\n");
}

inline std::string rpl_join(CmdInput& input)
{
	sd::channIt *channel = static_cast<sd::channIt *>(input.var->data);
	return(':' + input.serverData[input.index].getUserMask() + " JOIN #" + input.serverData[*channel].getName() + "\r\n");
}

inline std::string rpl_joinmode(CmdInput& input)
{
	sd::channIt *channel = static_cast<sd::channIt *>(input.var->data);
	return (':' + input.serverData.getName() + " MODE #" + input.serverData[*channel].getName() + " " + " +nt\r\n"); //RPL_UMODEIS 221
}

inline std::string rpl_namreply(CmdInput& input)
{
	sd::channIt *channel = static_cast<sd::channIt *>(input.var->data);
	return(':' + input.serverData.getName() + " 353 " + input.serverData[input.index].getNickname() + " = #" + input.serverData[*channel].getName() + " :" + input.serverData.getUserList(*channel) + "\r\n");
}

inline std::string rpl_endofnames(CmdInput& input)
{
	sd::channIt *channel = static_cast<sd::channIt *>(input.var->data);
	return (':' + input.serverData.getName() + " 366 " + input.serverData[input.index].getNickname() + " #" + input.serverData[*channel].getName() + " :End of /NAMES list\r\n"); 
}

inline std::string rpl_topic(CmdInput& input)
{
	sd::channIt *channel = static_cast<sd::channIt *>(input.var->data);
	return (':' + input.serverData.getName() + " 332 " + input.serverData[input.index].getNickname() + " #" + input.serverData[*channel].getName() + " :" + input.serverData[*channel].getTopic() + "\r\n"); 
}

inline std::string rpl_topicwhotime(CmdInput& input)
{
	sd::channIt *channel = static_cast<sd::channIt *>(input.var->data);
	return (':' + input.serverData.getName() + " 333 " + input.serverData[input.index].getNickname() + " #" + input.serverData[*channel].getName() + " " + input.serverData[*channel].getCreator() + " " + input.serverData[*channel].getCreationDate() + "\r\n");
}

inline std::string rpl_part(CmdInput& input)
{
	std::string *reason = static_cast<std::string *>(input.var->data);
	sd::channIt *channel = static_cast<sd::channIt *>(input.var->pnext->data);
	if (!(*reason).empty())
		return(":" + input.serverData[input.index].getUserMask() + " PART #" + input.serverData[*channel].getName() + " :" + *reason + "\r\n");
	else
		return(":" + input.serverData[input.index].getUserMask() + " PART #" + input.serverData[*channel].getName() + "\r\n");

}

inline std::string rpl_notopic(CmdInput& input)
{
	sd::channIt *channel = static_cast<sd::channIt *>(input.var->data);
	return(":" + input.serverData.getName() + " 331 " + input.serverData[input.index].getNickname() + " #" + input.serverData[*channel].getName() + " :No topic is set.\r\n");
}

inline std::string rpl_liststart(CmdInput& input)
{
	return(":" + input.serverData.getName() + " 321 Channel:Users Name\r\n");
}

inline std::string rpl_list(CmdInput& input)
{
	sd::channIt *channel = static_cast<sd::channIt *>(input.var->data);
	return (":" + input.serverData.getName() + " 322 " + input.serverData[input.index].getNickname() + " #" + input.serverData[*channel].getName() + " " +  std::to_string(input.serverData[*channel].getNumUser() - 1) + " :" + input.serverData[*channel].getTopic() + "\r\n");
}

inline std::string rpl_listend(CmdInput& input)
{
	return( ":" + input.serverData[input.index].getUserMask() + " 323 " + "\r\n");
}

inline std::string rpl_inviting(CmdInput& input)
{
	sd::clientIt *target_user = static_cast<sd::clientIt *>(input.var->data);
	return(":" + input.serverData[input.index].getUserMask() + " " + input.arguments[0] + " "+ input.serverData[*target_user].getNickname() + " " + input.arguments[2] + "\r\n");
}

inline std::string rpl_kick(CmdInput& input)
{
	std::string reason;
	if (input.arguments.size() == 4)
		reason = input.arguments[3];
	else
		reason = "The kick hammer has spoken!";
	sd::channIt *channel = static_cast<sd::channIt *>(input.var->data);
	sd::clientIt *client_idx = static_cast<sd::clientIt *>(input.var->pnext->data);
	return ( ":" + input.serverData[input.index].getUserMask() + " KICK " + " #" + input.serverData[*channel].getName() + " " + input.serverData[*client_idx].getNickname() + " :" + reason + "\r\n");
}

inline std::string rpl_privmsg(CmdInput& input)
{
	std::string *target = static_cast<std::string *>(input.var->data);
	return (":" + input.serverData[input.index].getNickname() +  " " +  input.arguments[0] + " " + *target + " :" + input.arguments[2] + "\r\n");
}

inline std::string rpl_away(CmdInput& input)
{
	sd::clientIt *user = static_cast<sd::clientIt*>(input.var->data);
	return (":" + input.serverData.getName() + " 301 " + input.serverData[input.index].getNickname() + " " + input.serverData[*user].getNickname() + " :" + input.serverData[*user].getAwayMsg() + "\r\n");
}

inline std::string rpl_unaway(CmdInput& input)
{
	return(":" + input.serverData[input.index].getUserMask() + " 305 " + input.serverData[input.index].getNickname() + " :You are no longer marked as being away\r\n");
}

inline std::string rpl_nowaway(CmdInput& input)
{
	return (":" + input.serverData[input.index].getUserMask() + " 306 " + input.serverData[input.index].getNickname() + " :You have been marked as being away\r\n");
}

inline std::string rpl_kill(CmdInput& input)
{
	
	sd::clientIt *user = static_cast<sd::clientIt *>(input.var->data);
	std::string *reason = static_cast<std::string *>(input.var->pnext->data);

	return (':' + input.serverData[input.index].getNickname() + " KILL " + input.serverData[*user].getNickname() + " : " + *reason + "\r\n");
}

inline std::string rpl_whois(CmdInput &input)
{
	sd::clientIt *user = static_cast<sd::clientIt*>(input.var->data);
	return ("311 " + input.serverData[*user].getNickname() + " " + input.serverData[*user].getNickname() + " " + input.serverData[*user].getUsername() + " " + input.serverData[*user].getHostname() + " * \r\n");
}

inline std::string rpl_helpstart(CmdInput &input)
{
	return (":" + input.serverData[input.index].getUserMask() + " 704 " + input.serverData[input.index].getNickname() + " :Start of /HELP command \r\n");
}

inline std::string rpl_helptxt(CmdInput &input)
{
	std::vector<std::string> lines = utils::split(std::string(INFO), '\n');
	for (std::vector<std::string>::iterator it = lines.begin(); it != lines.end(); it++) 
	{
		std::string motd_message = ":" + input.serverData[input.index].getUserMask() + " 705 " + input.serverData[input.index].getNickname() + " : " + *it + "\r\n";
		utils::sendMsgUser(input.serverData[(sd::pollfdIt)input.index].fd, motd_message);
	}
	std::string end = ":" + input.serverData[input.index].getUserMask() + " 706 " + input.serverData[input.index].getNickname() + " :End of /HELP command.\r\n";
	return(end);
}

const RplMap& getReplyMap()
{
	static RplMap rplMap;
	if (rplMap.empty())
	{
		rplMap[eRPL_WELCOME]		= &rpl_welcome; //001
		rplMap[eRPL_YOURHOST]		= &rpl_yourhost; //002
		rplMap[eRPL_CREATED]		= &rpl_created; //003
		rplMap[eRPL_MYINFO]			= &rpl_myinfo;  //004
		rplMap[eRPL_ISUPPORT]		= &rpl_isupport; //005
		rplMap[eRPL_PONG]			= &rpl_pong; //006
		rplMap[eRPL_NICK]			= &rpl_nick;//007
		rplMap[eRPL_QUIT]			= &rpl_quit;//008
		rplMap[eRPL_JOIN]			= &rpl_join;//009
		rplMap[eRPL_JOINMODE]		= &rpl_joinmode;//010
		rplMap[eRPL_PART]			= &rpl_part;//011
		rplMap[eRPL_KICK]			= &rpl_kick;//012
		rplMap[eRPL_PRIVMSG]		= &rpl_privmsg;//013
		rplMap[eRPL_KILL]			= &rpl_kill;//014
		rplMap[eRPL_LISTSTART]		= &rpl_liststart;//321
		rplMap[eRPL_LIST]			= &rpl_list ;//322
		rplMap[eRPL_LISTEND]		= &rpl_listend ;//323
		rplMap[eRPL_LUSERCLIENT]	= &rpl_luserclient;// 251
		rplMap[eRPL_LUSERME]		= &rpl_luserme;//255
		rplMap[eRPL_AWAY]			= &rpl_away;//301
		rplMap[eRPL_UNAWAY]			= &rpl_unaway;//305
		rplMap[eRPL_NOWAWAY]		= &rpl_nowaway;//306
		rplMap[eRPL_WHOIS]			= &rpl_whois;//318
		rplMap[eRPL_NOTOPIC]		= &rpl_notopic;//331
		rplMap[eRPL_TOPIC]			= &rpl_topic;//332
		rplMap[eRPL_TOPICWHOTIME]	= &rpl_topicwhotime;//333
		rplMap[eRPL_INVITING]		= &rpl_inviting;//341
		rplMap[eRPL_VERSION]		= &rpl_version;//351
		rplMap[eRPL_NAMREPLY]		= &rpl_namreply;// 353
		rplMap[eRPL_ENDOFNAMES]		= &rpl_endofnames;//366
		rplMap[eRPL_MOTD]			= &rpl_motd;//375
		rplMap[eRPL_YOUREOPER]		= &rpl_youreoper;//381
		rplMap[eRPL_HELPSTART] 		= &rpl_helpstart; //704
 		rplMap[eRPL_HELPTXT]		= &rpl_helptxt; //705
	}
	return rplMap;
}

END_ANONYMOUS_NAMESPACE

END_CMD_NAMESPACE