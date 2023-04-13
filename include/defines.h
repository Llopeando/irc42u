/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   defines.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecamara <ecamara@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/13 17:54:32 by ecamara           #+#    #+#             */
/*   Updated: 2023/04/13 20:01:47 by ecamara          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DEFINES_H
#define DEFINES_H

#define SERVER_FAILURE -1
#define CL_STATE_SERVER_PASSWORD 0
#define CL_STATE_LOG_IN 1
#define CL_STATE_SELECT_USERNAME 2
#define CL_STATE_INTRODUCE_PERSONAL_PASSWORD 3
#define CL_STATE_SELECT_NICKNAME 4
#define CL_STATE_LOBBY 5
#define CL_STATE_IN_CHANNEL 6

#define CL_STATE_ACTIVE 1
#define CL_STATE_INACTIVE 0

#define CL_ROOT 1

#define RCVBUFSIZE 32

#endif
