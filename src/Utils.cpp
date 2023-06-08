/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecamara <ecamara@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/25 19:12:34 by ecamara           #+#    #+#             */
/*   Updated: 2023/06/08 17:32:08 by ecamara          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Utils.hpp"

void sendMsgUser(int fd, const std::string &str)
{
	int buffer_size = 65536;

	setsockopt(fd, SOL_SOCKET, SO_SNDBUF, &buffer_size, sizeof(buffer_size));
	int code;
	if ((code = send(fd, str.c_str(), str.size(), 0)) <= 0)
	{
		std::error_code ec(errno, std::system_category());
		std::cerr << "[fd: " << fd << "]\n" << "An error ocurred sending the message: " << color::boldwhite << ec.message() << color::reset << std::endl;
	}
//std::cout << "bytes sent " << code << '\n';
}
/*
std::vector<std::string> spliti(const std::string &string, const char c)
{
	
	size_t pos = 0;
	std::vector<std::string> tokens;
	uint32_t line = 0;
	while ((pos = string.find(c)) != std::string::npos) {
	{
		tokens[line] = string.substr(0, pos);
		std::cout << color::blue << tokens[line] << color::reset << std::endl;
		pos +  tokens[line].size();
		line++;
		}
}
std::cout << s << std::endl;
}

*/

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
			//tokens.push_back(string.substr(start, colom - start));
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