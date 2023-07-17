/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mainServer.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecamara <ecamara@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/18 13:36:15 by ecamara           #+#    #+#             */
/*   Updated: 2023/05/19 14:42:55 by ecamara          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/UdpServer.hpp"
#include "../includes/Color.h"
#include <iostream>
#include <exception>

int main()
{
	ServerCreateInfo info = {};
	info.port = 12345;
	info.numOfRooms = 8;

	UdpServer *server;
	try{
		server = new UdpServer(info);
		server->run();
	}
	catch(std::exception &e){
		std::cout << color::boldred << "Error, " << e.what() << "\n" << color::reset;
		return 1;
	}
	delete server;
	return 0;
}