#include "../include/Channel.hpp"

/* --- CONSTRUCTORS and DESTRUCTOR --- */
Channel::Channel(std::string name, std::string username, UsersData *data)
{
	this->name = name;
	this->topic = "TEMP TOPIC";
	creator = username;
	this->data = data;
	users.resize(1);
	numOfUsers = 0;
}

Channel::~Channel() {
	
}

/* --- GETTER AND SETTERS --- */

std::string Channel::getName() const{
	return (name);
}

void Channel::addClient(clientIt index){
	users.push_back(index);
	//uint32_t user_pos = numOfUsers;
	numOfUsers++;
}

void Channel::setTopic(std::string topic){
	this->topic = topic;
}

std::string Channel::getTopic( void)const{
	return(topic);
}

uint32_t	Channel::getNumUser( void)const{
	return (numOfUsers);

}


#include <locale>
#include <codecvt>

void Channel::sendMsgUser(clientIt it, const std::string &str) const
{
	//int buffer_size = 65536;
	//std::locale::global(std::locale("en_US.UTF-8"));
	//setsockopt((*data)[(pollfdIt)it].fd, SOL_SOCKET, SO_SNDBUF, &buffer_size, sizeof(buffer_size));
	//std::string message = "PRIVMSG " + data[it].getUsername() + " : " + str;
	int code;
	//std::wstring_convert<std::codecvt_utf8<wchar_t> > converter;
	//std::wstring wideStr = converter.from_bytes(str);
	//std::string utf8Str = converter.to_bytes(wideStr);
	if ((code = send((*data)[(pollfdIt)it].fd, str.c_str(), str.size(), 0)) <= 0)
	{
		std::error_code ec(errno, std::system_category());
		std::cerr << "[fd: " << (*data)[(pollfdIt)it].fd << "] An error ocurred sending the message: " << color::boldwhite << ec.message() << color::reset << "it = " << it << std::endl;
	}
//	std::cout << "bytes sent " << code << '\n';
}


void Channel::removeClient(clientIt indexAct){
	
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
	//std::string infoUser = color::boldyellow + "ℹ️  You are back to the " + color::boldgreen + "Lobby" + color::boldyellow + " channel\n\n" + color::reset;
	//std::string infoMsg = color::boldgreen +  " left your channel 😢\n" + color::reset;
	//sendMsgUser(user_pos, infoUser);
	//sendMsgChannel(user_pos, infoMsg, true);
	//(*data)[indexAct].setState(CL_STATE_LOBBY);
}


uint32_t	Channel::findUser(clientIt indexAct)const {
	for (uint32_t i = 0; i < users.size();i++)
	{
		if (users[i] == indexAct)
			return i;
	}
	return (0);
}

void	Channel::broadcast(clientIt sender, std::string const &msg) {
	for(clientIt i = 1; i < users.size();i++)
	{
		
		if (users[i] != sender)
		{
			//std::cout << color::green << "SENDED TO: [" << i << "]" << (*data)[(clientIt)users[i]].getUsername() << "\n" << color::reset;
			sendMsgUser(users[i], msg);
		}

	}
}

std::string Channel::getUserList()const
{
	std::string result;
	for (std::deque<uint32_t>::const_iterator it = users.begin() + 1; it != users.end(); it++)
	{
		result += (*data)[(clientIt)(*it)].getNickname() + " ";
	}
	return result;
}