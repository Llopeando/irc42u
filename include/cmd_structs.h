#ifndef CMD_STRUCTS_H
#define CMD_STRUCTS_H

#include "Config.h"
#include "defines.hpp"
#include <vector>
#include "ServerDataStructs.h"
#include "ServerData.hpp"
#include <map>


START_CMD_NAMESPACE

enum eFlags{
	eSuccess,
	eNoSuchFunction,
};

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
	eRPL_NOTOPIC	= 331,
	eRPL_TOPIC		= 332,
	eRPL_TOPICWHOTIME = 333,
	eRPL_INVITING	= 341,
	eRPL_VERSION	= 351,
	eRPL_NAMREPLY	= 353,
	eRPL_ENDOFNAMES	= 366,
	eRPL_MOTD		= 375,
	eRPL_YOUREOPER	= 385,
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

typedef void (*pCmdFunction)(CmdInput& input);
typedef std::string (*pRplFunction)(CmdInput& input);
typedef std::unordered_map<std::string, pCmdFunction> CmdMap;
typedef std::map<eReply, pRplFunction> RplMap;

END_ANONYMOUS_NAMESPACE

END_CMD_NAMESPACE


#endif