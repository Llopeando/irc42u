/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mainClient.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecamara <ecamara@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/18 13:36:15 by ecamara           #+#    #+#             */
/*   Updated: 2023/05/18 14:05:20 by ecamara          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/UdpClient.hpp"
#include <iostream>
#include <exception>

int main()
{
	UdpClientCreateInfo info = {};
	info.port = 12345;
	info.ip = "10.13.8.2";

	UdpClient *client;
	try{
		client = new UdpClient(info);
		client->start();
	}
	catch(std::exception &e){
		e.what();
		return 1;
	}
	delete client;
	return 0;
}