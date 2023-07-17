/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UdpServer.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecamara <ecamara@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/18 10:37:29 by ecamara           #+#    #+#             */
/*   Updated: 2023/05/19 17:01:03 by ecamara          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>
#include <vector>

#include "ServerDefines.hpp"
#include "HandShakeServer.hpp"
#include "ServerData.hpp"
#include "ThreadedServer.hpp"
#include "StaticColor.hpp"


class UdpServer{
	public:
		UdpServer(ServerCreateInfo info);
		~UdpServer();

		void runHandShakeServer();
		void runServer(uint8_t num);
		const char	*getIpAddresses()const;
		const int	*getPorts()const;
		uint8_t		getActiveRooms()const;
	private:
		ServerCreateInfo	info;
		ServerData			data{info};
		HandShakeServer		handShakeServer{info, this, &data};
};