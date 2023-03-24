/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ullorent <ullorent@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/24 16:47:46 by ecamara           #+#    #+#             */
/*   Updated: 2023/03/24 20:16:23 by ullorent         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string>
#include <cstdio>
#include <unistd.h>
#include <poll.h>
#include <deque>

class Channel
{
	public:
		Channel(std::string name, std::deque<struct pollfd> *pollfds);
		~Channel();

		const char	*getName();
	private:
		std::string name;
		std::deque<struct pollfd> *pollfds;
		std::deque<size_t> userIndex;
		
}; 