#ifndef COMMAND_STRUCTS_H
#define COMMAND_STRUCTS_H

#include "defines.hpp"
#include <vector>
#include "ServerDataStructs.h"
#include "ServerData.hpp"


START_CMD_NAMESPACE

enum eFlags{
	eSuccess,
	eNoSuchFunction,
};

struct CmdInput{
	CmdInput(std::vector<std::string>& arguments, sd::ServerData &serverData,sd::clientIt index)
		: arguments(arguments), serverData(serverData), index(index){
	}

	std::vector<std::string>& arguments;
	sd::ServerData &serverData;
	sd::clientIt index;
};

END_CMD_NAMESPACE


#endif