/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ullorent <ullorent@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/10 16:14:43 by ecamara           #+#    #+#             */
/*   Updated: 2023/03/24 20:27:36 by ullorent         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <sys/socket.h>
#include <netinet/in.h>

#define SERVER_FAILURE -1
#define CL_STATE_PASSWORD 0
#define CL_STATE_SELECT_CHANNEL 1
#define CL_STATE_IN_CHANNEL 2
#define CL_STATE_ACTIVE 1
#define CL_STATE_INACTIVE 0

typedef struct s_serverInput{
	struct sockaddr_in address;
	std::string password;
}t_serverInput;

