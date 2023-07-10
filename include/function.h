#ifndef FUNCTION_H
#define FUNCTION_H

#include "commands.h"
#include "command_structs.h"

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

static eFlags callFunction(const std::string& key, CmdInput& input)
{
	CmdMap::const_iterator it = getFunctionMap().find(key);
	if (it != getFunctionMap().end()) {
		it->second(input);
		return eFlags::eSuccess;
	}
	else
	{
		return eFlags::eNoSuchFunction;
	}
}

START_ANONYMOUS_NAMESPACE
//anonymous namespace, similar to private in a class. Obscures access to types and cmdMap from outside.

typedef void (*pFunction)(CmdInput& input);
typedef std::unordered_map<std::string, pFunction> CmdMap;

static const CmdMap& getFunctionMap()
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
		cmdMap["CAP"]		= &cap;
		cmdMap["TOPIC"]		= &topic;
		cmdMap["LIST"]		= &list;
		cmdMap["PART"]		= &part;
		cmdMap["MOTD"]		= &motd;
		cmdMap["OPER"]		= &oper;
		cmdMap["KILL"]		= &kill;
	}
	return cmdMap;
}



END_ANONYMOUS_NAMESPACE
END_CMD_NAMESPACE

#endif