/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerDefines.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecamara <ecamara@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/18 11:03:49 by ecamara           #+#    #+#             */
/*   Updated: 2023/05/22 13:37:40 by ecamara          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <exception>

#include "StaticColor.hpp"

#define MAX_ROOMS 8

struct messageInfo{
	uint32_t ID;
	uint32_t size;
};

struct	ServerCreateInfo{
	int	port;
	uint8_t numOfRooms;
};

struct ServerInfo{
	char		*ipAddresses[15 * MAX_ROOMS];
	int			ports[MAX_ROOMS];
	uint32_t	ID;
	uint8_t		activeRooms;
	uint8_t		maxRooms;
	void	print()
	{
		std::cout << "MY ID: " << ID << '\n';
		std::cout << "SERVERS: \n";
		for(uint8_t i = 0; i < activeRooms;i++)
		{
			std::cout << "\t\t[" << (int)i << "]: " << ports[i] << '\n';
		}
	};
};

