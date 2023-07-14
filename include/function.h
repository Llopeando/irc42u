#ifndef FUNCTION_H
#define FUNCTION_H

#include "Config.h"
#include "commands.h"
#include "cmd_structs.h"

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
		//es una ñapa para arreglar el eExited que devuelve QUIT n 
		////////RECOGER eflags para saber si es eSucesss o eExited o eFailure de callFunction() para hacer ahi un bucle logico bien
		if (key == "QUIT")
			return eExited;
		return eSuccess;
	}
	else
	{
		return eNoSuchFunction;
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
		
		/*
		
			cmdMap["CAP_REQ"]	= &Server::cap_req;
		 	cmdMap["CAP_LS"]	= &Server::cap_ls;
		 	cmdMap["CAP_END"]	= &Server::cap_end;
		 	cmdMap["CAP_ACK"]	= &Server::cap_ack;
		 	cmdMap["CAP_NAK"]	= &Server::cap_nak;

		*/
	}
	return cmdMap;
}

END_ANONYMOUS_NAMESPACE

END_CMD_NAMESPACE

#endif