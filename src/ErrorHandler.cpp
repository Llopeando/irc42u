/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ErrorHandler.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecamara <ecamara@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/25 18:17:21 by ecamara           #+#    #+#             */
/*   Updated: 2023/05/25 20:17:34 by ecamara          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ErrorHandler.hpp"

ErrorHandler::ErrorHandler()
{
	errCodes[ERR_UNKNOWNERROR]		= "";
	errCodes[ERR_NOSUCHNICK]		= "";
	errCodes[ERR_NOSUCHSERVER]		= "";
	errCodes[ERR_NOSUCHCHANNEL]		= "No such channel";
	errCodes[ERR_CANNOTSENDTOCHAN]	= "";
	errCodes[ERR_TOOMANYCHANNELS]	= "You have joined too many channels";
	errCodes[ERR_WASNOSUCHNICK]		= "";
	errCodes[ERR_NOORIGIN]			= "";
	errCodes[ERR_NORECIPIENT]		= "";
	errCodes[ERR_NOTEXTTOSEND]		= "";
	errCodes[ERR_INPUTTOOLONG]		= "";
	errCodes[ERR_UNKNOWNCOMMAND]	= "";
	errCodes[ERR_NOMOTD]			= "";
	errCodes[ERR_NONICKNAMEGIVEN]	= "No nickname given";										//431
	errCodes[ERR_ERRONEUSNICKNAME]	= "Erroneus nickname";										//432
	errCodes[ERR_NICKNAMEINUSE]		= "Nickname is already in use"; 							//433
	errCodes[ERR_NICKCOLLISION]		= "";
	errCodes[ERR_USERNOTINCHANNEL]	= "";
	errCodes[ERR_NOTONCHANNEL]		= "";
	errCodes[ERR_USERONCHANNEL]		= "";
	errCodes[ERR_NOTREGISTERED]		= "";
	errCodes[ERR_NEEDMOREPARAMS]	= "Not enough parameters";                 //461 
	errCodes[ERR_ALREADYREGISTERED]	= "";
	errCodes[ERR_PASSWDMISMATCH]	= "";
	errCodes[ERR_YOUREBANNEDCREEP]	= "";
	errCodes[ERR_CHANNELISFULL]		= "";
	errCodes[ERR_UNKNOWNMODE]		= "";
	errCodes[ERR_INVITEONLYCHAN]	= "Cannot join channel (+i)";				//473
	errCodes[ERR_BANNEDFROMCHAN]	= "Cannot join channel (+b)";				//474
	errCodes[ERR_BADCHANNELKEY]		= "Cannot join channel (+k)";				//475
	errCodes[ERR_BADCHANMASK]		= "Bad Channel Mask";						//476
	errCodes[ERR_NOPRIVILEGES]		= "";
	errCodes[ERR_CHANOPRIVSNEEDED]	= "";
	errCodes[ERR_CANTKILLSERVER]	= "";
	errCodes[ERR_NOOPERHOST]		= "";
	errCodes[ERR_UMODEUNKNOWNFLAG]	= "";
	errCodes[ERR_USERSDONTMATCH]	= "";
	errCodes[ERR_HELPNOTFOUND]		= "";
	errCodes[ERR_INVALIDKEY]		= "";
	errCodes[RPL_STARTTLS]			= "";
	errCodes[RPL_WHOISSECURE]		= "";
	errCodes[ERR_STARTTLS]			= "";
	errCodes[ERR_INVALIDMODEPARAM]	= "";
	errCodes[RPL_HELPSTART]			= "";
	errCodes[RPL_HELPTXT]			= "";
	errCodes[RPL_ENDOFHELP]			= "";
	errCodes[ERR_NOPRIVS]			= "";
	errCodes[RPL_LOGGEDIN]			= "";
	errCodes[RPL_LOGGEDOUT]			= "";
	errCodes[ERR_NICKLOCKED]		= "";
	errCodes[RPL_SASLSUCCESS]		= "";
	errCodes[ERR_SASLFAIL]			= "";
	errCodes[ERR_SASLTOOLONG]		= "";
	errCodes[ERR_SASLABORTED]		= "";
	errCodes[ERR_SASLALREADY]		= "";
}

void ErrorHandler::setData(UsersData *data, std::string serverName)
{
	this->data = data;
	this->serverName = serverName;
}

void ErrorHandler::error(uint32_t index, uint32_t errorCode)
{
	std::string err_msg = ":" + serverName + " " + std::to_string(errorCode) + " " + (*data)[(clientIt)index].getNickname() + " :" + errCodes[errorCode] + "\r\n";
	sendMsgUser((*data)[(pollfdIt)index].fd, err_msg);
	std::cout << color::red << "ERROR: [" << err_msg.substr(0, err_msg.size() - 2) << "]\n" << color::reset;
	
}

//void ErrorHandler::error(uint32_t index, uint32_t errorCode, std::string command)
//{
//	(void)command;
//	std::string err_msg = ":" + serverName + " " + std::to_string(errorCode) + " " + (*data)[(clientIt)index].getNickname() +  + " ";
//	std::cout << color::red << "ERROR: [" << err_msg << "]\n" << color::reset;
//	sendMsgUser((*data)[(pollfdIt)index].fd, err_msg);
//	
//}

void ErrorHandler::error(uint32_t index, uint32_t errorCode, /*std::string command, */std::string param)
{

	//std::string err_msg = (*data)[(clientIt)index].getNickname() + " " + command + " :" + errCodes[errorCode] + "\r\n";
	std::string err_msg = ":" + serverName + " " + std::to_string(errorCode) + " " + (*data)[(clientIt)index].getNickname() + " " + param + " :" + errCodes[errorCode] + "\r\n";
	std::cout << color::red << "ERROR: [" << err_msg << "]\n" << color::reset;
	sendMsgUser((*data)[(pollfdIt)index].fd, err_msg);
	
}