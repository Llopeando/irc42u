#include "Config.h"
#include "cmd_structs.h"

START_CMD_NAMESPACE


START_ANONYMOUS_NAMESPACE

std::string	rpl_welcome(CmdInput& input){
	return ("001 " + input.serverData[input.index].getNickname() + " :Welcome to the A O I R C server\n"); 
}

std::string	rpl_yourhost(CmdInput& input){
	return ("002 " + input.serverData[input.index].getNickname() + " : Your host is " + input.serverData.getName() + ", running version " + std::to_string(VERSION) + "\r\n"); 
}

std::string	rpl_created(CmdInput& input){
	return ( "003 " + input.serverData[input.index].getNickname() + " : This server was created " + input.serverData.getCreationDate() + "\r\n");
}

std::string	rpl_myinfo(CmdInput& input){
	return ("004 " + input.serverData[input.index].getNickname() + " " + input.serverData.getName() + " " + std::to_string(VERSION) + "\r\n");
}
std::string	rpl_isupport(CmdInput& input){
	return ("005 " + input.serverData[input.index].getNickname() + " CHANTYPES=# PREFIX=(o)@ MODES=4 CHANLIMIT=#:20 NICKLEN=16 USERLEN=10 HOSTLEN=63 TOPICLEN=390 KICKLEN=307 CHANNELLEN=32" + " :are supported by this server\r\n");
}

std::string rpl_motdstart(CmdInput& input){
	return (":" + input.serverData[input.index].getUserMask() + " 375 " + input.serverData[input.index].getNickname() + " :- " + input.serverData.getName() + " Message of the day - \r\n");
}

std::string	rpl_motd(CmdInput& input) //375//372//376
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

std::string rpl_version(CmdInput& input)
{
	return ("351 " + input.serverData[input.index].getNickname() +  " " + std::to_string(VERSION) + " " + input.serverData.getName() + "/r/n");
}

std::string rpl_luserclient(CmdInput& input)
{
	return ("251 " + input.serverData[input.index].getNickname() + ":There are " + std::to_string(input.serverData. getNumOfClients()) + " users on 1 servers/r/n"); // we only have 1 server 
}

std::string rpl_luserme(CmdInput& input)
{
	return ("255 " + input.serverData[input.index].getNickname() + ":I have " + std::to_string(input.serverData. getNumOfClients()) + " clients and 0 servers/r/n"); //we will never have any server connected 
}


std::string rpl_pong(CmdInput& input)
{
	std::string mask = input.serverData[input.index].getUserMask();
	return(":" + mask + input.arguments[0] + " PONG " + utils::joinStr(input.arguments, 2) + "\r\n");
}

std::string rpl_nick(CmdInput& input)
{
	return (":" + input.serverData[input.index].getUserMask() + " NICK :" + input.arguments[1] + "\r\n");
}

std::string rpl_youreoper(CmdInput& input)
{
	return (":" + input.serverData.getName() + " 381 " + input.serverData[input.index].getNickname() + " :You are now an IRC operator\r\n");
}

std::string rpl_quit(CmdInput& input)
{
	std::string reason = "";
	if (input.arguments.size() == 2)
		reason = input.arguments[1];
	return (':' + input.serverData[input.index].getUserMask() + " QUIT :Quit:" + reason + "\r\n");
}

std::string rpl_join(CmdInput& input)
{
	sd::channIt *channel = static_cast<sd::channIt *>(input.var->data);
	return(':' + input.serverData[input.index].getUserMask() + " JOIN " + "#" + input.serverData[*channel].getName() + "\r\n");
}

std::string rpl_joinmode(CmdInput& input)
{
	sd::channIt *channel = static_cast<sd::channIt *>(input.var->data);
	return (':' + input.serverData.getName() + " MODE #" + input.serverData[*channel].getName() + " " + " +nt\r\n"); 
}

std::string rpl_namreply(CmdInput& input)
{
	sd::channIt *channel = static_cast<sd::channIt *>(input.var->data);
	return(':' + input.serverData.getName() + " 353 " + input.serverData[input.index].getNickname() + " = #" + input.serverData[*channel].getName() + " :" + input.serverData[*channel].getUserList() + "\r\n");
}

std::string rpl_endofnames(CmdInput& input)
{
	sd::channIt *channel = static_cast<sd::channIt *>(input.var->data);
	return (':' + input.serverData.getName() + " 366 " + input.serverData[input.index].getNickname() + " #" + input.serverData[*channel].getName() + " :End of /NAMES list\r\n"); 
}

std::string rpl_topic(CmdInput& input)
{
	sd::channIt *channel = static_cast<sd::channIt *>(input.var->data);
	return (':' + input.serverData.getName() + " 332 " + input.serverData[input.index].getNickname() + " #" + input.serverData[*channel].getName() + " :" + input.serverData[*channel].getTopic() + "\r\n"); 
}


std::string rpl_topicwhotime(CmdInput& input)
{
	sd::channIt *channel = static_cast<sd::channIt *>(input.var->data);
	return (':' + input.serverData.getName() + " 333 " + input.serverData[input.index].getNickname() + " #" + input.serverData[*channel].getName() + " " + input.serverData[*channel].getCreator() + " " + input.serverData[*channel].getCreationDate() + "\r\n");
}

std::string rpl_part(CmdInput& input)
{
	std::string *reason = static_cast<std::string *>(input.var->data);
	sd::channIt *channel = static_cast<sd::channIt *>(input.var->pnext->data);
	if (!(*reason).empty())
		return(":" + input.serverData[input.index].getUserMask() + " PART #" + input.serverData[*channel].getName() + " :" + *reason + "\r\n");
	else
		return(":" + input.serverData[input.index].getUserMask() + " PART #" + input.serverData[*channel].getName() + "\r\n");
}

std::string rpl_notopic(CmdInput& input)
{
	sd::channIt *channel = static_cast<sd::channIt *>(input.var->data);
	return(":" + input.serverData.getName() + " 331 " + input.serverData[input.index].getNickname() + " #" + input.serverData[*channel].getName() + " :No topic is set.\r\n");
}

std::string rpl_liststart(CmdInput& input)
{
	return(":" + input.serverData.getName() + " 321 Channel:Users Name\r\n");
}

std::string rpl_list(CmdInput& input)
{
	sd::channIt *channel = static_cast<sd::channIt *>(input.var->data);
	return (":" + input.serverData.getName() + " 322 " + input.serverData[input.index].getNickname() + " #" + input.serverData[*channel].getName() + " " +  std::to_string(input.serverData[*channel].getNumUser()) + " :" + input.serverData[*channel].getTopic() + "\r\n");
}

std::string rpl_inviting(CmdInput& input)
{
	sd::clientIt *target_user = static_cast<sd::clientIt *>(input.var->data);
	return(":" + input.serverData[input.index].getUserMask() + " " + input.arguments[0] + " "+ input.serverData[*target_user].getNickname() + " " + input.arguments[2] + "\r\n");
}

std::string rpl_kick(CmdInput& input)
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

std::string rpl_privmsg(CmdInput& input)
{
	std::string *target = static_cast<std::string *>(input.var->data);
	return (":" + input.serverData[input.index].getNickname() +  " " +  input.arguments[0] + " " + *target + " :" + input.arguments[2] + "\r\n");
}

std::string rpl_away(CmdInput& input)
{
	sd::clientIt *user = static_cast<sd::clientIt*>(input.var->data);
	return (":" + input.serverData.getName() + " 301 " + input.serverData[input.index].getNickname() + " " + input.serverData[*user].getNickname() + " :" + input.serverData[*user].getAwayMsg() + "\r\n");
}

std::string rpl_unaway(CmdInput& input)
{
	return(":" + input.serverData[input.index].getUserMask() + " 305 " + input.serverData[input.index].getNickname() + " :You are no longer marked as being away\r\n");
}
std::string rpl_nowaway(CmdInput& input)
{
	return (":" + input.serverData[input.index].getUserMask() + " 306 " + input.serverData[input.index].getNickname() + " :You have been marked as being away\r\n");
}



END_ANONYMOUS_NAMESPACE

END_CMD_NAMESPACE