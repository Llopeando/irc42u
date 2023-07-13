#ifndef FUNCTION_H
#define FUNCTION_H

#include "config.h"
#include "commands.h"
#include "cmd_structs.h"
#include "cmd_reply.h"

/*
*	Public:
*		TYPES
*		CmdFlags:	Return value of CmdReturn contains an enum of type CmdFlags, each refering to
*						to a different return valuey
*		CmdInput:	Struct containing all the variables needed to call the cmdMap functions
*		
*		FUNCTIONS
*		callFunction:	Will call any function that matches the key passed as a parameter.
*							Receives a key and a CmdInput. Returns a CmdReturn.
*	
*	Private:
*		pFunction:	Typedef of function pointer -> void __(clientIt index, std::vector<std::string>& arguments);
*		cmdMap:		Typedef of unordered map -> std::unordered_map<std::string, pFunction>.
*/

START_CMD_NAMESPACE

START_ANONYMOUS_NAMESPACE

const CmdMap& getFunctionMap();
const RplMap& getReplyMap();

END_ANONYMOUS_NAMESPACE

eFlags callFunction(const std::string& key, CmdInput& input)
{
	CmdMap::const_iterator it = getFunctionMap().find(key);
	if (it != getFunctionMap().end()) {
		it->second(input);
		return eSuccess;
	}
	else
	{
		return eNoSuchFunction;
	}
}

std::string reply(eReply key, CmdInput &input)
{
	RplMap::const_iterator it = getReplyMap().find(key);
	if (it != getReplyMap().find(key)) {
		return it->second(input);
	}
	else
	{
		return nullptr;
	}
}


START_ANONYMOUS_NAMESPACE
//anonymous namespace, similar to private in a class. Obscures access to types and cmdMap from outside.

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
		//cmdMap["NOTICE"]	= &notice;
		cmdMap["QUIT"]		= &quit;
		cmdMap["MODE"]		= &mode;
		cmdMap["NAMES"]		= &names;
		cmdMap["WHOIS"]		= &whois;
		cmdMap["KICK"]		= &kick;
		cmdMap["AWAY"]		= &away;
		cmdMap["INVITE"]	= &invite;
		cmdMap["PING"]		= &ping;
		//cmdMap["CAP"]		= &cap;
		cmdMap["TOPIC"]		= &topic;
		cmdMap["LIST"]		= &list;
		cmdMap["PART"]		= &part;
		cmdMap["MOTD"]		= &motd;
		cmdMap["OPER"]		= &oper;
		cmdMap["KILL"]		= &kill;
		cmdMap["VERSION"]	= &version;
		cmdMap["LUSERS"]	= &lusers;
	}
	return cmdMap;
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
		rplMap[eRPL_LISTSTART]		= &rpl_liststart;//321
		rplMap[eRPL_LIST]			= &rpl_list ;//322
		rplMap[eRPL_LUSERCLIENT]	= &rpl_luserclient;// 251
		rplMap[eRPL_LUSERME]		= &rpl_luserme;//255
		rplMap[eRPL_AWAY]			= &rpl_away;//301
		rplMap[eRPL_UNAWAY]			= &rpl_unaway;//305
		rplMap[eRPL_NOWAWAY]		= &rpl_nowaway;//306
		rplMap[eRPL_NOTOPIC]		= &rpl_notopic;//331
		rplMap[eRPL_TOPIC]			= &rpl_topic;//332
		rplMap[eRPL_TOPICWHOTIME]	= &rpl_topicwhotime;//333
		rplMap[eRPL_INVITING]		= &rpl_inviting;//341
		rplMap[eRPL_VERSION]		= &rpl_version;//351
		rplMap[eRPL_NAMREPLY]		= &rpl_namreply;// 353
		rplMap[eRPL_MOTD]			= &rpl_motd;//375
		rplMap[eRPL_YOUREOPER]		= &rpl_youreoper;//381



	}
	return rplMap;
}

END_ANONYMOUS_NAMESPACE

END_CMD_NAMESPACE

#endif