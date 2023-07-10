#include "defines.hpp"
#include <vector>
#include "ServerDataStructs.h"

class ServerData;

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