#include "../include/Client.hpp"

/* --- CONSTRUCTORS and DESTRUCTOR --- */
Client::Client(std::string hostName):hostname(hostName), role(1){
	state = CL_STATE_INPUT;
	this->isAway = 0;
	this->away_msg = "Im Away From the Keyboard... I'll be right back!";
	//this->hostname();
	
}

Client::Client(const Client &client)
{
	*this = client;
}

Client::~Client() {
	return ;
}

Client& Client::operator=(const Client& client) {
	if (this != &client) {
	this->nickname = client.nickname;
	this->username = client.username;
	this->password = client.password;
	this->role = client.role;
	this->state = client.state;
	this->isAway = client.isAway;
	this->hostname = client.getHostname();
    }
    return *this;
}



/* --- DATA SETTERS --- */
void Client::setRole(uint8_t role) {
	this->role = role;
}

void	Client::setAwayStatus(bool isAway) {
	this->isAway = isAway;
}

void	Client::setAwayMsg(std::string away_msg) {
	this->away_msg = away_msg;
}

void Client::setUsername(std::string username) {
	this->username = username;
}

void Client::setNickname(std::string nickname) {
	this->nickname = nickname;
}

void Client::setPassword(std::string password) { //ya veremos, esto no es muy seguro
	this->password = password;
}

/* --- DATA GETTERS --- */
std::string	Client::getUsername() const {
	return (username);
}

std::string	Client::getAwayMsg() const {
	return (away_msg);
}

std::string	Client::getNickname()const{
	return (this->nickname);
}

bool	Client::getAwayStatus() const {
	return (isAway);
}

uint8_t	Client::getState() const {
	return (state);
}

uint32_t	Client::getRole()const {
	return (role);
}

uint32_t	Client::getChannel()const {
	return (channel);
}

std::string Client::getHostname()const{
	return hostname;
}

std::string Client::getUserMask()const{
	return nickname + "!" + username + "@" + hostname;
}

/* --- CHECKER FUNCTIONS --- */
bool Client::checkPassword(std::string pass)const {

	if (this->password == pass)
		return (true);
	else
		return (false);
}
