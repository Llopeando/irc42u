/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecamara <ecamara@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/25 19:12:34 by ecamara           #+#    #+#             */
/*   Updated: 2023/05/25 19:13:58 by ecamara          ###   ########.fr       */
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