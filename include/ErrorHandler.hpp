/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ErrorHandler.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ullorent <ullorent@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/27 17:43:41 by ullorent          #+#    #+#             */
/*   Updated: 2023/07/31 13:28:38 by ullorent         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERRORS_HPP
#define ERRORS_HPP

#include <iostream>

#include "defines.h"
#include "Utils.hpp"
#include "ServerDataStructs.h"


#include "cmd_reply.h"

/*
*	Public:
*		Enum Type:	enum of all the error values;
*		error(): (2 arguments): sends error(code) message to specified client;
*		error(): (3 arguments): sends error(code) message to specified client with argument;
*		fatalError(): sends fatal error type(code) message to specified client

*		
*/

START_ERROR_NAMESPACE

typedef std::map<int, std::string> ErrMap;

enum Type{
	ERR_UNKNOWNERROR = 400,
	ERR_NOSUCHNICK = 401,
	ERR_NOSUCHSERVER = 402,
	ERR_NOSUCHCHANNEL = 403,
	ERR_CANNOTSENDTOCHAN = 404,
	ERR_TOOMANYCHANNELS = 405,
	ERR_WASNOSUCHNICK = 406,
	ERR_NOORIGIN = 409,
	ERR_NORECIPIENT = 411,
	ERR_NOTEXTTOSEND = 412,
	ERR_INPUTTOOLONG = 417,
	ERR_UNKNOWNCOMMAND = 421,
	ERR_NOMOTD = 422,
	ERR_NONICKNAMEGIVEN = 431,
	ERR_ERRONEUSNICKNAME = 432,
	ERR_NICKNAMEINUSE = 433,
	ERR_NICKCOLLISION = 436,
	ERR_USERNOTINCHANNEL = 441,
	ERR_NOTONCHANNEL = 442,
	ERR_USERONCHANNEL = 443,
	ERR_NOTREGISTERED = 451,
	ERR_NEEDMOREPARAMS = 461,
	ERR_ALREADYREGISTERED = 462,
	ERR_PASSWDMISMATCH = 464,
	ERR_YOUREBANNEDCREEP = 465,
	ERR_CHANNELISFULL = 471,
	ERR_UNKNOWNMODE = 472,
	ERR_INVITEONLYCHAN = 473,
	ERR_BANNEDFROMCHAN = 474,
	ERR_BADCHANNELKEY = 475,
	ERR_BADCHANMASK = 476,
	ERR_NOPRIVILEGES = 481,
	ERR_CHANOPRIVSNEEDED = 482,
	ERR_CANTKILLSERVER = 483,
	ERR_NOOPERHOST = 491,
	ERR_UMODEUNKNOWNFLAG = 501,
	ERR_USERSDONTMATCH = 502,
	ERR_HELPNOTFOUND = 524,
	ERR_INVALIDKEY = 525,
	RPL_STARTTLS = 670,
	RPL_WHOISSECURE = 671,
	ERR_STARTTLS = 691,
	ERR_INVALIDMODEPARAM = 696,
	RPL_HELPSTART = 704,
	RPL_HELPTXT = 705,
	RPL_ENDOFHELP = 706,
	ERR_NOPRIVS = 723,
	RPL_LOGGEDIN = 900,
	RPL_LOGGEDOUT = 901,
	ERR_NICKLOCKED = 902,
	RPL_SASLSUCCESS = 903,
	ERR_SASLFAIL = 904,
	ERR_SASLTOOLONG = 905,
	ERR_SASLABORTED = 906,
	ERR_SASLALREADY = 907,
	ERR_TOOMANYTARGETS = 407,
	ERR_BADPASSWORD = 1005,
	RPL_TOPIC = 332,
	RPL_TOPICWHOTIME = 333,
	RPL_NAMREPLY = 353,
	RPL_ENDOFNAMES = 366,
	RPL_AWAY =  301,
	RPL_UNAWAY =  305, 		
	RPL_NOWAWAY =  306,		
	RPL_MOTDSTART = 375, 	
	RPL_MOTD = 372, 		
	RPL_ENDOFMOTD = 376, 	
	ERR_NOMODEOPTION = 666,
	
};

START_ANONYMOUS_NAMESPACE

static const ErrMap& getErrorMap()
{
	static ErrMap errMap;
	if (errMap.empty())
	{
	errMap[ERR_UNKNOWNERROR]		= "Unknown Error";
	errMap[ERR_NOSUCHNICK]			= "No such nick/channel";
	errMap[ERR_NOSUCHSERVER]		= "No such server";
	errMap[ERR_NOSUCHCHANNEL]		= "No such channel";
	errMap[ERR_CANNOTSENDTOCHAN]	= "Cannot send to channel";
	errMap[ERR_TOOMANYCHANNELS]		= "Server channel limit reached";
	errMap[ERR_WASNOSUCHNICK]		= "There was no such nickname";
	errMap[ERR_NOORIGIN]			= "No origin specified";
	errMap[ERR_NOTEXTTOSEND]		= "No text to send";
	errMap[ERR_INPUTTOOLONG]		= "Input line was too long";
	errMap[ERR_UNKNOWNCOMMAND]		= "Unknown command";
	errMap[ERR_NOMOTD]				= "MOTD File is missing";
	errMap[ERR_NONICKNAMEGIVEN]		= "No nickname given";										//431
	errMap[ERR_ERRONEUSNICKNAME]	= "Erroneus nickname";										//432
	errMap[ERR_NICKNAMEINUSE]		= "Nickname is already in use"; 							//433
	errMap[ERR_USERNOTINCHANNEL]	= "They aren't on that channel";
	errMap[ERR_NOTONCHANNEL]		= "You're not on that channel"; 							//442
	errMap[ERR_USERONCHANNEL]		= "is already in channel";
	errMap[ERR_NEEDMOREPARAMS]		= "Not enough parameters";                 					//461 
	errMap[ERR_ALREADYREGISTERED]	= "You may not reregister"; 								//462
	errMap[ERR_PASSWDMISMATCH]		= "Password incorrect";										//464
	errMap[ERR_INVITEONLYCHAN]		= "Cannot join channel (+i)";								//473
	errMap[ERR_BANNEDFROMCHAN]		= "Cannot join channel (+b)";								//474
	errMap[ERR_BADCHANNELKEY]		= "Cannot join channel (+k)";								//475
	errMap[ERR_BADCHANMASK]			= "Bad Channel Mask";										//476
	errMap[ERR_NOPRIVILEGES]		= "Permission Denied- You're not an IRC operator";			//481
	errMap[ERR_CHANOPRIVSNEEDED]	= "You're not channel operator";							//482
	errMap[ERR_NOOPERHOST]			= "No O-lines for your host"; 								//491
	errMap[ERR_CANNOTSENDTOCHAN]	= "Cannot send to channel";									//404
	errMap[ERR_TOOMANYTARGETS]		= "Duplicate recipients. No message delivered"; 			//407
	errMap[ERR_BADPASSWORD]			= "Closing Link: localhost (Bad Password)";
	errMap[ERR_CHANNELISFULL]		= "Cannot join channel (+l): (Channel is full)";
	errMap[ERR_NOMODEOPTION]		= "No MODE option in A O I R C Server";
	errMap[ERR_UNKNOWNCOMMAND]		= ":Unknown command";										//421
	}
	return errMap;
}

END_ANONYMOUS_NAMESPACE

inline void	fatalError(cmd::CmdInput &bundle, Type errorCode)
{
	ErrMap::const_iterator it =  getErrorMap().find(errorCode);
	if (it != getErrorMap().end())
	{
		std::string errorMsg = "ERROR : " + it->second + "\r\n";
		utils::sendMsgUser(bundle.serverData[(sd::pollfdIt)bundle.index].fd, errorMsg);
	}
}

inline void	error(cmd::CmdInput &bundle, Type errorCode)
{
	ErrMap::const_iterator it =  getErrorMap().find(errorCode);
	if (bundle.serverData[(sd::clientIt)bundle.index].getNickname().empty())
	{
		std::string err_msg = ":" + bundle.serverData.getName() + " " + std::to_string(errorCode) + " " + "(unasigned)" + " :" + it->second + "\r\n";
		utils::sendMsgUser(bundle.serverData[(sd::pollfdIt)bundle.index].fd, err_msg);
	}
	else if (it != getErrorMap().end())
	{
		std::string err_msg = ":" + bundle.serverData.getName() + " " + std::to_string(errorCode) + " " + bundle.serverData[(sd::clientIt)bundle.index].getNickname() + " :" + it->second + "\r\n";
		utils::sendMsgUser(bundle.serverData[(sd::pollfdIt)bundle.index].fd, err_msg);
	}
	else 
	{
		std::string err_msg = ":" + bundle.serverData.getName() + " " + std::to_string(errorCode) + " " + bundle.serverData[(sd::clientIt)bundle.index].getNickname() + " :" + getErrorMap().begin()->second + "\r\n";
		utils::sendMsgUser(bundle.serverData[(sd::pollfdIt)bundle.index].fd, err_msg);
	}
	//std::cout << color::red << "ERROR: [" << err_msg.substr(0, err_msg << "]\n" << color::reset;
}

inline void	error(cmd::CmdInput &bundle, Type errorCode, std::string param)
{
	ErrMap::const_iterator it =  getErrorMap().find(errorCode);
	if (it != getErrorMap().end())
	{
		std::string err_msg = ":" + bundle.serverData.getName() + " " + std::to_string(errorCode) + " " + bundle.serverData[(sd::clientIt)bundle.index].getNickname() + " " + param + " :" + it->second + "\r\n";
		utils::sendMsgUser(bundle.serverData[(sd::pollfdIt)bundle.index].fd, err_msg);
	}
	else 
	{
		std::string err_msg = ":" + bundle.serverData.getName() + " " + std::to_string(errorCode) + " " + bundle.serverData[(sd::clientIt)bundle.index].getNickname() + " :" + getErrorMap().begin()->second + "\r\n";
		utils::sendMsgUser(bundle.serverData[(sd::pollfdIt)bundle.index].fd, err_msg);
	}
	//std::cout << color::red << "ERROR: [" << err_msg.substr(0, err_msg << "]\n" << color::reset;
}

END_ERROR_NAMESPACE

#endif