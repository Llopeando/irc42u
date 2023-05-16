#include "../include/Channel.hpp"

/* --- CONSTRUCTORS and DESTRUCTOR --- */
Channel::Channel(std::string name, std::string username, UsersData *data)
{
	this->name = name;
	creator = username;
	this->data = data;
	users.resize(0);
	numOfUsers = 0;
}

Channel::~Channel() {
	
}

/* --- GETTER --- */

std::string Channel::getName() const{
	return (name);
}

void Channel::addClient(clientIt index){
	users.push_back(index);
	uint32_t user_pos = numOfUsers;
	numOfUsers++;
	std::string infoUser = color::boldyellow + "ℹ️  You are in the " + color::boldgreen + this->name + color::boldyellow + " channel\n\n" + color::reset;
	std::string infoMsg = color::boldgreen +  " joined your channel 👋\n" + color::reset;
	(*data)[index].resetLastMsgIdx(); //resetea el index de log 
	sendMsgUser(user_pos, infoUser);
	broadcast(user_pos, infoMsg);
}

#include <locale>
#include <codecvt>

void Channel::sendMsgUser(clientIt it, const std::string &str) const
{
	int buffer_size = 65536;
	std::locale::global(std::locale("en_US.UTF-8"));
	setsockopt((*data)[(pollfdIt)it].fd, SOL_SOCKET, SO_SNDBUF, &buffer_size, sizeof(buffer_size));
	//std::string message = "PRIVMSG " + data[it].getUsername() + " : " + str;
	int code;
	std::wstring_convert<std::codecvt_utf8<wchar_t> > converter;
	std::wstring wideStr = converter.from_bytes(str);
	std::string utf8Str = converter.to_bytes(wideStr);
	if ((code = send((*data)[(pollfdIt)it].fd, utf8Str.c_str(), utf8Str.size(), 0)) <= 0)
	{
		std::error_code ec(errno, std::system_category());
		std::cerr << "[fd: " << (*data)[(pollfdIt)it].fd << "] An error ocurred sending the message: " << color::boldwhite << ec.message() << color::reset << "it = " << it << std::endl;
	}
	std::cout << "bytes sent " << code << '\n';
}

/*
void Channel::removeClient(uint32_t indexAct){
	
	//quitarlo del array
	uint32_t	user_pos = findUser(indexAct);
	if (user_pos == 0)
	{
		return;
	}

	//reubicar el array ??
	numOfUsers--;
	users[user_pos] = users[numOfUsers];
	users.pop_back();
	std::string infoUser = color::boldyellow + "ℹ️  You are back to the " + color::boldgreen + "Lobby" + color::boldyellow + " channel\n\n" + color::reset;
	std::string infoMsg = color::boldgreen +  " left your channel 😢\n" + color::reset;
	sendMsgUser(user_pos, infoUser);
	sendMsgChannel(user_pos, infoMsg, true);
	(*data)[indexAct].setState(CL_STATE_LOBBY);
}
*/
/* ------------------------------------------------------------ */
/*					SEND MESSAGES IN CHANNEL					*/
/* ------------------------------------------------------------ */
//de momento va duplicado, uno en channel y otro en server 
/*
void	Channel::sendMsgUser(uint32_t user_pos, std::string const &str) const {
	int buffer_size = 65536;      //en defines?
	setsockopt(data->getFd(users[user_pos]), SOL_SOCKET, SO_SNDBUF, &buffer_size, sizeof(buffer_size));
	send(data->getFd(users[user_pos]), str.c_str(), str.size(), 0);
}

void Channel::flushLog(Client &user, uint32_t user_pos){

	//check last msg
	uint32_t i;
	for (i = user.getLastMsgIdx(); i < msg_log.size(); i++)
		sendMsgUser(user_pos, "\t\t\t" + msg_log[i]);
	i -= user.getLastMsgIdx();
	user.addLastMsgIdx(i);
}*/
/*
void	Channel::sendInfoChannel(uint32_t user_pos, std::string const &str)
{
	flushLog((*data)[users[user_pos]], user_pos);
	for(uint32_t i = 0; i < users.size();i++)
	{
		if (!((*data)[users[i]].getInputBlock())) // solo en fiuncion de visualizacion
		{
			sendMsgUser(i, msg);
			(*data)[users[i]].addLastMsgIdx(1);
		}
	}
}*/

/*
uint32_t	Channel::findUser(uint32_t indexAct)const {
	for (uint32_t i = 0; i < users.size();i++)
	{
		if (users[i] == indexAct)
			return i;
	}
	return (0);
}
*/
void	Channel::broadcast(clientIt sender, std::string const &msg) {
	for(clientIt i = 0; i < users.size();i++)
	{
		if (i != sender)
		{
			sendMsgUser(i, msg);
		}
	}
}
/*
void Channel::refresh(uint32_t indexAct)
{
	uint32_t user_pos = findUser(indexAct);
	flushLog((*data)[indexAct], user_pos);
}
*/