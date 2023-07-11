#ifndef COMMAND_STRUCTS_H
#define COMMAND_STRUCTS_H

#include "defines.hpp"
#include <vector>
#include "ServerDataStructs.h"
#include "ServerData.hpp"

namespace cmd
{
	enum eFlags{
		eSuccess,
		eNoSuchFunction,
	};

	struct CmdInput{
		std::vector<std::string>& arguments;
		ServerData &serverData;
		sd::clientIt index;
	};
}


#endif