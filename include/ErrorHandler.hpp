#ifndef ERRORS_HPP
#define ERRORS_HPP

#include <iostream>
#include <unordered_map>

#include "Config.h"
#include "defines.hpp"
#include "Utils.hpp"
#include "command_structs.h"
#include "ServerData.hpp"
#include "ServerDataStructs.h"


/*
*	Public:
*		Enum Type:	enum of all the error values;
*		error(): (2 arguments): sends error(code) message to specified client;
*		error(): (3 arguments): sends error(code) message to specified client with argument;
*		fatalError(): sends fatal error type(code) message to specified client;
*		FUNCTIONS
*		callFunction:	Will call any function that matches the key passed as a parameter.
*							Receives a key and a CmdInput. Returns a CmdReturn.
*	
*	Private:
*		pFunction:	Typedef of function pointer -> void __(clientIt index, std::vector<std::string>& arguments);
*		cmdMap:		Typedef of unordered map -> std::unordered_map<std::string, pFunction>.
*/

START_ERROR_NAMESPACE

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
	ERR_BADPASSWORD = 1000,
	RPL_TOPIC = 332,
	RPL_TOPICWHOTIME = 333,
	RPL_NAMREPLY = 353,
	RPL_ENDOFNAMES = 366,
	RPL_AWAY =  301,
	RPL_UNAWAY =  305, //When the user is no longer away
	RPL_NOWAWAY =  306,//When the user is away
	RPL_MOTDSTART = 375, //Start of the MOTD
	RPL_MOTD = 372, //Server reply with each line of the MOTD
	RPL_ENDOFMOTD = 376, //Indicates the end of MOTD to the client.
};


void	fatalError(const cmd::CmdInput &bundle, Type errorCode)
{
	ErrMap::const_iterator it =  getErrorMap().find(errorCode);
	if (it != getErrorMap().end()) {
		std::string errorMsg = "ERROR : " + it->second + "\r\n";
		sendMsgUser(bundle.serverData[(sd::pollfdIt)bundle.index].fd, errorMsg);
	}	
}

void	error(const cmd::CmdInput &bundle, Type errorCode)
{
	
}

void	error(const cmd::CmdInput &bundle, Type errorCode, std::string param)
{
	
}


/*

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

void ErrorHandler::error(uint32_t index, uint32_t errorCode, std::string param)
{

	//std::string err_msg = (*data)[(clientIt)index].getNickname() + " " + command + " :" + errCodes[errorCode] + "\r\n";
	std::string err_msg = ":" + serverName + " " + std::to_string(errorCode) + " " + (*data)[(clientIt)index].getNickname() + " " + param + " :" + errCodes[errorCode] + "\r\n";
	std::cout << color::red << "ERROR: [" << err_msg << "]\n" << color::reset;
	sendMsgUser((*data)[(pollfdIt)index].fd, err_msg);
	
}

*/
START_ANONYMOUS_NAMESPACE

typedef std::unordered_map<int, std::string> ErrMap;

static const ErrMap& getErrorMap()
{
	static ErrMap errMap;
	if (errMap.empty())
	{
	errMap[Type::ERR_UNKNOWNERROR]		= "";
	errMap[Type::ERR_NOSUCHNICK]		= "No such nick/channel";
	errMap[Type::ERR_NOSUCHSERVER]		= "No such server";
	errMap[Type::ERR_NOSUCHCHANNEL]		= "No such channel";
	errMap[Type::ERR_CANNOTSENDTOCHAN]	= "Cannot send to channel";
	errMap[Type::ERR_TOOMANYCHANNELS]	= "You have joined too many channels";
	errMap[Type::ERR_WASNOSUCHNICK]		= "There was no such nickname";
	errMap[Type::ERR_NOORIGIN]			= "No origin specified";
	errMap[Type::ERR_NORECIPIENT]		= "";
	errMap[Type::ERR_NOTEXTTOSEND]		= "No text to send";
	errMap[Type::ERR_INPUTTOOLONG]		= "Input line was too long";
	errMap[Type::ERR_UNKNOWNCOMMAND]	= "Unknown command";
	errMap[Type::ERR_NOMOTD]			= "MOTD File is missing";
	errMap[Type::ERR_NONICKNAMEGIVEN]	= "No nickname given";										//431
	errMap[Type::ERR_ERRONEUSNICKNAME]	= "Erroneus nickname";										//432
	errMap[Type::ERR_NICKNAMEINUSE]		= "Nickname is already in use"; 							//433
	errMap[Type::ERR_NICKCOLLISION]		= "";
	errMap[Type::ERR_USERNOTINCHANNEL]	= "They aren't on that channel";
	errMap[Type::ERR_NOTONCHANNEL]		= "You're not on that channel"; //442
	errMap[Type::ERR_USERONCHANNEL]		= "is already in channel";
	errMap[Type::ERR_NOTREGISTERED]		= "";
	errMap[Type::ERR_NEEDMOREPARAMS]	= "Not enough parameters";                 //461 
	errMap[Type::ERR_ALREADYREGISTERED]	= "You may not reregister"; 				//462
	errMap[Type::ERR_PASSWDMISMATCH]	= "Password incorrect";						//464
	errMap[Type::ERR_YOUREBANNEDCREEP]	= "";
	errMap[Type::ERR_CHANNELISFULL]		= "";
	errMap[Type::ERR_UNKNOWNMODE]		= "";
	errMap[Type::ERR_INVITEONLYCHAN]	= "Cannot join channel (+i)";				//473
	errMap[Type::ERR_BANNEDFROMCHAN]	= "Cannot join channel (+b)";				//474
	errMap[Type::ERR_BADCHANNELKEY]		= "Cannot join channel (+k)";				//475
	errMap[Type::ERR_BADCHANMASK]		= "Bad Channel Mask";						//476
	errMap[Type::ERR_NOPRIVILEGES]		= "Permission Denied- You're not an IRC operator";	//481
	errMap[Type::ERR_CHANOPRIVSNEEDED]	= "You're not channel operator";	//482
	errMap[Type::ERR_CANTKILLSERVER]	= "";
	errMap[Type::ERR_NOOPERHOST]		= "No O-lines for your host"; 				//491
	errMap[Type::ERR_UMODEUNKNOWNFLAG]	= "";
	errMap[Type::ERR_USERSDONTMATCH]	= "";
	errMap[Type::ERR_HELPNOTFOUND]		= "";
	errMap[Type::ERR_INVALIDKEY]		= "";
	errMap[Type::RPL_STARTTLS]			= "";
	errMap[Type::RPL_WHOISSECURE]		= "";
	errMap[Type::ERR_STARTTLS]			= "";
	errMap[Type::ERR_INVALIDMODEPARAM]	= "";
	errMap[Type::RPL_HELPSTART]			= "";
	errMap[Type::RPL_HELPTXT]			= "";
	errMap[Type::RPL_ENDOFHELP]			= "";
	errMap[Type::ERR_NOPRIVS]			= "";
	errMap[Type::RPL_LOGGEDIN]			= "";
	errMap[Type::RPL_LOGGEDOUT]			= "";
	errMap[Type::ERR_NICKLOCKED]		= "";
	errMap[Type::RPL_SASLSUCCESS]		= "";
	errMap[Type::ERR_SASLFAIL]			= "";
	errMap[Type::ERR_SASLTOOLONG]		= "";
	errMap[Type::ERR_SASLABORTED]		= "";
	errMap[Type::ERR_SASLALREADY]		= "";
	errMap[Type::ERR_CANNOTSENDTOCHAN]	= "Cannot send to channel";					//404
	errMap[Type::ERR_TOOMANYTARGETS]	= "Duplicate recipients. No message delivered"; // 407
	errMap[Type::ERR_BADPASSWORD]		= "Closing Link: localhost (Bad Password)";
	}
	return errMap;
}

//struct ErrorHandler{
//	ErrorHandler();
//		void	setData(ServerData *data, std::string serverName);
//		void	fatalError(uint32_t index, uint32_t errorCode);
//		void	error(uint32_t index, uint32_t errorCode);
//		//void	error(uint32_t index, uint32_t errorCode, std::string command);
//		void	error(uint32_t index, uint32_t errorCode, /*std::string command, */std::string param);
//	private:
//		std::unordered_map<uint32_t, std::string> errCodes;
//		ServerData *data;
//		std::string serverName;
//};

END_ANONYMOUS_NAMESPACE

END_ERROR_NAMESPACE

#endif