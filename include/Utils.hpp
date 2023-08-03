/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ullorent <ullorent@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/27 17:43:36 by ullorent          #+#    #+#             */
/*   Updated: 2023/08/03 20:02:11 by ullorent         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
#define UTILS_HPP

#include <iostream>
#include <string>
#include <sys/socket.h>
#include <vector>
#include <string>
#include <sstream>
#include <ifaddrs.h>
#include <netinet/in.h>
#include <net/if.h>
#include <netdb.h>
#include <fstream>
#include <span>

#include "defines.h"

namespace utils {
	typedef std::chrono::system_clock t_chrono;
	
	std::string printIp();
	std::string joinStr(std::vector<std::string>& arguments, uint32_t index);
	std::vector<std::string> split(const std::string &string, char c);
	std::vector<std::string> splitIrcPrameters(const std::string &string, const char c);
	
	void sendMsgUser(int fd, std::string str);
	
	std::string readFile(const char *filepath);

	void printBinary(uint32_t value);

	void tolower(std::string &string);
	inline std::string removeHashtag(std::string& a){return a.substr(a[0] == '#');}
}

#endif