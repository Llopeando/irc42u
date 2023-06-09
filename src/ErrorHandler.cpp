/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ErrorHandler.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ullorent <ullorent@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/25 18:17:21 by ecamara           #+#    #+#             */
/*   Updated: 2023/06/09 17:03:31 by ullorent         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ErrorHandler.hpp"

ErrorHandler::ErrorHandler()
{
	errCodes[ERR_UNKNOWNERROR]		= "";
	errCodes[ERR_NOSUCHNICK]		= "No such nick/channel";
	errCodes[ERR_NOSUCHSERVER]		= "No such server";
	errCodes[ERR_NOSUCHCHANNEL]		= "No such channel";
	errCodes[ERR_CANNOTSENDTOCHAN]	= "Cannot send to channel";
	errCodes[ERR_TOOMANYCHANNELS]	= "You have joined too many channels";
	errCodes[ERR_WASNOSUCHNICK]		= "There was no such nickname";
	errCodes[ERR_NOORIGIN]			= "No origin specified";
	errCodes[ERR_NORECIPIENT]		= "";
	errCodes[ERR_NOTEXTTOSEND]		= "No text to send";
	errCodes[ERR_INPUTTOOLONG]		= "Input line was too long";
	errCodes[ERR_UNKNOWNCOMMAND]	= "Unknown command";
	errCodes[ERR_NOMOTD]			= "MOTD File is missing";
	errCodes[ERR_NONICKNAMEGIVEN]	= "No nickname given";										//431
	errCodes[ERR_ERRONEUSNICKNAME]	= "Erroneus nickname";										//432
	errCodes[ERR_NICKNAMEINUSE]		= "Nickname is already in use"; 							//433
	errCodes[ERR_NICKCOLLISION]		= "";
	errCodes[ERR_USERNOTINCHANNEL]	= "They aren't on that channel";
	errCodes[ERR_NOTONCHANNEL]		= "You're not on that channel"; //442
	errCodes[ERR_USERONCHANNEL]		= "is already in channel";
	errCodes[ERR_NOTREGISTERED]		= "";
	errCodes[ERR_NEEDMOREPARAMS]	= "Not enough parameters";                 //461 
	errCodes[ERR_ALREADYREGISTERED]	= "You may not reregister"; 				//462
	errCodes[ERR_PASSWDMISMATCH]	= "Password incorrect";						//464
	errCodes[ERR_YOUREBANNEDCREEP]	= "";
	errCodes[ERR_CHANNELISFULL]		= "";
	errCodes[ERR_UNKNOWNMODE]		= "";
	errCodes[ERR_INVITEONLYCHAN]	= "Cannot join channel (+i)";				//473
	errCodes[ERR_BANNEDFROMCHAN]	= "Cannot join channel (+b)";				//474
	errCodes[ERR_BADCHANNELKEY]		= "Cannot join channel (+k)";				//475
	errCodes[ERR_BADCHANMASK]		= "Bad Channel Mask";						//476
	errCodes[ERR_NOPRIVILEGES]		= "Permission Denied- You're not an IRC operator";	//481
	errCodes[ERR_CHANOPRIVSNEEDED]	= "You're not channel operator";	//482
	errCodes[ERR_CANTKILLSERVER]	= "";
	errCodes[ERR_NOOPERHOST]		= "No O-lines for your host"; 				//491
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
	errCodes[ERR_CANNOTSENDTOCHAN]	= "Cannot send to channel";					//404
	errCodes[ERR_TOOMANYTARGETS]	= "Duplicate recipients. No message delivered"; // 407
	errCodes[ERR_BADPASSWORD]		= "Closing Link: localhost (Bad Password)";
}

void ErrorHandler::setData(UsersData *data, std::string serverName)
{
	this->data = data;
	this->serverName = serverName;
}

void ErrorHandler::fatalError(uint32_t index, uint32_t errorCode)
{
	std::string errorMsg = "ERROR : " + errCodes[errorCode] + "\r\n";
	sendMsgUser((*data)[(pollfdIt)index].fd, errorMsg);
	//S <-   ERROR :Closing Link: localhost (Bad Password)
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