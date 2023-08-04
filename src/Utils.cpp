#include "../include/Utils.hpp"

namespace utils{
	void sendMsgUser(int fd, std::string str)
	{
		int buffer_size = 65536;
	
		setsockopt(fd, SOL_SOCKET, SO_SNDBUF, &buffer_size, sizeof(buffer_size));
		int code;
		if ((code = send(fd, str.c_str(), str.size(), 0)) <= 0)
		{
			std::error_code ec(errno, std::system_category());
			std::cerr << color::red << "ERROR "<< color::boldwhite <<"An error ocurred sending the message: [" << str << "] error [" << color::boldwhite << ec.message() << "]" << color::reset << std::endl;
		}
	}

	std::vector<std::string> split(const std::string &string, const char c)
	{
		std::vector<std::string> tokens;
		std::string::size_type start = string.find_first_not_of(c);
		std::string::size_type end = string.find_first_of(c, start);
			
		while (start != std::string::npos)
		{
			if (end != std::string::npos)
			{
				tokens.push_back(string.substr(start, end - start));
				start = string.find_first_not_of(c, end);
				end = string.find_first_of(c, start);
			}
			else
			{
				tokens.push_back(string.substr(start));
				break;
			}
		}
		return tokens;
	}

	std::vector<std::string> splitIrcPrameters(const std::string &string, const char c)
	{
		std::vector<std::string> tokens;
		std::string::size_type start = string.find_first_not_of(c);
		std::string::size_type end = string.find_first_of(c, start);
		std::string::size_type colom = string.find_first_of(':', start);
			
		while (start != std::string::npos)
		{
			if (colom < end)
			{
				if (string.size() - colom - 1 == 0)
					break ;
				tokens.push_back(string.substr(colom + 1, string.size() - colom - 1));
				break ;
			}
			if (end != std::string::npos)
			{
				tokens.push_back(string.substr(start, end - start));
				start = string.find_first_not_of(c, end);
				end = string.find_first_of(c, start);
			}
			else
			{
				tokens.push_back(string.substr(start));
				break;
			}
		}
		return tokens;
	}

	std::string joinStr(std::vector<std::string>& arguments, uint32_t index)
	{
		std::string finalString = "";
		for (uint32_t i = index; i < arguments.size(); i++)
		{
			if (i != index)
				finalString += ' ';
			finalString += arguments[i];
		}
		return finalString;
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

	std::string readFile(const char * filename)
	{
		
		std::string source;
		std::ifstream fileStream(filename, std::ios::in);
		if (fileStream.is_open()) {
			std::stringstream sstr;
			sstr << fileStream.rdbuf();
			source = sstr.str();
			fileStream.close();
			return source;
		}
		else {
			throw std::runtime_error("Failed to open file");
		}
		return source;
		
	}

	void printBinary(uint32_t value)
	{
		int size = 31;
		while (size >= 0)
		{
			std::cout << (int)(bool)(value & (1 << size));
			size--;
		}
	}

	void tolower(std::string &string)
	{
		for (std::string::iterator it = string.begin(); it != string.end(); it++)
		{
			*it = std::tolower(*it);
		}
	}
}
