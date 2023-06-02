/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ErrorHandler.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ullorent <ullorent@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/25 18:07:30 by ecamara           #+#    #+#             */
/*   Updated: 2023/06/02 18:06:09 by ullorent         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERRORS_HPP
#define ERRORS_HPP

#include <iostream>
#include <unordered_map>
#include "UsersData.hpp"
#include "defines.hpp"
#include "Utils.hpp"

#define ERR_UNKNOWNERROR 400
#define ERR_NOSUCHNICK 401
#define ERR_NOSUCHSERVER 402
#define ERR_NOSUCHCHANNEL 403
#define ERR_CANNOTSENDTOCHAN 404
#define ERR_TOOMANYCHANNELS 405
#define ERR_WASNOSUCHNICK 406
#define ERR_NOORIGIN 409
#define ERR_NORECIPIENT 411
#define ERR_NOTEXTTOSEND 412
#define ERR_INPUTTOOLONG 417
#define ERR_UNKNOWNCOMMAND 421
#define ERR_NOMOTD 422
#define ERR_NONICKNAMEGIVEN 431
#define ERR_ERRONEUSNICKNAME 432
#define ERR_NICKNAMEINUSE 433
#define ERR_NICKCOLLISION 436
#define ERR_USERNOTINCHANNEL 441
#define ERR_NOTONCHANNEL 442
#define ERR_USERONCHANNEL 443
#define ERR_NOTREGISTERED 451
#define ERR_NEEDMOREPARAMS 461
#define ERR_ALREADYREGISTERED 462
#define ERR_PASSWDMISMATCH 464
#define ERR_YOUREBANNEDCREEP 465
#define ERR_CHANNELISFULL 471
#define ERR_UNKNOWNMODE 472
#define ERR_INVITEONLYCHAN 473
#define ERR_BANNEDFROMCHAN 474
#define ERR_BADCHANNELKEY 475
#define ERR_BADCHANMASK 476
#define ERR_NOPRIVILEGES 481
#define ERR_CHANOPRIVSNEEDED 482
#define ERR_CANTKILLSERVER 483
#define ERR_NOOPERHOST 491
#define ERR_UMODEUNKNOWNFLAG 501
#define ERR_USERSDONTMATCH 502
#define ERR_HELPNOTFOUND 524
#define ERR_INVALIDKEY 525
#define RPL_STARTTLS 670
#define RPL_WHOISSECURE 671
#define ERR_STARTTLS 691
#define ERR_INVALIDMODEPARAM 696
#define RPL_HELPSTART 704
#define RPL_HELPTXT 705
#define RPL_ENDOFHELP 706
#define ERR_NOPRIVS 723
#define RPL_LOGGEDIN 900
#define RPL_LOGGEDOUT 901
#define ERR_NICKLOCKED 902
#define RPL_SASLSUCCESS 903
#define ERR_SASLFAIL 904
#define ERR_SASLTOOLONG 905
#define ERR_SASLABORTED 906
#define ERR_SASLALREADY 907
#define ERR_TOOMANYTARGETS 407

#define ERR_BADPASSWORD 1000

#define RPL_TOPIC 332
#define RPL_TOPICWHOTIME 333
#define RPL_NAMREPLY 353
#define RPL_ENDOFNAMES 366

#define RPL_AWAY 301
#define RPL_UNAWAY 305 //When the user is no longer away
#define RPL_NOWAWAY 306 //When the user is away

#define RPL_MOTDSTART 375 //Start of the MOTD
#define RPL_MOTD 372 //Server reply with each line of the MOTD
#define RPL_ENDOFMOTD 376 //Indicates the end of MOTD to the client.

struct ErrorHandler{
	ErrorHandler();
	void	setData(UsersData *data, std::string serverName);
	void	fatalError(uint32_t index, uint32_t errorCode);
	void	error(uint32_t index, uint32_t errorCode);
	//void	error(uint32_t index, uint32_t errorCode, std::string command);
	void	error(uint32_t index, uint32_t errorCode, /*std::string command, */std::string param);
	private:
		std::unordered_map<uint32_t, std::string> errCodes;
		UsersData *data;
		std::string serverName;
};

#endif