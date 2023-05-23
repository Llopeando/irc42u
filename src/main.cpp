#include "../include/Server.hpp"

#include <ifaddrs.h>
#include <netinet/in.h>
#include <net/if.h>
#include <netdb.h>

std::vector<std::string> split(const std::string &string, const char c)
{
	std::vector<std::string> finalString;
	bool		lock = false;
	uint32_t	index = 0;
	uint32_t	i;
	for (i = 0; i < string.size(); i++)
	{
		if (string[i] == c && lock == true && i != index)
		{
			//std::cout << "[" << i << "][" << string.substr(index, i - index) << "]\n" << std::flush;
			finalString.push_back(string.substr(index, i - index));
			index = i + 1;
			lock = false;
		}
		if (string[i] != c)
			lock = true;
	}
	if (lock == true)
	{
		//std::cout << "["<< i << "][" << string.substr(index, i - index) << "]\n";
		finalString.push_back(string.substr(index, i - index));
	}
	//std::cout << "split end size = " << finalString.size() << '\n';
	return finalString;
}

std::vector<std::string> splitIrcPrameters(const std::string &string, const char c)
{
	std::vector<std::string> finalString;
	bool		lock = false;
	uint32_t	index = 0;
	uint32_t	i;
	for (i = 0; i < string.size(); i++)
	{
		if (string[i] == ':') //Quitar :
		{
			lock = false;
			if (i - index > 0)
				finalString.push_back(string.substr(index, i - index));
			finalString.push_back(string.substr(i + 1, string.size() - i));
			return finalString;
		}
		if (string[i] == c && lock == true && i != index)
		{
			//std::cout << "[" << i << "][" << string.substr(index, i - index) << "]\n" << std::flush;
			finalString.push_back(string.substr(index, i - index));
			index = i + 1;
			lock = false;
		}
		if (string[i] != c)
			lock = true;
	}
	if (lock == true)
	{
		//std::cout << "["<< i << "][" << string.substr(index, i - index) << "]\n";
		finalString.push_back(string.substr(index, i - index));
	}
	//std::cout << "split end size = " << finalString.size() << '\n';
	return finalString;
}

void	serverCreateInfo(char **argv, int argc, t_serverInput *serverInfo)
{
	//check ( argumentos = 2 , puerto y password, puerto valido,...)
	if (argc != 3)
		throw std::runtime_error("bad arguments!"); //ya veremos si hacer excepciones definidas
	
	//RELLENAR STRUCT
	int port = std::atoi(argv[0]); //y de paso chequea si es un int valido
	serverInfo->password = std::string(argv[1]);
	serverInfo->address.sin_family = AF_INET;
	serverInfo->address.sin_addr.s_addr = INADDR_ANY;
	serverInfo->address.sin_port = htons( port ); //mirar htons
}

std::string printIp()
{
	struct ifaddrs *ifaddr, *ifa;
	char host[NI_MAXHOST];
	if (getifaddrs(&ifaddr) == -1) {
		std::cerr << "getifaddrs failed" << std::endl;
		exit (1);
	}
	for (ifa = ifaddr; ifa != nullptr; ifa = ifa->ifa_next) {
		if (ifa->ifa_addr == nullptr) {
			continue;
		}
		if (ifa->ifa_addr->sa_family == AF_INET && !(ifa->ifa_flags & IFF_LOOPBACK)) {
			
			int res = getnameinfo(ifa->ifa_addr, sizeof(struct sockaddr_in), host, NI_MAXHOST, nullptr, 0, NI_NUMERICHOST);
			if (res != 0) {
				std::cerr << "getnameinfo failed: " << gai_strerror(res) << std::endl;
			} else {
				std::cout << "IP address: " << host << '\n';
			}
			break;
		}
	}
	freeifaddrs(ifaddr);
	return std::string((char *)host);
}

std::string joinStr(std::vector<std::string>& arguments, uint32_t index)
{
	std::string finalString = "";
	for (uint32_t i = index; i < arguments.size(); i++)
	{
		finalString += arguments[i];
	}
	return finalString;
}

int	main(int argc, char *argv[])
{
	t_serverInput serverInfo;

	serverInfo.IP = printIp();	
	try {
		serverCreateInfo(argv + 1, argc, &serverInfo);
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}
	Server	*server = new Server(&serverInfo);
	//server->setSocket(serverInfo);
	server->run();
	delete server;
	return (0);
}
