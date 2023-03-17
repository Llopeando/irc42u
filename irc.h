/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ullorent <ullorent@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/10 16:14:43 by ecamara           #+#    #+#             */
/*   Updated: 2023/03/17 19:45:12 by ullorent         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

typedef struct s_serverInput{
	struct sockaddr_in address;
	std::string password;
}t_serverInput;


void handleTCPClient(int client_fd);

