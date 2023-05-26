/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ullorent <ullorent@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/25 19:12:28 by ecamara           #+#    #+#             */
/*   Updated: 2023/05/26 18:09:00 by ullorent         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef UTILS_HPP
#define UTILS_HPP

#include <iostream>
#include <string>
#include <sys/socket.h>
#include "defines.hpp"
#include <vector>
#include <string>

#include <ifaddrs.h>
#include <netinet/in.h>
#include <net/if.h>
#include <netdb.h>

std::string printIp();
std::string joinStr(std::vector<std::string>& arguments, uint32_t index);
std::vector<std::string> split(const std::string &string, char c);
std::vector<std::string> splitIrcPrameters(const std::string &string, const char c);

void sendMsgUser(int fd, const std::string &str);

#endif